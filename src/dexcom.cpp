#include "dexcom.h"
#include <sstream>
#include <iostream>

#include <boost/format.hpp>
#include "../../json/include/nlohmann/json.hpp"

namespace json = nlohmann;


using namespace std;

uint64_t dexcom::getBG( string& a_strRet )
{
    uint64_t unRet = 0;
    int32_t a_count =1 ;
    const int32_t cnHttpOk = 200;
    string strUrl = m_strUrl;
    strUrl.append( "/api/v1/entries.json" );
    strUrl.append( "?count=" );
    strUrl.append( std::to_string( a_count ) );


    cpr::Response resp;
    resp = cpr::Post(
          cpr::Url{ strUrl },
          cpr::Header{ { "User-Agent", "xDrip+" } },
          cpr::Body( "" )  // without body, content-length is not sent
          );

    // [{\"date\":1599700969000,
    //   \"dateString\":\"2020-09-10T01:22:49Z\",
    //   \"device\":\"share2\",
    //   \"direction\":\"Flat\",
    //   \"sgv\":95,
    //   \"sysTime\":\"2020-09-10T01:22:49+0000\",
    //   \"trend\":4,
    //   \"type\":\"sgv\",
    //   \"utcOffset\":-4 }]"

    // [ {
    //   "date":1599700969000,
    //   "dateString":"2020-09-10T01:22:49Z",
    //   "device":"share2",
    //   "direction":"Flat",
    //   "sgv":95,
    //   "sysTime":"2020-09-10T01:22:49+0000",
    //   "trend":4,
    //   "type":"sgv",
    //   "utcOffset":-4
    //   }
    //   ]"

    // "[
    // {
    // "DT":"Date(1587664805000+0000)",
    // "ST\":Date(1587679205000)",
    // "Trend":5,
    // "Value":74,
    // "WT":"Date(1587679205000)"
    // }
    // ]"

    if( resp.status_code == cnHttpOk )
    {
        json::json js_results;
        js_results = json::json::parse( resp.text );
        data::bg_data bg_value;

        for( auto &[key, value] : js_results.items() )
        {
            string   strDt;
            string   strDir;
            uint64_t unDate;
            int32_t  nBG;
            int32_t  nTrend;

            try
            {
                unDate   = value[ "date" ];
                strDt    = value[ "dateString" ];
                nBG      = value[ "sgv" ];
                strDir   = value[ "direction"];
                nTrend   = value[ "trend" ];
            } catch( std::exception &e )
            {
               stringstream sstr;
               sstr << "bg bad value" << value;
               continue;
            }

            bg_value.strDT = strDt;
            bg_value.DT    = stoll( strDt );
            //bg_value.DTz   = stoll( strDtZone );
            bg_value.value = nBG;
            bg_value.trend = nTrend;

            unRet = unDate;

            a_strRet = (boost::format( "%s BG:%d Trend:%s" ) % bg_value.strDT % bg_value.value % strDir ).str();
        }
    } else
    {
        stringstream sstr;
        sstr << "post error:" << resp.status_code;
        setError( sstr.str() );
        return -1;
    }
    return unRet;
}
