
#include "common/Precompiled.hpp"
#include <iostream>

CVarBase::CVarBase( const char* name, const char* defaultValue, uint16_t flags, const char* description )
	: varName(name), varValue(defaultValue), varFlags(flags), varDescription(description), isCommand(false)
{

}

CVarBase::CVarBase( const char* name, ConsoleCommandFn* function, const char* description )
	: varName(name), conCommand(function), varDescription(description), isCommand(true)
{

}

int CVarBase::GetInt() const
{
	if ( varValue.empty() )
		return 0;

	return std::stoi( varValue );
}

float CVarBase::GetFloat() const
{
	if ( varValue.empty() )
		return 0.0f;

	return std::stof( varValue );
}

bool CVarBase::GetBool() const
{
	if ( varValue.empty() )
		return false;

	return varValue[0] != '0';
}

const char* CVarBase::GetCString() const
{
	if ( varValue.empty() )
		return nullptr;

	return varValue.c_str();
}

StringRef CVarBase::GetString() const
{
	return varValue;
}

void CVarBase::SetInt( const int& value )
{
	varValue = std::to_string( value );
}

void CVarBase::SetFloat( const float& value )
{
	varValue = std::to_string( value );
}

void CVarBase::SetBool( const bool& value )
{
	varValue = value ? "1" : "0";
}

void CVarBase::SetCString( const char* value )
{
	if ( nullptr == value )
	{
		varValue = "";
		return;
	}

	varValue = value;
}

void CVarBase::SetString( StringRef value )
{
	varValue = value;
}

bool CVarBase::Execute( StringRef args )
{
	if ( isCommand && nullptr != conCommand )
	{
		return conCommand( args );
	}

	// Empty args = checking for information
	if ( args.empty() || !args.at(0) )
	{
		detail::gConsole->Print( adm::format( "CVar '%s' info:", varName.c_str() ) );
		detail::gConsole->Print( adm::format( " L__Value: '%s'", varValue.c_str() ) );
		detail::gConsole->Print( adm::format( " L__Description: '%s'", varDescription.c_str() ) );
		return true;
	}

	// Users can't modify readonly CVars
	if ( varFlags & CVar_ReadOnly )
	{
		detail::gConsole->Print( adm::format( "'%s' is a read-only CVar", varName.c_str() ) );
		return false;
	}

	size_t firstSpace = args.find_first_of( ' ' );
	SetString( args.substr( 0, firstSpace-1 ) );

	detail::gConsole->Print( adm::format( "'%s' is now '%s'", varName.c_str(), varValue.c_str() ) );
	return true;
}
