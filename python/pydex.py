#! /usr/bin/env python3

import sys
import requests
import json
import time


def getBG( a_url, a_count ):
    #[{"date":1599784072000,"dateString":"2020-09-11T00:27:52Z","device":"share2","direction":"Flat","sgv":        
    # 98,"sysTime":"2020-09-11T00:27:52+0000","trend":4,"type":"sgv","utcOffset":-4},{"date" :
    #1599783773000,"dateString":"2020-09-11T00:22:53Z","device":"share2","direction":"Flat","sgv":
    # 103,"sysTime":"2020-09-11T00:22:53+0000","trend":4,"type":"sgv","utcOffset":-4}]

   url = a_url + '/api/v1/entries.json?count=' + str( a_count )
   header = { "User-Agent" : "xDrip+", "accept" : "application/json" }
   res = requests.post( url, headers=header )


   if res.status_code == 200:
      return res.text
      # ( int( jres[0].get( "date" ) ), res.text() )
   else:
      return res

def formatJson( jsonLine ):
      return '{}  {:3}      {}'.format( jsonLine[ "dateString" ], jsonLine[ "sgv" ], jsonLine[ "direction" ] ) 

def formatJson_delta( jsonLine, delta ):
      return '{}  {:3}:{:+3}  {}'.format( jsonLine[ "dateString" ], jsonLine[ "sgv" ], delta, jsonLine[ "direction" ] ) 

def getDT( jsonLine ):
      return jsonLine[ "date" ]

def getSGV( jsonLine ):
      return int( jsonLine[ "sgv" ] )


def main():
    if len( sys.argv ) < 2:
        print( 'provide url' )
        sys.exit( -1 )
    
    url = sys.argv[1]

    res  = getBG( url, 20 )
    jres = json.loads( res )

    nCount = 0
    oldSvg = 0
    for item in reversed( jres ):
        if nCount == 0:
            oldSvg = getSGV( item )
            print( formatJson( item ) )
        else:
            newSvg = getSGV( item )
            print( formatJson_delta( item, oldSvg-newSvg ) )
        nCount = nCount + 1
        
    tmp    = getDT( jres[0] )
    oldSvg = getSGV( jres[0] )

    while True:
       res = getBG( url, 1 )
       jres = json.loads( res )
       dt = getDT( jres[0] )
       newSvg = getSGV( jres[0] )
       delta = newSvg - oldSvg
       oldSvg = newSvg

       if dt != tmp:
         tmp = dt
         print( formatJson_delta( jres[0], delta ) )
       time.sleep( 10 )


if __name__ == "__main__":
    main( )
