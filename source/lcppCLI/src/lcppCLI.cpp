// lcppCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <functional>

class State;

struct Id
{
    size_t value;
    Id(size_t value) : value(value){}
};

class State
{
    ezString m_name;
public:
    typedef std::function<void(const State&, const State&)> TransitionCallback;

    State(const ezString& name) : m_name(name) {}

    Id registerCallback(TransitionCallback callback){ return Id(0); }
    void unregisterCallback(Id id){}

    ezString name() const { return m_name; }
    void name( ezString value ) { m_name = value; }
};

struct StateMachineCInfo
{
    ezMap<ezString, State*> m_states;
    void addTransition(State* from, State* to) {  }
};

class StateMachine
{
    ezMap<ezString, State*> m_states;
public:
    StateMachine(const StateMachineCInfo& cinfo) {}

    bool startTransition(const ezString& from, const ezString& to){ return true; }
};

int _tmain(int argc, _TCHAR* argv[])
{
    using namespace lcpp;

    ezStartup::StartupCore();

    //StateMachineCInfo info;
    //info.addTransition(new State("a"), new State("b"));
    //info.addTransition(new State("b"), new State("c"));
    //
    //StateMachine sm(info);
    //sm.startTransition("a", "b");

    auto* smCinfo = new StateMachineCInfo();

    ezStartup::ShutdownBase();

    return 0;
}

