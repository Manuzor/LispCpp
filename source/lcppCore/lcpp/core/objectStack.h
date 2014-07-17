#pragma once

namespace lcpp
{
    class LispRuntime;
    class LispObject;

    class LCPP_CORE_API LispObjectStack
    {
        friend LispRuntime;
    public:

        static Ptr<LispObjectStack> create(const ezString& name);

    public:

            /// \brief Pops the element from the stack at the given index.
        ezResult pop();

            /// \brief Pushes \a pObject onto the top of the stack.
        void push(Ptr<LispObject> pObject);

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
        Ptr<LispObject> get(ezInt32 relativeIndex);

            /// \brief The absolute index of the top of the stack.
            /// \remark Is basically size() - 1
        ezUInt32 top();

            /// \brief The current size of the stack.
        ezUInt32 size();

            /// \brief Whether this stack is empty or not.
        bool isEmpty();

        void clear();

        ezString& name();
        const ezString& name() const;
        void name(const ezString& name);

    private:

        ezString m_name;

        ezHybridArray<Ptr<LispObject>, 64> m_stack;

    private:

        LispObjectStack(const ezString& name);

        ezUInt32 convertToAbsolute(ezInt32 relativeIndex);
    };
}

#include "lcpp/core/implementation/objectStack.inl"
