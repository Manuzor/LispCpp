
namespace lcpp
{
    namespace number
    {
        inline
        Ptr<LispObject> create(FloatAlternative_t value)
        {
            return create(Float_t(value));
        }

        template<typename T_Number>
        inline
        Ptr<LispObject> create(T_Number value)
        {
            return create(Integer_t(value));
        }
    }
}
