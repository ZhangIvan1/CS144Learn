#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _receiver.stream_out().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return _time_passed - _time_last_segment_received; }

void TCPConnection::segment_received(const TCPSegment &seg) {
    _time_last_segment_received = _time_passed;

    if (seg.header().rst) {
        set_error();
        _statue = false;
        return;
    }

    if (seg.header().ack) {
        _receiver.segment_received(seg);
        _sender.ack_received(seg.header().ackno, seg.header().win);
        return;
    }

    if (seg.payload().size()) {
        _receiver.segment_received(seg);
        TCPSegment ackSeg;
        ackSeg.header().ack = true;
        _sender.send_empty_segment(ackSeg);
        return;
    }

    response_to_keepalive(seg);
}

bool TCPConnection::active() const { return _statue; }

size_t TCPConnection::write(const string &data) {
    if (!_statue)
        return {};
    return _sender.stream_in().write(data);
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) {
    if (!_statue)
        return;

    _time_passed += ms_since_last_tick;
}

void TCPConnection::end_input_stream() {
    _sender.stream_in().end_input();
    _sender.fill_window();
}

void TCPConnection::connect() {
    if (!_statue && _sender.is_syn_avaliable()) {
        _statue = true;
        _sender.fill_window();
    }
}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
            sent_rst();
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}

void TCPConnection::set_error() {
    _sender.stream_in().set_error();
    _receiver.stream_out().set_error();
}

void TCPConnection::sent_rst() {
    _statue = false;
    TCPSegment rstSeg;
    rstSeg.header().rst = true;
    _sender.send_empty_segment(rstSeg);
    set_error();
}

void TCPConnection::response_to_keepalive(TCPSegment seg) {
    if (_receiver.ackno().has_value() and (seg.length_in_sequence_space() == 0) and
        seg.header().seqno == _receiver.ackno().value() - 1) {
        TCPSegment keepaliveSeg;
        _sender.send_empty_segment(keepaliveSeg);
    }
}
