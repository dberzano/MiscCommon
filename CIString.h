/************************************************************************/
/**
 * @file CIString.h
 * @brief Case Insensitive String
 * @author Anar Manafov A.Manafov@gsi.de
 */ /*
 
        version number:   $LastChangedRevision$
        created by:          Anar Manafov
                                  2007-05-28
        last changed by:   $LastChangedBy$ $LastChangedDate$
 
        Copyright (c) 2007 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef CISTRING_H_
#define CISTRING_H_

namespace MiscCommon
{
    template <class _Elem>
    struct char_traits_ci_base : std::char_traits<_Elem>
    {
        static bool eq(const _Elem& _Left, const _Elem& _Right);
        static bool lt(const _Elem& _Left, const _Elem& _Right);
        static int compare(const _Elem *_First1, const _Elem *_First2, size_t _Count);
    };
    template <>
    struct char_traits_ci_base<char> : std::char_traits<char>
    {
        typedef char _Elem;
        static bool eq(const _Elem& _Left, const _Elem& _Right)
        {   // test for element equality
            return (::toupper(_Left) == ::toupper(_Right));
        }
        static bool lt(const _Elem& _Left, const _Elem& _Right)
        {   // test if _Left precedes _Right
            return (::toupper(_Left) < ::toupper(_Right));
        }
    };
    template <>
    struct char_traits_ci_base<wchar_t> : std::char_traits<wchar_t>
    {
        typedef wchar_t _Elem;
        static bool eq(const _Elem& _Left, const _Elem& _Right)
        {   // test for element equality
            return (::towupper(_Left) == ::towupper(_Right));
        }
        static bool lt(const _Elem& _Left, const _Elem& _Right)
        {   // test if _Left precedes _Right
            return (::towupper(_Left) < ::towupper(_Right));
        }
    };

    template <class _Elem>
    struct char_traits_ci_t : char_traits_ci_base<_Elem>
    {
        typedef std::_Secure_char_traits_tag _Secure_char_traits;

        static int compare(const _Elem *_First1, const _Elem *_First2, size_t _Count)
        {   // compare [_First1, _First1 + _Count) with [_First2, ...)
            return (::_memicmp(_First1, _First2, _Count * sizeof(_Elem)));
        }
        static const _Elem *find(const _Elem *_First, size_t _Count,
                                 const _Elem& _Ch)
        {   // look for _Ch in [_First, _First + _Count)
            for (; 0 < _Count; --_Count, ++_First)
                if (eq(*_First, _Ch))
                    return (_First);
            return (0);
        }
    };

    typedef std::basic_string<wchar_t, char_traits_ci_t<wchar_t>, std::allocator<wchar_t> > ci_wstring;
    typedef std::basic_string<char, char_traits_ci_t<char>, std::allocator<char> > ci_string;

    // Define case insensitive string type.
#if defined(_GLIBCPP_USE_WCHAR_T) && defined(_UNICODE)
    typedef ci_wstring ci_tstring;
#else
    typedef ci_string ci_tstring;
#endif
};

#endif /*CISTRING_H_*/
