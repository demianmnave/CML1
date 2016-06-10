/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#include <cml/vector/fixed_external.h>
#include <cml/vector/comparison.h>
#include <cml/vector/types.h>

/* Testing headers: */
#define BOOST_TEST_MODULE fixed_external_vector1
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(array_construct)
{
  double data[] = { 1., 2., 3. };
  cml::external3d v(data);
  BOOST_REQUIRE_EQUAL(v.size(), 3);
  BOOST_CHECK_EQUAL(v.data(), &data[0]);
  BOOST_CHECK_EQUAL(v.data()[0], 1.);
  BOOST_CHECK_EQUAL(v[0], 1.);
}

BOOST_AUTO_TEST_CASE(array_assign)
{
  double av[3];
  double data[] = { 1., 2., 3. };
  cml::external3d v(av);
  v = data;
  BOOST_REQUIRE_EQUAL(v.size(), 3);
  BOOST_CHECK_EQUAL(v.data()[0], 1.);
  BOOST_CHECK_EQUAL(v[0], 1.);
}

BOOST_AUTO_TEST_CASE(list_assign)
{
  double av[3];
  cml::external3d v(av);
  v = { 1., 2., 3. };
  BOOST_REQUIRE_EQUAL(v.size(), 3);
  BOOST_CHECK_EQUAL(v.data()[0], 1.);
  BOOST_CHECK_EQUAL(v[0], 1.);
}

BOOST_AUTO_TEST_CASE(write1)
{
  double data[] = { 1., 2., 3. };
  cml::external3d v(data);
  BOOST_REQUIRE_EQUAL(v.size(), 3);
  v[0] = 1.;
  BOOST_CHECK_EQUAL(v.data()[0], 1.);
  BOOST_CHECK_EQUAL(v[0], 1.);
}

BOOST_AUTO_TEST_CASE(size_check1)
{
  double av[3];
  cml::external3d v(av);
  BOOST_REQUIRE_EQUAL(v.size(), 3);
  BOOST_CHECK_THROW(
    (v = { 1., 2., 3., 4. }), cml::incompatible_vector_size_error);
}

BOOST_AUTO_TEST_CASE(const1)
{
  const double av[] = { 1., 2., 3. };
  cml::external3cd v(av);
  BOOST_REQUIRE_EQUAL(v.size(), 3);
}

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2