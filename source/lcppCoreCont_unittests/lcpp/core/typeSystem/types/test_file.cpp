#include "stdafx.h"
#include "lcpp/core/typeSystem/object.h"
#include "lcpp/core/typeSystem/type.h"
#include "lcpp/core/typeSystem/typeCheck.h"

#include "lcpp/core/runtime.h"

#include "lcpp/core/typeSystem/types/file.h"
#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"
#include "lcpp/core/typeSystem/types/bool.h"

LCPP_TestGroup(File);

LCPP_TestCase(File, Basics)
{
    ezStringBuilder filePath;
    getCurrentWorkingDirectory(filePath);
    filePath.AppendPath("data1", "test", "testFile.txt");
    filePath.MakeCleanPath();

    auto pFile = file::create();
    typeCheck(pFile, Type::File);
    
    auto pResult = LCPP_pNil;

    pResult = file::isOpen(pFile);
    CUT_ASSERT.isTrue(isFalse(pResult));

    pResult = file::open(pFile, str::create(filePath.GetData()), str::create("r"));
    CUT_ASSERT.isTrue(isTrue(pResult));

    pResult = file::isOpen(pFile);
    CUT_ASSERT.isTrue(isTrue(pResult));

    file::close(pFile);

    pResult = file::isOpen(pFile);
    CUT_ASSERT.isTrue(isFalse(pResult));
}

LCPP_TestCase(File, toString)
{
    auto pFile = file::create();
    auto pString = LCPP_pNil;
    auto pResult = LCPP_pNil;
    ezStringBuilder expectedStringOutput;

    ezStringBuilder filePath;
    getCurrentWorkingDirectory(filePath);
    filePath.AppendPath("data1", "test", "testFile.txt");
    filePath.MakeCleanPath();

    pString = object::toString(pFile);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual("<closed file>"));

    //////////////////////////////////////////////////////////////////////////
    pResult = file::open(pFile, str::create(filePath.GetData()), str::create("r"));
    CUT_ASSERT.isTrue(isTrue(pResult));

    expectedStringOutput.Format("<open file: \"%s\">", filePath.GetData());

    pString = object::toString(pFile);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual(expectedStringOutput.GetData()));

    //////////////////////////////////////////////////////////////////////////
    file::close(pFile);

    expectedStringOutput.Format("<closed file: \"%s\">", filePath.GetData());

    pString = object::toString(pFile);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual(expectedStringOutput.GetData()));

    //////////////////////////////////////////////////////////////////////////
    file::setFileName(pFile, LCPP_pNil);

    expectedStringOutput.Format("<closed file>", filePath.GetData());

    pString = object::toString(pFile);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual(expectedStringOutput.GetData()));

    //////////////////////////////////////////////////////////////////////////
    ezStringBuilder nonExistantFile("This/Is/No/Path.extension");
    pResult = file::open(pFile, str::create(nonExistantFile.GetData()), str::create("r"));
    CUT_ASSERT.isTrue(isFalse(pResult));

    expectedStringOutput.Format("<closed file: \"%s\">", nonExistantFile.GetData());

    pString = object::toString(pFile);
    CUT_ASSERT.isTrue(str::getValue(pString).IsEqual(expectedStringOutput.GetData()));
}
