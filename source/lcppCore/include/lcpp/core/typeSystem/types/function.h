#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class SchemeRuntime;
    class Environment;
    class SchemeCons;

    class LCPP_CORE_API SchemeFunction : public SchemeObject
    {
    public:

        SchemeFunction(Ptr<SchemeRuntime> pRuntime,
                       const ezString& name,
                       Ptr<Environment> pEnv);

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeFunction& rhs) const;

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual ezString dump() const = 0;

        virtual Ptr<SchemeObject> call(Ptr<SchemeObject> pArgList) = 0;

        ezString& name();
        const ezString& name() const;

        Ptr<Environment> env();
        Ptr<const Environment> env() const;

    protected:
        ezString m_name;
        Ptr<SchemeRuntime> m_pRuntime;
        Ptr<Environment> m_pEnv;
    };

    template<>
    struct TypeInfo< SchemeFunction >
    {
        inline static const Type& type()
        {
            static_assert(Type::Version == 2,
                          "Type version was updated. Adjust your implementation accordingly!");
            static auto theType = Type::create(
                "SchemeFunction",
                MemoryInfo(sizeof(SchemeFunction), sizeof(SchemeFunction))
                );
            return theType;
        }
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeFunctionBuiltin : public SchemeFunction
    {
        friend class TypeFactory;
    public:
        typedef Ptr<SchemeObject>(*ExecutorPtr_t)(Ptr<SchemeRuntime>, Ptr<Environment>, Ptr<SchemeObject>);

        SchemeFunctionBuiltin(Ptr<SchemeRuntime> pRuntime,
                              const ezString& name,
                              Ptr<Environment> pEnv,
                              ExecutorPtr_t pExec);

        virtual Ptr<SchemeObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual ezString toString() const LCPP_OVERRIDE;
        virtual ezString dump() const LCPP_OVERRIDE;

        virtual Ptr<SchemeObject> call(Ptr<SchemeObject> pArgList) LCPP_OVERRIDE;

    private:
        ExecutorPtr_t m_pExec;
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeFunctionUserDefined : public SchemeFunction
    {
        friend class TypeFactory;
    public:

        SchemeFunctionUserDefined(Ptr<SchemeRuntime> pRuntime,
                                  Ptr<Environment> pEnv,
                                  Ptr<SchemeObject> pArgNameList,
                                  Ptr<SchemeCons> pBody);

        virtual Ptr<SchemeObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual ezString toString() const LCPP_OVERRIDE;
        virtual ezString dump() const LCPP_OVERRIDE;

        virtual Ptr<SchemeObject> call(Ptr<SchemeObject> pArgList) LCPP_OVERRIDE;

    private:
        Ptr<SchemeObject> m_pArgNameList;
        ezUInt32 m_numArgs;
        Ptr<SchemeCons> m_pBody;
#ifdef _DEBUG
        ezString m_dump;
#endif // _DEBUG

        void processArguments(Ptr<SchemeObject> pArgs);
        /// \return \c true if \a pArgs is a cons, \c false if it is nil.
        ///         In other words, this returns wether further processing needs to be done.
        bool checkArgumentCount(Ptr<SchemeObject> pArgs);
    };
}

#include "lcpp/core/typeSystem/types/implementation/function.inl"
