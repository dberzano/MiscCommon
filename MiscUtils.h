/************************************************************************/
/**
 * @file MiscUtils.h
 * @brief a set of general helpers
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:    $LastChangedRevision$
        created by:        Anar Manafov
                                    2006-03-28
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2006 GSI GridTeam. All rights reserved.
************************************************************************/
#ifndef MISCUTILS_H
#define MISCUTILS_H

#include <iostream>
#include <algorithm>

/** @namespace MiscCommon
 *  @brief Miscellaneous functions and helpers located
 **/
namespace MiscCommon
{

    /** @brief A helper class. Helps to automatically track environment variables. */
    class auto_setenv
    {
            auto_setenv()
            {}
            ~auto_setenv()
            {
                unset();
            }
        public:
            static auto_setenv& Instance()
            {
                static auto_setenv env;
                return env;
            }
            void Init( const std::string &_VarName, const std::string &_NewValue )
            {
                // unsetting variable in case if called second of more times
                unset();

                m_sVarName = _VarName;
                m_sNewValue = _NewValue;

                char *chTmp = getenv( m_sVarName.c_str() );
                if ( chTmp )
                    m_sOldValue = chTmp;
                // TODO: check error code
                setenv( m_sVarName.c_str(), m_sNewValue.c_str(), 1 );
            }
            void unset()
            {
                if ( !m_sOldValue.empty() )
                    setenv( m_sVarName.c_str(), m_sOldValue.c_str(), 1 );
            }

        private:
            std::string m_sVarName;
            std::string m_sNewValue;
            std::string m_sOldValue;
    };

    /** @fn _T* smart_append( _T* _pString, const typename  _T::value_type _ItemToAdd )
     *  @brief appends character _ItemToAdd to the string _pString if there is no such suffix on the end of _pString.
     *  @param _pString - [in] The string to be processed.
     *  @param _ItemToAdd - [in] The target characters to be checked and added.
     *  @return A pointer to the processed string object.
     **/
    template <typename _T>
    _T* smart_append( _T* _pString, const typename _T::value_type _ItemToAdd )
    {
        if ( !_pString )
            return _pString;

        if ( _pString->empty() || ( *_pString ) [ _pString->size() - 1 ] != _ItemToAdd )
            _pString->push_back( _ItemToAdd );

        return _pString;
    }

    /** @fn _T& trim(_T* _pString, const _T& _strWhat)
     *  @brief trims trailing and leading characters from the string.
     *  @param _pString - [in] The string to be trimmed.
     *  @param _strWhat - [in] The target characters to be trimmed.
     *  @return A reference to the string object from which the elements have been trimmed.
     */
    template <typename _T>
    _T& trim( _T* _pString, const _T& _strWhat )
    {
        return trim_right( &trim_left( _pString, _strWhat ), _strWhat );
    }

    /** @fn _T& trim_right(_T* _pString, const _T& _strWhat)
     *  @brief trims trailing characters from the string.
     *  @param _pString - [in] The string to be trimmed.
     *  @param _strWhat - [in] The target characters to be trimmed.
     *  @return A reference to the string object from which the elements have been trimmed.
     */
    template <typename _T>
    _T& trim_right( _T* _pString, const _T& _strWhat )
    {
        return _pString->erase( _pString->find_last_not_of( _strWhat ) + 1 );
    }

    /** @fn _T& trim_left(_T* _pString, const _T& _strWhat)
     *  @brief trims leading characters from the string.
     *  @param _pString - [in] The string to be trimmed.
     *  @param _strWhat - [in] The target characters to be trimmed.
     *  @return A reference to the string object from which the elements have been trimmed.
     */
    template <typename _T>
    _T& trim_left( _T* _pString, const _T& _strWhat )
    {
        return _pString->erase( 0, _pString->find_first_not_of( _strWhat ) );
    }

    /** @fn _T& replace(_T* _pString, const _T& _what, const _T& _with)
     *  @brief finds elements in a string match a specified string and replaces it.
     *  @param _pString - [in,out] The string to be processed.
     *  @param _what - [in,out] String to be replaced.
     *  @param _with - [in] Replacing string.
     *  @return A reference to the string object from which the elements have been replaced.
     */
    template <typename _T>
    _T& replace( _T* _pString, const _T& _what, const _T& _with )
    {
        typename _T::size_type pos = 0;
        typename _T::size_type withLen = _with.length();
        typename _T::size_type whatLen = _what.length();
        while ( ( pos = _pString->find( _what, pos ) ) != _T::npos )
        {
            _pString->replace( pos, _what.length(), _with );
            if ( withLen > whatLen )
                pos += withLen - whatLen + 1;
        }
        return ( *_pString );
    }

    // HACK: because of the bug in gcc 3.3 we need to use this nesty ToLower and ToUpper instead of direct calls of tolower...
    struct ToLower
    {
        char operator() ( char c ) const { return std::tolower( c ); }
    };

    struct ToUpper
    {
        char operator() ( char c ) const { return std::toupper( c ); }
    };

    /** @fn  _T& to_upper(_T& _str)
     *  @brief convert string to upper case.
     *  @param _str - [in] Sting to convert.
     *  @return Converted string.
     */
    template <typename _T>
    _T& to_upper( _T& _str )
    {
        std::transform( _str.begin(), _str.end(), _str.begin(), ToUpper() );
        return _str;
    }

    /** @fn _T& to_lower(_T& _str)
     *  @brief convert string to lower case.
     *  @param _str - [in] Sting to convert.
     *  @return Converted string.
     */
    template <typename _T>
    _T& to_lower( _T& _str )
    {
        std::transform( _str.begin(), _str.end(), _str.begin(), ToLower() );
        return _str;
    }
    
};
#endif
