#include <iostream>
#include <string>
#include <cstdlib>

#include <boost/format.hpp>

#include "dexcom.h"


using namespace std;

void usage( const char* a_pszMessage )
{
    cout << "usage:" << endl;
    cout << " " << a_pszMessage << " url" << endl;
}

int main( int argc, char* argv[] )
{
    // http -f POST https://dev.shawanga.com/api/v1/entries.json?count=3 "User-Agent: xDrip+"
    // "http://127.0.0.1/api/v1/entries.json?count=1&rr=1587961452708" -H  "accept: application/json" -H "User-Agent: xDrip+"| python3 -m json.tool

    if( argc != 2 )
    {
        usage( argv[0] );
        return EXIT_FAILURE;
    }

    dexcom dex;
    dex.setUrl( argv[1] );
    uint64_t tmp = 0;

    string strRet;
    while( true )
    {
        auto ret = dex.getBG( strRet );
        if( ret != tmp )
        {
            tmp = ret;
            cout << strRet << endl;
        }

        sleep( 10 );
    }




    return EXIT_SUCCESS;
}
