
inline
lcpp::SchemeBool::SchemeBool()
{
#ifdef _DEBUG
    static ezUInt8 s_instances = 0U;
    EZ_ASSERT(s_instances < 2, "Instantiating SchemeBool more than 2 times is not allowed!");
    ++s_instances;
#endif // _DEBUG
}

inline
lcpp::SchemeBool::~SchemeBool()
{
}

inline
const lcpp::SchemeBool&
lcpp::SchemeBool::operator ==( const SchemeObject& obj ) const
{
    return convert(this == &obj); // identity
}

inline
ezString
lcpp::SchemeBool::toString() const
{
    EZ_ASSERT(this == &SCHEME_TRUE || this == &SCHEME_FALSE, "There cannot be another instance of SchemeBool other that SCHEME_TRUE and SCHEME_FALSE!");
    return this == &SCHEME_TRUE ? "#t" : "#f";
}

inline
lcpp::SchemeBool::operator bool() const
{
    EZ_ASSERT(this == &SCHEME_TRUE || this == &SCHEME_FALSE, "There cannot be another instance of SchemeBool other that SCHEME_TRUE and SCHEME_FALSE!");
    return this == &SCHEME_TRUE;
}
