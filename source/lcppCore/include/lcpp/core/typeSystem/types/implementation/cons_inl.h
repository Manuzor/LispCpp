
inline
lcpp::SchemeCons::SchemeCons(Ptr<SchemeObject> car, Ptr<SchemeObject> cdr) :
    m_pCar(car),
    m_pCdr(cdr)
{
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeCons& toCopy) :
    m_pCar(toCopy.m_pCar),
    m_pCdr(toCopy.m_pCdr)
{
}

inline
lcpp::SchemeCons::SchemeCons(SchemeCons&& toMove) :
    m_pCar(toMove.m_pCar),
    m_pCdr(toMove.m_pCdr)
{
    toMove.m_pCar = &SCHEME_NIL;
    toMove.m_pCdr = &SCHEME_NIL;
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeObject& obj) const
{
    if(obj.is<SchemeCons>())
    {
        return static_cast<const SchemeCons&>(obj) == *this;
    }
    return false;
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeCons& rhs) const
{
    return *m_pCar == *rhs.m_pCar
        && *m_pCdr == *rhs.m_pCdr;
}

inline
ezString
lcpp::SchemeCons::toString() const
{
    ezStringBuilder builder;
    builder.Append('(');
    toStringHelper(builder);
    builder.Append(')');
    return builder;
}

inline
void
lcpp::SchemeCons::toStringHelper(ezStringBuilder& builder) const
{
    // car
    // \remark This is more efficient than simply calling m_car->toString()
    if(m_pCar->is<SchemeCons>())
    {
        builder.Append('(');
        m_pCar.cast<SchemeCons>()->toStringHelper(builder);
        builder.Append(')');
    }
    else
    {
        builder.Append(m_pCar->toString().GetData());
    }

    // cdr
    if(isNil(m_pCdr))
    {
        return;
    }

    builder.Append(' ');

    if(m_pCdr->is<SchemeCons>())
    {
        m_pCdr.cast<SchemeCons>()->toStringHelper(builder);
    }
    else
    {
        builder.AppendFormat(". %s", m_pCdr->toString().GetData());
    }
}

// set car
inline
void
lcpp::SchemeCons::car(Ptr<SchemeObject> pObject)
{
    m_pCar = pObject;
}

// get car
inline
lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeCons::car()
{
    return m_pCar;
}

// set cdr
inline
void
lcpp::SchemeCons::cdr(Ptr<SchemeObject> pObject)
{
    m_pCdr = pObject;
}

// get cdr
inline
lcpp::Ptr<lcpp::SchemeObject>
lcpp::SchemeCons::cdr()
{
    return m_pCdr;
}


inline
ezResult
lcpp::count(Ptr<SchemeCons> pRegularList, ezUInt32& out_count)
{
    out_count = 0;

    while(true)
    {
        ++out_count;
        if(isNil(pRegularList->cdr()))
        {
            break;
        }
        if(!pRegularList->cdr()->is<SchemeCons>())
        {
            return EZ_FAILURE;
        }
        pRegularList = pRegularList->cdr().cast<SchemeCons>();
    }

    return EZ_SUCCESS;
}
