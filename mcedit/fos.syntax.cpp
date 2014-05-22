#ifndef __FOS_SYNTAX__
#define __FOS_SYNTAX__

#include "../fonline-minimal.h"
#include "fonline.h"

#include <stdarg.h>

#include <algorithm>

FONLINE_DLL_ENTRY(compiler) {}

string ssprintf( const string fmt, ... )
{
    int size = 100;
    string str;
    va_list ap;
    while( 1 )
    {
	str.resize(size);
	va_start(ap, fmt);
	int n = vsnprintf( (char*)str.c_str(), size, fmt.c_str(), ap );
	va_end(ap);
	if( (n > -1) && (n < size) )
	{
	    str.resize(n);
	    return( str );
	}
	if (n > -1)
	    size = n + 1;
	else
	    size *= 2;
    }

    return( str );
}

void saveArray( vector<string> vec, ScriptArray& arr )
{
    sort( vec.begin(), vec.end() );
    vec.erase( unique( vec.begin(), vec.end() ), vec.end() );

    for( auto str : vec )
    {
	ScriptString& sstr = ScriptString::Create( str.c_str() );
	arr.InsertLast( &sstr );
    }
}

EXPORT void CollectFOnlineInfo(ScriptArray& arrVars, ScriptArray& arrFuncs )
{
    arrVars.Resize(0);
    arrFuncs.Resize(0);

    vector<string> vars;
    vector<string> funcs;
    asUINT vLen = ASEngine->GetGlobalPropertyCount();
    asUINT fLen = ASEngine->GetGlobalFunctionCount();

    if( vLen )
    {
	for( asUINT v=0; v<vLen; v++ )
	{
	    const char* name;
	    ASEngine->GetGlobalPropertyByIndex( v, &name );
	    vars.push_back( name );
	}
	
	saveArray( vars, arrVars );
    }

    if( fLen )
    {
	for( asUINT f=0; f<fLen; f++ )
	{
	    asIScriptFunction* func = ASEngine->GetGlobalFunctionByIndex( f );
	    const char* name = func->GetName();
	    funcs.push_back( name );
	}

	saveArray( funcs, arrFuncs );
    }
}

#endif // __FOS_SYNTAX__ //
