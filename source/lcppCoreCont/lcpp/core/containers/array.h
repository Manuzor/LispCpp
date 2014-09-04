#pragma once

namespace lcpp
{
    template<typename T>
    class Array
    {
    public:

        Array();

        Array(T* ptr, std::size_t uiSize);
        
        template<std::size_t N>
        explicit Array(T (&staticArray)[N]);

        ~Array();

        T& operator[](std::size_t index);
        const T& operator[](std::size_t index) const;
        
        template<std::size_t N>
        void operator=(T (&staticArray)[N]);
        
        template<std::size_t N>
        void assign(T (&staticArray)[N]);

        void assign(T* pData, std::size_t uiSize);

        /// \brief Gets the raw data this Array wraps.
        /// \remark You should only use this when you know what you're doing! You will lose all bounds checking, etc.
        T* getData();

        /// \brief Gets the raw data this Array wraps.
        /// \remark You should only use this when you know what you're doing! You will lose all bounds checking, etc.
        const T* getData() const;

        std::size_t getSize() const;

        /// \brief Creates a new Array instance which is a view on a part of this Array's data.
        ///
        /// \param uiFrom Index from which the view will start.
        /// \param uiCount How many elements for the new array to contain.
        Array getSubArray(std::size_t uiFrom) const;
        

        /// \brief Creates a new Array instance which is a view on a part of this Array's data.
        ///
        /// \param uiFrom Index from which the view will start.
        /// \param uiCount How many elements for the new array to contain.
        Array getSubArray(std::size_t uiFrom, std::size_t uiCount) const;

        /// \brief More convenient use of getSubArray
        /// \see getSubArray
        Array operator()(std::size_t uiFrom) const;

        /// \brief More convenient use of getSubArray
        /// \see getSubArray
        Array operator()(std::size_t uiFrom, std::size_t uiCount) const;

        void reset();

    private:

        T* m_pData;
        std::size_t m_uiSize;
    };

    template<typename T>
    bool operator==(const Array<T>& lhs, const Array<T>& rhs);

    template<typename T>
    bool operator!=(const Array<T>& lhs, const Array<T>& rhs);
}

#include "lcpp/core/containers/impl/array.inl"
