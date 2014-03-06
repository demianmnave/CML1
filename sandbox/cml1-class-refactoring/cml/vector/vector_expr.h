/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief Vector linear expression classes.
 */

#ifndef vector_expr_h
#define vector_expr_h

#include <cmath>
#include <cml/et/size_checking.h>
#include <cml/vector/vector_traits.h>
#include <cml/vector/vector_promotions.h>

/* XXX Don't know which it should be just yet, since RVO seems to obviate
 * need for a reference type.  However, copy by value copies the entire
 * expression tree rooted at the VectorXpr<>, so this choice is bound to
 * affect performace for some compiler or another:
 */
#define VECXPR_ARG_TYPE  const et::VectorXpr<XprT>&
#define VECXPR_ARG_TYPE_N(_N_)  const et::VectorXpr<XprT##_N_>&

//#define VECXPR_ARG_TYPE         const et::VectorXpr<XprT>
//#define VECXPR_ARG_TYPE_N(_N_)  const et::VectorXpr<XprT##_N_>

namespace cml {
namespace et {

/* Forward declarations: */
template<class ExprT> class VectorXpr;
template<class ExprT, class OpT> class UnaryVectorOp;
template<class LeftT, class RightT, class OpT> class BinaryVectorOp;

template<class Derived>
class VectorExprBase
{
  public:

    /* Expression traits for the derived class: */
    typedef ExprTraits<Derived>                         expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::result_type           result_type;
    typedef typename result_type::temporary_type        temporary_type;


  public:

    /** Return square of the length. */
    value_type length_squared() const {
        return cml::dot(
                VectorXpr<Derived>((const Derived&) *this),
                VectorXpr<Derived>((const Derived&) *this));
    }

    /** Return the length. */
    value_type length() const {
        return std::sqrt(length_squared());
    }

    /** Return the result as a normalized vector. */
    result_type normalize() const {
        result_type v(VectorXpr<Derived>((const Derived&) *this));
        return v.normalize();
    }
};

/** Expression traits class for VectorXpr<>. */
template<class ExprT>
struct ExprTraits< VectorXpr<ExprT> >
{
    /* VectorXpr traits: */
    typedef VectorXpr<ExprT>                            expr_type;
    typedef expr_type                                   reference;
    typedef expr_type                                   const_reference;
    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::not_assignable_tag                 assignable_tag;
    typedef expr_node_tag                               node_tag;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::value_type             value_type;
    typedef typename sub_traits::result_type            result_type;

    /* Result type traits: */
    typedef ExprTraits<result_type>                     result_traits;
    typedef typename result_traits::size_tag            size_tag;

    enum { array_size = result_traits::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& e) const { return e.size(); }
};

/** A placeholder for a vector expression in an expression tree. */
template<class ExprT>
class VectorXpr
{
  public:

    typedef VectorXpr<ExprT>                            expr_type;

    /* Copy the expression by value into higher-up expressions: */
    typedef expr_type                                   expr_reference;
    typedef expr_type                                   expr_const_reference;

    /* VectorXpr traits: */
    typedef ExprTraits<expr_type>                       expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::result_type           result_type;
    typedef typename result_type::temporary_type        temporary_type;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::const_reference        sub_reference;


  public:

    /** Compute value at index i of the result vector. */
    value_type operator[](size_t i) const {
        return sub_traits().get(m_expr,i);
    }


  public:

    /** Return square of the length. */
    value_type length_squared() const {
        return m_expr.length_squared();
    }

    /** Return the length. */
    value_type length() const {
        return m_expr.length();
    }

    /** Return the result as a normalized vector. */
    result_type normalize() const {
        return m_expr.normalize();
    }


  public:

    /** Return size of this expression (same as subexpression's size). */
    size_t size() const {
        return sub_traits().size(m_expr);
    }

    /** Return reference to contained expression. */
    sub_reference expression() const { return m_expr; }


  public:

    /** Construct from the subexpression to store. */
    VectorXpr(sub_reference expr) : m_expr(expr) {}

    /** Copy constructor. */
    VectorXpr(const expr_type& e) : m_expr(e.m_expr) {}


  protected:

    sub_reference               m_expr;


  private:

    /* Cannot be assigned to: */
    expr_type& operator=(const expr_type&);
};


/** Expression traits class for UnaryVectorOp<>. */
template<class ExprT, class OpT>
struct ExprTraits< UnaryVectorOp<ExprT,OpT> >
{
    /* UnaryVectorOp traits: */
    typedef UnaryVectorOp<ExprT,OpT>                    expr_type;
    typedef typename OpT::value_type                    value_type;
    typedef expr_type                                   reference;
    typedef expr_type                                   const_reference;
    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::not_assignable_tag                 assignable_tag;
    typedef expr_node_tag                               node_tag;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::result_type            result_type;

    /* Result type traits: */
    typedef ExprTraits<result_type>                     result_traits;
    typedef typename result_traits::size_tag            size_tag;

