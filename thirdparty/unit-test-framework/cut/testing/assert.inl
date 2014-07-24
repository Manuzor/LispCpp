#include "cut/testing/assert-handler.h"

template<typename T_Exception>
void cut::Assert::throws(Lambda_t lambda, StringRef message /*= nullptr*/)
{
	try
	{
		lambda();
		IAssertHandler::instance().handleFailure(fileName, lineNumber, message);
	}
	catch(T_Exception&) { return; }
	catch(...)
	{
		IAssertHandler::instance().handleFailure(fileName, lineNumber, message);
	}
}
