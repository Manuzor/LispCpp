// static
inline
lcpp::LispVoid&
lcpp::LispVoid::instance()
{
    static LispVoid v;
    return v;
}


inline
lcpp::LispVoid::LispVoid()
{
}

inline
lcpp::LispVoid::~LispVoid()
{
}

inline
bool
lcpp::LispVoid::operator ==(const LispObject& obj) const
{
    return obj.is<LispVoid>();
}

inline
ezString
lcpp::LispVoid::toString() const
{
    static ezString voidString("#v");
    return voidString;
}
