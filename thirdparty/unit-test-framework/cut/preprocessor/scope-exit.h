#pragma once

#include "cut/preprocessor/concat.h"
#include "cut/preprocessor/source-line.h"

namespace cut
{
	template<typename T_Callable>
	struct ScopeExit_tpl
	{
		T_Callable m_onExitFunction;

		ScopeExit_tpl(T_Callable func) : m_onExitFunction(func) {}
		~ScopeExit_tpl() { if(m_onExitFunction) m_onExitFunction(); }
	};

	typedef ScopeExit_tpl< ::std::function<void()> > ScopeExit;
}

// Executes code when the current scope exits. Use within a scope only!
// Example:
//	CUT_SCOPE_EXIT
//	{
//		printf("You will always see me when the current scope exits!\n");
//	}; // <- mind the semicolon!
#define CUT_SCOPE_EXIT ::cut::ScopeExit CUT_CONCAT_2(scopeExit_, CUT_SOURCE_LINE) = [&]()
