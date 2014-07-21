
inline
ezString&
lcpp::Reader::context()
{
    return m_pSyntaxCheckResult->cursor.currentPosition().context;
}

inline
const ezString&
lcpp::Reader::context() const
{
    return m_pSyntaxCheckResult->cursor.currentPosition().context;
}

inline
void
lcpp::Reader::context(const ezString& newContext)
{
    m_pSyntaxCheckResult->cursor.currentPosition().context = newContext;
}

template<typename T_Object>
inline
lcpp::Ptr<T_Object>
lcpp::Reader::create()
{
    auto pResult = T_Object::create();
    pResult->sourcePosition(m_pSyntaxCheckResult->cursor.currentPosition());
    return pResult;
}

template<typename T_Object, typename T_Arg0>
inline
lcpp::Ptr<T_Object>
lcpp::Reader::create(T_Arg0 arg0)
{
    auto pResult = T_Object::create(arg0);
    pResult->sourcePosition(m_pSyntaxCheckResult->cursor.currentPosition());
    return pResult;
}

template<typename T_Object, typename T_Arg0, typename T_Arg1>
inline
lcpp::Ptr<T_Object>
lcpp::Reader::create(T_Arg0 arg0, T_Arg1 arg1)
{
    auto pResult = T_Object::create(arg0, arg1);
    pResult->sourcePosition(m_pSyntaxCheckResult->cursor.currentPosition());
    return pResult;
}
