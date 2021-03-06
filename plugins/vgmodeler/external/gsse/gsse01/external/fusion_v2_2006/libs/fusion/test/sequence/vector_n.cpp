/*=============================================================================
    Copyright (c) 2001-2006 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/fusion/sequence/container/vector/vector10.hpp>
#include <boost/fusion/sequence/adapted/mpl.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/at.hpp>
#include <boost/fusion/sequence/intrinsic/value_at.hpp>

#include <boost/fusion/sequence/container/vector/vector20.hpp>
#include <boost/fusion/sequence/container/vector/vector30.hpp>
#include <boost/fusion/sequence/container/vector/vector40.hpp>
#include <boost/fusion/sequence/container/vector/vector50.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>
#include <iostream>

#include <boost/fusion/algorithm/transformation/push_back.hpp>
#include <boost/mpl/vector_c.hpp>


int
main()
{
    using namespace boost::fusion;
    using namespace boost;
    using namespace std;

    {
        vector0 vec;
        (void) vec;
        cout << "(): " << sizeof(vec) << endl;
    }

    {
        typedef vector1<int> type;
        type vec;
        BOOST_STATIC_ASSERT(result_of::size<type>::value == 1);

        BOOST_TEST(at_c<0>(vec) == 0);
        BOOST_STATIC_ASSERT((is_same<int, result_of::value_at_c<type, 0>::type>::value));

        // prove that it is mutable
        at_c<0>(vec) = 987;
        BOOST_TEST(at_c<0>(vec) == 987);
    }

    {
        typedef vector1<int> type;
        type vec(123);
        BOOST_TEST(at_c<0>(vec) == 123);
        cout << "(int): " << sizeof(vec) << endl;
    }

    { // testing const vector
        vector1<short> const vec(999);
        BOOST_TEST(at_c<0>(vec) == 999);

#ifdef FUSION_TEST_COMPILE_FAIL
        at_c<0>(vec) = 321;
#endif
    }

    {
        vector1<int> t1(123L); // try conversion from long to int
        vector1<double> t2(t1); // try copy
        (void)t2;
    }

    {
        typedef vector2<int, char> type;
        type vec;
        BOOST_STATIC_ASSERT(result_of::size<type>::value == 2);

        BOOST_TEST(at_c<0>(vec) == 0);
        BOOST_TEST(at_c<1>(vec) == char());

        BOOST_STATIC_ASSERT((is_same<int, result_of::value_at_c<type, 0>::type>::value));
        BOOST_STATIC_ASSERT((is_same<char, result_of::value_at_c<type, 1>::type>::value));
    }

    {
        typedef vector2<int, char> type;
        type vec(123, 'x');
        BOOST_TEST(at_c<0>(vec) == 123);
        BOOST_TEST(at_c<1>(vec) == 'x');
        cout << "(int, char): " << sizeof(vec) << endl;
    }

    {
        vector2<int, int> t1(123, 456);
        vector2<double, float> t2(t1);
        (void)t2;
    }

    {
        typedef vector3<int, char, double> type;
        type vec;
        BOOST_STATIC_ASSERT(result_of::size<type>::value == 3);

        BOOST_TEST(at_c<0>(vec) == 0);
        BOOST_TEST(at_c<1>(vec) == char());
        BOOST_TEST(at_c<2>(vec) == double());

        BOOST_STATIC_ASSERT((is_same<int, result_of::value_at_c<type, 0>::type>::value));
        BOOST_STATIC_ASSERT((is_same<char, result_of::value_at_c<type, 1>::type>::value));
        BOOST_STATIC_ASSERT((is_same<double, result_of::value_at_c<type, 2>::type>::value));
    }

    {
        typedef vector3<int, char, double> type;
        type vec(123, 'x', 123.456);
        BOOST_TEST(at_c<0>(vec) == 123);
        BOOST_TEST(at_c<1>(vec) == 'x');
        BOOST_TEST(at_c<2>(vec) >= 123.455 && at_c<2>(vec) <= 123.457);
        cout << "(int, char, double): " << sizeof(vec) << endl;
    }

    {
        typedef vector4<int, char, double, bool> type;
        type vec(123, 'x', 123.456, true);
        cout << "(int, char, double, bool): " << sizeof(vec) << endl;
    }

    {
        typedef vector4<int, char, bool, double> type;
        type vec(123, 'x', true, 123.456);
        cout << "(int, char, bool, double): " << sizeof(vec) << endl;
    }

    {
        typedef vector7<bool, char, short, int, long, float, double> type;
        type vec(false, 'x', 3, 4, 5, 6.0, 7.0);

        BOOST_TEST(at_c<0>(vec) == false);
        BOOST_TEST(at_c<1>(vec) == 'x');
        BOOST_TEST(at_c<2>(vec) == 3);
        BOOST_TEST(at_c<3>(vec) == 4);
        BOOST_TEST(at_c<4>(vec) == 5);
        BOOST_TEST(at_c<5>(vec) >= 5.9 && at_c<5>(vec) <= 6.1);
        BOOST_TEST(at_c<6>(vec) >= 6.9 && at_c<6>(vec) <= 7.1);

        BOOST_STATIC_ASSERT((is_same<bool, result_of::value_at_c<type, 0>::type>::value));
        BOOST_STATIC_ASSERT((is_same<char, result_of::value_at_c<type, 1>::type>::value));
        BOOST_STATIC_ASSERT((is_same<short, result_of::value_at_c<type, 2>::type>::value));
        BOOST_STATIC_ASSERT((is_same<int, result_of::value_at_c<type, 3>::type>::value));
        BOOST_STATIC_ASSERT((is_same<long, result_of::value_at_c<type, 4>::type>::value));
        BOOST_STATIC_ASSERT((is_same<float, result_of::value_at_c<type, 5>::type>::value));
        BOOST_STATIC_ASSERT((is_same<double, result_of::value_at_c<type, 6>::type>::value));
        cout << "(bool, char, short, int, long, float, double): " << sizeof(vec) << endl;
    }

    {
        typedef vector10<int, int, int, int, int, int, int, int, int, int> type;
        type vec; // compile check only
        cout << "vector10 of int: " << sizeof(vec) << endl;
    }

    {
        typedef vector20<
            int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int> type;

        type vec; // compile check only
        cout << "vector20 of int: " << sizeof(vec) << endl;
    }

    {
        typedef vector30<
            int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int> type;

        type vec; // compile check only
        cout << "vector30 of int: " << sizeof(vec) << endl;
    }

    {
        typedef vector40<
            int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int> type;

        type vec; // compile check only
        cout << "vector40 of int: " << sizeof(vec) << endl;
    }

    {
        typedef vector50<
            int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int
          , int, int, int, int, int, int, int, int, int, int> type;

        type vec; // compile check only
        cout << "vector50 of int: " << sizeof(vec) << endl;
    }
    
    {
        // testing copy and assign from a view
        vector0 empty;
        fusion::vector2<int, long> v(fusion::push_back(fusion::push_back(empty, 123), 456));
        BOOST_TEST(at_c<0>(v) == 123);
        BOOST_TEST(at_c<1>(v) == 456);
        v = fusion::push_back(fusion::push_back(empty, 123), 456); // test assign
        BOOST_TEST(at_c<0>(v) == 123);
        BOOST_TEST(at_c<1>(v) == 456);
    }

    {
        // testing copy and assign from a vector_c
        mpl::vector_c<int, 123, 456> vec_c;
        fusion::vector2<int, long> v(vec_c);
        BOOST_TEST(at_c<0>(v) == 123);
        BOOST_TEST(at_c<1>(v) == 456);
        v = mpl::vector_c<int, 123, 456>(); // test assign
        BOOST_TEST(at_c<0>(v) == 123);
        BOOST_TEST(at_c<1>(v) == 456);
    }

    return boost::report_errors();
}

