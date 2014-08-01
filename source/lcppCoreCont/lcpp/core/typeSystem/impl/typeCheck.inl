
namespace lcpp
{
#if EZ_DISABLED(LCPP_RUNTIME_TYPE_CHECK)

    EZ_FORCE_INLINE
    void typeCheck(const Ptr<LispObject>&, const Type&) {}

    EZ_FORCE_INLINE
    void typeCheck(const Ptr<LispObject>&, Type::Enum) {}

#endif
}
