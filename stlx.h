/************************************************************************/
/**
 * @file stlx.h
 * @brief STL extensions
 * @author Anar Manafov A.Manafov@gsi.de
 *//*

        version number:     $LastChangedRevision$
        created by:         Anar Manafov
                            2008-01-04
        last changed by:    $LastChangedBy$ $LastChangedDate$

        Copyright (c) 2008 GSI GridTeam. All rights reserved.
*************************************************************************/
#ifndef STLX_H_
#define STLX_H_

namespace MiscCommon
{
    /**
     *
     * @brief STL extensions
     *
     */
    namespace stlx
    {
        template <typename _T>
        struct remove_cref
        {
            typedef _T type;
        };

        template <typename _T>
        struct remove_cref<const _T&>
        {
            typedef _T type;
        };

        template <typename _Result, typename _Class, typename _Argument>
        class mem_fun1_t : public std::binary_function <
            _Class*,
            typename remove_cref<_Argument>::type, // was: Argument
            _Result >
        {
            public:
                explicit mem_fun1_t( _Result( _Class::*member )( _Argument ) ): member_( member )
                {
                }

                _Result operator()( _Class* object, _Argument argument ) const
                {
                    return ( object->*member_ )( argument );
                }
            private:
                _Result( _Class::*member_ )( _Argument );
        };
        /**
         *
         * @brief The mem_fun() template is a custom mem_fun adapter, which extends std::mem_fun
         * @brief in order to accept function with const reference as an argument.
         *
         */
        template <typename _Result, typename _Class, typename _Argument>
        mem_fun1_t<_Result, _Class, _Argument> mem_fun( _Result( _Class::*member )( _Argument ) )
        {
            return mem_fun1_t<_Result, _Class, _Argument>( member );
        }
        /*
         *
         * @brief The select1st function object takes a single argument, a pair, and returns the pair's first element.
         *
         */
        template <class _Pair>
        struct select1st : public std::unary_function<_Pair, typename _Pair::first_type>
        {
            typename _Pair::first_type& operator()( _Pair& __x ) const
            {
                return __x.first;
            }
            const typename _Pair::first_type& operator()( const _Pair& __x ) const
            {
                return __x.first;
            }
        };
        /**
         *
         * @brief The select2nd function object takes a single argument, a pair, and returns the pair's second element.
         *
         */
        template <class _Pair>
        struct select2nd : public std::unary_function<_Pair, typename _Pair::second_type>
        {
            typename _Pair::second_type& operator()( _Pair& __x ) const
            {
                return __x.second;
            }
            const typename _Pair::second_type& operator()( const _Pair& __x ) const
            {
                return __x.second;
            }
        };
    };
};
#endif /*STLX_H_*/
