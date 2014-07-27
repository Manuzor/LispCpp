
namespace lcpp
{
    namespace env
    {
        inline
        void Data::initialize(Ptr<LispObject> pName,
                              Ptr<LispObject> pParent,
                              Ptr<HashTable> pTable)
        {
            m_pName = pName.get();
            m_pParent = pParent.get();
            m_pTable = pTable.get();
        }

        inline
        void Data::destroy()
        {
            m_pTable = nullptr;
            m_pParent = nullptr;
            m_pName = nullptr;
        }

    };
}
