
namespace lcpp
{
    namespace cont
    {
        inline
        Ptr<LispObject> Data::getParent()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pParent);
        }

        inline
        Function_t Data::getFunction()
        {
            return reinterpret_cast<Function_t&>(m_pFunction);
        }

        inline
        Stack& Data::getStack()
        {
            return reinterpret_cast<Stack&>(m_stack);
        }
    };
}
