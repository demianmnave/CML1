/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
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
        return dot(*this,*this);
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
            (*this)[i] = random_real(min,max);
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

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
