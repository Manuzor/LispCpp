#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispObject;

    class LCPP_API_CORE_CONT LispEnvironment
    {
    public:

        static Ptr<LispObject> createTopLevel(Ptr<LispObject> pName = Ptr<LispObject>());
        static Ptr<LispObject> create(Ptr<LispObject> pParent);
        static Ptr<LispObject> create(Ptr<LispObject> pName, Ptr<LispObject> pParent);

            /// \brief Used for testing purposes only. Creates a LispSymbol internally.
        static Ptr<LispObject> create(const String& name, Ptr<LispObject> pParent);

        static Ptr<LispObject> createCopy(const LispEnvironment& toCopy);

    public:

            /// \brief Adds the key-value pair to this environment.
            /// \remark An existing key will be overwritten.
        void add(Ptr<LispObject> pKey, Ptr<LispObject> pValue);

            /// \brief Tries to set the value of an existing key.
            /// \remark Searches the parent environment if the key can not be found in this environment.
            /// \return EZ_FAILURE if the key does not exist in this or the parent environment.
        ezResult set(Ptr<LispObject> pKey, Ptr<LispObject> pValue);

            /// \brief Tries to get the value of the given key.
            /// \return EZ_FAILURE if the key does not exist in this or the parent environment.
        ezResult get(Ptr<LispObject> pKey, Ptr<LispObject>& out_value);

            /// \brief Checks whether a value exists for \a pKey.
            /// \return \c true if it exists, \c false otherwise.
        bool exists(Ptr<LispObject> pKey);

            /// \brief Gets a Ptr to the parent environment.
        Ptr<      LispObject> parent();

            /// \brief Gets a Ptr to the const parent environment.
        Ptr<const LispObject> parent() const;

        /// \brief Sets the name of this environment.
        void name(Ptr<LispObject> pName);

            /// \brief Gets the pointer to the name of this environment.
        Ptr<      LispObject> name();

            /// \brief Gets the const pointer to the name of this environment.
        Ptr<const LispObject> name() const;

            /// \brief Constructs and returns the fully qualified name of this environment.
            /// If there is no parent, the name will be "/<name>".
            /// If there is a parent, the name will be something like "/<parent0>/<parent1>/.../<name>".
        String qualifiedName() const;

        String toString() const;

        const Type& type() const;

        Ptr<LispObject> copy() const;

    private:

        Ptr<LispObject> m_pParent;
        Ptr<LispObject> m_pName;
        ezHashTable<LispObject*, LispObject*> m_symbols;

    private:

        /// \brief Creats a top-level environment
        explicit LispEnvironment(Ptr<LispObject> pName);

        /// \brief Creats a child environment
        LispEnvironment(Ptr<LispObject> pName, Ptr<LispObject> pParent);

        void qualifiedNameHelper(ezStringBuilder& builder) const;
    };

    bool operator ==(const LispEnvironment& lhs, const LispEnvironment& rhs);

}

#include "lcpp/core/containers/impl/environment.inl"
