#include "tcp_timer.hh"

Timer::Timer(size_t _init_rto) : _rto(_init_rto) {}

void Timer::double_rto() { _rto = (_rto << 1) > _rto ? (_rto << 1) : _rto; }

bool Timer::tick(const size_t ms_since_last_tick, bool need_double_rto) {
    if (!_active) {
        return false;
    }
    if (_ms + ms_since_last_tick < _ms || _ms + ms_since_last_tick >= _rto) {
        // overflow or overtime, return true and reset _ms
        _ms = 0;
        if (need_double_rto) {
            double_rto();
        }
        _consecutive_retransmissions++;
        return true;
    }
    _ms += ms_since_last_tick;
    return false;
}