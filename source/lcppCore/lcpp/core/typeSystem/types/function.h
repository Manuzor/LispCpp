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

        static const Type& typeInfo();

    public:

        LispFunction(Ptr<Environment> pEnv);

        virtual bool operator==(const LispObject& obj) const LCPP_OVERRIDE;
        bool operator==(const LispFunction& rhs) const;

        virtual const Type& type() const LCPP_OVERRIDE;

        virtual ezString dump() const = 0;

        virtual Ptr<LispObject> call(Ptr<LispObject> pArgList) = 0;

        void name(Ptr<LispSymbol> pNewName);
        Ptr<const LispSymbol> name() const;
        Ptr<LispSymbol> name();

        Ptr<Environment> env();
        Ptr<const Environment> env() const;

    protected:
        Ptr<LispSymbol> m_pName;
        Ptr<Environment> m_pEnv;
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API LispFunction_BuiltIn : public LispFunction
    {
    public:

        typedef Ptr<LispObject>(*ExecutorPtr_t)(Ptr<Environment>, Ptr<LispObject>);

    public:

        static Ptr<LispFunction_BuiltIn> create(Ptr<Environment> pParentEnv,
                                                ExecutorPtr_t executor);

            /// \brief Used for testing purposes only.
        static Ptr<LispFunction_BuiltIn> create(const ezString& name,
                                                Ptr<Environment> pParentEnv,
                                                ExecutorPtr_t executor);

        static Ptr<LispFunction_BuiltIn> copy(const LispFunction_BuiltIn& toCopy);

        static const Type& typeInfo();

    public:

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;

        virtual ezString toString() const LCPP_OVERRIDE;
        virtual ezString dump() const LCPP_OVERRIDE;

        virtual Ptr<LispObject> call(Ptr<LispObject> pArgList) LCPP_OVERRIDE;

    private:
        ExecutorPtr_t m_pExec;

    private:

        LispFunction_BuiltIn(Ptr<Environment> pEnv,
                             ExecutorPtr_t pExec);
    };

    //////////////////////////////////////////////////////////////////////////

    class LCPP_CORE_API LispFunction_UserDefined : public LispFunction
    {
    public:

        static Ptr<LispFunction_UserDefined> create(Ptr<Environment> pEnv,
                                                    Ptr<LispObject> pArgNameList,
                                                    Ptr<LispCons> pBody);

        static Ptr<LispFunction_UserDefined> copy(const LispFunction_UserDefined& toCopy);

        static const Type& typeInfo();

    public:

        virtual Ptr<LispObject> copy() const LCPP_OVERRIDE;

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

    private:

        LispFunction_UserDefined(Ptr<Environment> pEnv,
                                 Ptr<LispObject> pArgNameList,
                                 Ptr<LispCons> pBody);

        void processArguments(Ptr<LispObject> pArgs);
        /// \return \c true if \a pArgs is a cons, \c false if it is nil.
        ///         In other words, this returns wether further processing needs to be done.
        bool checkArgumentCount(Ptr<LispObject> pArgs);
    };
}

#include "lcpp/core/typeSystem/types/implementation/function.inl"
