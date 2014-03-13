
inline
lcpp::SchemeVoid::SchemeVoid()
{
}

inline
lcpp::SchemeVoid::~SchemeVoid()
{
}

inline
bool
lcpp::SchemeVoid::operator ==(const SchemeObject& obj) const
{
    return obj.is(SchemeType::Void);
}

inline
ezString
lcpp::SchemeVoid::toString() const
{
    return "#v";
}
