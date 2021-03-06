/*=============================================================================
    Copyright (c) 2005 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_TUPLE_10032005_0810)
#define FUSION_TUPLE_10032005_0810

#include <boost/fusion/tuple/tuple_fwd.hpp>
#include <boost/fusion/sequence/container/vector/vector.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/comparison.hpp>
#include <boost/fusion/sequence/io.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_const.hpp>

namespace boost { namespace fusion
{
    template <BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, typename T)>
    struct tuple : vector<BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)>
    {
        typedef vector<
            BOOST_PP_ENUM_PARAMS(FUSION_MAX_VECTOR_SIZE, T)> 
        base_type;

        tuple()
            : base_type() {}
        
        template <typename T>
        explicit tuple(T const& rhs)
            : base_type(rhs) {}

        #include <boost/fusion/tuple/detail/tuple_forward_ctor.hpp>

        template <typename T>
        tuple&
        operator=(T const& rhs)
        {
            base_type::operator=(rhs);
            return *this;
        }
    };

    template <typename Tuple> 
    struct tuple_size : result_of::size<Tuple> {};

    template <int N, typename Tuple> 
    struct tuple_element : result_of::value_at_c<Tuple, N> {};

    template <int N, typename Tuple>
    inline typename 
        lazy_disable_if<
            is_const<Tuple>
          , result_of::at_c<Tuple, N>
        >::type
    get(Tuple& tup)
    {
        typedef result_of::at_c<Tuple, N> at_meta;
        return extension::at_impl<typename traits::tag_of<Tuple>::type>::
            template apply<Tuple, mpl::int_<N> >::call(tup);
    }

    template <int N, typename Tuple>
    inline typename result_of::at_c<Tuple const, N>::type
    get(Tuple const& tup)
    {
        typedef result_of::at_c<Tuple const, N> at_meta;
        return extension::at_impl<typename traits::tag_of<Tuple>::type>::
            template apply<Tuple const, mpl::int_<N> >::call(tup);
    }
}}

#endif
