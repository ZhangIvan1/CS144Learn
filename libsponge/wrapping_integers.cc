#include "wrapping_integers.hh"


// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
//    DUMMY_CODE(n, isn);
        const uint64_t mod = 1UL  << 32;
        WrappingInt32 seqno ((n % mod + isn.raw_value()) % mod);
        return seqno;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
//    DUMMY_CODE(n, isn, checkpoint);
        const uint64_t mod = 1UL << 32;
        uint64_t absolute_seqno_wrap = n.raw_value() - isn.raw_value();
        uint64_t absolute_seqno_right = absolute_seqno_wrap;
        uint64_t absolute_seqno_left = absolute_seqno_wrap;
        while (absolute_seqno_wrap <= checkpoint){
            if (UINT64_MAX - absolute_seqno_wrap < mod || absolute_seqno_wrap == checkpoint)
                return absolute_seqno_wrap;
            else
                absolute_seqno_wrap += mod;
            if (absolute_seqno_wrap < checkpoint) absolute_seqno_left = absolute_seqno_wrap;
            if (absolute_seqno_wrap > checkpoint) absolute_seqno_right = absolute_seqno_wrap;
        }
        return absolute_seqno_right - checkpoint > checkpoint - absolute_seqno_left ?
                          absolute_seqno_left : absolute_seqno_right;
}