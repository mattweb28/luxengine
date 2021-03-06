/****************************
Copyright © 2006-2012 Luke Salisbury
This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.

Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
****************************/
#include "engine.h"
#include "portal.h"
#include "core.h"
#include "config.h"

#ifdef CUSTOMLOADER
namespace custom {
	void preload();
}
#endif


void main_args( int argc, char *argv[] )
{
	int c = argc;
	while ( c > 1 ) {
		--c;
		if ( strcmp(argv[c], "--portal") == 0 )
		{
			luxportal::use = true;
		}
		else if ( strcmp(argv[c], "--window") == 0 )
		{
			lux::config->SetBoolean("display.fullscreen", false);
		}
		else if ( strcmp(argv[c], "--fullscreen") == 0 )
		{
			lux::config->SetBoolean("display.fullscreen", true);
		}
		else if ( strcmp(argv[c], "--ogl") == 0 )
		{
			luxportal::opengl = true;
			lux::config->SetString("display.mode", "OpenGL");
		}
		else if ( strcmp(argv[c], "--sw") == 0 )
		{
			lux::config->SetString("display.mode", "native");
			lux::config->SetString("display.surface", "software");
		}
		else if ( strcmp(argv[c], "--hw") == 0 )
		{
			lux::config->SetString("display.mode", "native");
			lux::config->SetString("display.surface", "hardware");
		}
		else if ( strcmp(argv[c], "--db") == 0 )
		{
			lux::config->SetBoolean("display.dbuff", true);
		}
		else if ( strcmp(argv[c], "--limitframes") == 0 )
		{
			lux::config->SetBoolean("display.limit", true);
		}
		else if ( argv[c][0] == '-' && strlen(argv[c]) == 6 )
		{
			if (argv[c][2] == 'b' && argv[c][3] == 'p' && argv[c][4] == 'p' )
			{
				if (argv[c][5] >= 48 && argv[c][5] < 52)
				{
					lux::config->SetNumber("display.bpp", argv[c][5] - 48);
				}
			}
		}
		else if ( argv[c][0] != '-' )
		{
			lux::config->SetString("project.file", argv[c]);
			luxportal::add( argv[c] );
			luxportal::use = false;
			luxportal::active = false;
		}
	}
}

#ifdef DREAMCAST
	#define DEFAULT_LOCATION "/rd/MokoiGaming/luxengine"
#elif defined(__NDS__) || defined(GEKKO)
	#define DEFAULT_LOCATION "/MokoiGaming/luxengine"
#else
	#define DEFAULT_LOCATION ""
#endif

extern "C" int main( int argc, char *argv[] )
{
	std::string base_directory;

	base_directory = elix::path::GetBase( (argc ? argv[0] : DEFAULT_LOCATION ), true );
	/*
	luxportal::add( base_directory + "demos"LUX_DIR_SSEPARATOR"Audio"LUX_DIR_SSEPARATOR );
	luxportal::add( base_directory + "demos"LUX_DIR_SSEPARATOR"Camera"LUX_DIR_SSEPARATOR );
	luxportal::add( base_directory + "demos"LUX_DIR_SSEPARATOR"Collision"LUX_DIR_SSEPARATOR );
	luxportal::add( base_directory + "demos"LUX_DIR_SSEPARATOR"Display"LUX_DIR_SSEPARATOR );
	luxportal::add( base_directory + "demos"LUX_DIR_SSEPARATOR"Entity"LUX_DIR_SSEPARATOR );
	luxportal::add( base_directory + "demos"LUX_DIR_SSEPARATOR"Input"LUX_DIR_SSEPARATOR );
	luxportal::add( base_directory + "demos"LUX_DIR_SSEPARATOR"Save"LUX_DIR_SSEPARATOR );
	*/
	lux::engine = new LuxEngine( base_directory + "demos"LUX_DIR_SSEPARATOR"Collision"LUX_DIR_SSEPARATOR, base_directory );
	main_args( argc, argv );
	luxportal::open();
	if ( luxportal::use )
	{
		while ( luxportal::active )
		{
			if ( luxportal::run() )
			{
				if ( lux::engine->Start() )
				{
					lux::engine->Loop();
				}
				lux::engine->Close();
			}
		}
	}
	else
	{
		if ( lux::engine->Start() )
		{
			lux::engine->Loop();
		}
		lux::engine->Close();
	}
	luxportal::close();
	delete lux::engine;
	

	return 0;
}







