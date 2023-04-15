#include "stream_reassembler.hh"
#include <iostream>
// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) :
    _buffer(capacity,'\0') ,
    _flag(capacity,false), 
    _output(capacity),
    _capacity(capacity){}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    //size_t _first_unread = _start_index + _output.bytes_read();
    size_t _first_unassembled = _output.bytes_written();
    size_t _first_unaccept = _first_unassembled + _capacity;
    //超出待读取范围的直接扔掉
    if(index >= _first_unaccept || index + data.length() < _first_unassembled){ return ;}
    
    //裁剪字符串开始和结尾的index
    size_t begin_index = index;
    size_t end_index = index + data.length();

    if(begin_index < _first_unassembled){ begin_index = _first_unassembled;}
    if(end_index >= _first_unaccept){   end_index = _first_unaccept;}

    //元素入队 
    for(size_t i = begin_index;i < end_index;i++){
        if(!_flag[i - _first_unassembled]){
            _buffer[i - _first_unassembled] = data[i - index];
            _unassembled_bytes ++;
            _flag[i - _first_unassembled] = true;
        }
    }

    string wait_str = "";
    while(_flag.front() ){
        wait_str += _buffer.front();
        _buffer.pop_front();
        _flag.pop_front();
        //为了保持队列容量不变需要在后面添加空元素占位
        _buffer.emplace_back('\0');
        _flag.emplace_back(false);
        
    }

    if(wait_str.length() > 0){
        stream_out().write(wait_str);
        _unassembled_bytes -= wait_str.length();
    }

    if(eof){ 
        _is_eof = true;
        _eof_index = end_index;
    }
    if(_is_eof && _eof_index == _output.bytes_written()){
        _output.end_input();
    }

}

size_t StreamReassembler::unassembled_bytes() const { return _unassembled_bytes; }

bool StreamReassembler::empty() const { return unassembled_bytes() == 0; }