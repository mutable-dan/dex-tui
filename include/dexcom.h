#pragma once

#include <string>
#include "bg_cache.h"

#pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wshadow"
    #pragma GCC diagnostic ignored "-Weffc++"
    // https://github.com/whoshuu/cpr/
    #include "cpr/cpr.h"
#pragma GCC diagnostic pop


class dexcom final
{
    private:
        std::string     m_strUrl;
        std::string     m_strError;
        bool            m_bError    = false;

        data::bg_cache  m_sugarCache;

        void setError( const std::string a_strError ) { m_bError = true; m_strError = a_strError; }
        void clearError()                             { m_bError = false; m_strError.clear(); }

    public:
        void setUrl( std::string a_straUrl ) noexcept { m_strUrl = a_straUrl; }
        uint64_t getBG( std::string& strRet );
};
