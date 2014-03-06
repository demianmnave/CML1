/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#ifndef vector_base_h
#define vector_base_h

#include <cml/vector/vector_expr.h>
#include <cml/vector/vector_subvector.h>
#include <cml/vector/vector_products.h>
#include <cml/vector/vector_unroller.h>

namespace cml {

template<class Derived>
class vector_base
{
  public:

    typedef vector_base<Derived>                        self;
    typedef et::ExprTraits<Derived>                     expr_traits;
    typedef typename expr_traits::value_type            value_type;
    typedef typename expr_traits::subvector_type        subvector_type;
    typedef typename expr_traits::supervector_type      supervector_type;


  public:

    /** Return square of the length. */
    value_type length_squared() const {
        return cml::dot((const Derived&) *this, (const Derived&) *this);
    }

    /** Return the length. */
    value_type length() const {
        return std::sqrt(length_squared());
    }

    /** Return a normalized copy of the vector. */
    Derived normalize() const {
        Derived v((const Derived&) *this);
        return v /= length();
    }

    /** Return a subvector by removing element i. */
    subvector_type subvector(size_t i) const {
        return subvector_type(cml::subvector((Derived&) *this, i));
    };


  private:

    self& operator=(const self&);
};

template<class Derived>
class assignable_vector_base
: public vector_base<Derived>
{
  public:

    typedef assignable_vector_base<Derived>             self;
    typedef et::ExprTraits<Derived>                     expr_traits;
    typedef typename expr_traits::value_type            value_type;


  public:

    /** Normalize the vector. */
    Derived& normalize() {
        return ((Derived&) *this) /= length();
    }

    /** Set this vector to [0]. */
    Derived& zero() {
        typedef cml::et::OpAssign<value_type,value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this, value_type(0));
        return (Derived&) *this;
    }

    /** Set this vector to a cardinal vector. */
    Derived& cardinal(size_t i) {
        zero();
        ((Derived&) *this)[i] = value_type(1);
        return (Derived&) *this;
    }

    /** Pairwise minimum of this vector with another. */
    template<typename E, class AT>
    void minimize(const vector<E,AT>& v) {
      /* XXX This should probably use ScalarPromote: */
      for (size_t i = 0; i < this->size(); ++i) {
        ((Derived&) *this)[i] = std::min(((Derived&) *this)[i],v[i]);
      }
    }

    /** Pairwise maximum of this vector with another. */
    template<typename E, class AT>
    void maximize(const vector<E,AT>& v) {
      /* XXX This should probably use ScalarPromote: */
      for (size_t i = 0; i < this->size(); ++i) {
        ((Derived&) *this)[i] = std::max(((Derived&) *this)[i],v[i]);
      }
    }

    /** Fill vector with random elements. */
    void random(value_type min, value_type max) {
        for (size_t i = 0; i < this->size(); ++i) {
            ((Derived&) *this)[i] = cml::random_real(min,max);
        }
    }


    Derived& operator=(const self& m) {
        typedef cml::et::OpAssign<value_type,value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this, (Derived&) m);
        return (Derived&) *this;
    }

    template<class Derived2> Derived&
    operator=(const vector_base<Derived2>& m) {
        typedef et::ExprTraits<Derived2> src_traits;
        typedef typename src_traits::value_type src_value_type;
        typedef cml::et::OpAssign<value_type,src_value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this, (Derived2&) m);
        return (Derived&) *this;
    }

    template<class Derived2> Derived&
    operator+=(const vector_base<Derived2>& m) {
        typedef et::ExprTraits<Derived2> src_traits;
        typedef typename src_traits::value_type src_value_type;
        typedef cml::et::OpAddAssign<value_type,src_value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this, (Derived2&) m);
        return (Derived&) *this;
    }

    template<class Derived2> Derived&
    operator-=(const vector_base<Derived2>& m) {
        typedef et::ExprTraits<Derived2> src_traits;
        typedef typename src_traits::value_type src_value_type;
        typedef cml::et::OpSubAssign<value_type,src_value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this, (Derived2&) m);
        return (Derived&) *this;
    }

    template<class XprT> Derived&
    operator=(const et::VectorXpr<XprT>& e) {
        typedef et::ExprTraits<XprT> src_traits;
        typedef typename src_traits::value_type src_value_type;
        typedef cml::et::OpAssign<value_type,src_value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this,e);
        return (Derived&) *this;
    }

    template<class XprT> Derived&
    operator+=(const et::VectorXpr<XprT>& e) {
        typedef et::ExprTraits<XprT> src_traits;
        typedef typename src_traits::value_type src_value_type;
        typedef cml::et::OpAddAssign<value_type,src_value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this,e);
        return (Derived&) *this;
    }

    template<class XprT> Derived&
    operator-=(const et::VectorXpr<XprT>& e) {
        typedef et::ExprTraits<XprT> src_traits;
        typedef typename src_traits::value_type src_value_type;
        typedef cml::et::OpSubAssign<value_type,src_value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this,e);
        return (Derived&) *this;
    }

    Derived& operator*=(value_type s) {
        typedef cml::et::OpMulAssign<value_type,value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this,s);
        return (Derived&) *this;
    }

    Derived& operator/=(value_type s) {
        typedef cml::et::OpDivAssign<value_type,value_type> OpT;
        cml::et::UnrollAssignment<OpT>((Derived&) *this,s);
        return (Derived&) *this;
    }
};

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2
