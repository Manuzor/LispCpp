
inline
lcpp::SchemeNil::SchemeNil()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 1, "Instantiating SchemeNil more than once is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
lcpp::SchemeNil::~SchemeNil()
{
}

inline
const lcpp::SchemeBool&
lcpp::SchemeNil::operator ==(const SchemeObject& obj) const
{
    return convert(&obj == this); // identity
}
inline
ezString
lcpp::SchemeNil::toString() const
{
    return "()";
}
