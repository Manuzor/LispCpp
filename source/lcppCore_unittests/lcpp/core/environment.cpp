#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("EnvironmentTests");

    UnitTest g_test1(g_group, "Basics", []{
        auto pRuntime = createTestRuntime();
        auto pEnv = pRuntime->globalEnvironment();
        auto pFactory = pRuntime->factory();

        Ptr<LispObject> pResult;

        CUT_ASSERT.isFalse(pEnv->get(pFactory->createSymbol("x"), pResult).IsSuccess(), "'get' returned true for a non-existant key!");
        CUT_ASSERT.isTrue(pResult.isNull(), "'get' altered the out_value even though it failed!");
        CUT_ASSERT.isFalse(pEnv->set(pFactory->createSymbol("x"), pFactory->createInteger(42)).IsSuccess(), "Set can NOT succeed if there is existing key to set!");
        pEnv->add(pFactory->createSymbol("x"), pFactory->createInteger(42));
        CUT_ASSERT.isTrue(pEnv->get(pFactory->createSymbol("x"), pResult).IsSuccess(), "'get' returned false for an existing key!");
        CUT_ASSERT.isTrue(pResult->is<LispInteger>(), "Wrong type stored in environment!");
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 42, "Wrong type stored in environment!");
    });

    UnitTest g_test2(g_group, "Parent", []{
        auto pRuntime = createTestRuntime();
        auto pFactory = pRuntime->factory();

        auto pTopEnv = pRuntime->globalEnvironment();
        auto pChildEnv = Environment::create("child", pTopEnv);

        Ptr<LispObject> pResult;

        auto pSymbol = pFactory->createSymbol("x");
        auto pSymbol2 = pFactory->createSymbol("y");

        pTopEnv->add(pSymbol, pFactory->createInteger(42));
        pChildEnv->add(pSymbol, pFactory->createInteger(1337));

        CUT_ASSERT.isTrue(pTopEnv->get(pSymbol, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 42);

        CUT_ASSERT.isTrue(pChildEnv->get(pSymbol, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 1337);

        auto pSubChildEnv = Environment::create("subChild", pChildEnv);

        pSubChildEnv->add(pSymbol, pFactory->createInteger(666));

        CUT_ASSERT.isTrue(pSubChildEnv->get(pSymbol, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 666);

        // Set in top-level env from child env

        pTopEnv->add(pSymbol2, pFactory->createString("hello"));

        CUT_ASSERT.isTrue(pChildEnv->get(pSymbol2, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<LispString>());
        CUT_ASSERT.isTrue(pResult.cast<LispString>()->value().IsEqual("hello"));

        pChildEnv->set(pSymbol2, pFactory->createInteger(-123));

        CUT_ASSERT.isTrue(pChildEnv->get(pSymbol2, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == -123);
        CUT_ASSERT.isTrue(pTopEnv->get(pSymbol2, pResult).IsSuccess());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == -123);
    });

    UnitTest g_test3(g_group, "QualifiedName", []{
        auto pRuntime = createTestRuntime();

        CUT_ASSERT.isTrue(pRuntime->syntaxEnvironment()->name().IsEqual("syntax"), "Invalid local name for syntax environment in runtime.");
        CUT_ASSERT.isTrue(pRuntime->syntaxEnvironment()->qualifiedName().IsEqual("/"), "Invalid qualified name for syntax environment in runtime.");
        CUT_ASSERT.isTrue(pRuntime->globalEnvironment()->name().IsEqual("global"), "Invalid local name for global environment in runtime.");
        CUT_ASSERT.isTrue(pRuntime->globalEnvironment()->qualifiedName().IsEqual("/global"), "Invalid qualified name for global environment in runtime.");

        auto topLevelEnv = Environment::createTopLevel();
        auto sub1 = Environment::create("sub1", topLevelEnv);
        auto sub2 = Environment::create("sub2", sub1);
        auto sub3 = Environment::create("sub3", sub2);

        CUT_ASSERT.isTrue(topLevelEnv->name().IsEqual(""));
        CUT_ASSERT.isTrue(sub1->name().IsEqual("sub1"));
        CUT_ASSERT.isTrue(sub2->name().IsEqual("sub2"));
        CUT_ASSERT.isTrue(sub3->name().IsEqual("sub3"));

        CUT_ASSERT.isTrue(topLevelEnv->qualifiedName().IsEqual("/"));
        CUT_ASSERT.isTrue(sub1->qualifiedName().IsEqual("/sub1"));
        CUT_ASSERT.isTrue(sub2->qualifiedName().IsEqual("/sub1/sub2"));
        CUT_ASSERT.isTrue(sub3->qualifiedName().IsEqual("/sub1/sub2/sub3"));
    });
}
