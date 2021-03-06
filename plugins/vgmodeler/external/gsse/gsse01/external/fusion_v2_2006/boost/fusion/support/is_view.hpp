/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_IS_VIEW_03202006_0015)
#define FUSION_IS_VIEW_03202006_0015

#include <boost/fusion/support/detail/is_view.hpp>
#include <boost/fusion/support/tag_of.hpp>

namespace boost { namespace fusion 
{
    namespace extension
    {
        template<typename Tag>
        struct is_view_impl
        {
            template<typename T>
            struct apply
                : detail::fusion_is_view<T>
            {};
        };
    }

    namespace traits
    {
        template <typename T>
        struct is_view : 
            extension::is_view_impl<typename traits::tag_of<T>::type>::
                template apply<T>::type
        {};
}}}

#endif
