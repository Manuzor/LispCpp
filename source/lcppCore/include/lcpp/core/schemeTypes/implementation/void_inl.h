
inline
lcpp::SchemeVoid::SchemeVoid()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 1, "Instantiating SchemeVoid more than once is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
lcpp::SchemeVoid::~SchemeVoid()
{
}

inline
const lcpp::SchemeBool&
lcpp::SchemeVoid::operator ==(const SchemeObject& obj) const
{
    return convert(&obj == this); // identity
}

inline
ezString
lcpp::SchemeVoid::toString() const
{
    return "#v";
}
