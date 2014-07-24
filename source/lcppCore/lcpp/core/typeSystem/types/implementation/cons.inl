
inline
lcpp::LispCons::LispCons(Ptr<LispObject> car, Ptr<LispObject> cdr) :
    m_pCar(car),
    m_pCdr(cdr)
{
}

inline
lcpp::LispCons::LispCons(const LispCons& toCopy) :
    m_pCar(toCopy.m_pCar),
    m_pCdr(toCopy.m_pCdr)
{
}

inline
lcpp::LispCons::LispCons(LispCons&& toMove) :
    m_pCar(toMove.m_pCar),
    m_pCdr(toMove.m_pCdr)
{
    toMove.m_pCar = LCPP_NIL;
    toMove.m_pCdr = LCPP_NIL;
}

inline
bool
lcpp::LispCons::operator ==(const LispObject& obj) const
{
    if(obj.is<LispCons>())
    {
        return static_cast<const LispCons&>(obj) == *this;
    }
    return false;
}

inline
bool
lcpp::LispCons::operator ==(const LispCons& rhs) const
{
    return *m_pCar == *rhs.m_pCar
        && *m_pCdr == *rhs.m_pCdr;
}

inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispCons::copy() const
{
    auto pCloned = create(LCPP_NIL, LCPP_NIL);
    pCloned->m_pCar = m_pCar->copy();
    pCloned->m_pCdr = m_pCdr->copy();
    return pCloned;
}

inline
const lcpp::Type&
lcpp::LispCons::type() const
{
    return LispCons::typeInfo();
}

inline
lcpp::String
lcpp::LispCons::toString() const
{
    ezStringBuilder builder;
    builder.Append('(');
    toStringHelper(builder);
    builder.Append(')');
    return builder;
}

inline
void
lcpp::LispCons::toStringHelper(ezStringBuilder& builder) const
{
    // car
    // \remark This is more efficient than simply calling m_car->toString()
    if(m_pCar->is<LispCons>())
    {
        builder.Append('(');
        m_pCar.cast<LispCons>()->toStringHelper(builder);
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

    if(m_pCdr->is<LispCons>())
    {
        m_pCdr.cast<LispCons>()->toStringHelper(builder);
    }
    else
    {
        builder.AppendFormat(". %s", m_pCdr->toString().GetData());
    }
}

// set car
inline
void
lcpp::LispCons::car(Ptr<LispObject> pObject)
{
    m_pCar = pObject;
}

// get car
inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispCons::car()
{
    return m_pCar;
}

// set cdr
inline
void
lcpp::LispCons::cdr(Ptr<LispObject> pObject)
{
    m_pCdr = pObject;
}

// get cdr
inline
lcpp::Ptr<lcpp::LispObject>
lcpp::LispCons::cdr()
{
    return m_pCdr;
}


inline
ezResult
lcpp::count(Ptr<LispCons> pRegularList, ezUInt32& out_count)
{
    out_count = 0;

    while(true)
    {
        ++out_count;
        if(isNil(pRegularList->cdr()))
        {
            break;
        }
        if(!pRegularList->cdr()->is<LispCons>())
        {
            return EZ_FAILURE;
        }
        pRegularList = pRegularList->cdr().cast<LispCons>();
    }

    return EZ_SUCCESS;
}

template<typename T_Array>
inline
void
lcpp::toArray(Ptr<LispObject> pObject, T_Array& out_array)
{
    if(!pObject->is<LispCons>())
    {
        return;
    }

    auto pCons = pObject.cast<LispCons>();

    out_array.PushBack(pCons->car());
    toArray(pCons->cdr(), out_array);
}

