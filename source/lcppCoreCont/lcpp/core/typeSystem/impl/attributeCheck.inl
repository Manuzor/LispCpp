
namespace lcpp
{
#if EZ_DISABLED(LCPP_RUNTIME_TYPE_CHECK)

    EZ_FORCE_INLINE
    void attributeCheckAll(const Ptr<LispObject>&, const AttributeFlags& expectedFlags) {}

    EZ_FORCE_INLINE
    void attributeCheckAny(const Ptr<LispObject>&, const AttributeFlags& expectedFlags) {}

    EZ_FORCE_INLINE
    void attributeCheckAll(const Ptr<LispObject>&, ezUInt32) {}

    EZ_FORCE_INLINE
    void attributeCheckAny(const Ptr<LispObject>&, ezUInt32) {}

#endif
}
