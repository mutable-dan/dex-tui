#include <iostream>
#include <string>

#include <cstdlib>

#include <boost/format.hpp>

#include "../../json/include/nlohmann/json.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Weffc++"
   // https://github.com/whoshuu/cpr/
//#include "cpr/include/cpr/cpr.h"
    #include "cpr/cpr.h"
#pragma GCC diagnostic pop


using namespace std;
namespace json = nlohmann;

void usage( const char* a_pszMessage )
{
    cout << "usage:" << endl;
    cout << " " << a_pszMessage << " url" << endl;
}

int main( int argc, char* argv[] )
{
    // "http://127.0.0.1/api/v1/entries.json?count=1&rr=1587961452708" -H  "accept: application/json" -H "User-Agent: xDrip+"| python3 -m json.tool
    if( argc != 2 )
    {
        usage( argv[0] );
        return EXIT_FAILURE;
    }

    const int32_t cnHttpOk = 200;
    string strUrl = argv[1];
    strUrl.append( "/api/v1/entries.json" );
    strUrl.append( "?count=1" );

    cout << "url: " << strUrl.c_str() << endl;

    cpr::Response resp;
    resp = cpr::Post(
          cpr::Url{ strUrl },
          cpr::Parameters{ { "User-Agent", "xDrip+" } },
          cpr::Body( "" )  // without body, content-length is not sent
          );

    if( resp.status_code == cnHttpOk )
    {
        json::json js_results;
        js_results = json::json::parse( resp.text );
        for( auto &[key, value] : js_results.items() )
        {
            cout << "key:" << key << " <> " << value << endl;
        }
    } else
    {
        cerr << "post error code:" << resp.status_code << endl;
    }

    return EXIT_SUCCESS;
}
