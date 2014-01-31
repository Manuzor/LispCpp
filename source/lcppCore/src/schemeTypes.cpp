#include "stdafx.h"
#include "schemeTypes.h"

const SchemeBool& lcpp::SchemeNil::operator==( const SchemeObject& obj ) const
{
	return SchemeBool::create(&obj == &g_nil);
}
