#include "common.h"
#include <fstream>
#include <sys/stat.h>
#include "BitField.h"
#include "BCDnum.h"

#ifdef __unix__
#	include <unistd.h>
#	define O_BINARY 0
#	define DIRECTORY_SEPARATOR '/'
#else
#	include "getopt.h"
#	include <io.h>
#	define STDIN_FILENO 0
#	define STDOUT_FILENO 1
#	pragma warning( disable: 4996)
#	define DIRECTORY_SEPARATOR '\\'
#endif

//----------------------------------------------------------------
#define FRIEND_OPERATORS( cls)										\
friend std::ostream& operator<<( std::ostream &os, const cls &obj ) { obj.print( os); return os;}	\
friend std::istream& operator>>( std::istream &is,       cls &obj ) { obj.read( is); return is; }

//================================================================
class Frequency : public BCDnum< uint32_t, 0x0, 8, 5>
{
	typedef BCDnum< uint32_t, 0x0, 8, 5> Super;

public:

	void read( std::istream& input )
	{
		if( input >>= '-' )
		{
			input >> any_word;
			value = 0xffffffffu;
			return;
		}
		Super::read( input );
	}
	void print( std::ostream& output ) const
	{
		const auto flag = 0xff000000u & value;
		//if( flag == 0 || flag == 0xff000000u )
		if( flag == 0xff000000u )
		{
			output << "---------";
			return;
		}
		Super::print( output );
	}
	FRIEND_OPERATORS( Frequency )
};

//================================================================
const char polarity_bit_str[] = { 'N', 'I' };

class Tone
{
	static constexpr uint16_t digital_tone_bit = 1 << 15;

	union
	{
#define TPS(x) const char, (x), (sizeof(x)/sizeof(*(x)))
		uint16_t bin;
		BitField< uint16_t, 14, TPS( polarity_bit_str )> polarity_bit;

		BCDnum	< uint16_t, 0x0, 4, 1> analog_tone;
		BCDnum	< uint16_t, '0', 3, 0> digital_tone;
#undef TPS
	} u;
public:

	void read( std::istream& input )
	{
		if( input >>= "OFF" )
		{
			u.bin = 0xFFFFu;
			return;
		}

		if( input >>= 'D' )
		{
			input >> u.digital_tone >> u.polarity_bit;
			u.bin |= digital_tone_bit;
			return;
		}

		input >> u.analog_tone;
	}
	void print( std::ostream& output ) const
	{
		if( 0xFFFFu == u.bin )
		{
			output << "OFF";
			return;
		}
		if( u.bin & digital_tone_bit )
		{
			output << 'D' << u.digital_tone << u.polarity_bit;
			return;
		}
		output << u.analog_tone;
	}
	FRIEND_OPERATORS( Tone )
};

//================================================================
const char *const busy2str [] = { "OFF",	"QT/DQT", "Carrier",	NULL	};
const char *const scan2str [] = { "Delete",	"Add"	};
const char *const power2str[] = { "Low",	"High"	};

class Flags
{
	union
	{
#define TPS(x) const char *const, (x), (sizeof(x)/sizeof(*(x)))
		BitField< uint8_t, 0, TPS( busy2str )> busy;
		BitField< uint8_t, 3, TPS( scan2str )> scan;
		BitField< uint8_t, 4, TPS( power2str)> power;
#undef TPS
	} u;
public:

	void read( std::istream& input )
	{
		input	>> u.busy >> '\t'
			>> u.scan >> '\t'
			>> u.power;
	}
	void print( std::ostream& output ) const
	{
		output	<< u.busy << '\t'
			<< u.scan << '\t'
			<< u.power;
	}
	FRIEND_OPERATORS( Flags )
};

//----------------------------------------------------------------
class Channel
{
	Frequency receive;
	Frequency transmit;
	Tone rx_tone;
	Tone tx_tone;
	Flags flags;
	uint8_t x01;	// хз что, там лежит 0x01
	uint8_t x01_;	// хз что, там лежит 0x01
	uint8_t xFF;	// хз что, там лежит 0xFF

public:

	void read( std::istream& input )
	{
		if( input.eof() )
			return;

		input	>> receive >> '\t' >> transmit >> '\t'
			>> rx_tone >> '\t' >> tx_tone >> '\t'
			>> flags;
	}
	void print( std::ostream& output ) const
	{
		output	<< receive << '\t' << transmit << '\t'
			<< rx_tone << '\t' << tx_tone << '\t'
			<< flags;
	}
	FRIEND_OPERATORS( Channel )
};

//================================================================
struct Sin
{
	union
	{
		uint8_t buf[2048];
		struct {
			uint8_t head[16];
			Channel channels[80];
		} s;
	} u;

	void save( const char* filename ) const
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
	void load( const char* filename )
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
	void read( std::istream& input )
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
	void print( std::ostream& output ) const
	{
		output <<
			"#Receive(MHz)\tTransmit(MHz)\tRxTone\tTxTone\tBusy\tScan\tPower\n"
			"#~~~~~~~~~~~~\t~~~~~~~~~~~~~\t~~~~~~\t~~~~~~\t~~~~\t~~~~\t~~~~~\n";

		for( const auto &channel : u.s.channels )
			output << channel << any_line;
		//for( size_t i = 0; i < 10; i++ )
		//	output << u.s.channels[i] << str_tail;
	}
	FRIEND_OPERATORS( Sin )
};

//----------------------------------------------------------------
#define FF	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
#define E4	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe4, 0xff, 0xff, 0xff
#define x39(x)	x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x
#define x40(x)	x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x

Sin SIN =
{ FF
, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff
, x39( E4 )
, x40( E4 )
, x40( FF )
, FF
, FF
, FF
, 0xff, 0x04, 0x03, 0x02, 0x09, 0x0a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
, 0xff, 0xff, 0xf0, 0x37, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
, FF
, FF
};

//================================================================
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
