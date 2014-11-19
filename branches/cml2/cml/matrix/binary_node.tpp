/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#ifndef __CML_MATRIX_BINARY_NODE_TPP
#error "matrix/binary_node.tpp not included correctly"
#endif

#include <cml/matrix/size_checking.h>

namespace cml {

/* matrix_binary_node 'structors: */

template<class Sub1, class Sub2, class Op>
matrix_binary_node<Sub1,Sub2,Op>::matrix_binary_node(Sub1 left, Sub2 right)
: m_left(std::move(left)), m_right(std::move(right))
{
  cml::check_same_size(this->m_left, this->m_right);
}

template<class Sub1, class Sub2, class Op>
matrix_binary_node<Sub1,Sub2,Op>::matrix_binary_node(node_type&& other)
: m_left(std::move(other.m_left)), m_right(std::move(other.m_right))
{
}



/* Public methods: */

template<class Sub1, class Sub2, class Op> int
matrix_binary_node<Sub1,Sub2,Op>::rows() const
{
  return this->m_left.rows();
}

template<class Sub1, class Sub2, class Op> int
matrix_binary_node<Sub1,Sub2,Op>::cols() const
{
  return this->m_left.cols();
}

template<class Sub1, class Sub2, class Op> auto
matrix_binary_node<Sub1,Sub2,Op>::get(int i, int j) const -> immutable_value
{
  return Op().apply(this->m_left.get(i,j), this->m_right.get(i,j));
}

} // namespace cml

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2
