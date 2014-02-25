// lcppCLI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <functional>

namespace lcpp
{
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

        Id registerCallback(TransitionCallback callback){ m_callbacks.PushBack(callback); return Id(m_callbacks.GetCount() - 1); }
        void unregisterCallback(Id id){ /* TODO: Implement me. */ }

        ezString name() const { return m_name; }
        void name( ezString value ) { m_name = value; }

    private:
        ezDynamicArray<TransitionCallback> m_callbacks;
    };

    struct StateMachineCInfo
    {
        ezMap<ezString, State*> m_states;
        ezResult addTransition(const ezString& from, const ezString& to) { return EZ_SUCCESS; }

        StateMachineCInfo()
        {
        }
    };

    class StateMachine
    {
        ezMap<ezString, State*> m_states;
    public:
        StateMachine(const StateMachineCInfo& cinfo) {}

        bool startTransition(const ezString& from, const ezString& to){ return true; }
    };

    void entryFunction()
    {
        //StateMachineCInfo info;
        //info.addTransition(new State("a"), new State("b"));
        //info.addTransition(new State("b"), new State("c"));
        //
        //StateMachine sm(info);
        //sm.startTransition("a", "b");
    } 
}

//int _tmain(int argc, _TCHAR* argv[])
//{
//    ezStartup::StartupCore();
//
//    lcpp::entryFunction();
//
//    ezStartup::ShutdownBase();
//
//    return 0;
//}

int main(int argc, const char* argv[])
{
    sf::Window window(sf::VideoMode(800, 600), "My First SFML Application");
    sf::Event evt;

    while (window.isOpen())
    {
        while (window.pollEvent(evt))
        {
            switch (evt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        window.setActive();
        window.display();
    }
    return 0;
}
