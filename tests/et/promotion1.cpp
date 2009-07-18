/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#include <typeinfo>
#include <cml/et/scalar_promotions.h>

#define BOOST_TEST_MODULE Promotion1
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

BOOST_AUTO_TEST_CASE(promotion1)
{
    typedef cml::et::ScalarPromote<double,int> double_int_promote_type;
    BOOST_REQUIRE(typeid(double_int_promote_type::type) == typeid(double));

    typedef cml::et::ScalarPromote<int,double> int_double_promote_type;
    BOOST_REQUIRE(typeid(int_double_promote_type::type) == typeid(double));

    typedef cml::et::ScalarPromote<int,unsigned> int_unsigned_promote_type;
    BOOST_REQUIRE(typeid(int_unsigned_promote_type::type) == typeid(int));
}

// -------------------------------------------------------------------------
// vim:ft=cpp
