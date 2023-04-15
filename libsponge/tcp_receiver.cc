#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
    TCPHeader head = seg.header();
    if(!_is_syn && !head.syn)   return ;

    if(head.syn){
        _is_syn = true;
        _isn = head.seqno; 
    }
    // if(!head.syn && head.seqno != ackno()) return;
    string payload = seg.payload().copy();
    WrappingInt32 seqno = head.syn ? head.seqno + 1 : head.seqno; //第一个有效字符的序列号    
    uint64_t checkpoint = stream_out().bytes_written();
    //uwarp出来是 absolute seqno 需要 -1 才能转化成 stream index
    uint64_t index = unwrap(seqno,_isn,checkpoint) - 1;

    _reassembler.push_substring(payload, index, _is_syn && head.fin);
}

optional<WrappingInt32> TCPReceiver::ackno() const {
    if(!_is_syn)    return {};
    else{
        size_t ack = stream_out().bytes_written() + 1;
        if(stream_out().input_ended())   return wrap(ack + 1,_isn); //此时fin为1，所以要多加1
        else    return wrap(ack,_isn); 
    }
}

size_t TCPReceiver::window_size() const {
    return stream_out().remaining_capacity();
}
