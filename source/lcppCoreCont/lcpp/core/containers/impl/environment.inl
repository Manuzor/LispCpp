
namespace lcpp
{
    inline
    Ptr<LispObject>
    LispEnvironment::parent()
    {
        return m_pParent;
    }

    inline
        Ptr<const LispObject>
    LispEnvironment::parent() const
    {
        return m_pParent;
    }

    inline
    void
    LispEnvironment::name(Ptr<LispObject> pName)
    {
        m_pName = pName;
    }

    inline
    Ptr<const LispObject>
    LispEnvironment::name() const
    {
        return m_pName;
    }

    inline
    Ptr<LispObject>
    LispEnvironment::name()
    {
        return m_pName;
    }

    inline
    String
    LispEnvironment::qualifiedName() const
    {
        if(!m_pParent) { return "/"; }

        ezStringBuilder builder;
        qualifiedNameHelper(builder);
        return builder;
    }

    inline
    String
    LispEnvironment::toString() const
    {
        return qualifiedName();
    }

    inline
    const Type&
    LispEnvironment::type() const
    {
        LCPP_NOT_IMPLEMENTED;
    }

    inline
    Ptr<LispObject>
    LispEnvironment::copy() const
    {
        return LispEnvironment::createCopy(*this);
    }

    inline
    bool
    operator==(const LispEnvironment& lhs, const LispEnvironment& rhs)
    {
        return lhs.qualifiedName().IsEqual(rhs.qualifiedName().GetData());
    }
}

