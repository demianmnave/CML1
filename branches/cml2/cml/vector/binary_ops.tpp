/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#ifndef __CML_VECTOR_BINARY_OPS_TPP
#error "vector/binary_ops.tpp not included correctly"
#endif

namespace cml {

/** Helper function to generate a vector_binary_node from two vector types
 * (i.e. derived from readable_vector<>).
 */
template<class Op, class Sub1, class Sub2,
  cml::enable_if_vector_t<Sub1>* = nullptr,
  cml::enable_if_vector_t<Sub2>* = nullptr
> inline auto
make_vector_binary_node(Sub1&& sub1, Sub2&& sub2)
-> vector_binary_node<
actual_operand_type_of_t<decltype(sub1)>,
actual_operand_type_of_t<decltype(sub2)>,
Op
>
{
  static_assert(std::is_same<
    decltype(sub1), decltype(std::forward<Sub1>(sub1))>::value,
    "internal error: unexpected expression type (sub1)");
  static_assert(std::is_same<
    decltype(sub2), decltype(std::forward<Sub2>(sub2))>::value,
    "internal error: unexpected expression type (sub2)");

  /* Deduce the operand types of the subexpressions (&, const&, &&): */
  typedef actual_operand_type_of_t<decltype(sub1)> sub1_type;
  typedef actual_operand_type_of_t<decltype(sub2)> sub2_type;
  return vector_binary_node<
    sub1_type, sub2_type, Op>((sub1_type) sub1, (sub2_type) sub2);
}

template<class Sub1, class Sub2,
  cml::enable_if_vector_t<Sub1>* = nullptr,
  cml::enable_if_vector_t<Sub2>* = nullptr
>
inline auto operator-(Sub1&& sub1, Sub2&& sub2)
-> decltype(make_vector_binary_node<vector_binary_minus<Sub1,Sub2>>(
    std::forward<Sub1>(sub1), std::forward<Sub2>(sub2)))
{
  return make_vector_binary_node<vector_binary_minus<Sub1,Sub2>>
    (std::forward<Sub1>(sub1), std::forward<Sub2>(sub2));
}

template<class Sub1, class Sub2,
  cml::enable_if_vector_t<Sub1>* = nullptr,
  cml::enable_if_vector_t<Sub2>* = nullptr
>
inline auto operator+(Sub1&& sub1, Sub2&& sub2)
-> decltype(make_vector_binary_node<vector_binary_plus<Sub1,Sub2>>(
    std::forward<Sub1>(sub1), std::forward<Sub2>(sub2)))
{
  return make_vector_binary_node<vector_binary_plus<Sub1,Sub2>>
    (std::forward<Sub1>(sub1), std::forward<Sub2>(sub2));
}

} // namespace cml

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2
