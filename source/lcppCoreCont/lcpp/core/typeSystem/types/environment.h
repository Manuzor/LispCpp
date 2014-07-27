#pragma once

namespace lcpp
{
    class LispObject;
    class MetaInfo;

    namespace env
    {
        LCPP_API_CORE_CONT const MetaInfo& metaInfo();

        LCPP_API_CORE_CONT Ptr<LispObject> create(Ptr<LispObject> pName, Ptr<LispObject> pParent);

        LCPP_API_CORE_CONT Ptr<LispObject> createTopLevel(Ptr<LispObject> pName);

        LCPP_API_CORE_CONT Ptr<LispObject> createAnonymous(Ptr<LispObject> pParent);

        //////////////////////////////////////////////////////////////////////////
        
        class Data
        {
        public:

            void initialize(Ptr<LispObject> pName,
                            Ptr<LispObject> pParent,
                            Ptr<HashTable> pTable);
            void destroy();

        public:

            LispObject* m_pName;

            LispObject* m_pParent;

            HashTable* m_pTable;

        };

        //////////////////////////////////////////////////////////////////////////

        /// \brief Adds the key-value pair at \a indexOfKeyValuePair to the env at \a indexOfKeyValuePair \c - \c 2
        ///
        /// Example:
        /// \a indexOfKeyValuePair = -1 => value (object to add to the env)
        /// \a indexOfKeyValuePair = -2 => key (symbol-key of the object to add)
        /// \a indexOfKeyValuePair = -3 => env (the environment object to add the key-value pair to.)
        ///
        /// This means that you have to push in this order:
        /// push(env); push(key); push(value);
        ///
        /// \remark This operation pops these three values from the stack!
        LCPP_API_CORE_CONT void add(ezInt32 indexOfKeyValuePair = -1);
    };
}

#include "lcpp/core/typeSystem/types/impl/environment.inl"
