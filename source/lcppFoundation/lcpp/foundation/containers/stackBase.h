#pragma once

namespace lcpp
{
    template<typename T_Element, ezUInt32 N_StaticSize>
    class StackBase
    {
    public:

            /// \brief Pops the element from the top of the stack.
        ezResult pop();

            /// \brief Pops the element from the stack at the given index, shifting all elements above it down.
        ezResult pop(ezInt32 relativeIndex);

            /// \brief Pushes \a newElement onto the top of the stack.
        void push(const T_Element& newElement);

            /// \brief Random access into the stack.
            ///
            /// The index can be relative:
            /// size = 7, top = 6 => size - 1
            /// contents:       [ a,  b,  c,  d,  e,  f,  g]
            /// absolute index: [ 0,  1,  2,  3,  4,  5,  6]
            /// relative index: [-7, -6, -5, -4, -3, -2, -1]
            ///
            /// This means that -1 is always the top of the stack.
            /// \return LCPP_NIL if the index is out of bounds.
        T_Element& get(ezInt32 relativeIndex);
        const T_Element& get(ezInt32 relativeIndex) const;

            /// \brief The absolute index of the top of the stack.
            /// \remark Is basically size() - 1
        ezUInt32 top() const;

            /// \brief The current size of the stack.
        ezUInt32 size() const;

            /// \brief Whether this stack is empty or not.
        bool isEmpty() const;

        void clear();

        void setNilElement(const T_Element& newNilElement);
        const T_Element& getNilElement() const;
        T_Element& getNilElement();

        ezUInt32 convertToAbsolute(ezInt32 relativeIndex) const;

    private:

        Ptr<ezAllocatorBase> m_pAllocator;

        ezHybridArray<T_Element, N_StaticSize> m_stack;

        T_Element m_nilElement;

    protected:

        StackBase(Ptr<ezAllocatorBase> pAllocator);
    };
}

#include "lcpp/foundation/containers/implementation/stackBase.inl"
