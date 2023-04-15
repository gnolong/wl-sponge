#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()}))
    , _initial_retransmission_timeout{retx_timeout}
    ,_timer(static_cast<size_t>(retx_timeout))
    , _stream(capacity){}

uint64_t TCPSender::bytes_in_flight() const {
    return _next_seqno - _ackno;
}

void TCPSender::fill_window() {

    uint64_t payload_size =
        _rwnd > (_next_seqno - _ackno) ? (_rwnd - (_next_seqno - _ackno)) : 0;
    if (_rwnd == 0 && _track.size() != 0) {
        return;
    }
    // payload_size = _rwnd;
    payload_size = _rwnd == 0 ? 1 : payload_size;//当接收方给我们的窗口大小为0时，且我们发送的数据全被确认，我们不能再接受到窗口和ackno信息，
    //此时为一个ack有效的回复报文，窗口大小默认构造时为0，就需要把窗口当作1继续发送，不然tcp就不能继续进行
    for (;;) {
        TCPSegment segment{};
        TCPHeader &header = segment.header();
        // If connection had not been established yet, send SYN
        if (_next_seqno == 0) {
            header.syn = true;
            payload_size--;
        }
        Buffer buf{_stream.read(payload_size > TCPConfig::MAX_PAYLOAD_SIZE
                                    ? TCPConfig::MAX_PAYLOAD_SIZE
                                    : payload_size)};
        segment.payload() = buf;
        payload_size -= buf.size();
        // If eof, set FIN
        if (_stream.eof() && !_fin_send && payload_size > 0) {//如果没有fin-send进行标志，窗口很大的话，会一直发送fin包
            header.fin = true;
            _fin_send = true;
            payload_size--;
        }
        if (segment.length_in_sequence_space() != 0) {
            header.seqno = wrap(_next_seqno, _isn);
            _next_seqno += segment.length_in_sequence_space();
            // Push it in _segments_out
            _segments_out.push(segment);
            // Push it in _track
            _track.push(segment);
            // update_checkpoint
            _checkpoint += segment.length_in_sequence_space();
            // Active timer
            _timer.active();
        }
        if (segment.length_in_sequence_space() == 0) {
            break;
        }
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
void TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size) { 
    //更新_ackno和窗口大小，将已经确认的报文从队列_track中拿走
    uint64_t abs_no = unwrap(ackno, _isn, _checkpoint);
    // Impossible ackno (beyond next seqno) or overdue ackno are ignored
    if (abs_no > _next_seqno || abs_no < _ackno) {
        return;
    }
    // Refresh timer when ack new data
    if (abs_no > _ackno) {
        _timer.reset(_initial_retransmission_timeout);
    }
    // Valid Ackno need update the _rwnd
    _rwnd = window_size;
    _ackno = abs_no;
    // Pop the acknowledgement segment
    for (; _track.size() > 0;) {
        auto old_segment = _track.front();
        if (unwrap(old_segment.header().seqno, _isn, _checkpoint) +
                old_segment.length_in_sequence_space() <=
            abs_no) {
            _track.pop();
            continue;
        }
        break;
    }
    if (_track.size() == 0) {
        // No more data need to track, banned the timer
        _timer.banned();
    }

}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick) {
    if (_timer.tick(ms_since_last_tick, !(_rwnd == 0 && _ackno))) {//不能改成_next_seqno是因为后者包含队列segment中的，发过一次就变了
                                                                    //但_ackno在建立连接是没有收到对方的第一个报文就一直为0
        if (_track.size() == 0) {
            _timer.banned();
            _timer.reset(_initial_retransmission_timeout);
            return;
        }
        // retransmit first segment in _track
        auto segment = _track.front();
        _segments_out.push(segment);
    }
    //这里要注意_rwnd == 0 && _ackno的时，不要让RTO翻倍
}

unsigned int TCPSender::consecutive_retransmissions() const { 
    //返回最新rtos,超时次数
    return _timer.get_consecutive_retransmissions();
}

void TCPSender::send_empty_segment() {
    TCPSegment segment{};
    TCPHeader &header = segment.header();
    header.seqno = wrap(_next_seqno, _isn);
    // Push it in _segments_out
    // cerr<<wrap(_ackno, _isn).raw_value()<<endl;
    _segments_out.push(segment);
}
