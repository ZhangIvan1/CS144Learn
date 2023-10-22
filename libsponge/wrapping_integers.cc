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
        const uint32_t checkpoint_32 = checkpoint % mod;
        const uint32_t absolute_n = n.raw_value() - isn.raw_value();
        if (UINT64_MAX - checkpoint < mod || absolute_n == checkpoint_32)
            return (checkpoint / mod) * mod + absolute_n;
        if (checkpoint_32 > absolute_n)
            return checkpoint_32 - absolute_n > mod - checkpoint_32 + absolute_n ?
                        ((checkpoint / mod) + 1) * mod + absolute_n : (checkpoint / mod) * mod + absolute_n;
        else {
            if (checkpoint < mod) return absolute_n;
            return absolute_n - checkpoint_32 > mod + checkpoint_32 - absolute_n
                       ? ((checkpoint / mod) - 1) * mod + absolute_n
                       : (checkpoint / mod) * mod + absolute_n;
        }
}