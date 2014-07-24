#pragma once

#include "cut/preprocessor/parenthesis.h"

// CUT_VA_NUM_ARGS() is a very nifty macro to retrieve the number of arguments handed to a variable-argument macro
// unfortunately, VS 2010 still has this compiler bug which treats a __VA_ARGS__ argument as being one single parameter:
// https://connect.microsoft.com/VisualStudio/feedback/details/521844/variadic-macro-treating-va-args-as-a-single-parameter-for-other-macros#details
#if _MSC_VER >= 1400
#define CUT_VA_NUM_ARGS_HELPER(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, N, ...)    N
#define CUT_VA_NUM_ARGS_REVERSE_SEQUENCE 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
#define CUT_VA_NUM_ARGS(...) CUT_VA_NUM_ARGS_HELPER CUT_PARENTHESIS_LEFT __VA_ARGS__, CUT_VA_NUM_ARGS_REVERSE_SEQUENCE CUT_PARENTHESIS_RIGHT
#endif

// CUT_PASS_VA passes __VA_ARGS__ as multiple parameters to another macro, working around the above-mentioned bug
#if _MSC_VER >= 1400
#define CUT_PASS_VA(...) CUT_PARENTHESIS_LEFT __VA_ARGS__ CUT_PARENTHESIS_RIGHT
#endif

// Passes variadic preprocessor arguments to another macro
#ifndef CUT_PASS_VA
#define CUT_PASS_VA(...) (__VA_ARGS__)
#endif
