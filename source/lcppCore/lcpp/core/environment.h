#pragma once

namespace lcpp
{
    class LispObject;
    class LispSymbol;

    class LCPP_CORE_API Environment
    {
    public:

        static Ptr<Environment> createTopLevel(Ptr<LispSymbol> pName = Ptr<LispSymbol>());
        static Ptr<Environment> create(Ptr<Environment> pParent);
        static Ptr<Environment> create(Ptr<LispSymbol> pName, Ptr<Environment> pParent);

            /// \brief Used for testing purposes only. Creates a LispSymbol internally.
        static Ptr<Environment> create(const ezString& name, Ptr<Environment> pParent);

    public:

            /// \brief Adds the key-value pair to this environment.
            /// \remark An existing key will be overwritten.
        void add(Ptr<LispSymbol> pKey, Ptr<LispObject> pValue);

            /// \brief Tries to set the value of an existing key.
            /// \remark Searches the parent environment if the key can not be found in this environment.
            /// \return EZ_FAILURE if the key does not exist in this or the parent environment.
        ezResult set(Ptr<LispSymbol> pKey, Ptr<LispObject> pValue);

            /// \brief Tries to get the value of the given key.
            /// \return EZ_FAILURE if the key does not exist in this or the parent environment.
        ezResult get(Ptr<LispSymbol> pKey, Ptr<LispObject>& out_value);

            /// \brief Checks whether a value exists for \a pKey.
            /// \return \c true if it exists, \c false otherwise.
        bool exists(Ptr<LispSymbol> pKey);

            /// \brief Gets a Ptr to the parent environment.
        Ptr<      Environment> parent();

            /// \brief Gets a Ptr to the const parent environment.
        Ptr<const Environment> parent() const;

        /// \brief Sets the name of this environment.
        void name(Ptr<LispSymbol> pName);

            /// \brief Gets the pointer to the name of this environment.
        Ptr<      LispSymbol> name();

            /// \brief Gets the const pointer to the name of this environment.
        Ptr<const LispSymbol> name() const;

            /// \brief Constructs and returns the fully qualified name of this environment.
            /// If there is no parent, the name will be "/<name>".
            /// If there is a parent, the name will be something like "/<parent0>/<parent1>/.../<name>".
        ezString qualifiedName() const;

    private:

        Ptr<Environment> m_pParent;
        Ptr<LispSymbol> m_pName;
        ezHashTable<LispSymbol*, LispObject*> m_symbols;

    private:

        /// \brief Creats a top-level environment
        explicit Environment(Ptr<LispSymbol> pName);

        /// \brief Creats a child environment
        Environment(Ptr<LispSymbol> pName, Ptr<Environment> pParent);

        void qualifiedNameHelper(ezStringBuilder& builder) const;
    };


}

#include "lcpp/core/implementation/environment.inl"
