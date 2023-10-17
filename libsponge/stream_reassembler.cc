#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : buffer_string(""), buffer_check(""), _reassembled_pt(0), _end_pt(-1), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
//    DUMMY_CODE(data, index, eof);
    size_t expended_length = index + data.length();
    size_t max_expended_length = _reassembled_pt + _capacity;

    if (index > max_expended_length)
        return;

    if (eof && expended_length <= max_expended_length)
        _end_pt = expended_length;

    if (expended_length > buffer_string.length() ) {
        buffer_string.resize(expended_length);
        buffer_check.resize(expended_length);
    }

        buffer_string.replace(index, data.length(), data);
        buffer_check.replace(index, data.length(), data.length(), '1');

    if (buffer_string.length() > max_expended_length){
        buffer_string.resize(max_expended_length);
        buffer_check.resize(max_expended_length);
    }

    if (buffer_check[_reassembled_pt] && _output.remaining_capacity()){
        size_t write_len = 0;
        while(buffer_check[_reassembled_pt + write_len] && _output.remaining_capacity() - write_len) write_len++;
        _output.write(buffer_string.substr(_reassembled_pt,write_len));
        _reassembled_pt += write_len;
    }

    if (_reassembled_pt == _end_pt)
        _output.end_input();
}

size_t StreamReassembler::unassembled_bytes() const {
    size_t bytes_cnt = 0;
    for (size_t i = _reassembled_pt; i != buffer_check.length(); i++)
        if(buffer_check[i] == '1') bytes_cnt++;
    return bytes_cnt;
}

bool StreamReassembler::empty() const { return !unassembled_bytes(); }
