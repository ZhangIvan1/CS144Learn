#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    //    DUMMY_CODE(seg);
    if (seg.header().syn && !_syn) {
        _isn = seg.header().seqno;
        _syn = seg.header().syn;
    } else if (!_syn) return;

    _reassembler.push_substring(seg.payload().copy(),
                                unwrap(seg.header().seqno, _isn, _reassembler.stream_out().bytes_written() + 1) - 1 +
                                    seg.header().syn,
                                seg.header().fin);

    if (seg.header().fin) _fin = seg.header().fin;
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    return _syn ? optional<WrappingInt32>{wrap(_reassembler.stream_out().bytes_written() + _syn + (_fin && _reassembler.empty()), _isn)}
                : nullopt;
}

size_t TCPReceiver::window_size() const { return _capacity - _reassembler.stream_out().buffer_size(); }