/*************************************************************************
> File Name: DebugUtils.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: OpenGL-Insights Sample chapter
> Purpose: Debug utility function collection
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/DebugUtils.hpp>
#include <Core/Utils/Logging.hpp>
#include <glad/glad.h>
#include <iostream>
#include <cstdio>

#if defined(CUBBYFLOW_WINDOWS)
	#include <windows.h>
	#include <DbgHelp.h>
	#pragma comment(lib,"Dbghelp")
	#define STDCALL __stdcall
#else
	#define STDCALL
#endif

#if defined(CUBBYFLOW_LINUX)
	#define UNW_LOCAL_ONLY
	#include <libunwind.h>
	#include <stdio.h>
	#include <string.h>
	#include <execinfo.h>
	#include <stdlib.h>
	#include <cxxabi.h>
	#include <execinfo.h>
#endif

namespace Vox {

	// output the call stack
    #if defined(CUBBYFLOW_WINDOWS)
    void StackTrace::PrintStack() 
	{
        unsigned int   i;
        void         * stack[ 100 ];
        unsigned short frames;
        SYMBOL_INFO  * symbol;
        HANDLE         process;

        process = GetCurrentProcess();

        SymSetOptions(SYMOPT_LOAD_LINES);

        SymInitialize( process, NULL, TRUE );

        frames               = CaptureStackBackTrace( 0, 200, stack, NULL );
        symbol               = ( SYMBOL_INFO * )calloc( sizeof( SYMBOL_INFO ) + 256 * sizeof( char ), 1 );
        symbol->MaxNameLen   = 255;
        symbol->SizeOfStruct = sizeof( SYMBOL_INFO );

		printf("---------------------Stack Trace---------------------\n");
        for( i = 0; i < frames; i++ )
        {
        	SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );
        	DWORD  dwDisplacement;
        	IMAGEHLP_LINE64 line;

        	line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        	if (!strstr(symbol->Name,"VSDebugLib::") && SymGetLineFromAddr64(process, ( DWORD64 )( stack[ i ] ), &dwDisplacement, &line)) 
			{
				printf("Function : %s - line : %lu", symbol->Name, line.LineNumber);
        	}

        	if (0 == strcmp(symbol->Name,"main"))
        		break;
        }
		printf("-----------------------------------------------------\n");
        free( symbol );
    }
    #elif defined(CUBBYFLOW_LINUX)
    void StackTrace::PrintStack() 
	{
    	unw_cursor_t cursor; 
        unw_context_t uc;
        unw_word_t ip, sp, off;
        char name[256];
        int status;

        unw_getcontext(&uc);
        unw_init_local(&cursor, &uc);

		printf("---------------------Stack Trace---------------------\n");
        while (unw_step(&cursor) > 0) 
		{
        	unw_get_reg(&cursor, UNW_REG_IP, &ip);
			printf("0x%lx", ip);
        	unw_get_reg(&cursor, UNW_REG_SP, &sp);

        	unw_get_proc_name (&cursor, name, sizeof (name), &off);

			char *realname = abi::__cxa_demangle(name, 0, 0, &status);
        	if (realname)
			{
				printf("0x%lx (%s+0x%lx)\n", ip, realname, off);
        		free(realname);
        	}
			else
			{
				printf("0x%lx (%s+0x%lx)\n", ip, name, off);
        	}
    	}
		printf("-----------------------------------------------------\n");
    }
    #endif

	namespace Detail
    {
        // aux function to translate source to string
        std::string GetStringForSource(GLenum source) {
        
        	switch(source) {
        		case GL_DEBUG_SOURCE_API_ARB: 
        			return("API");
        		case GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB:
        			return("Window System");
        		case GL_DEBUG_SOURCE_SHADER_COMPILER_ARB:
        			return("Shader Compiler");
        		case GL_DEBUG_SOURCE_THIRD_PARTY_ARB:
        			return("Third Party");
        		case GL_DEBUG_SOURCE_APPLICATION_ARB:
        			return("Application");
        		case GL_DEBUG_SOURCE_OTHER_ARB:
        			return("Other");
        		default:
        			return("");
        	}
        }

        // aux function to translate severity to string
        std::string GetStringForSeverity(GLenum severity) {
        
        	switch(severity) {
        		case GL_DEBUG_SEVERITY_HIGH_ARB: 
        			return("High");
        		case GL_DEBUG_SEVERITY_MEDIUM_ARB:
        			return("Medium");
        		case GL_DEBUG_SEVERITY_LOW_ARB:
        			return("Low");
        		default:
        			return("");
        	}
        }

        // aux function to translate type to string
        std::string GetStringForType(GLenum type) {
        
        	switch(type) {
        		case GL_DEBUG_TYPE_ERROR_ARB: 
        			return("Error");
        		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB:
        			return("Deprecated Behaviour");
        		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB:
        			return("Undefined Behaviour");
        		case GL_DEBUG_TYPE_PORTABILITY_ARB:
        			return("Portability Issue");
        		case GL_DEBUG_TYPE_PERFORMANCE_ARB:
        			return("Performance Issue");
        		case GL_DEBUG_TYPE_OTHER_ARB:
        			return("Other");
        		default:
        			return("");
        	}
        }
    }

    void GLDebug::DebugLog(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const GLvoid* userParam)
    {
        std::cerr << "[Type] : "       << Detail::GetStringForType(type)           << 
                	 "[Source] : "     << Detail::GetStringForSource(source)       <<
                	 "[ID] : "         <<              id                          <<
                	 "[Serverity] : "  << Detail::GetStringForSeverity(severity)   << std::endl;
                          
        std::cerr << "[Message] : "    << message << std::endl;
        
        StackTrace::PrintStack();
    }
};