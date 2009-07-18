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
#include <cml/vector/vector_expr.h>
#include <cml/vector/class_ops.h>
#include <cml/vector/vector_unroller.h>
#include <cml/vector/external.h>
#include <cml/util.h>

namespace cml {

#if 0
/** Fixed-size, fixed-memory vector. */
template<typename Element, int Size>
class vector< Element, fixed<Size> >
: public fixed_1D<Element,Size>
{
  public:

    /* Shorthand for the generator: */
    typedef fixed<Size> generator_type;

    /* Shorthand for the array type: */
    typedef fixed_1D<Element,Size> array_type;

    /* Shorthand for the type of this vector: */
    typedef vector<Element,generator_type> vector_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector_type temporary_type;
    typedef vector< Element, fixed<Size-1> > subvector_type;

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

    /** @name Attributes */
    /*@{*/

    /** Return square of the length. */
    value_type length_squared() const {
        return cml::dot(*this,*this);
    }

    /** Return the length. */
    value_type length() const {
        return std::sqrt(length_squared());
    }

    /*@}*/


    /** @name Mutators */
    /*@{*/

    /** Normalize the vector. */
    vector_type& normalize() {
        return (*this /= length());
    }

    /** Set this vector to [0]. */
    vector_type& zero() {
        typedef cml::et::OpAssign<Element,Element> OpT;
        cml::et::UnrollAssignment<OpT>(*this,Element(0));
        return *this;
    }

    /** Set this vector to a cardinal vector. */
    vector_type& cardinal(size_t i) {
        zero();
        (*this)[i] = Element(1);
        return *this;
    }

    /** Pairwise minimum of this vector with another. */
    template<typename E, class AT> vector_type&
        minimize(const vector<E,AT>& v) {
            /* XXX This should probably use ScalarPromote: */
            for (size_t i = 0; i < this->size(); ++i) {
                (*this)[i] = std::min((*this)[i],v[i]);
            }
            return (*this);
        }

    /** Pairwise maximum of this vector with another. */
    template<typename E, class AT> vector_type&
        maximize(const vector<E,AT>& v) {
            /* XXX This should probably use ScalarPromote: */
            for (size_t i = 0; i < this->size(); ++i) {
                (*this)[i] = std::max((*this)[i],v[i]);
            }
        }

    /** Fill vector with random elements. */
    vector_type& random(value_type min, value_type max) {
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] = cml::random_real(min,max);
        }
        return (*this);
    }

    /*@}*/


  public:

    vector() : array_type() {}


  public:

    /** @name Common constructors */
    /*@{*/

    vector(ELEMENT_ARG_TYPE e0, ELEMENT_ARG_TYPE e1) {
        (*this)[0] = e0;
        (*this)[1] = e1;
    }

    vector(ELEMENT_ARG_TYPE e0, ELEMENT_ARG_TYPE e1, ELEMENT_ARG_TYPE e2) {
        (*this)[0] = e0;
        (*this)[1] = e1;
        (*this)[2] = e2;
    }

    vector(ELEMENT_ARG_TYPE e0, ELEMENT_ARG_TYPE e1, ELEMENT_ARG_TYPE e2,
            ELEMENT_ARG_TYPE e3)
    {
        (*this)[0] = e0;
        (*this)[1] = e1;
        (*this)[2] = e2;
        (*this)[3] = e3;
    }


    vector(const subvector_type& s, ELEMENT_ARG_TYPE e) {
        for(size_t i = 0; i < s.size(); ++ i) (*this)[i] = s[i];
        (*this)[s.size()] = e;
    }

    vector(const value_type v[Size]) {
        for(size_t i = 0; i < Size; ++ i) (*this)[i] = v[i];
    }

    /*@}*/

    /** @name Copy Constructors */
    /*@{*/

    vector(const vector_type& v) { copy<et::OpAssign>(v); }

    template<typename E, class AT>
        vector(const vector<E,AT>& v) { copy<et::OpAssign>(v); }

    template<class XprT>
        vector(VECXPR_ARG_TYPE e) { copy<et::OpAssign>(e); }

    /*@}*/

    /** @name Copy-Assignment */
    /*@{*/

    vector_type& operator=(const vector_type& v) {
        return copy<et::OpAssign>(v);
    }

    template<typename E, class AT> vector_type&
        operator=(const cml::vector<E,AT>& v) {
            return copy<et::OpAssign>(v);
        }

    template<class XprT> vector_type&
        operator=(const XprT& e) {
            return copy<et::OpAssign>(e);
        }

    /*@}*/

    /** @name Accumulation-assignment */
    /*@{*/

    template<typename E, class AT> vector_type&
        operator+=(const cml::vector<E,AT>& v) {
            return copy<et::OpAddAssign>(v);
        }

    template<typename E, class AT> vector_type&
        operator-=(const cml::vector<E,AT>& v) {
            return copy<et::OpSubAssign>(v);
        }

    template<class XprT> vector_type&
        operator+=(VECXPR_ARG_TYPE e) {
            return copy<et::OpAddAssign>(e);
        }

    template<class XprT> vector_type&
        operator-=(VECXPR_ARG_TYPE e) {
            return copy<et::OpSubAssign>(e);
        }

    vector_type& operator*=(value_type s) {
        return copy<et::OpMulAssign>(s);
    }

    vector_type& operator/=(value_type s) {
        return copy<et::OpDivAssign>(s);
    }

    /*@}*/


  protected:

    /** Templated copy function, used by copy and assignment methods.
     *
     * @internal Note: VC7.1 takes a performance hit with this function,
     * since it apparently cannot inline through the extra function call.
     * Fortunately, GCC3.4.5, GCC4.2.1, VC8, and VC9 can fully inline and
     * optimize this function.
     */
    template<template<typename,typename> class AssignOp, class RHS>
        vector_type& copy(const RHS& rhs) {
            typedef typename et::ExprTraits<RHS>::value_type src_value_type;
            typedef AssignOp<value_type,src_value_type> OpT;
            et::UnrollAssignment<OpT>(*this,rhs);
            return *this;
        }
};
#else
/** Fixed-size, fixed-memory vector. */
template<typename Element, int Size>
class vector< Element, fixed<Size> >
: public fixed_1D<Element,Size>
{
  public:

    /* Shorthand for the generator: */
    typedef fixed<> storage_type;
    typedef fixed<Size> generator_type;

    /* Shorthand for the array type: */
    typedef fixed_1D<Element,Size> array_type;

    /* Shorthand for the type of this vector: */
    typedef vector<Element,generator_type> vector_type;

    /* For integration into the expression template code: */
    typedef vector_type expr_type;

    /* For integration into the expression template code: */
    typedef vector_type temporary_type;
    typedef vector< Element, fixed<Size-1> > subvector_type;

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

    /** Return square of the length. */
    value_type length_squared() const {
        return cml::dot(*this,*this);
    }

    /** Return the length. */
    value_type length() const {
        return std::sqrt(length_squared());
    }

    /** Normalize the vector. */
    vector_type& normalize() {
        return (*this /= length());
    }

    /** Set this vector to [0]. */
    vector_type& zero() {
        typedef cml::et::OpAssign<Element,Element> OpT;
        cml::et::UnrollAssignment<OpT>(*this,Element(0));
        return *this;
    }

    /** Set this vector to a cardinal vector. */
    vector_type& cardinal(size_t i) {
        zero();
        (*this)[i] = Element(1);
        return *this;
    }

    /** Pairwise minimum of this vector with another. */
    template<typename E, class AT>
    void minimize(const vector<E,AT>& v) {
      /* XXX This should probably use ScalarPromote: */
      for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] = std::min((*this)[i],v[i]);
      }
    }

    /** Pairwise maximum of this vector with another. */
    template<typename E, class AT>
    void maximize(const vector<E,AT>& v) {
      /* XXX This should probably use ScalarPromote: */
      for (size_t i = 0; i < this->size(); ++i) {
        (*this)[i] = std::max((*this)[i],v[i]);
      }
    }

    /** Fill vector with random elements. */
    void random(value_type min, value_type max) {
        for (size_t i = 0; i < this->size(); ++i) {
            (*this)[i] = cml::random_real(min,max);
        }
    }


  public:

    vector() : array_type() {}


  public:

    /* Define common class operators: */

    CML_CONSTRUCT_VEC_2(/**/)
    CML_CONSTRUCT_VEC_3(/**/)
    CML_CONSTRUCT_VEC_4(/**/)

    CML_CONSTRUCT_FROM_SUBVEC(/**/)

    CML_VEC_COPY_FROM_FIXED_ARRAY(array_type::array_size,/**/)
    CML_VEC_COPY_FROM_VECTYPE(: array_type())
    CML_VEC_COPY_FROM_VEC
    CML_VEC_COPY_FROM_VECXPR

    CML_ASSIGN_VEC_2
    CML_ASSIGN_VEC_3
    CML_ASSIGN_VEC_4

    CML_VEC_ASSIGN_FROM_VECTYPE

    CML_VEC_ASSIGN_FROM_VEC(=, cml::et::OpAssign)
    CML_VEC_ASSIGN_FROM_VEC(+=, cml::et::OpAddAssign)
    CML_VEC_ASSIGN_FROM_VEC(-=, cml::et::OpSubAssign)

    CML_VEC_ASSIGN_FROM_VECXPR(=, cml::et::OpAssign)
    CML_VEC_ASSIGN_FROM_VECXPR(+=, cml::et::OpAddAssign)
    CML_VEC_ASSIGN_FROM_VECXPR(-=, cml::et::OpSubAssign)

    CML_VEC_ASSIGN_FROM_SCALAR(*=, cml::et::OpMulAssign)
    CML_VEC_ASSIGN_FROM_SCALAR(/=, cml::et::OpDivAssign)
};
#endif

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