    enum { array_size = result_traits::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& e) const { return e.size(); }
};

/** Unary operator in a vector expression tree. */
template<class ExprT, class OpT>
class UnaryVectorOp
: public VectorExprBase< UnaryVectorOp<ExprT,OpT> >
{
  public:

    /* Shorthand for the expression type: */
    typedef UnaryVectorOp<ExprT,OpT>                    expr_type;

    /* UnaryVectorOp traits: */
    typedef ExprTraits<expr_type>                       expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::result_type           result_type;
    typedef typename result_type::temporary_type        temporary_type;

    /* Subexpression traits: */
    typedef ExprTraits<ExprT>                           sub_traits;
    typedef typename sub_traits::const_reference        sub_reference;


  public:

    /** Compute value at index i of the result vector. */
    value_type operator[](size_t i) const {
        return OpT().apply(sub_traits().get(m_expr,i));
    }


  public:

    /** Return size of this expression (same as argument's size). */
    size_t size() const {
        return sub_traits().size(m_expr);
    }

    /** Return reference to contained expression. */
    sub_reference expression() const { return m_expr; }


  public:

    /** Construct from the subexpression. */
    UnaryVectorOp(sub_reference expr) : m_expr(expr) {}

    /** Copy constructor. */
    UnaryVectorOp(const expr_type& e) : m_expr(e.m_expr) {}


  protected:

    sub_reference               m_expr;


  private:

    /* Cannot be assigned to: */
    expr_type& operator=(const expr_type&);
};


/** Expression traits for BinaryVectorOp. */
template<class LeftT, class RightT, class OpT>
struct ExprTraits< BinaryVectorOp<LeftT,RightT,OpT> >
{
    /* BinaryVectorOp traits: */
    typedef BinaryVectorOp<LeftT,RightT,OpT>            expr_type;
    typedef typename OpT::value_type                    value_type;
    typedef expr_type                                   reference;
    typedef expr_type                                   const_reference;
    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::not_assignable_tag                 assignable_tag;
    typedef expr_node_tag                               node_tag;

    /* Left subexpression traits: */
    typedef ExprTraits<LeftT>                           left_traits;
    typedef typename left_traits::const_reference       left_reference;

    /* Right subexpression traits: */
    typedef ExprTraits<RightT> right_traits;
    typedef typename right_traits::const_reference      right_reference;

    /* Result type traits: */
    typedef typename VectorPromote<
        typename left_traits::result_type,
        typename right_traits::result_type>::type       result_type;
    typedef ExprTraits<result_type>                     result_traits;
    typedef typename result_traits::size_tag            size_tag;

    enum { array_size = result_traits::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& e) const { return e.size(); }
};

/** Binary operator in a vector expression tree. */
template<class LeftT, class RightT, class OpT>
class BinaryVectorOp
: public VectorExprBase< BinaryVectorOp<LeftT,RightT,OpT> >
{
  public:

    /* Shorthand for the expression type: */
    typedef BinaryVectorOp<LeftT,RightT,OpT>            expr_type;

    /* Copy the expression by value into higher-up expressions: */
    typedef expr_type                                   expr_reference;
    typedef expr_type                                   expr_const_reference;

    /* BinaryVectorOp traits: */
    typedef ExprTraits<expr_type>                       expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::result_type           result_type;
    typedef typename result_type::temporary_type        temporary_type;

    /* Left subexpression traits: */
    typedef ExprTraits<LeftT>                           left_traits;
    typedef typename left_traits::const_reference       left_reference;

    /* Right subexpression traits: */
    typedef ExprTraits<RightT>                          right_traits;
    typedef typename right_traits::const_reference      right_reference;

    /* Define a size checker: */
    typedef typename expr_traits::size_tag              size_tag;
    typedef GetCheckedSize<LeftT,RightT,size_tag>       checked_size;


  public:

    /** Compute value at index i of the result vector. */
    value_type operator[](size_t i) const {
        return OpT().apply(
                left_traits().get(m_left,i),
                right_traits().get(m_right,i));
    }


  public:

    /** Return the size of the vector result.
     *
     * @throws std::invalid_argument if the expressions do not have the same
     * size and CML_CHECK_VECTOR_EXPR_SIZES is defined.
     */
    size_t size() const {
        return CheckedSize(m_left,m_right,size_tag());
    }

    /** Return reference to left expression. */
    left_reference left_expression() const { return m_left; }

    /** Return reference to right expression. */
    right_reference right_expression() const { return m_right; }


  public:

    /** Construct from the two subexpressions. */
    BinaryVectorOp(left_reference left, right_reference right)
        : m_left(left), m_right(right) {}

    /** Copy constructor. */
    BinaryVectorOp(const expr_type& e)
        : m_left(e.m_left), m_right(e.m_right) {}


  protected:

    left_reference              m_left;
    right_reference             m_right;


  private:

    /* This ensures that a compile-time size check is executed: */
    typedef typename checked_size::check_type _dummy;


  private:

    /* Cannot be assigned to: */
    expr_type& operator=(const expr_type&);
};


/* Helper struct to verify that both arguments are vector expressions: */
template<typename LeftT, typename RightT>
struct VectorExpressions
{
    /* Left argument result type: */
    typedef et::ExprTraits<LeftT> left_traits;
    typedef typename left_traits::result_tag left_result;

    /* Right argument result type: */
    typedef et::ExprTraits<RightT> right_traits;
    typedef typename right_traits::result_tag right_result;

    /* Require that both arguments are vector expressions: */
    enum { is_true = (same_type<left_result,et::vector_result_tag>::is_true
            && same_type<right_result,et::vector_result_tag>::is_true) };
};

namespace detail {

template<typename VecT, typename RT, typename MT> inline
void Resize(VecT&,size_t,RT,MT) {}

template<typename VecT> inline
void Resize(VecT& v, size_t S, resizable_tag, dynamic_memory_tag) {
    v.resize(S);
}

template<typename VecT> inline
void Resize(VecT& v, size_t S) {
    Resize(v, S, typename VecT::resizing_tag(), typename VecT::memory_tag());
}

} // namespace detail

} // namespace et
} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
