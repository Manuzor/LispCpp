#include "stdafx.h"
#include "lcpp/testRuntime.h"

using namespace cut;
using namespace lcpp;

namespace
{
    UnitTestGroup g_group("LispEnvironmentTests");

    UnitTest g_test1(g_group, "Basics", []{
        auto pRuntime = resetRuntime();
        auto pEnv = pRuntime->globalEnvironment();

        Ptr<LispObject> pResult;

        CUT_ASSERT.isFalse(pEnv->get(LispSymbol::create("x"), pResult).Succeeded(), "'get' returned true for a non-existant key!");
        CUT_ASSERT.isTrue(pResult.isNull(), "'get' altered the out_value even though it failed!");
        CUT_ASSERT.isFalse(pEnv->set(LispSymbol::create("x"), LispInteger::create(42)).Succeeded(), "Set can NOT succeed if there is existing key to set!");
        pEnv->add(LispSymbol::create("x"), LispInteger::create(42));
        CUT_ASSERT.isTrue(pEnv->get(LispSymbol::create("x"), pResult).Succeeded(), "'get' returned false for an existing key!");
        CUT_ASSERT.isTrue(pResult->is<LispInteger>(), "Wrong type stored in environment!");
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 42, "Wrong type stored in environment!");
    });

    UnitTest g_test2(g_group, "Parent", []{
        auto pRuntime = resetRuntime();

        auto pTopEnv = pRuntime->globalEnvironment();
        auto pChildEnv = LispEnvironment::create("child", pTopEnv);

        Ptr<LispObject> pResult;

        auto pSymbol = LispSymbol::create("x");
        auto pSymbol2 = LispSymbol::create("y");

        pTopEnv->add(pSymbol, LispInteger::create(42));
        pChildEnv->add(pSymbol, LispInteger::create(1337));

        CUT_ASSERT.isTrue(pTopEnv->get(pSymbol, pResult).Succeeded());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 42);

        CUT_ASSERT.isTrue(pChildEnv->get(pSymbol, pResult).Succeeded());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 1337);

        auto pSubChildEnv = LispEnvironment::create("subChild", pChildEnv);

        pSubChildEnv->add(pSymbol, LispInteger::create(666));

        CUT_ASSERT.isTrue(pSubChildEnv->get(pSymbol, pResult).Succeeded());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == 666);

        // Set in top-level env from child env

        pTopEnv->add(pSymbol2, LispString::create("hello"));

        CUT_ASSERT.isTrue(pChildEnv->get(pSymbol2, pResult).Succeeded());
        CUT_ASSERT.isTrue(pResult->is<LispString>());
        CUT_ASSERT.isTrue(pResult.cast<LispString>()->value().IsEqual("hello"));

        pChildEnv->set(pSymbol2, LispInteger::create(-123));

        CUT_ASSERT.isTrue(pChildEnv->get(pSymbol2, pResult).Succeeded());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == -123);
        CUT_ASSERT.isTrue(pTopEnv->get(pSymbol2, pResult).Succeeded());
        CUT_ASSERT.isTrue(pResult->is<LispInteger>());
        CUT_ASSERT.isTrue(pResult.cast<LispInteger>()->value() == -123);
    });

    UnitTest g_test3(g_group, "QualifiedName", []{
        auto pRuntime = resetRuntime();

        CUT_ASSERT.isTrue(pRuntime->syntaxEnvironment()->name()->value().IsEqual("syntax"), "Invalid local name for syntax environment in runtime.");
        CUT_ASSERT.isTrue(pRuntime->syntaxEnvironment()->qualifiedName().IsEqual("/"), "Invalid qualified name for syntax environment in runtime.");
        CUT_ASSERT.isTrue(pRuntime->globalEnvironment()->name()->value().IsEqual("global"), "Invalid local name for global environment in runtime.");
        CUT_ASSERT.isTrue(pRuntime->globalEnvironment()->qualifiedName().IsEqual("/global"), "Invalid qualified name for global environment in runtime.");

        auto topLevelEnv = LispEnvironment::createTopLevel();
        auto sub1 = LispEnvironment::create("sub1", topLevelEnv);
        auto sub2 = LispEnvironment::create("sub2", sub1);
        auto sub3 = LispEnvironment::create("sub3", sub2);

        CUT_ASSERT.isTrue(topLevelEnv->name()->value().IsEqual(""));
        CUT_ASSERT.isTrue(sub1->name()->value().IsEqual("sub1"));
        CUT_ASSERT.isTrue(sub2->name()->value().IsEqual("sub2"));
        CUT_ASSERT.isTrue(sub3->name()->value().IsEqual("sub3"));

        CUT_ASSERT.isTrue(topLevelEnv->qualifiedName().IsEqual("/"));
        CUT_ASSERT.isTrue(sub1->qualifiedName().IsEqual("/sub1"));
        CUT_ASSERT.isTrue(sub2->qualifiedName().IsEqual("/sub1/sub2"));
        CUT_ASSERT.isTrue(sub3->qualifiedName().IsEqual("/sub1/sub2/sub3"));
    });
}
