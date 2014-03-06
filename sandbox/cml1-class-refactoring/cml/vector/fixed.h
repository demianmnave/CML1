/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief Specialization for fixed-size, fixed-memory vectors.
 */

#ifndef fixed_vector_h
#define fixed_vector_h

#include <cml/core/fixed_1D.h>
#include <cml/vector/vector_base.h>
#include <cml/vector/vector_expr.h>
#include <cml/vector/class_ops.h>
#include <cml/vector/vector_unroller.h>
#include <cml/util.h>

namespace cml {
namespace et {

/** Expression traits for a fixed<> vector type. */
template<typename E, size_t N>
struct ExprTraits< vector<E, fixed<N> > >
{
    typedef typename vector<E, fixed<N> >               expr_type;
    typedef fixed<>                                     storage_type;

    typedef E                                           value_type;
    typedef expr_type&                                  reference;
    typedef const expr_type&                            const_reference;

    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::fixed_size_tag                         size_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::assignable_tag                     assignable_tag;
    typedef expr_leaf_tag                               node_tag;

    typedef expr_type                                   result_type;
    typedef vector< E, fixed<N-1> >                     subvector_type;
    typedef vector< E, fixed<N+1> >                     supervector_type;

    enum { array_size = N };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& v) const { return v.size(); }
};

} // namespace et

/** Fixed-size, fixed-memory vector. */
template<typename Element, int Size>
class vector< Element, fixed<Size> >
: public fixed_1D<Element,Size>
, public assignable_vector_base< vector<Element, fixed<Size> > >
{
  public:

    using assignable_vector_base::operator=;
    using assignable_vector_base::operator+=;
    using assignable_vector_base::operator-=;
    using assignable_vector_base::operator*=;


  public:

    typedef vector< Element, fixed<Size> > self;
    typedef assignable_vector_base<self> super;

    /* Shorthand for the array type: */
    typedef fixed<> storage_type;
    typedef fixed_1D<Element,Size> array_type;

    /* Shorthand for the type of this vector: */
    typedef self vector_type;

    /* The vector coordinate type: */
    typedef Element coordinate_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector_type temporary_type;

    /* The type for a vector in one lower dimension: */
    typedef vector< Element, fixed<Size-1> > subvector_type;

    /* The type for a vector in one higher dimension: */
    typedef vector< Element, fixed<Size+1> > supervector_type;

    /* Standard: */
    typedef typename array_type::value_type value_type;
    typedef typename array_type::reference reference;
    typedef typename array_type::const_reference const_reference;

    /* For integration into the expression templates code: */
    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by result-type: */
    typedef cml::et::vector_result_tag result_tag;

    /* For matching by assignability: */
    typedef cml::et::assignable_tag assignable_tag;


  public:

    /** Static constant containing the vector's space dimension. */
    enum { dimension = Size };


  public:

    vector() {}


  public:

    /* Define common class operators: */

    vector(const subvector_type& s, value_type e) {
        for(size_t i = 0; i < s.size(); ++ i) (*this)[i] = s[i];
        (*this)[s.size()] = e;
    }

    typedef value_type c_array[Size];
    vector(const c_array& v) {
        typedef et::OpAssign<value_type,value_type> OpT;
        et::UnrollAssignment<OpT>(*this, v);
    }

    template<typename E, class AT> vector(const vector<E,AT>& m) {
        typedef et::OpAssign<Element,E> OpT;
        et::UnrollAssignment<OpT>(*this,m);
    }

    template<class XprT> vector(const et::VectorXpr<XprT>& e) {
        /* Verify that a promotion exists at compile time: */
        typedef typename et::VectorPromote<
            vector_type, typename XprT::result_type>::type result_type;
        typedef typename XprT::value_type src_value_type;
        typedef et::OpAssign<Element,src_value_type> OpT;
        et::UnrollAssignment<OpT>(*this, e);
    }
};

