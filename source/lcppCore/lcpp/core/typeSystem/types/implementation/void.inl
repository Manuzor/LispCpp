﻿
inline
lcpp::LispVoid::LispVoid()
{
}

inline
lcpp::LispVoid::~LispVoid()
{
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispVoid::copy() const
{
    return create();
}

inline
const lcpp::Type&
lcpp::LispVoid::type() const
{
    return LispVoid::typeInfo();
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