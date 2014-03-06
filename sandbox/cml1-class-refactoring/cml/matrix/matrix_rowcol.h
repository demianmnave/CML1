/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief Expressions to extract a row or column of a matrix.
 */

#ifndef matrix_rowcol_h
#define matrix_rowcol_h

#include <cml/vector/vector_expr.h>
#include <cml/matrix/matrix_expr.h>

namespace cml {
namespace et {

/* Forward declarations: */
template<class ExprT> class MatrixRowOp;
template<class ExprT> class MatrixColOp;

/** Expression traits class for MatrixRowOp<>. */
template<class ExprT>
struct ExprTraits< MatrixRowOp<ExprT> >
{
    /* MatrixRowOp traits: */
    typedef MatrixRowOp<ExprT>                          expr_type;
    typedef expr_type                                   reference;
    typedef expr_type                                   const_reference;
    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::not_assignable_tag                 assignable_tag;
    typedef expr_node_tag                               node_tag;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::value_type             value_type;
    typedef typename sub_traits::result_type            matrix_result_type;
    typedef typename matrix_result_type::row_vector_type result_type;

    /* Result type traits: */
    typedef ExprTraits<result_type>                     result_traits;
    typedef typename result_traits::size_tag            size_tag;

    enum { array_size = result_traits::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& e) const { return e.size(); }
};

template<class ExprT>
class MatrixRowOp
{
  public:

    typedef MatrixRowOp<ExprT>                          expr_type;

    /* Copy the expression by value into higher-up expressions: */
    typedef expr_type                                   expr_reference;
    typedef expr_type                                   expr_const_reference;

    /* MatrixRowOp traits: */
    typedef ExprTraits<expr_type>                       expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::result_type           result_type;
    typedef typename result_type::temporary_type        temporary_type;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::const_reference        sub_reference;


  public:

    /** Compute value at index i of the row vector. */
    value_type operator[](size_t i) const {
        return sub_traits().get(m_expr,m_row,i);
    }


  public:

    /** The length of the row is the number of matrix columns. */
    size_t size() const {
        return sub_traits().cols(m_expr);
    }

    /** Return the result as a normalized vector. */
    result_type normalize() const {
        result_type v(VectorXpr<expr_type>(*this));
        return v.normalize();
    }

    /** Return reference to contained expression. */
    expr_reference expression() const { return m_expr; }


  public:

    /** Construct from the subexpression to store and the index of the row
     * to extract.
     */
    MatrixRowOp(const ExprT& expr, size_t row)
        : m_expr(expr), m_row(row) {}

    /** Copy constructor. */
    MatrixRowOp(const expr_type& e)
        : m_expr(e.m_expr), m_row(e.m_row) {}


  protected:

    sub_reference               m_expr;
    const size_t                m_row;


  private:

    /* Cannot be assigned to: */
    expr_type& operator=(const expr_type&);
};

/** Expression traits class for MatrixColOp<>. */
template<class ExprT>
struct ExprTraits< MatrixColOp<ExprT> >
{
    /* MatrixColOp traits: */
    typedef MatrixColOp<ExprT>                          expr_type;
    typedef expr_type                                   reference;
    typedef expr_type                                   const_reference;
    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::not_assignable_tag                 assignable_tag;
    typedef expr_node_tag                               node_tag;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::value_type             value_type;
    typedef typename sub_traits::result_type            matrix_result_type;
    typedef typename matrix_result_type::col_vector_type result_type;

    /* Result type traits: */
    typedef ExprTraits<result_type>                     result_traits;
    typedef typename result_traits::size_tag            size_tag;

    enum { array_size = result_traits::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& e) const { return e.size(); }
};

template<class ExprT>
class MatrixColOp
{
  public:

    typedef MatrixColOp<ExprT>                          expr_type;

    /* Copy the expression by value into higher-up expressions: */
    typedef expr_type                                   expr_reference;
    typedef expr_type                                   expr_const_reference;

    /* MatrixColOp traits: */
    typedef ExprTraits<expr_type>                       expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::result_type           result_type;
    typedef typename result_type::temporary_type        temporary_type;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::const_reference        sub_reference;


  public:

    /** Compute value at index i of the column vector. */
    value_type operator[](size_t i) const {
        return sub_traits().get(m_expr,i,m_col);
    }


  public:

    /** The length of the column is the number of matrix rows. */
    size_t size() const {
        return sub_traits().rows(m_expr);
    }

    /** Return the result as a normalized vector. */
    result_type normalize() const {
        result_type v(VectorXpr<expr_type>(*this));
        return v.normalize();
    }

    /** Return a reference to contained expression. */
    expr_reference expression() const { return m_expr; }


  public:

    /** Construct from the subexpression to store and the index of the
     * column to extract.
     */
    MatrixColOp(const ExprT& expr, size_t col)
        : m_expr(expr), m_col(col) {}

    /** Copy constructor. */
    MatrixColOp(const expr_type& e)
        : m_expr(e.m_expr), m_col(e.m_col) {}


  protected:

    sub_reference               m_expr;
    const size_t                m_col;


  private:

    /* Cannot be assigned to: */
    expr_type& operator=(const expr_type&);
};

} // namespace et

/* Define the row and column operators in the cml namespace: */

/** Matrix row operator taking a matrix operand. */
template<typename E, class AT, typename BO, typename L>
et::VectorXpr< et::MatrixRowOp< matrix<E,AT,BO,L> > >
row(const matrix<E,AT,BO,L>& expr, size_t i)
{
    typedef et::MatrixRowOp< matrix<E,AT,BO,L> > ExprT;
    return et::VectorXpr<ExprT>(ExprT(expr,i));
}

/** Matrix row operator taking an et::MatrixXpr operand.
 *
 * The parse tree is automatically compressed by hoisting the MatrixXpr's
 * subexpression into the subexpression of the MatrixRowOp.
 */
template<class XprT>
et::VectorXpr< et::MatrixRowOp<XprT> >
row(const et::MatrixXpr<XprT>& expr, size_t i)
{
    typedef et::MatrixRowOp<XprT> ExprT;
    return et::MatrixXpr<ExprT>(ExprT(expr.expression(),i));
}

/** Matrix col operator taking a matrix operand. */
template<typename E, class AT, typename BO, typename L>
et::VectorXpr< et::MatrixColOp< matrix<E,AT,BO,L> > >
col(const matrix<E,AT,BO,L>& expr, size_t i)
{
    typedef et::MatrixColOp< matrix<E,AT,BO,L> > ExprT;
    return et::VectorXpr<ExprT>(ExprT(expr,i));
}

/** Matrix col operator taking an et::MatrixXpr operand.
 *
 * The parse tree is automatically compressed by hoisting the MatrixXpr's
 * subexpression into the subexpression of the MatrixColOp.
 */
template<class XprT>
et::VectorXpr< et::MatrixColOp<XprT> >
col(const et::MatrixXpr<XprT>& expr, size_t i)
{
    typedef et::MatrixColOp<XprT> ExprT;
    return et::VectorXpr<ExprT>(ExprT(expr.expression(),i));
}

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