/** 2-element, fixed-memory vector. */
template<typename Element>
class vector< Element, fixed<2> >
: public fixed_1D<Element,2>
, public assignable_vector_base< vector<Element, fixed<2> > >
{
  public:

    using assignable_vector_base::operator=;
    using assignable_vector_base::operator+=;
    using assignable_vector_base::operator-=;
    using assignable_vector_base::operator*=;


  public:

    typedef vector< Element, fixed<2> > self;
    typedef assignable_vector_base<self> super;

    /* Shorthand for the array type: */
    typedef fixed<> storage_type;
    typedef fixed_1D<Element,2> array_type;

    /* Shorthand for the type of this vector: */
    typedef self vector_type;

    /* The vector coordinate type: */
    typedef Element coordinate_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector_type temporary_type;

    /* The type for a vector in one lower dimension: */
    typedef vector< Element, fixed<2-1> > subvector_type;

    /* The type for a vector in one higher dimension: */
    typedef vector< Element, fixed<2+1> > supervector_type;

    /* Standard: */
    typedef typename array_type::value_type value_type;
    typedef typename array_type::reference reference;
    typedef typename array_type::const_reference const_reference;

    /* For integration into the expression templates code: */
    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by result-type: */
    typedef cml::et::vector_result_tag result_tag;

    /* For matching by assignability: */
    typedef cml::et::assignable_tag assignable_tag;


  public:

    /** Static constant containing the vector's space dimension. */
    enum { dimension = 2 };


  public:

    vector() {}

    vector(value_type a, value_type b) {
        (*this)[0] = a;
        (*this)[1] = b;
    }


  public:

    /* Define common class operators: */

    vector(const subvector_type& s, value_type e) {
        for(size_t i = 0; i < s.size(); ++ i) (*this)[i] = s[i];
        (*this)[s.size()] = e;
    }

    typedef value_type c_array[2];
    vector(const c_array& v) {
        typedef et::OpAssign<value_type,value_type> OpT;
        et::UnrollAssignment<OpT>(*this, v);
    }

    template<typename E, class AT> vector(const vector<E,AT>& m) {
        typedef et::OpAssign<Element,E> OpT;
        et::UnrollAssignment<OpT>(*this,m);
    }

    template<class XprT> vector(const et::VectorXpr<XprT>& e) {
        /* Verify that a promotion exists at compile time: */
        typedef typename et::VectorPromote<
            vector_type, typename XprT::result_type>::type result_type;
        typedef typename XprT::value_type src_value_type;
        typedef et::OpAssign<Element,src_value_type> OpT;
        et::UnrollAssignment<OpT>(*this, e);
    }
};


/** 3-element, fixed-memory vector. */
template<typename Element>
class vector< Element, fixed<3> >
: public fixed_1D<Element,3>
, public assignable_vector_base< vector<Element, fixed<3> >
>
{
  public:

    using assignable_vector_base::operator=;
    using assignable_vector_base::operator+=;
    using assignable_vector_base::operator-=;
    using assignable_vector_base::operator*=;


  public:

    typedef vector< Element, fixed<3> > self;
    typedef assignable_vector_base<self> super;

    /* Shorthand for the array type: */
    typedef fixed<> storage_type;
    typedef fixed_1D<Element,3> array_type;

    /* Shorthand for the type of this vector: */
    typedef self vector_type;

    /* The vector coordinate type: */
    typedef Element coordinate_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector_type temporary_type;

    /* The type for a vector in one lower dimension: */
    typedef vector< Element, fixed<3-1> > subvector_type;

    /* The type for a vector in one higher dimension: */
    typedef vector< Element, fixed<3+1> > supervector_type;

    /* Standard: */
    typedef typename array_type::value_type value_type;
    typedef typename array_type::reference reference;
    typedef typename array_type::const_reference const_reference;

    /* For integration into the expression templates code: */
    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by result-type: */
    typedef cml::et::vector_result_tag result_tag;

    /* For matching by assignability: */
    typedef cml::et::assignable_tag assignable_tag;


  public:

    /** Static constant containing the vector's space dimension. */
    enum { dimension = 3 };


  public:

    vector() {}

    vector(value_type a, value_type b, value_type c) {
        (*this)[0] = a;
        (*this)[1] = b;
        (*this)[2] = c;
    }


  public:

    /* Define common class operators: */

    vector(const subvector_type& s, value_type e) {
        for(size_t i = 0; i < s.size(); ++ i) (*this)[i] = s[i];
        (*this)[s.size()] = e;
    }

    typedef value_type c_array[3];
    vector(const c_array& v) {
        typedef et::OpAssign<value_type,value_type> OpT;
        et::UnrollAssignment<OpT>(*this, v);
    }

    template<typename E, class AT> vector(const vector<E,AT>& m) {
        typedef et::OpAssign<Element,E> OpT;
        et::UnrollAssignment<OpT>(*this,m);
    }

    template<class XprT> vector(const et::VectorXpr<XprT>& e) {
        /* Verify that a promotion exists at compile time: */
        typedef typename et::VectorPromote<
            vector_type, typename XprT::result_type>::type result_type;
        typedef typename XprT::value_type src_value_type;
        typedef et::OpAssign<Element,src_value_type> OpT;
        et::UnrollAssignment<OpT>(*this, e);
    }
};

