#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {
}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : string_buffer(""), _buffer_size(capacity), _bytes_written(0), _bytes_read(0) {}

size_t ByteStream::write(const string &data) {
//    DUMMY_CODE(data);
    size_t write_cnt = min(remaining_capacity(), data.length());
    string_buffer += std::move(data.substr(0,write_cnt));
    _bytes_written += write_cnt;

    return write_cnt;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
//    DUMMY_CODE(len);
    return string_buffer.substr(0,len);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
//    DUMMY_CODE(len);
    string_buffer.erase(0,len);
    _bytes_read+=len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    //    DUMMY_CODE(len);
    std::string copied_str = peek_output(len);
    pop_output(len);

    return copied_str;
}

void ByteStream::end_input() { _is_input_ended = true; }

bool ByteStream::input_ended() const { return _is_input_ended; }

size_t ByteStream::buffer_size() const { return string_buffer.length(); }

bool ByteStream::buffer_empty() const { return string_buffer.empty(); }

bool ByteStream::eof() const { return _is_input_ended && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _buffer_size - buffer_size(); }
