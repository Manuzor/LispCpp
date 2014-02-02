#include "stdafx.h"
#include "schemeTypes.h"

ezStatic<const lcpp::SchemeVoid> lcpp::singletons::g_void = ezStatic<const lcpp::SchemeVoid>();
ezStatic<const lcpp::SchemeNil>  lcpp::singletons::g_nil = ezStatic<const lcpp::SchemeNil>();
ezStatic<const lcpp::SchemeBool> lcpp::singletons::g_true = ezStatic<const lcpp::SchemeBool>();
ezStatic<const lcpp::SchemeBool> lcpp::singletons::g_false = ezStatic<const lcpp::SchemeBool>();
