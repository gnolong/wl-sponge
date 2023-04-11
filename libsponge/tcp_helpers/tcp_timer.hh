#ifndef SPONGE_LIBSPONGE_TCP_TIMER
#define SPONGE_LIBSPONGE_TCP_TIMER

#include <functional>

class Timer{
    public:
    Timer(size_t _init_rto);
    bool tick(const size_t ms_since_last_tick, bool need_double_rto);
    void active() { _active = true; }
    void banned() { _active = false; }
    void reset(size_t rto) {
        _rto = rto;
        _ms = 0;
        _consecutive_retransmissions = 0;
    }
    unsigned int get_consecutive_retransmissions() const {
        return _consecutive_retransmissions;
    }

   private:
    bool is_active() const { return _active; }
    void double_rto();

    size_t _rto;
    size_t _ms{0};
    bool _active{false};
    unsigned int _consecutive_retransmissions{0};
};
#endif //SPONGE_LIBSPONGE_TCP_TIMER