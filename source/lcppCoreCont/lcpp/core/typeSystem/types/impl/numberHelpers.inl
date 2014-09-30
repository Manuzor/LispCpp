
namespace lcpp
{
    namespace number
    {
        inline
        StackPtr<LispObject> create(FloatAlternative_t value)
        {
            return create(Float_t(value));
        }

        template<typename T_Number>
        inline
        StackPtr<LispObject> create(T_Number value)
        {
            return create(Integer_t(value));
        }
    }
}
