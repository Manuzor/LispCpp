#pragma once

namespace lcpp
{
    class UserInterface
    {
    public:
        UserInterface(){}
        ~UserInterface(){}

        void initialize();
        void shutdown();

        void run();

    protected:
    private:
    };
}
