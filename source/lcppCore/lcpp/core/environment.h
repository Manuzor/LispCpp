#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispObject;
    class LispSymbol;

    class LCPP_API_CORE LispEnvironment :
        public LispObject
    {
    public:

        static Ptr<LispEnvironment> createTopLevel(Ptr<LispSymbol> pName = Ptr<LispSymbol>());
        static Ptr<LispEnvironment> create(Ptr<LispEnvironment> pParent);
        static Ptr<LispEnvironment> create(Ptr<LispSymbol> pName, Ptr<LispEnvironment> pParent);

            /// \brief Used for testing purposes only. Creates a LispSymbol internally.
        static Ptr<LispEnvironment> create(const String& name, Ptr<LispEnvironment> pParent);

        static Ptr<LispEnvironment> createCopy(const LispEnvironment& toCopy);

        static const Type& typeInfo();

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
        Ptr<      LispEnvironment> parent();

            /// \brief Gets a Ptr to the const parent environment.
        Ptr<const LispEnvironment> parent() const;

        /// \brief Sets the name of this environment.
        void name(Ptr<LispSymbol> pName);

            /// \brief Gets the pointer to the name of this environment.
        Ptr<      LispSymbol> name();

            /// \brief Gets the const pointer to the name of this environment.
        Ptr<const LispSymbol> name() const;

            /// \brief Constructs and returns the fully qualified name of this environment.
            /// If there is no parent, the name will be "/<name>".
            /// If there is a parent, the name will be something like "/<parent0>/<parent1>/.../<name>".
        String qualifiedName() const;

    private:

        Ptr<LispEnvironment> m_pParent;
        Ptr<LispSymbol> m_pName;
        ezHashTable<LispSymbol*, LispObject*> m_symbols;

    private:

        /// \brief Creats a top-level environment
        explicit LispEnvironment(Ptr<LispSymbol> pName);

        /// \brief Creats a child environment
        LispEnvironment(Ptr<LispSymbol> pName, Ptr<LispEnvironment> pParent);

        void qualifiedNameHelper(ezStringBuilder& builder) const;

        virtual bool operator ==(const LispObject& other) const override
        {
            throw std::logic_error("The method or operation is not implemented.");
        }

        virtual String toString() const override;

        virtual const Type& type() const override;

        virtual Ptr<LispObject> copy() const override;




    };

    bool operator ==(const LispEnvironment& lhs, const LispEnvironment& rhs);

}

#include "lcpp/core/implementation/environment.inl"
