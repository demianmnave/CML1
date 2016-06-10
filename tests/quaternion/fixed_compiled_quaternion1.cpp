/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

// Make sure the main header compiles cleanly:
#include <cml/quaternion/fixed.h>

#include <cml/vector/fixed.h>
#include <cml/vector/types.h>
#include <cml/quaternion/types.h>

/* Testing headers: */
#define BOOST_TEST_MODULE fixed_compiled_quaternion1
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(array_construct1)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_ip q(data);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(array_construct2)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_rp q(data);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
}

BOOST_AUTO_TEST_CASE(array_temp_construct1)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_ip q = data;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(array_temp_construct2)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_rp q = data;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
}

BOOST_AUTO_TEST_CASE(element_construct1)
{
  cml::quaterniond_ip q(1., 2., 3., 4.);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(element_construct2)
{
  cml::quaterniond_rp q(1., 2., 3., 4.);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
}

BOOST_AUTO_TEST_CASE(combine_construct1)
{
  cml::quaterniond_ip q(cml::vector3d(1., 2., 3.), 4.);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_construct2)
{
  cml::quaterniond_rp q(cml::vector3d(1., 2., 3.), 4.);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_construct3)
{
  cml::quaterniond_ip q(4., cml::vector3d(1., 2., 3.));
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_construct4)
{
  cml::quaterniond_rp q(4., cml::vector3d(1., 2., 3.));
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_construct5)
{
  double data[] = { 1., 2., 3. };
  cml::quaterniond_ip q(data, 4.);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_construct6)
{
  double data[] = { 1., 2., 3. };
  cml::quaterniond_rp q(data, 4.);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_construct7)
{
  double data[] = { 1., 2., 3. };
  cml::quaterniond_ip q(4., data);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_construct8)
{
  double data[] = { 1., 2., 3. };
  cml::quaterniond_rp q(4., data);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 4.);
}

BOOST_AUTO_TEST_CASE(array_assign1)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_ip q;
  q = data;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(array_assign2)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_rp q;
  q = data;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
}

BOOST_AUTO_TEST_CASE(pointer_construct1)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_ip q(&data[0]);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(pointer_construct2)
{
  double data[] = { 1., 2., 3., 4. };
  cml::quaterniond_rp q(&data[0]);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
}

BOOST_AUTO_TEST_CASE(list_temp_construct1)
{
  cml::quaterniond_ip q = { 1., 2., 3., 4. };
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(list_temp_construct2)
{
  cml::quaterniond_rp q = { 1., 2., 3., 4. };
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
}

BOOST_AUTO_TEST_CASE(list_assign1)
{
  cml::quaterniond_ip q;
  q = { 1., 2., 3., 4. };
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(list_assign2)
{
  cml::quaterniond_rp q;
  q = { 1., 2., 3., 4. };
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
}

BOOST_AUTO_TEST_CASE(combine_set1)
{
  cml::quaterniond_ip q;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  q.set(cml::vector3d(1., 2., 3.), 4.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_set2)
{
  cml::quaterniond_rp q;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  q.set(cml::vector3d(1., 2., 3.), 4.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_set3)
{
  cml::quaterniond_ip q;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  q.set(4., cml::vector3d(1., 2., 3.));
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(combine_set4)
{
  cml::quaterniond_rp q;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  q.set(4., cml::vector3d(1., 2., 3.));
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 4.);
}

BOOST_AUTO_TEST_CASE(size_check1)
{
  cml::quaterniond_rp q;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_REQUIRE_THROW(
    (q = { 1., 2., 3. }), cml::quaternion_size_error);
}

BOOST_AUTO_TEST_CASE(coordinates1)
{
  double data[] = { 1., 2., 3., 4. };
  const cml::quaterniond_ip q(data);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q.x(), 1.);
  BOOST_CHECK_EQUAL(q.y(), 2.);
  BOOST_CHECK_EQUAL(q.z(), 3.);
  BOOST_CHECK_EQUAL(q.w(), 4.);
}

BOOST_AUTO_TEST_CASE(coordinates2)
{
  double data[] = { 1., 2., 3., 4. };
  const cml::quaterniond_rp q(data);
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  BOOST_CHECK_EQUAL(q.x(), 2.);
  BOOST_CHECK_EQUAL(q.y(), 3.);
  BOOST_CHECK_EQUAL(q.z(), 4.);
  BOOST_CHECK_EQUAL(q.w(), 1.);
}

BOOST_AUTO_TEST_CASE(coordinate_assign1)
{
  cml::quaterniond_ip q;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  q.x() = 1.;
  q.y() = 2.;
  q.z() = 3.;
  q.w() = 4.;
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::X], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::Y], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::Z], 3.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_ip::W], 4.);
}

BOOST_AUTO_TEST_CASE(coordinate_assign2)
{
  cml::quaterniond_rp q;
  BOOST_REQUIRE_EQUAL(q.size(), 4);
  q.w() = 1.;
  q.x() = 2.;
  q.y() = 3.;
  q.z() = 4.;
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::W], 1.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::X], 2.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::Y], 3.);
  BOOST_CHECK_EQUAL(q[cml::quaterniond_rp::Z], 4.);
}

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2