#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity){}

size_t ByteStream::write(const string &data) {
    size_t wr = 0;
    for(const auto  c : data){
        if(0 >=_capacity - _buffer_size )
            break;
        else{
            _stream.push_back(c);
            ++_buffer_size;
            ++_bytes_written;
            ++wr;
        }
    }
    return wr;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    auto ite = _stream.begin();
    string str = "";
    for(size_t i = 0; i < len; i++){
        if(_buffer_size < i+1) break;
        str += *ite;
        ite++;
    }
    return str;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    for(size_t i = 0; i < len; i++){
        _stream.pop_front();
        ++_bytes_read;
        if(!--_buffer_size) break;
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string str = "";
    for(size_t i = 0; i < len; i++){
        if(!_buffer_size) break;
        str += _stream.front();
        _stream.pop_front();
        ++_bytes_read;
        _buffer_size--;
    }
    return str;
}

void ByteStream::end_input() {
    _input_ended = true;
}

bool ByteStream::input_ended() const { 
    return _input_ended;
}

size_t ByteStream::buffer_size() const { return _buffer_size; }

bool ByteStream::buffer_empty() const { return _buffer_size ? false : true; }

bool ByteStream::eof() const { return _input_ended && buffer_empty(); }

size_t ByteStream::bytes_written() const { return _bytes_written; }

size_t ByteStream::bytes_read() const { return _bytes_read; }

size_t ByteStream::remaining_capacity() const { return _capacity - _buffer_size; }
