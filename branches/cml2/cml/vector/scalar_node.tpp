/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#ifndef __CML_VECTOR_SCALAR_NODE_TPP
#error "vector/scalar_node.tpp not included correctly"
#endif

namespace cml {

/* vector_scalar_node 'structors: */

template<class Sub, class Scalar, class Op>
vector_scalar_node<Sub,Scalar,Op>::vector_scalar_node(
  Sub left, const right_type& right
  )
: m_left(std::move(left)), m_right(right)
{
}

template<class Sub, class Scalar, class Op>
vector_scalar_node<Sub,Scalar,Op>::vector_scalar_node(node_type&& other)
: m_left(std::move(other.m_left)), m_right(std::move(other.m_right))
{
}



/* Public methods: */

template<class Sub, class Scalar, class Op> int
vector_scalar_node<Sub,Scalar,Op>::size() const
{
  return this->m_left.size();
}

template<class Sub, class Scalar, class Op> auto
vector_scalar_node<Sub,Scalar,Op>::get(int i) const -> immutable_value
{
  return Op().apply(this->m_left.get(i), this->m_right);
}

} // namespace cml

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2
