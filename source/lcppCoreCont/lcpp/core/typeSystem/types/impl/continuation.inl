
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
            return m_pFunction;
        }

        inline
        void Data::setFunction(Function_t pFunction)
        {
            m_pFunction = pFunction;
        }

        inline
        Stack& Data::getStack()
        {
            return reinterpret_cast<Stack&>(m_stack);
        }
    };
}
