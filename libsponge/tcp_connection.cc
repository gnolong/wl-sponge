#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;
void TCPConnection::deliver_segments(){
    while(!_sender.segments_out().empty()){
        TCPSegment t = _sender.segments_out().front();
        _sender.segments_out().pop();
        if(_receiver.ackno().has_value()){
            t.header().ack = true;
            t.header().ackno = _receiver.ackno().value();
            t.header().win = static_cast<uint16_t>(_receiver.window_size());
        }
        // cerr<<t.header().summary()<<" "<<t.payload().str().size()<<endl;
        _segments_out.push(t);
    }
}

// void TCPConnection::update(){
//     _sender.fill_window();
//     deliver_segments();
// }

// void TCPConnection::time_wait(){
//     TCPSegment segment{};
//     TCPHeader &header = segment.header();
//     if(_receiver.ackno().has_value()){
//         header.ack = true;
//         header.ackno = _receiver.ackno().value();
//     }
//     header.win = static_cast<uint16_t>(_receiver.window_size());
//     _segments_out.push(segment);
//     _time_wait = true;
// }
void TCPConnection::unclean_shut(){
    // 设置流为error
    _receiver.stream_out().set_error();
    _sender.stream_in().set_error();
    _active = false;

    //发送RST给对面的主机,告诉它连接的本机出现了问题
    TCPSegment seg = _sender.segments_out().front();
    _sender.segments_out().pop();
    seg.header().ack = true;
    if (_receiver.ackno().has_value())
        seg.header().ackno = _receiver.ackno().value();
    seg.header().win = _receiver.window_size();
    seg.header().rst = true;
    _segments_out.push(seg);

}
size_t TCPConnection::remaining_outbound_capacity() const { 
    return _sender.stream_in().remaining_capacity();
}

size_t TCPConnection::bytes_in_flight() const { 
    return _sender.bytes_in_flight();
}

size_t TCPConnection::unassembled_bytes() const { 
    return _receiver.unassembled_bytes();
}

size_t TCPConnection::time_since_last_segment_received() const { 
    return _ms;
}

void TCPConnection::segment_received(const TCPSegment &seg) {
    if(!_active) return;
    
    TCPHeader header = seg.header();
    if(header.rst){//收到rst
        _receiver.stream_out().set_error();
        _sender.stream_in().set_error();
        _active = false;
        return;
    }
    _ms = 0;
    if(!_receiver.ackno().has_value() && header.syn){//syn_rcvd
        if(!_sender.next_seqno_absolute()){
            _receiver.segment_received(seg);
            _sender.ack_received(header.ackno,header.win);
            connect();
            return;
        }

        _receiver.segment_received(seg);// from syn_rcvd
        _sender.ack_received(header.ackno,header.win);
        _sender.send_empty_segment();
        deliver_segments();  
        _receiver.segment_received(seg);
        _sender.ack_received(header.ackno,header.win);
        _sender.fill_window();
        deliver_segments();
        return; 
    }
    if(header.fin && !_sender._fin_send){//close wait
        _receiver.segment_received(seg);
        _receiver.stream_out().end_input();
        _sender.ack_received(header.ackno,header.win);
        _sender.send_empty_segment();
        deliver_segments();

        // end_input_stream();

        _linger_after_streams_finish = false;
        return;
    }
    if(header.fin && _sender._fin_send){//from close wait to last ack and close
        _receiver.segment_received(seg);
        _receiver.stream_out().end_input();
        _sender.ack_received(header.ackno,header.win);
        _sender.send_empty_segment();
        deliver_segments();
        _time_wait = true;
        return;
    }
    if(!_linger_after_streams_finish && _sender.stream_in().input_ended()){//last_ack  -  closed
        _receiver.segment_received(seg);
        _sender.ack_received(header.ackno,header.win);
        if(_sender.next_seqno() == header.ackno)
            _active = false;
        return;
    }
    if(!_receiver._is_syn) return;//established below
    WrappingInt32 t = _receiver.ackno().value();
    // size_t s = _receiver.stream_out().bytes_written() + _receiver.unassembled_bytes();
    _receiver.segment_received(seg);//这个位置
    if(!check_win(header.seqno, t)) {//不合法要返回ack ackno
        // _sender.ack_received(header.ackno,header.win);

        _sender.send_empty_segment();
        deliver_segments();
        return;
    }
    _receiver.segment_received(seg);
    _sender.ack_received(header.ackno,header.win);
    
    // if(s != (_receiver.stream_out().bytes_written() + _receiver.unassembled_bytes() )
    //     && !_sender.bytes_in_flight() || )
    //     _sender.send_empty_segment();
    // else{
    //     // if(header.win == 0) return;
    //     _sender.fill_window();
    // }
    _sender.fill_window();
    if(//s != (_receiver.stream_out().bytes_written() + _receiver.unassembled_bytes() )
        seg.payload().size() > 0            //返回 ack 1
        && _sender.segments_out().empty())
        _sender.send_empty_segment();
        
    deliver_segments();
}
bool TCPConnection::check_win( WrappingInt32 &a,  WrappingInt32 &b){ //seqno是否合法
    uint64_t checkpoint = _receiver.stream_out().bytes_written();
    uint64_t a_ab = unwrap(a,_receiver._isn,checkpoint);
    uint64_t b_ab = unwrap(b,_receiver._isn,checkpoint);
    if((a_ab >= b_ab && a_ab < b_ab + _receiver.window_size()) 
        || (a_ab == b_ab && _receiver.window_size() == 0)) return true;//
    return false;
}
bool TCPConnection::active() const { 
    return _active; 
}

size_t TCPConnection::write(const string &data) {
    if(!_active) return 0;
    size_t n =  _sender.stream_in().write(data);
    _sender.fill_window();
    deliver_segments();
    return n;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick) { 
    if(!_active) return;
    _ms += ms_since_last_tick;
    if(_time_wait && _ms >= _cfg.rt_timeout * 10){
        _active = false;
        return;
    }
    _sender.tick(ms_since_last_tick);
    if (_sender.consecutive_retransmissions() > TCPConfig::MAX_RETX_ATTEMPTS)
        unclean_shut();
    deliver_segments();
}

void TCPConnection::end_input_stream() {
    _sender.stream_in().end_input();
    _sender.fill_window();
    deliver_segments();
}

void TCPConnection::connect() {
    if(_sender.next_seqno_absolute()) return;
    _sender.fill_window();
    deliver_segments();

}

TCPConnection::~TCPConnection() {
    try {
        if (active()) {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
            _sender.send_empty_segment();
            unclean_shut();
        }
    } catch (const exception &e) {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
