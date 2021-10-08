#include "common.h"
#include <fstream>
#ifdef __unix__
#	include <unistd.h>
#	define DIRECTORY_SEPARATOR '/'
#else
#	include "getopt.h"
#	define DIRECTORY_SEPARATOR '\\'
#endif

#include "Sin.h"

NO_RETURN void usage( const char* cmd )
{
	// нету в студии basename'а...
	size_t cmdlen = strlen( cmd );
	cmd += cmdlen;
	for( ; cmdlen > 0; --cmdlen )
	{
		if( *--cmd == DIRECTORY_SEPARATOR )
		{
			cmd++;
			break;
		}
	}

	fprintf( stderr,
		"Usage:  %s [-h] [-l file] [-s file] [-r file] [-p file]\n"
		"\n"
		"Parameters:\n"
		"	-l file		Load file.sin\n"
		"	-s file		Save file.sin\n"
		"	-r file		Read text file\n"
		"	-p file		Print text file\n"
		"	-h		Print %s usage\n"
		"\n"
		"example:\n"
		"	%s -l ROGER_KP-23.sin -p ROGER_KP-23.txt\n", cmd, cmd, cmd
		);
	exit( EXIT_FAILURE );
}

//----------------------------------------------------------------
inline bool is_minus( const char* s )
{
	return( '-' == s[0] && '\0' == s[1] );
}

//----------------------------------------------------------------
int main( int argc, const char* argv[] )
{
	setlocale( LC_ALL, "" );

	const char* cmd = argv[0];
	if( argc == 1 )
		usage( cmd );

	int exit_code = EXIT_SUCCESS;
	std::fstream fs;
	while( true)
	{
		switch( getopt( argc, argv, "hl:s:p:r:" ) )
		{
		case 'p':
			if( is_minus( optarg ) )
				cout << SIN;
			else
			{
				fs.open( optarg, std::fstream::out | std::fstream::trunc );
				if( ! fs.is_open())
					fatal( optarg );
				fs << SIN;
				fs.close();
			}
			break;

		case 'r':
			if( is_minus( optarg ) )
				cin >> SIN;
			else
			{
				fs.open( optarg, std::fstream::in );
				if( !fs.is_open() )
					fatal( optarg );
				fs >> SIN;
				fs.close();
			}
			break;

		case -1:
			for( int i = optind; i < argc; i++ )
			{
				fprintf( stderr, "Non-option argument %s\n", argv[i] );
				exit_code = EXIT_FAILURE;
			}
			exit( exit_code );
			break;

		case 'l': SIN.load( optarg );	break;
		case 's': SIN.save( optarg );	break;
		case 'h': usage( cmd );		break;
		default:  exit( EXIT_FAILURE );	break;
		}
	}
}
