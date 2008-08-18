/* -*- C++ -*- ------------------------------------------------------------
   @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#include <iostream>
#include "crtp_vector.h"

#if 0
template<typename Element>
class vector< Element, fixed<3> >
: public VectorNode< vector< Element, fixed<3> > >
, public DenseFixed1D<Element, 3>	// ReadWriteArray1D interface
{
  // XXX BOOST_STATIC_REQUIRE(3 > 0)
  public:

    typedef DenseFixed1D<Element, 3>			array_base;
    typedef VectorNode<vector>				super;
    typedef vector					self;

    typedef ExprTraits<self>				expr_traits;
    typedef typename expr_traits::value_type		value_type;
    typedef typename expr_traits::array_type		array_type;
    typedef typename expr_traits::expr_reference	expr_reference;
    typedef typename expr_traits::expr_const_reference	expr_const_reference;

  public:

    /** Default constructor, does not initialize values. */
    vector() : array_base() {}

    /** @name ReadWriteVector Interface */
    /*@{*/

    /** Copy-from-same constructor. */
    vector(expr_const_reference other) {
      m_data[0] = other. _get(0);
      m_data[1] = other. _get(1);
      m_data[2] = other. _get(2);
    }

    /** Copy-from-expression constructor. */
    template<class OtherT> vector(const VectorXpr<OtherT>& other) {
      m_data[0] = other[0];
      m_data[1] = other[1];
      m_data[2] = other[2];
    }

    /** Assign-from-same constructor. */
    expr_reference operator=(expr_const_reference other) {
      m_data[0] = other[0];
      m_data[1] = other[1];
      m_data[2] = other[2];
      return *this;
    }

    /** Assign-from-expression constructor. */
    template<class OtherT> expr_reference
      operator=(const VectorXpr<OtherT>& other) {
#if 0
	(*this)[0] = other[0];
	(*this)[1] = other[1];
	(*this)[2] = other[2];
#else
      m_data[0] = other[0];
      m_data[1] = other[1];
      m_data[2] = other[2];
#endif
	return *this;
      }

    /*@}*/
};
#endif

/* Forward declare: */
template<class LeftT, class RightT, class BinaryOpT> class BinaryVectorXpr;

template<class LeftT, class RightT, class BinaryOpT>
struct ExprTraits< BinaryVectorXpr<LeftT,RightT,BinaryOpT> >
{
  typedef BinaryVectorXpr<LeftT,RightT,BinaryOpT>	expr_type;
  typedef typename BinaryOpT::value_type		value_type;
  typedef expr_type&					expr_reference;
  typedef expr_type					expr_const_reference;
  //typedef const expr_type&				expr_const_reference;
};

template<class LeftT, class RightT, class BinaryOpT>
class BinaryVectorXpr
: public VectorXpr< BinaryVectorXpr<LeftT,RightT,BinaryOpT> >
{
  public:

    typedef VectorXpr<BinaryVectorXpr>			super;
    typedef BinaryVectorXpr				self;
    typedef ExprTraits<self>				expr_traits;
    typedef typename expr_traits::value_type		value_type;
    typedef typename expr_traits::expr_reference	expr_reference;
    typedef typename expr_traits::expr_const_reference	expr_const_reference;

    typedef ExprTraits<LeftT>				left_traits;
    typedef ExprTraits<RightT>				right_traits;
    typedef typename left_traits::expr_reference	left_reference;
    typedef typename right_traits::expr_reference	right_reference;
    typedef typename left_traits::expr_const_reference	left_const_reference;
    typedef typename right_traits::expr_const_reference	right_const_reference;

  public:

    /** @name ReadOnlyArray1D Interface */
    /*@{*/

    size_t _size() const {
      //return CheckedVectorSize(m_left, m_right);
      return m_left.size();
    }

    value_type _get(size_t i) const {
      return BinaryOpT().eval(m_left. _get(i), m_right. _get(i));
    }

    /*@}*/


  public:

    BinaryVectorXpr(const LeftT& left, const RightT& right)
      : m_left(left), m_right(right)
      {
      }

  protected:

#if 1
    left_const_reference	m_left;
    right_const_reference	m_right;
#else
    LeftT			m_left;
    RightT			m_right;
#endif
};

template<typename LeftT, typename RightT>
class OpAdd
{
  public:

    // typedef typename ScalarPromote<LeftT,RightT>::type value_type;
    typedef LeftT value_type;

  public:

    value_type eval(LeftT left, RightT right) const {
      return left + right;
    }
};

template<class LeftT, class RightT>
BinaryVectorXpr<
  LeftT, RightT,
  OpAdd<typename LeftT::value_type, typename RightT::value_type>
>
operator+(const VectorXpr<LeftT>& left, const VectorXpr<RightT>& right)
{
  typedef OpAdd<typename LeftT::value_type, typename RightT::value_type> OpT;
  return BinaryVectorXpr<LeftT,RightT,OpT>(left.exact(), right.exact());
  //return BinaryVectorXpr<LeftT,RightT,OpT>(left, right);
}

int main()
{
  vector< double, fixed<3> > a, b, c, d, e;

#if 1
#if 0
  for(int i = 0; i < 3; ++ i) {
    a[i] = 1.; b[i] = 2.; c[i] = 3.; e[i] = 4.;
  }
#elif 1
  a[0] = 1.; b[0] = 2.; c[0] = 3.; e[0] = 4.;
  a[1] = 1.; b[1] = 2.; c[1] = 3.; e[1] = 4.;
  a[2] = 1.; b[2] = 2.; c[2] = 3.; e[2] = 4.;
#else
#endif

#if 1
  d = a+b+c+e;
  // d = a+b+c;
  //d[0] = a[0]+b[0]+c[0];
  //d[1] = a[1]+b[1]+c[1];
  //d[2] = a[2]+b[2]+c[2];
#else
  for(int i = 0; i < 3; ++ i)
    d[i] = a[i]+b[i]+c[i]+e[i];
#endif

  for(int i = 0; i < 3; ++ i)
    //std::clog << "(a+b+c+e)[i] = " << d[i] << "\n";
    std::clog << "(a+b+c)[i] = " << d[i] << "\n";
#endif

  return 0;
}

// -------------------------------------------------------------------------
// vim:ft=cpp
