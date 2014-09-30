
namespace lcpp
{
    namespace symbol
    {
        EZ_FORCE_INLINE
        StackPtr<LispObject> create(ezUInt32 character)
        {
            auto characterString = ezStringBuilder();
            characterString.Append(character);
            return create(characterString);
        }
    }
}
