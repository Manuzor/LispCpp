#include "stdafx.h"
#include "lcpp/cli/interpreter.h"
#include "lcpp/cli/ioUtils.h"
#include <iostream>
#include <string>

#include "lcpp/core/runtime.h"
#include "lcpp/core/printer.h"

#include "lcpp/core/typeSystem/types/nil.h"
#include "lcpp/core/typeSystem/types/string.h"

lcpp::Interpreter::Interpreter() :
    m_state(),
    m_out(std::cout),
    m_in(std::cin),
    m_szDataDir()
{
}

lcpp::Interpreter::~Interpreter()
{
}

void lcpp::Interpreter::initialize()
{
    ezFileSystem::RegisterDataDirectoryFactory(ezDataDirectory::FolderType::Factory);

    auto dataDir = ezStringBuilder();
    auto applicationDir = getCurrentWorkingDirectory();
    dataDir.AppendPath(applicationDir.GetData(), "data");
    dataDir.MakeCleanPath();
    auto result = ezFileSystem::AddDataDirectory(dataDir.GetData(), ezFileSystem::ReadOnly, "data");

    if (!result.Succeeded())
    {
        dataDir.Prepend("Unable add data dir: ");
        throw std::exception(dataDir.GetData());
    }
    m_szDataDir = dataDir;

    m_state.initialize();
}

void
lcpp::Interpreter::shutdown()
{
    m_state.shutdown();
}

ezInt32 lcpp::Interpreter::repl()
{
    std::ios_base::sync_with_stdio(false);

    Ptr<LispObject> pResult = LCPP_pNil;

    ezStringBuilder buffer;
    std::string inputBuffer("");

    auto prompt = ezStringBuilder();
    auto currentLine = ezUInt32(0);

    auto& syntaxCheck = m_state.getReaderState()->m_syntaxCheckResult;
    auto& outputStream = *m_state.getPrinterState()->m_pOutStream;

    outputStream << "=== Scheme interpreter 'lcpp' ===\n";

    while(true)
    {
        std::getline(m_in, inputBuffer);
        buffer.AppendFormat("%s\n", inputBuffer.c_str());
    }
}
