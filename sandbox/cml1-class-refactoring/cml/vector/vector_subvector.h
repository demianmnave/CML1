/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#ifndef vector_subvector_h
#define vector_subvector_h

#include <cml/vector/vector_expr.h>

namespace cml {
namespace et {

/* Forward declarations: */
template<class ExprT> class SubvectorOp;

/** Expression traits class for SubvectorOp<>. */
template<class ExprT>
struct ExprTraits< SubvectorOp<ExprT> >
{
    /* SubvectorOp traits: */
    typedef SubvectorOp<ExprT>                          expr_type;
    typedef expr_type                                   reference;
    typedef expr_type                                   const_reference;
    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::not_assignable_tag                 assignable_tag;
    typedef expr_node_tag                               node_tag;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::value_type             value_type;
    typedef typename sub_traits::result_type            sub_result_type;
    typedef typename sub_result_type::subvector_type    result_type;

    /* Result type traits: */
    typedef ExprTraits<result_type>                     result_traits;
    typedef typename result_traits::size_tag            size_tag;

    enum { array_size = result_traits::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& e) const { return e.size(); }
};

/** Expression template node for computing a subvector of a vector by
 * removing an element at a specified index.
 */
template<class ExprT>
class SubvectorOp
{
  public:

    typedef SubvectorOp<ExprT>                          expr_type;

    /* Copy the expression by value into higher-up expressions: */
    typedef expr_type                                   expr_reference;
    typedef expr_type                                   expr_const_reference;

    /* SubvectorOp traits: */
    typedef ExprTraits<expr_type>                       expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::result_type           result_type;
    typedef typename result_type::temporary_type        temporary_type;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::const_reference        sub_reference;


  public:

    /** Compute value at index i. */
    value_type operator[](size_t i) const {
        return m_expr[(i < m_skip) ? i : (i+1)];
    }


  public:

    size_t size() const {
        return sub_traits().size(m_expr)-1;
    }

    /** Return the result as a normalized vector. */
    result_type normalize() const {
        result_type v(VectorXpr<expr_type>(*this));
        return v.normalize();
    }

    /** Return reference to contained expression. */
    expr_reference expression() const { return m_expr; }


  public:

    /** Construct from the subexpression to store and the index of the
     * value to skip.
     */
    SubvectorOp(const ExprT& expr, size_t skip)
        : m_expr(expr), m_skip(skip) {}

    /** Copy constructor. */
    SubvectorOp(const expr_type& e)
        : m_expr(e.m_expr), m_skip(e.m_skip) {}


  protected:

    sub_reference               m_expr;
    const size_t                m_skip;


  private:

    /* Cannot be assigned to: */
    expr_type& operator=(const expr_type&);
};

} // namespace et

/* Define the subvector operator in the cml namespace: */

/** Subvector operator taking a vector operand. */
template<typename E, class AT>
et::VectorXpr< et::SubvectorOp< vector<E,AT> > >
subvector(const vector<E,AT>& expr, size_t i)
{
    typedef et::SubvectorOp< vector<E,AT> > ExprT;
    return et::VectorXpr<ExprT>(ExprT(expr,i));
}

/** Subvector operator taking an et::VectorXpr operand.
 *
 * The parse tree is automatically compressed by hoisting the VectorXpr's
 * subexpression into the subexpression of the SubvectorOp.
 */
template<class XprT>
et::VectorXpr< et::SubvectorOp<XprT> >
subvector(const et::VectorXpr<XprT>& expr, size_t i)
{
    typedef et::SubvectorOp<XprT> ExprT;
    return et::VectorXpr<ExprT>(ExprT(expr.expression(),i));
}

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2
