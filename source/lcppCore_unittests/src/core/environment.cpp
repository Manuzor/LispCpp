#include "stdafx.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("EnvironmentTests");

    UnitTest g_test1(g_group, "Basics", [](){
        Environment env = Environment::createTopLevelInstance();
        TypeFactory factory;
        Ptr<SchemeObject> pResult;

        CUT_ASSERT.isFalse(env.get(factory.createSymbol("x"), pResult).IsSuccess(), "'get' returned true for a non-existant key!");
        CUT_ASSERT.isTrue(pResult.isNull(), "'get' altered the out_value even though it failed!");
        CUT_ASSERT.isFalse(env.set(factory.createSymbol("x"), factory.createInteger(42)).IsSuccess(), "Set can NOT succeed if there is existing key to set!");
        env.add(factory.createSymbol("x"), factory.createInteger(42));
        CUT_ASSERT.isTrue(env.get(factory.createSymbol("x"), pResult).IsSuccess(), "'get' returned false for an existing key!");
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>(), "Wrong type stored in environment!");
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42, "Wrong type stored in environment!");
    });

    UnitTest g_test2(g_group, "Parent", [](){
        Environment topEnv = Environment::createTopLevelInstance();
        Environment childEnv("child", &topEnv);
        TypeFactory factory;
        Ptr<SchemeObject> pResult;
        auto pSymbol = factory.createSymbol("x");
        auto pSymbol2 = factory.createSymbol("y");

        topEnv.add(pSymbol, factory.createInteger(42));
        childEnv.add(pSymbol, factory.createInteger(1337));

        CUT_ASSERT.isTrue(topEnv.get(pSymbol, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 42);

        CUT_ASSERT.isTrue(childEnv.get(pSymbol, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 1337);

        Environment subChildEnv("subChild", &childEnv);

        subChildEnv.add(pSymbol, factory.createInteger(666));

        CUT_ASSERT.isTrue(subChildEnv.get(pSymbol, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == 666);

        // Set in top-level env from child env

        topEnv.add(pSymbol2, factory.createString("hello"));

        CUT_ASSERT.isTrue(childEnv.get(pSymbol2, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<SchemeString>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeString>()->value().IsEqual("hello"));

        childEnv.set(pSymbol2, factory.createInteger(-123));

        CUT_ASSERT.isTrue(childEnv.get(pSymbol2, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == -123);
        CUT_ASSERT.isTrue(topEnv.get(pSymbol2, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<SchemeInteger>());
        CUT_ASSERT.isTrue(pResult.cast<SchemeInteger>()->value() == -123);
    });

    UnitTest g_test3(g_group, "QualifiedName", [](){
        auto topLevelEnv = Environment::createTopLevelInstance();
        Environment sub1("sub1", &topLevelEnv);
        Environment sub2("sub2", &sub1);
        Environment sub3("sub3", &sub2);

        CUT_ASSERT.isTrue(topLevelEnv.name().IsEqual(""));
        CUT_ASSERT.isTrue(sub1.name().IsEqual("sub1"));
        CUT_ASSERT.isTrue(sub2.name().IsEqual("sub2"));
        CUT_ASSERT.isTrue(sub3.name().IsEqual("sub3"));

        CUT_ASSERT.isTrue(topLevelEnv.qualifiedName().IsEqual("/"));
        CUT_ASSERT.isTrue(sub1.qualifiedName().IsEqual("/sub1"));
        CUT_ASSERT.isTrue(sub2.qualifiedName().IsEqual("/sub1/sub2"));
        CUT_ASSERT.isTrue(sub3.qualifiedName().IsEqual("/sub1/sub2/sub3"));
    });
}