/** 4-element, fixed-memory vector. */
template<typename Element>
class vector< Element, fixed<4> >
: public fixed_1D<Element,4>
, public assignable_vector_base< vector<Element, fixed<4> >
>
{
  public:

    using assignable_vector_base::operator=;
    using assignable_vector_base::operator+=;
    using assignable_vector_base::operator-=;
    using assignable_vector_base::operator*=;


  public:

    typedef vector< Element, fixed<4> > self;
    typedef assignable_vector_base<self> super;

    /* Shorthand for the array type: */
    typedef fixed<> storage_type;
    typedef fixed_1D<Element,4> array_type;

    /* Shorthand for the type of this vector: */
    typedef self vector_type;

    /* The vector coordinate type: */
    typedef Element coordinate_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector_type temporary_type;

    /* The type for a vector in one lower dimension: */
    typedef vector< Element, fixed<4-1> > subvector_type;

    /* The type for a vector in one higher dimension: */
    typedef vector< Element, fixed<4+1> > supervector_type;

    /* Standard: */
    typedef typename array_type::value_type value_type;
    typedef typename array_type::reference reference;
    typedef typename array_type::const_reference const_reference;

    /* For integration into the expression templates code: */
    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by result-type: */
    typedef cml::et::vector_result_tag result_tag;

    /* For matching by assignability: */
    typedef cml::et::assignable_tag assignable_tag;


  public:

    /** Static constant containing the vector's space dimension. */
    enum { dimension = 4 };


  public:

    vector() {}

    vector(value_type a, value_type b, value_type c, value_type d) {
        (*this)[0] = a;
        (*this)[1] = b;
        (*this)[2] = c;
        (*this)[3] = d;
    }


  public:

    /* Define common class operators: */

    vector(const subvector_type& s, value_type e) {
        for(size_t i = 0; i < s.size(); ++ i) (*this)[i] = s[i];
        (*this)[s.size()] = e;
    }

    typedef value_type c_array[4];
    vector(const c_array& v) {
        typedef et::OpAssign<value_type,value_type> OpT;
        et::UnrollAssignment<OpT>(*this, v);
    }

    template<typename E, class AT> vector(const vector<E,AT>& m) {
        typedef et::OpAssign<Element,E> OpT;
        et::UnrollAssignment<OpT>(*this,m);
    }

    template<class XprT> vector(const et::VectorXpr<XprT>& e) {
        /* Verify that a promotion exists at compile time: */
        typedef typename et::VectorPromote<
            vector_type, typename XprT::result_type>::type result_type;
        typedef typename XprT::value_type src_value_type;
        typedef et::OpAssign<Element,src_value_type> OpT;
        et::UnrollAssignment<OpT>(*this, e);
    }
};

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
