#pragma once

namespace lcpp
{
    class SchemeObject;
    class SchemeSymbol;

    class Environment
    {
    public:

        static Environment createTopLevelInstance();

        /// \brief Creats a top-level environment
        Environment(const ezString& name, Ptr<ezAllocatorBase> pAllocator);

        /// \brief Creats a child environment
        Environment(const ezString& name, Ptr<Environment> pParent);

        /// \brief Adds the key-value pair to this environment.
        /// \remark An existing key will be overwritten.
        void add(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue);

        /// \brief Tries to set the value of an existing key.
        /// \remark Searches the parent environment if the key can not be found in this environment.
        /// \return EZ_FAILURE if the key does not exist in this or the parent environment.
        ezResult set(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject> pValue);

        /// \brief Tries to get the value of the given key.
        /// \return EZ_FAILURE if the key does not exist in this or the parent environment.
        ezResult get(Ptr<SchemeSymbol> pKey, Ptr<SchemeObject>& out_value);

        /// \brief Checks whether a value exists for \a pKey.
        /// \return \c true if it exists, \c false otherwise.
        bool exists(Ptr<SchemeSymbol> pKey);

        /// \brief Gets a Ptr to the parent environment.
        Ptr<      Environment> parent();

        /// \brief Gets a Ptr to the const parent environment.
        Ptr<const Environment> parent() const;

        /// \brief Gets the a ref to the name of this environment.
        ezString& name();
        /// \brief Gets the a const ref  to the name of this environment.
        const ezString& name() const;
        /// \brief Constructs and returns the fully qualified name of this environment.
        /// If there is no parent, the name will be "/<name>".
        /// If there is a parent, the name will be something like "/<parent0>/<parent1>/.../<name>".
        ezString qualifiedName() const;

    private:
        Ptr<ezAllocatorBase> m_pAllocator;
        Ptr<Environment> m_pParent;
        ezString m_name;
        ezHashTable<SchemeSymbol*, SchemeObject*> m_symbols;

        void qualifiedNameHelper(ezStringBuilder& builder) const;
    };


}

#include "lcpp/core/implementation/environment.inl"
