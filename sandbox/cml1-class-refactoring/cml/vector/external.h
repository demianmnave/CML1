/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief Specializations for external-memory vectors.
 *
 *  @note Copy-constructing one external<> vector from another is not
 *  supported, since an external<> vector is essentially a wrapper for a
 *  pointer and has no allocated storage of its own.
 */

#ifndef external_vector_h
#define external_vector_h

#include <cml/core/external_1D.h>
#include <cml/vector/vector_base.h>
#include <cml/vector/vector_expr.h>
#include <cml/vector/class_ops.h>
#include <cml/vector/vector_unroller.h>
#include <cml/vector/dynamic.h>

namespace cml {
namespace et {

/** Expression traits for a fixed-size external<> vector type. */
template<typename E, size_t N>
struct ExprTraits< vector<E, external<N> > >
{
    typedef typename vector<E, external<N> >            expr_type;
    typedef external<>                                  storage_type;

    typedef E                                           value_type;
    typedef expr_type&                                  reference;
    typedef const expr_type&                            const_reference;

    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::fixed_size_tag                         size_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::assignable_tag                     assignable_tag;
    typedef expr_leaf_tag                               node_tag;

    typedef vector< E, fixed<N> >                       result_type;
    typedef vector< E, fixed<N-1> >                     subvector_type;
    typedef vector< E, fixed<N+1> >                     supervector_type;

    enum { array_size = N };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& v) const { return v.size(); }
};

/** Expression traits for a runtime-sized external<> vector type. */
template<typename E>
struct ExprTraits< vector<E, external<> > >
{
    typedef typename vector<E, external<> >             expr_type;
    typedef external<>                                  storage_type;

    typedef E                                           value_type;
    typedef expr_type&                                  reference;
    typedef const expr_type&                            const_reference;

    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::fixed_size_tag                         size_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::assignable_tag                     assignable_tag;
    typedef expr_leaf_tag                               node_tag;

    typedef vector< E, dynamic<> >                      result_type;
    typedef vector< E, dynamic<> >                      subvector_type;
    typedef vector< E, dynamic<> >                      supervector_type;

    enum { array_size = result_type::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& v) const { return v.size(); }
};

} // namespace et

/** Fixed-size, external-memory vector. */
template<typename Element, int Size>
class vector< Element, external<Size> >
: public external_1D<Element,Size>
, public assignable_vector_base< vector<Element, external<Size> > >
{
  public:

    using assignable_vector_base::operator=;
    using assignable_vector_base::operator+=;
    using assignable_vector_base::operator-=;
    using assignable_vector_base::operator*=;


  public:

    typedef vector< Element, external<Size> > self;
    typedef assignable_vector_base<self> super;

    /* Shorthand for the array type: */
    typedef external<> storage_type;
    typedef external_1D<Element,Size> array_type;

    /* Shorthand for the type of this vector: */
    typedef self vector_type;

    /* The vector coordinate type: */
    typedef Element coordinate_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector<typename cml::remove_const<Element>::type,
            fixed<Size> > temporary_type;
    /* Note: this ensures that an external vector is copied into the proper
     * temporary; external<> temporaries are not allowed.
     */

    /* The type for a vector in one lower dimension: */
    typedef typename temporary_type::subvector_type subvector_type;

    /* Standard: */
    typedef typename array_type::value_type value_type;
    typedef typename array_type::reference reference;
    typedef typename array_type::const_reference const_reference;

    /* For integration into the expression templates code: */
    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by storage type: */
    typedef typename array_type::memory_tag memory_tag;

    /* For matching by size type: */
    typedef typename array_type::size_tag size_tag;

    /* For matching by result-type: */
    typedef cml::et::vector_result_tag result_tag;

    /* For matching by assignability: */
    typedef cml::et::assignable_tag assignable_tag;


  public:

    /** Static constant containing the vector's space dimension. */
    enum { dimension = Size };


  public:

    /** Construct from a pointer to an array of values. */
    vector(Element* const array) : array_type(array) {}
};

/** Run-time sized vector. */
template<typename Element>
class vector< Element, external<> >
: public external_1D<Element>
, public assignable_vector_base< vector<Element, external<> > >
{
  public:

    using assignable_vector_base::operator=;
    using assignable_vector_base::operator+=;
    using assignable_vector_base::operator-=;
    using assignable_vector_base::operator*=;


  public:

    typedef vector< Element, external<> > self;
    typedef assignable_vector_base<self> super;

    /* Shorthand for the array type: */
    typedef external<> storage_type;
    typedef external_1D<Element> array_type;

    /* Shorthand for the type of this vector: */
    typedef self vector_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector<typename cml::remove_const<Element>::type,
            dynamic<> > temporary_type;
    /* Note: this ensures that an external vector is copied into the proper
     * temporary; external<> temporaries are not allowed.
     */

    /* The type for a vector in one lower dimension: */
    typedef typename temporary_type::subvector_type subvector_type;

    /* Standard: */
    typedef typename array_type::value_type value_type;
    typedef typename array_type::reference reference;
    typedef typename array_type::const_reference const_reference;

    /* For integration into the expression templates code: */
    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by storage type: */
    typedef typename array_type::memory_tag memory_tag;

    /* For matching by size type: */
    typedef typename array_type::size_tag size_tag;

    /* For matching by resizability: */
    typedef typename array_type::resizing_tag resizing_tag;

    /* For matching by result-type: */
    typedef cml::et::vector_result_tag result_tag;

    /* For matching by assignability: */
    typedef cml::et::assignable_tag assignable_tag;


  public:

    /** Construct from an array of values and the size. */
    vector(Element* const array, size_t size)
        : array_type(array, size) {}
};

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
