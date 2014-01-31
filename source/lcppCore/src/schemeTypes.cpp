#include "stdafx.h"
#include "schemeTypes.h"

const SchemeBool& lcpp::SchemeNil::operator==( const SchemeObject& obj ) const
{
    return SchemeBool::create(&obj == &g_nil);
}

const SchemeBool& lcpp::SchemeBool::create( bool value )
{
    return value ? g_true : g_false;
}
