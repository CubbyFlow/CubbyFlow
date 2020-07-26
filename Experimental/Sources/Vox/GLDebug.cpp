/*************************************************************************
> File Name: GLDebug.hpp
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> References: OpenGL-Insights Sample chapter
> Purpose: Debug utility functions for GL_ARB_debug_output extension.
> Created Time: 2020/07/26
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/
#include <Vox/GLDebug.hpp>
#include <Core/Utils/Logging.h>
#include <glad/glad.h>

#ifdef _LINUX
#include <execinfo.h>
#endif

using namespace CubbyFlow;

namespace Vox {

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

        // output the call stack
        #ifdef _WIN32
        void PrintStack() {
        
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

        	for( i = 0; i < frames; i++ )
        	{
        		SymFromAddr( process, ( DWORD64 )( stack[ i ] ), 0, symbol );
        		DWORD  dwDisplacement;
        		IMAGEHLP_LINE64 line;

        		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);
        		if (!strstr(symbol->Name,"VSDebugLib::") &&
        			SymGetLineFromAddr64(process, ( DWORD64 )( stack[ i ] ), &dwDisplacement, &line)) {
                        CUBBYFLOW_INFO << "Function : " << symbol->Name << " - line : " << line.LineNumber;
        		}
        		if (0 == strcmp(symbol->Name,"main"))
        			break;
             }

             free( symbol );
        }
        #endif

        #ifdef _LINUX
        int GetFileAndLine (unw_word_t addr, char *file, size_t flen, int *line)
        {
        	static char buf[256];
        	char *p;

        	// prepare command to be executed
        	// our program need to be passed after the -e parameter
        	sprintf (buf, "/usr/bin/addr2line -C -e ./%s -f -i %lx", __progname, addr);

        	FILE* f = popen (buf, "r");

        	if (f == NULL)
        	{
        		perror (buf);
        		return 0;
        	}

        	// get function name
        	fgets (buf, 256, f);

        	// get file and line
        	fgets (buf, 256, f);

        	if (buf[0] != '?')
        	{
        		int l;
        		char *p = buf;

        		// file name is until ':'
        		while (*p != ':')
        		{
        			p++;
        		}

        		*p++ = 0;
        		// after file name follows line number
        		strcpy (file , buf);
        		sscanf (p,"%d", line);
        	}
        	else
        	{
        		strcpy (file,"unknown");
        		*line = -1;
        	}

        	pclose(f);
        }

        void PrintStack() {
        
          unw_cursor_t cursor; 
          unw_context_t uc;
          unw_word_t ip, sp, off;
          unw_proc_info_t pi;
          char file[256], name[256];
          int line;
          int status;

          unw_getcontext(&uc);
          unw_init_local(&cursor, &uc);
          while (unw_step(&cursor) > 0) {
            unw_get_reg(&cursor, UNW_REG_IP, &ip);
            unw_get_reg(&cursor, UNW_REG_SP, &sp);

            unw_get_proc_name (&cursor, name, sizeof (name), &off);
            GetFileAndLine((long)ip, file, 256, &line);

            if (line >= 0) {
              char *realname;
              realname = abi::__cxa_demangle(name, 0, 0, &status);

              if (realname) {
            CUBBYFLOW_INFO << realname << ": " << file << ", " << line;
        	free(realname);
              } else {
            CUBBYFLOW_INFO << name << ": " << file << ", " << line;
              }
            }
          }
        }
        #endif
    }
};