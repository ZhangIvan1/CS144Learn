#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    , _stream(capacity) {}

uint64_t TCPSender::bytes_in_flight() const { return _next_seqno - _ackno; }

void TCPSender::fill_window() {
    size_t fill_size = (_window_size ? _window_size : 1) - bytes_in_flight();
    while (_status == CLOSED || (fill_size && _status != FIN_SENT)) {
        TCPSegment segment;
        if (_status == CLOSED or _status == SYN_SENT) {
            segment.header().syn = true;
            segment.header().seqno = _isn;
            _status = SYN_SENT;
            if (!_timer.is_running()) {
                _timer.start(_initial_retransmission_timeout);
            }
        }
        if (_status == SYN_SENT or (_status == SYN_ACKED && !_stream.buffer_empty())) {
            size_t payload_len =
                min(_window_size - bytes_in_flight() - segment.length_in_sequence_space(), TCPConfig::MAX_PAYLOAD_SIZE);
            segment.header().seqno = wrap(_next_seqno, _isn);
            segment.payload() = _stream.read(payload_len);
            if (_stream.eof()) {
                segment.header().fin = true;
                _status = FIN_SENT;
            }
            _segments_out.push(segment);
            _segments_cache.push(segment);
            _next_seqno += segment.length_in_sequence_space();
        }
        if (!segment.length_in_sequence_space())
            return;
        fill_size -= segment.length_in_sequence_space();
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) {
    uint64_t absoult_ackno = unwrap(ackno, _isn, _ackno);
    if (absoult_ackno < _ackno || absoult_ackno > _next_seqno) {
        return;
    }

    if (_status == SYN_SENT)
        _status = SYN_ACKED;
    if (_status == FIN_SENT) {
        _status = FIN_ACKED;
        return;
    }
    if (_status == SYN_ACKED) {
        _ackno = absoult_ackno;
        _window_size = window_size;

        while (!_segments_cache.empty() &&
               unwrap(_segments_cache.front().header().seqno, _isn, _ackno) < unwrap(ackno, _isn, _ackno)) {
            _segments_cache.pop();
        }

        if (_segments_cache.empty())
            _timer.stop();

        _timer.reset(_initial_retransmission_timeout);
    }
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    if (_timer.check_expired(ms_since_last_tick)) {
        if (!_window_size)
            _timer.slow_start();
        if (consecutive_retransmissions() <= TCPConfig::MAX_RETX_ATTEMPTS) {
            _segments_out.push(_segments_cache.front());
            _timer.restart();
        } else {
            _status = ERROR;
        }
    }
}

unsigned int TCPSender::consecutive_retransmissions() const { return _timer.consecutive_retransmissions_times(); }

void TCPSender::send_empty_segment() {
    TCPSegment segment;
    segment.header().seqno = wrap(_next_seqno, _isn);
    _segments_out.push(segment);
}

Timer::Timer() = default;

void Timer::start(const unsigned int _initial_retransmission_timeout) {
    _is_running = true;
    _consecutive_retransmissions_times = 0;
    _retransmission_timeout = _initial_retransmission_timeout;
}

void Timer::stop() { _is_running = false; }

void Timer::reset(const unsigned int _initial_retransmission_timeout) {
    stop();
    start(_initial_retransmission_timeout);
}

void Timer::restart() { _time_passed = 0; }

void Timer::time_update(const unsigned int ms_since_last_tick) {
    if (_is_running)
        _time_passed += ms_since_last_tick;
}

void Timer::slow_start() {
    _consecutive_retransmissions_times = 0;
    _retransmission_timeout *= 2;
}

bool Timer::check_expired(const unsigned int ms_since_last_tick) {
    if (_is_running) {
        time_update(ms_since_last_tick);
    }
    if (_time_passed >= _retransmission_timeout) {
        return true;
    }
    return false;
}
