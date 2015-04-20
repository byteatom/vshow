#ifndef FSM_H
#define FSM_H

template<class Host, class Dim> class Fsm
{
public:
    typedef void (Host::*Action)(typename Dim::Event event, void *data);

    static Action actions[Dim::MAX_STATE][Dim::MAX_EVENT];

    Fsm(Host *obj) : obj{obj}, state{Dim::IDLE}{}
    ~Fsm(){}

    void fire(typename Dim::Event event, void *data = nullptr) {
        Action action = actions[state][event];
        if (action) (obj->*action)(event, data);
    }

    Host *obj;
    typename Dim::State state;
};

#endif // FSM_H
