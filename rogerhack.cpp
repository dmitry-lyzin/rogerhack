#include "common.h"
#include "Sin.h"

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
	
	Sin sin;
	sin.load( argv[1] );
	//sin.load( "C:\\Users\\lda\\source\\repos\\proc_sin\\Debug\\ROGER.sin" );
	cin >> sin;
	cout << sin;
}
