#include "common.h"
#include "Sin.h"
#include "ROGER_KP_23_empty.h"
Sin& SIN = ROGER_KP_23_empty.sin;

#define USAGE "Usage\n"


//----------------------------------------------------------------
int main( int argc, const char* argv[] )
{
	setlocale( LC_ALL, "" );

	if( argc == 1 )
	{
		cerr << USAGE;
		exit( EXIT_FAILURE );
	}
	
	//SIN.load( "C:\\Users\\lda\\source\\repos\\proc_sin\\Debug\\ROGER.sin" );
	cin >> SIN;
	SIN.save( argv[1] );
	//cout << SIN;
}
