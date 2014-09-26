
namespace lcpp
{
    namespace cons
    {
        EZ_FORCE_INLINE
        Ptr<LispObject>& Data::getCar()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pCar);
        }

        EZ_FORCE_INLINE
        Ptr<LispObject>& Data::getCdr()
        {
            return reinterpret_cast<Ptr<LispObject>&>(m_pCdr);
        }
    }
}
