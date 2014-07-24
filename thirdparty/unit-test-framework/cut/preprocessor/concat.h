#pragma once

#include "cut/preprocessor/vararg-macros.h"

#define _CUT_CONCAT_HELPER_9(a, b) a ## b
#define _CUT_CONCAT_HELPER_8(a, b) _CUT_CONCAT_HELPER_9(a, b)
#define _CUT_CONCAT_HELPER_7(a, b) _CUT_CONCAT_HELPER_8(a, b)
#define _CUT_CONCAT_HELPER_6(a, b) _CUT_CONCAT_HELPER_7(a, b)
#define _CUT_CONCAT_HELPER_5(a, b) _CUT_CONCAT_HELPER_6(a, b)
#define _CUT_CONCAT_HELPER_4(a, b) _CUT_CONCAT_HELPER_5(a, b)
#define _CUT_CONCAT_HELPER_3(a, b) _CUT_CONCAT_HELPER_4(a, b)
#define _CUT_CONCAT_HELPER_2(a, b) _CUT_CONCAT_HELPER_3(a, b)
#define _CUT_CONCAT_HELPER_1(a, b) _CUT_CONCAT_HELPER_2(a, b)
#define _CUT_CONCAT_HELPER(a, b)   _CUT_CONCAT_HELPER_1(a, b)

#define CUT_CONCAT_2(a, b)                   _CUT_CONCAT_HELPER(a, b)
#define CUT_CONCAT_3(a, b, c)                _CUT_CONCAT_HELPER(CUT_CONCAT_2(a, b), c)
#define CUT_CONCAT_4(a, b, c, d)             _CUT_CONCAT_HELPER(CUT_CONCAT_3(a, b, c), d)
#define CUT_CONCAT_5(a, b, c, d, e)          _CUT_CONCAT_HELPER(CUT_CONCAT_4(a, b, c, d), e)
#define CUT_CONCAT_6(a, b, c, d, e, f)       _CUT_CONCAT_HELPER(CUT_CONCAT_5(a, b, c, d, e), f)
#define CUT_CONCAT_7(a, b, c, d, e, f, g)    _CUT_CONCAT_HELPER(CUT_CONCAT_6(a, b, c, d, e, f), g)
#define CUT_CONCAT_8(a, b, c, d, e, f, g, h) _CUT_CONCAT_HELPER(CUT_CONCAT_7(a, b, c, d, e, f, g), h)

// Note: You cannot pass a number to this function directly. For example, use CUT_TOSTRING(1) instead.
#define CUT_VACONCAT(...) _CUT_CONCAT_HELPER(CUT_CONCAT_, CUT_VA_NUM_ARGS(__VA_ARGS__)) CUT_PASS_VA(__VA_ARGS__)
