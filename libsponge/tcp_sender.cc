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

uint64_t TCPSender::bytes_in_flight() const { return {}; }

void TCPSender::fill_window() {}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) { DUMMY_CODE(ackno, window_size); }

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) { DUMMY_CODE(ms_since_last_tick); }

unsigned int TCPSender::consecutive_retransmissions() const { return {}; }

void TCPSender::send_empty_segment() {}

Timer::Timer() {}

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

void Timer::check_expired(const unsigned int ms_since_last_tick) {
    if (_is_running) {
        time_update(ms_since_last_tick);
    }
    if (_time_passed >= _retransmission_timeout) {
        slow_start();
        restart();
    }
}
