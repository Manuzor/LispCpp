
namespace lcpp
{
    TypeTags::TypeTags() :
        m_flags(None)
    {
    }

    TypeTags::TypeTags(ezUInt32 flags) :
        m_flags(flags)
    {
    }

    bool TypeTags::isCallable() const
    {
        return (m_flags & Callable) != 0;
    }

    bool TypeTags::isNameable() const
    {
        return (m_flags & Nameable) != 0;
    }

}
