#include "Sin.h"

#ifdef __unix__
#	include <unistd.h>
#	define O_BINARY 0
#else
#	include <io.h>
#	define STDIN_FILENO 0
#	define STDOUT_FILENO 1
#	pragma warning( disable: 4996)
#endif

#define fatal(x) do { perror( x); exit( EXIT_FAILURE); } while (0)

//----------------------------------------------------------------
void Sin::save( const char* filename ) const
{
	int filedesc = STDOUT_FILENO;
	if( (filedesc = ::open( filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, S_IREAD | S_IWRITE )) < 0 )
		fatal( filename );

	int wrote;
	if( (wrote = ::write( filedesc, u.buf, sizeof( u.buf ) )) < 0 )
		fatal( filename );

	if( wrote < sizeof( u.buf ) )
	{
		cerr << filename << ": no room for " << sizeof( u.buf ) << " bytes\n";
		exit( EXIT_FAILURE );
	}

	::close( filedesc );
}

//----------------------------------------------------------------
void Sin::load( const char* filename )
{
	int filedesc = STDIN_FILENO;
	if( (filedesc = ::open( filename, O_RDONLY | O_BINARY )) < 0 )
		fatal( filename );

	int readed;
	if( (readed = ::read( filedesc, u.buf, sizeof( u.buf ) )) < 0 )
		fatal( filename );

	if( readed != sizeof( u.buf ) )
	{
		cerr << filename << ": must be " << sizeof( u.buf ) << " bytes\n";
		exit( EXIT_FAILURE );
	}

	::close( filedesc );
}

//----------------------------------------------------------------
void Sin::read( std::istream& input )
{
	for( auto& channel : u.s.channels )
	{
		if( input.eof() )
			return;

		while( input.peek() == '#' )
			input >> any_line;

		input >> channel >> any_line;
	}
}

//----------------------------------------------------------------
void Sin::print( std::ostream& output ) const
{
	output <<
		"#Receive(MHz)\tTransmit(MHz)\tRxTone\tTxTone\tBusy\tScan\tPower\n"
		"#~~~~~~~~~~~~\t~~~~~~~~~~~~~\t~~~~~~\t~~~~~~\t~~~~\t~~~~\t~~~~~\n";

	for( const auto &channel : u.s.channels )
		output << channel << any_line;
	//for( size_t i = 0; i < 10; i++ )
	//	output << u.s.channels[i] << str_tail;
}
