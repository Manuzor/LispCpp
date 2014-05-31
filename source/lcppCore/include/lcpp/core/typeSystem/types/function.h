#pragma once
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/environment.h"
#include "lcpp/core/typeSystem/types/cons.h"

namespace lcpp
{
    class IEvaluator;

    class LCPP_CORE_API SchemeFunction : public SchemeExtend<SchemeFunction, SchemeObject>
    {
    public:

        SchemeFunction(const ezString& name, Ptr<Environment> pParentEnv);

        virtual bool operator==(const SchemeObject& obj) const LCPP_OVERRIDE;
        bool operator==(const SchemeFunction& rhs) const;

        virtual Ptr<SchemeObject> call(Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList) = 0;

        ezString& name();
        const ezString& name() const;

        Ptr<Environment> env();
        Ptr<const Environment> env() const;

    protected:
        ezString m_name;
        Environment m_env;
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
        typedef std::function<Ptr<SchemeObject>(Ptr<Environment>, Ptr<IEvaluator>, Ptr<SchemeObject>)> Executor;

        SchemeFunctionBuiltin(const ezString& name, Ptr<Environment> pParentEnv, Executor exec);

        virtual ezString toString() const LCPP_OVERRIDE;

        Ptr<SchemeObject> call(Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList);

    private:
        Executor m_exec;
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API SchemeFunctionUserDefined : public SchemeFunction
    {
        friend class TypeFactory;
    public:

        SchemeFunctionUserDefined(const ezString& name, Ptr<Environment> pParentEnv, Ptr<SchemeCons> pBody);

        virtual ezString toString() const LCPP_OVERRIDE;

        Ptr<SchemeObject> call(Ptr<IEvaluator> pEvaluator, Ptr<SchemeObject> pArgList);

    private:
        Ptr<SchemeCons> m_pBody;
    };
}

#include "lcpp/core/typeSystem/types/implementation/function.inl"
