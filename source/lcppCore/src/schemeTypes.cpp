#include "stdafx.h"
#include "schemeTypes.h"

const lcpp::SchemeNil g_nil;
const lcpp::SchemeBool g_true;
const lcpp::SchemeBool g_false;

#pragma region Nil
const lcpp::SchemeBool& lcpp::SchemeNil::operator==(const lcpp::SchemeObject& obj) const
{
    bool res = &obj == this;
    return SchemeBool::create(&obj == this);
}

lcpp::String lcpp::SchemeNil::toString() const
{
    return _T("#v");
}

#pragma endregion

#pragma region Bool
const lcpp::SchemeBool& lcpp::SchemeBool::create( bool value )
{
    return value ? g_true : g_false;
}

const lcpp::SchemeBool& lcpp::SchemeBool::operator==( const SchemeObject& obj ) const 
{
    return g_false;
}

lcpp::SchemeBool::operator bool() const
{
    return &g_true == this;
}

lcpp::String lcpp::SchemeBool::toString() const
{
    if (this == &g_true)
    {
        return _T("#t");
    }
    else
    {
        return _T("#f");
    }
}

#pragma endregion

#pragma region Cons
const lcpp::SchemeBool& lcpp::SchemeCons::operator==( const SchemeObject& obj ) const 
{
    return g_false;
}
#pragma endregion
