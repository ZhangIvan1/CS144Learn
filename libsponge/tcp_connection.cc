#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const { return _receiver.stream_out().remaining_capacity(); }

size_t TCPConnection::bytes_in_flight() const { return _sender.bytes_in_flight(); }

size_t TCPConnection::unassembled_bytes() const { return _receiver.unassembled_bytes(); }

size_t TCPConnection::time_since_last_segment_received() const { return _time_passed - _time_last_segment_received; }

void TCPConnection::segment_received(const TCPSegment &seg) {
    _time_last_segment_received = _time_passed;
}

bool TCPConnection::active() const { return _statue; }

size_t TCPConnection::write(const string &data) {
    if (!_statue) return {};
    return _sender.stream_in().write(data);
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) {
    if (!_statue) return;

    _time_passed += ms_since_last_tick;
}

void TCPConnection::end_input_stream() {}

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

void TCPConnection::sent_rst() {
    _statue = false;
    TCPSegment rstSeg;
    rstSeg.header().rst = true;
    _sender.send_empty_segment(rstSeg);
    _sender.stream_in().set_error();
    _receiver.stream_out().set_error();
}



