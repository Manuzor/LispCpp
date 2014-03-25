inline
ezAllocatorBase& lcpp::SchemeCons::getAllocator()
{
    return *ezFoundation::GetDefaultAllocator();
}

inline
lcpp::SchemeCons::SchemeCons() :
    m_car(nullptr),
    m_cdr(nullptr)
{
    set(m_car, SCHEME_NIL);
    set(m_cdr, SCHEME_NIL);

    EZ_ASSERT(m_car != nullptr, "set failed to initialize m_car properly!");
    EZ_ASSERT(m_cdr != nullptr, "set failed to initialize m_cdr properly!");
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car) :
    m_car(nullptr),
    m_cdr(nullptr)
{
    EZ_ASSERT(&car != nullptr, "Invalid value for car!");

    set(m_car, car);
    set(m_cdr, SCHEME_NIL);

    EZ_ASSERT(m_car != nullptr, "set failed to initialize m_car properly!");
    EZ_ASSERT(m_cdr != nullptr, "set failed to initialize m_cdr properly!");
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeObject& car, const SchemeObject& cdr) :
    m_car(nullptr),
    m_cdr(nullptr)
{
    EZ_ASSERT(&car != nullptr, "Invalid value for car!");
    EZ_ASSERT(&cdr != nullptr, "Invalid value for car!");

    set(m_car, car);
    set(m_cdr, cdr);

    EZ_ASSERT(m_car != nullptr, "set failed to initialize m_car properly!");
    EZ_ASSERT(m_cdr != nullptr, "set failed to initialize m_cdr properly!");
}

inline
lcpp::SchemeCons::SchemeCons(const SchemeCons& copy) :
    m_car(nullptr),
    m_cdr(nullptr)
{
    set(m_car, *copy.m_car);
    set(m_cdr, *copy.m_cdr);

    EZ_ASSERT(m_car != nullptr, "set failed to initialize m_car properly!");
    EZ_ASSERT(m_cdr != nullptr, "set failed to initialize m_cdr properly!");
}

inline
void
lcpp::SchemeCons::operator =(SchemeCons copy)
{
    // The actual copying is done by the copy constructor since \a copy is passed by value.
    std::swap(m_car, copy.m_car);
    std::swap(m_cdr, copy.m_cdr);
}

inline
lcpp::SchemeCons::SchemeCons(SchemeCons&& toMove) :
    m_car(toMove.m_car),
    m_cdr(toMove.m_cdr)
{
    EZ_ASSERT(toMove.m_car != nullptr, "Invalid value for the other's m_car!");
    EZ_ASSERT(toMove.m_cdr != nullptr, "Invalid value for the other's m_cdr!");

    set(toMove.m_car, SCHEME_NIL);
    set(toMove.m_cdr, SCHEME_NIL);
}

inline
lcpp::SchemeCons::~SchemeCons()
{
    if (m_car != nullptr)
    {
        m_car->~SchemeObject();
        getAllocator().Deallocate((void*)m_car);
    }
    if (m_cdr != nullptr)
    {
        m_cdr->~SchemeObject();
        getAllocator().Deallocate((void*)m_cdr);
    }
}

inline
bool
lcpp::SchemeCons::operator ==(const SchemeObject& obj) const
{
    if (obj.is(SchemeTypeInfo<SchemeCons>::type()))
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
    if (m_car->is(SchemeTypeInfo<SchemeCons>::type()))
    {
        builder.Append('(');
        static_cast<const SchemeCons*>(m_car)->toStringHelper(builder);
        builder.Append(')');
    }
    else
    {
        builder.Append(m_car->toString().GetData());
    }

    // cdr
    if (*m_cdr == SCHEME_NIL)
    {
        return;
    }

    builder.Append(' ');

    if (m_cdr->is(SchemeTypeInfo<SchemeCons>::type()))
    {
        static_cast<const SchemeCons*>(m_cdr)->toStringHelper(builder);
    }
    else
    {
        builder.AppendFormat(". %s", m_cdr->toString().GetData());
    }
}

// get car
inline
const lcpp::SchemeObject&
lcpp::SchemeCons::car() const
{
    EZ_ASSERT(m_car != nullptr, "Operating on invalid object!");
    return *m_car;
}

// get cdr
inline
const lcpp::SchemeObject&
lcpp::SchemeCons::cdr() const
{
    EZ_ASSERT(m_cdr != nullptr, "Operating on invalid object!");
    return *m_cdr;
}

inline
void
lcpp::SchemeCons::set(const SchemeObject*& member, const SchemeObject& from)
{
    size_t size = from.type().size;
    void* mem = nullptr;

    if (member == nullptr)
    {
        mem = getAllocator().Allocate(size, from.type().alignment);
    }
    else if (size > member->type().size)
    {
        member->~SchemeObject();
        getAllocator().Deallocate((void*)member);
        mem = getAllocator().Allocate(size, from.type().alignment);
    }
    else
    {
        mem = (void*)member;
    }

    EZ_ASSERT(mem != nullptr, "Out of memory or uninitialized mem!");

    // copy the other objects data
    from.copyTo(mem);
    member = static_cast<SchemeObject*>(mem);
}
