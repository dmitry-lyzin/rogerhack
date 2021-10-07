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
#include "ROGER_KP_23_empty.h"
Sin& SIN = ROGER_KP_23_empty.sin;


NO_RETURN void usage( const char* cmd )
{
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
		"Usage:  %s [-d|-e] [-i input_file] [-o output_file]\n"
		"\n"
		"Parameters:\n"
		"	-d		Decode from sin to text\n"
		"	-e		Encode from text to sin\n"
		"	-i file		Input file\n"
		"	-o file		Output file\n"
		"	-h		Print %s usage\n"
		"\n"
		"example:\n"
		"	%s -d -i ROGER_KP-23.sin -o ROGER_KP-23.txt\n", cmd, cmd, cmd
		);
	exit( EXIT_FAILURE );
}

//----------------------------------------------------------------
int main( int argc, const char* argv[] )
{
	setlocale( LC_ALL, "" );
	const char* cmd = argv[0];
	if( argc == 1 )
		usage( cmd );

	enum { encode = 1, decode = 2};

	int xxcode = 0;

	const char*  input = "-";
	const char* output = "-";
	int c;

	while( (c = getopt( argc, argv, "dei:o:h" )) != -1 )
	{
		switch( c )
		{
		case 'd':
			xxcode |= decode;
			break;
		case 'e':
			xxcode |= encode;
			break;
		case 'i':
			input = optarg;
			break;
		case 'o':
			output = optarg;
			break;
		case 'h':
			usage( cmd );
			break;
		default:
			exit( EXIT_FAILURE );
		}
	}

	for( int i = optind; i < argc; i++ )
		fprintf( stderr, "Non-option argument %s\n", argv[i] );


	std::fstream fs;
	switch( xxcode )
	{
	case decode:
		SIN.load( input );
		if( '-' == output[0] && '\0' == output[1] )
			cout << SIN;
		else
		{
			fs.open( output, std::fstream::out | std::fstream::trunc );
			fs << SIN;
			fs.close();
		}
		exit( EXIT_SUCCESS );
		break;

	case encode:
		if( '-' == input[0] && '\0' == input[1] )
			cin >> SIN;
		else
		{
			fs.open( input, std::fstream::in );
			fs >> SIN;
			fs.close();
		}
		SIN.save( output );
		exit( EXIT_SUCCESS );
		break;

	case decode | encode:
		fprintf( stderr, "Illegal combination of options -d and -e\n" );
		exit( EXIT_FAILURE );
		break;

	default:
		fprintf( stderr, "Not present options neither -d nor -e\n" );
		exit( EXIT_FAILURE );
		break;
	}
}
