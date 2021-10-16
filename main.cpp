#include "common.h"
#include <fstream>
#include "Sin.h"
#ifdef __unix__
#	include <unistd.h>
#	define DIRECTORY_SEPARATOR '/'
#else
#	include "getopt.h"
#	define DIRECTORY_SEPARATOR '\\'
#endif

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

	cerr << "Usage:  " << cmd << " [-h] [-d .|,] [-l file] [-s file] [-r file] [-p file]\n"
		"\n"
		"Parameters:\n"
		"	-d .|,		Decimal separator\n"
		"	-l file		Load file.sin\n"
		"	-s file		Save file.sin\n"
		"	-r file		Read text file\n"
		"	-p file		Print text file\n"
		"	-h		Print " << cmd << " usage\n"
		"\n"
		"example:\n"
		"	" << cmd << " -l ROGER_KP-23.sin -p ROGER_KP-23.txt\n";
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
	static const char stdin_filename[] = "STDIN";

	const char* cmd = argv[0];
	if( argc == 1 )
		usage( cmd );

	int exit_code = EXIT_SUCCESS;
	std::fstream fs;
	while( true)
	{
		switch( getopt( argc, argv, "hd:l:s:p:r:" ) )
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
			filename = stdin_filename;
			if( is_minus( optarg ) )
				cin >> SIN;
			else
			{
				fs.open( optarg, std::fstream::in );
				if( !fs.is_open() )
					fatal( optarg );
				filename = optarg;
				fs >> SIN;
				fs.close();
			}
			break;

		case -1:
			for( int i = optind; i < argc; i++ )
			{
				cerr << "Non-option argument " << argv[i] << '\n';
				exit_code = EXIT_FAILURE;
			}
			exit( exit_code );
			break;

		case 'l': SIN.load( optarg );		break;
		case 's': SIN.save( optarg );		break;
		case 'd': dec_separator = optarg[0];	break;
		case 'h': usage( cmd );			break;
		default:  exit( EXIT_FAILURE );		break;
		}
	}
}
