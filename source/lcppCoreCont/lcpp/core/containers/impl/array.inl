
namespace lcpp
{
    template<typename T>
    EZ_FORCE_INLINE
    Array<T>::Array() :
        m_pData(nullptr),
        m_uiSize(0)
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    Array<T>::Array(T* ptr, std::size_t uiSize) :
        m_pData(ptr),
        m_uiSize(uiSize)
    {
        if (m_uiSize == 0)
        {
            m_pData = nullptr;
        }
    }
    
    template<typename T>
    template<std::size_t N>
    EZ_FORCE_INLINE
    Array<T>::Array(T (&staticArray)[N]) :
        m_pData(staticArray),
        m_uiSize(N)
    {
    }

    template<typename T>
    EZ_FORCE_INLINE
    Array<T>::~Array()
    {
        reset();
    }

    template<typename T>
    EZ_FORCE_INLINE
    T& Array<T>::operator[](std::size_t index)
    {
        EZ_ASSERT(m_pData, "No data to index!");
        EZ_ASSERT(index < m_uiSize, "Index out of bounds!");
        return m_pData[index];
    }

    template<typename T>
    EZ_FORCE_INLINE
    const T& Array<T>::operator[](std::size_t index) const
    {
        EZ_ASSERT(m_pData, "No data to index!");
        EZ_ASSERT(index < m_uiSize, "Index out of bounds!");
        return m_pData[index];
    }
    
    template<typename T>
    template<std::size_t N>
    EZ_FORCE_INLINE
    void Array<T>::operator=(T(&staticArray)[N])
    {
        assign(staticArray);
    }

    template<typename T>
    template<std::size_t N>
    void lcpp::Array<T>::assign(T(&staticArray)[N])
    {
        m_pData = staticArray;
        m_uiSize = N;
    }

    template<typename T>
    void Array<T>::assign(T* pData, std::size_t uiSize)
    {
        m_pData = pData;
        m_uiSize = uiSize;
    }

    template<typename T>
    EZ_FORCE_INLINE
    T* Array<T>::getData()
    {
        return m_pData;
    }

    template<typename T>
    EZ_FORCE_INLINE
    const T* Array<T>::getData() const
    {
        return m_pData;
    }

    template<typename T>
    EZ_FORCE_INLINE
    std::size_t Array<T>::getSize() const
    {
        return m_uiSize;
    }

    template<typename T>
    EZ_FORCE_INLINE
    Array<T> Array<T>::getSubArray(std::size_t uiFrom) const
    {
        EZ_ASSERT(uiFrom < m_uiSize, "Index out of bounds");

        return Array(m_pData + uiFrom, m_uiSize - uiFrom);
    }

    template<typename T>
    EZ_FORCE_INLINE
    Array<T> Array<T>::getSubArray(std::size_t uiFrom, std::size_t uiCount) const
    {
        EZ_ASSERT(uiFrom + uiCount <= m_uiSize, "uiCount too high.");

        return Array(m_pData + uiFrom, uiCount);
    }

    template<typename T>
    EZ_FORCE_INLINE
    Array<T> Array<T>::operator()(std::size_t uiFrom) const
    {
        return getSubArray(uiFrom);
    }

    template<typename T>
    EZ_FORCE_INLINE
    Array<T> Array<T>::operator()(std::size_t uiFrom, std::size_t uiCount) const
    {
        return getSubArray(uiFrom, uiCount);
    }

    template<typename T>
    EZ_FORCE_INLINE
    void Array<T>::reset()
    {
        m_pData = nullptr;
        m_uiSize = 0;
    }

    //////////////////////////////////////////////////////////////////////////

    template<typename T>
    bool operator==(const Array<T>& lhs, const Array<T>& rhs)
    {
        return lhs.getData() == rhs.getData() && lhs.getSize() == rhs.getSize();
    }

    template<typename T>
    bool operator!=(const Array<T>& lhs, const Array<T>& rhs)
    {
        return lhs.getData() != rhs.getData() || lhs.getSize() != rhs.getSize();
    }

}
