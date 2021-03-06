/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman
    Copyright (c) 2005-2006 Dan Marsden

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_FUSION_END_IMPL_20060222_2042)
#define BOOST_FUSION_END_IMPL_20060222_2042

#include "../example_struct_iterator.hpp"

namespace boost { namespace fusion {
    
    struct example_sequence_tag;

    namespace extension
    {
        template<typename Tag>
        struct end_impl;

        template<>
        struct end_impl<example_sequence_tag>
        {
            template<typename Sequence>
            struct apply
            {
                typedef example_struct_iterator<Sequence, 2> type;

                static type
                call(Sequence& seq)
                {
                    return type(seq);
                }
            };
        };
    }
}}

#endif
