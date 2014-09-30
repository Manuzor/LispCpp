
namespace lcpp
{
    namespace file
    {
        EZ_FORCE_INLINE
        ezOSFile& Data::getFile()
        {
            return reinterpret_cast<ezOSFile&>(m_file);
        }

        EZ_FORCE_INLINE
        const ezOSFile& Data::getFile() const
        {
            return reinterpret_cast<const ezOSFile&>(m_file);
        }

        EZ_FORCE_INLINE
        Ptr<LispObject> Data::getFileName()
        {
            return reinterpret_cast<StackPtr<LispObject>&>(m_pFileName);
        }

        EZ_FORCE_INLINE
        void Data::setFileName(StackPtr<LispObject> pValue)
        {
            reinterpret_cast<StackPtr<LispObject>&>(m_pFileName) = pValue;
        }
    }
}
