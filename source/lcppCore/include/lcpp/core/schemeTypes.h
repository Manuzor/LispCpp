//////////////////////////////////////////////////////////////////////////
/// NOTE: This is the file you should include. Do not include the      ///
/// schemeTypes by themselves. They have been split up into several    ///
/// in order to not clutter one file with everything.                  ///
//////////////////////////////////////////////////////////////////////////

#pragma once

// Include headers of scheme types
//////////////////////////////////////////////////////////////////////////

#include "lcpp/core/schemeTypes/conversion.h"
#include "lcpp/core/schemeTypes/type.h"
#include "lcpp/core/schemeTypes/object.h"
#include "lcpp/core/schemeTypes/void.h"
#include "lcpp/core/schemeTypes/bool.h"
#include "lcpp/core/schemeTypes/number.h"
#include "lcpp/core/schemeTypes/cons.h"
#include "lcpp/core/schemeTypes/nil.h"

// Include singletons header
//////////////////////////////////////////////////////////////////////////
#include "lcpp/core/schemeTypes/singletons.h"

// Include inline files
//////////////////////////////////////////////////////////////////////////
#include "lcpp/core/schemeTypes/implementation/conversion_inl.h"
#include "lcpp/core/schemeTypes/implementation/void_inl.h"
#include "lcpp/core/schemeTypes/implementation/bool_inl.h"
#include "lcpp/core/schemeTypes/implementation/number_inl.h"
#include "lcpp/core/schemeTypes/implementation/cons_inl.h"
#include "lcpp/core/schemeTypes/implementation/nil_inl.h"
