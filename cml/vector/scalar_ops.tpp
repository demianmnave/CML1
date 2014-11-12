/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#ifndef __CML_VECTOR_SCALAR_OPS_TPP
#error "vector/scalar_ops.tpp not included correctly"
#endif

namespace cml {

/* Boilerplate for scalar-vector multiply: */
__CML_MAKE_RIGHT_VECTOR_SCALAR_OPERATORS( * , scalar_multiply_type)

/* Boilerplate for vector-scalar multiply: */
__CML_MAKE_LEFT_VECTOR_SCALAR_OPERATORS( * , scalar_multiply_type)

/* Boilerplate for vector-scalar divide: */
__CML_MAKE_LEFT_VECTOR_SCALAR_OPERATORS( / , scalar_divide_type)

} // namespace cml

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2