#ifndef SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
#define SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH

#include "byte_stream.hh"

#include <cstdint>
#include <string>
#include <deque>

//! \brief A class that assembles a series of excerpts from a byte stream (possibly out of order,
//! possibly overlapping) into an in-order byte stream.
class StreamReassembler {
  private:
    // Your code here -- add private members as necessary.
    std::deque < char > _buffer ;
    std::deque < bool > _flag ;
    bool _first_in = true;
    bool _is_eof = false;
    size_t _eof_index = 0;
    size_t _unassembled_bytes = 0;
    ByteStream _output;  //!< The reassembled in-order byte stream
    size_t _capacity;    //!< The maximum number of bytes

  public:

    StreamReassembler(const size_t capacity);   

    void push_substring(const std::string &data, const uint64_t index, const bool eof);

    const ByteStream &stream_out() const { return _output; }
    ByteStream &stream_out() { return _output; }

    size_t unassembled_bytes() const;

    bool empty() const;
};

#endif  // SPONGE_LIBSPONGE_STREAM_REASSEMBLER_HH
