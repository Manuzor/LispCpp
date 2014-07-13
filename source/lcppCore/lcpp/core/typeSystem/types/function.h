#pragma once
#include "lcpp/core/typeSystem/object.h"

namespace lcpp
{
    class LispRuntime;
    class Environment;
    class LispCons;

    class LCPP_CORE_API LispFunction : public LispObject
    {
    public:

        static Ptr<LispFunction> create(const ezString& name,
                                        Ptr<Environment> pEnv);

        static const Type& typeInfo();

    public:

        LispFunction(Ptr<LispRuntime> pRuntime,
                       const ezString& name,
                       Ptr<Environment> pEnv);

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        bool operator==(const LispFunction& rhs) const;

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual ezString dump() const = 0;

        virtual Ptr<LispObject> call(Ptr<LispObject> pArgList) = 0;

        void name(const ezString& newName);
        ezString& name();
        const ezString& name() const;

        Ptr<Environment> env();
        Ptr<const Environment> env() const;

    protected:
        ezString m_name;
        Ptr<LispRuntime> m_pRuntime;
        Ptr<Environment> m_pEnv;
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API LispFunctionBuiltin : public LispFunction
    {
        friend class TypeFactory;
    public:
        typedef Ptr<LispObject>(*ExecutorPtr_t)(Ptr<LispRuntime>, Ptr<Environment>, Ptr<LispObject>);

        LispFunctionBuiltin(Ptr<LispRuntime> pRuntime,
                              const ezString& name,
                              Ptr<Environment> pEnv,
                              ExecutorPtr_t pExec);

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual ezString toString() const LCPP_OVERRIDE;
        virtual ezString dump() const LCPP_OVERRIDE;

        virtual Ptr<LispObject> call(Ptr<LispObject> pArgList) LCPP_OVERRIDE;

    private:
        ExecutorPtr_t m_pExec;
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API LispFunctionUserDefined : public LispFunction
    {
        friend class TypeFactory;
    public:

        LispFunctionUserDefined(Ptr<LispRuntime> pRuntime,
                                  Ptr<Environment> pEnv,
                                  Ptr<LispObject> pArgNameList,
                                  Ptr<LispCons> pBody);

        virtual Ptr<LispObject> clone(ezAllocatorBase* pAllocator) const LCPP_OVERRIDE;

        virtual ezString toString() const LCPP_OVERRIDE;
        virtual ezString dump() const LCPP_OVERRIDE;

        virtual Ptr<LispObject> call(Ptr<LispObject> pArgList) LCPP_OVERRIDE;

    private:
        Ptr<LispObject> m_pArgNameList;
        ezUInt32 m_numArgs;
        Ptr<LispCons> m_pBody;
#ifdef _DEBUG
        ezString m_dump;
#endif // _DEBUG

        void processArguments(Ptr<LispObject> pArgs);
        /// \return \c true if \a pArgs is a cons, \c false if it is nil.
        ///         In other words, this returns wether further processing needs to be done.
        bool checkArgumentCount(Ptr<LispObject> pArgs);
    };
}

#include "lcpp/core/typeSystem/types/implementation/function.inl"
