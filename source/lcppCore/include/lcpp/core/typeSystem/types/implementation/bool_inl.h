﻿// static
inline
lcpp::SchemeBool&
lcpp::SchemeBool::trueInstance()
{
    static SchemeBool t(true);
    return t;
}

// static
inline
lcpp::SchemeBool&
lcpp::SchemeBool::falseInstance()
{
    static SchemeBool f(false);
    return f;
}


inline
lcpp::SchemeBool::SchemeBool(bool value) :
    m_value(value)
{
}

inline
lcpp::SchemeBool::~SchemeBool()
{
}

inline
bool
lcpp::SchemeBool::operator ==( const SchemeObject& rhs ) const
{
    if(rhs.is<SchemeBool>())
    {
        return *this == static_cast<const SchemeBool&>(rhs);
    }
    return false;
}

inline
ezString
lcpp::SchemeBool::toString() const
{
    static ezString trueString("#t");
    static ezString falseString("#f");

    return m_value ? trueString : falseString;
}

inline
bool
lcpp::SchemeBool::value() const
{
    return m_value;
}

inline
bool
lcpp::isTrue(Ptr<SchemeObject> pObject)
{
    return pObject == SCHEME_TRUE_PTR;
}

inline
bool
lcpp::isFalse(Ptr<SchemeObject> pObject)
{
    return pObject == SCHEME_FALSE_PTR;
}

inline
bool
lcpp::operator ==(const SchemeBool& lhs, const SchemeBool& rhs)
{
    return lhs.value() == rhs.value();
}
