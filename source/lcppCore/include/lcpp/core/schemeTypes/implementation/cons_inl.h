
inline
lcpp::SchemeCons::SchemeCons() :
    m_car(&SCHEME_NIL),
    m_cdr(&SCHEME_NIL)
{
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car) :
    m_car(&SCHEME_NIL),
    m_cdr(&SCHEME_NIL)
{
    EZ_ASSERT(&car != nullptr, "Invalid value for car!");
    EZ_ASSERT(&car != this, "Cannot include myself as car!");

    this->car(car);
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car, const SchemeObject& cdr) :
    m_car(&SCHEME_NIL),
    m_cdr(&SCHEME_NIL)
{
    EZ_ASSERT(&car != nullptr, "Invalid value for car!");
    EZ_ASSERT(&car != this, "Cannot include myself as car!");

    EZ_ASSERT(&cdr != nullptr, "Invalid value for car!");
    EZ_ASSERT(&cdr != this, "Cannot include myself as cdr!");

    this->car(car);
    this->cdr(cdr);
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeCons& copy) :
    m_car(&SCHEME_NIL),
    m_cdr(&SCHEME_NIL)
{
    this->car(*copy.m_car);
    this->cdr(*copy.m_cdr);
}

inline
lcpp::SchemeCons::~SchemeCons()
{
    if (m_car != &SCHEME_NIL)
    {
        ezFoundation::GetDefaultAllocator()->Deallocate(const_cast<SchemeObject*>(m_car));
    }
    if (m_cdr != &SCHEME_NIL)
    {
        ezFoundation::GetDefaultAllocator()->Deallocate(const_cast<SchemeObject*>(m_cdr));
    }
}

inline
void
lcpp::SchemeCons::operator =(SchemeCons copy)
{
    std::swap(m_car, copy.m_car);
    std::swap(m_cdr, copy.m_cdr);
}

inline
lcpp::SchemeCons::SchemeCons(SchemeCons&& toMove) :
    m_car(toMove.m_car),
    m_cdr(toMove.m_cdr)
{
    toMove.m_car = toMove.m_cdr = &SCHEME_NIL;
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeObject& obj) const
{
    if (obj.is(SchemeType::Cons))
    {
        return static_cast<const SchemeCons&>(obj) == *this;
    }
    return false;
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeCons& rhs) const
{
   return car() == rhs.car()
       && cdr() == rhs.cdr();
}

inline
ezString
lcpp::SchemeCons::toString() const
{
    ezStringBuilder builder;
    builder.AppendFormat("(%s %s)", m_car->toString().GetData(), m_cdr->toString().GetData());
    return builder.GetData();
}

// get car
inline
const lcpp::SchemeObject&
lcpp::SchemeCons::car() const
{
    EZ_ASSERT(m_car != nullptr, "Operating on invalid object!");
    return *m_car;
}

// set car
inline
void
lcpp::SchemeCons::car(const SchemeObject& value)
{
    EZ_ASSERT(&value != nullptr, "Invalid value for car!");
    if (value == SCHEME_NIL) m_car = &SCHEME_NIL;
    else if (&value != m_car) set(m_car, value);
}

// get cdr
inline
const lcpp::SchemeObject&
lcpp::SchemeCons::cdr() const
{
    EZ_ASSERT(m_cdr != nullptr, "Operating on invalid object!");
    return m_cdr == nullptr ? SCHEME_NIL : *m_cdr;
}

// set cdr
inline
void
lcpp::SchemeCons::cdr(const SchemeObject& value)
{
    EZ_ASSERT(&value != nullptr, "Invalid value for cdr!");
    if (value == SCHEME_NIL) m_cdr = &SCHEME_NIL;
    else if (&value != m_cdr) set(m_cdr, value);
}

inline
void
lcpp::SchemeCons::set(const SchemeObject*& member, const SchemeObject& from)
{
    EZ_ASSERT(from != SCHEME_NIL, "The argument 'from' is not allowed to be SchemeNil here!");

    size_t size = from.size();
    void* mem = nullptr;

    bool needNewAllocation = member == &SCHEME_NIL || size > member->size();
    bool needDeallocation = needNewAllocation && member != &SCHEME_NIL;

    if (needDeallocation)
    {
        EZ_ASSERT(needNewAllocation, "Prevented attempt to allocate new memory without relasing the old memory first!");

        ezFoundation::GetDefaultAllocator()->Deallocate(const_cast<SchemeObject*>(member));
    }
    else if (member != &SCHEME_NIL)
    {
        // Call the destructor...
        member->~SchemeObject();
        // ...and re-use the memory
        mem = const_cast<SchemeObject*>(member);
    }

    if (needNewAllocation)
    {
        mem = ezFoundation::GetDefaultAllocator()->Allocate(size, from.alignment());
    }

    // copy the other objects data
    memcpy(mem, &from, size);

    member = static_cast<SchemeObject*>(mem);
}
