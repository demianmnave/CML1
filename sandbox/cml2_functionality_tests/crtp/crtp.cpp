/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 *
 * Compiler notes:
 *
 * - VC7.1, 8, 9: /O2 optimizes away the cast to the derived
 *   class in Any<>::exact().
 *
 * - GCC421: -O3
 */


#if 0
  typedef typename PromoteElement<
    typename LeftT::element_type,
    typename RightT::element_type>::type	element_type;

  typedef typename DeduceStorage<
    typename LeftT::storage_type,
    typename RightT::storage_type>::type	storage_type;
#endif

#include <cstddef>
#include <cstdio>


/* Only works if T is completely resolved when it's used.  In particular,
 * ExprTraits for A below has to appear before A is declared:
 *
 * template<> struct ExprTraits<A> { typedef double value_type; };
 * class A : public OneD<A> { public: double get(size_t) { return 1.; } };
 */
template<typename T>
struct ExprTraits
{
  typedef typename T::value_type			value_type;
  typedef T&						expr_reference;
  typedef const T&					expr_const_reference;
};

template<typename T>
struct AccessTraits
{
  typedef typename ExprTraits<T>::value_type		value_type;
  typedef typename ExprTraits<T>::expr_const_reference	access_reference;
  inline value_type get(access_reference o, size_t i) const { return o(i); }
};

template<typename ScalarT>
struct ScalarTraits
{
  typedef ScalarT					reference;
  typedef const ScalarT					const_reference;
};



struct Itself {};

template<class ExactT>
class Any
{
  public:
    inline ExactT& exact() { return * (ExactT*) this; }
    inline const ExactT& exact() const { return * (ExactT*) this; }
};

template<class DerivedT>
class OneD
: public Any<DerivedT> 
{
  public:
    typedef ExprTraits<DerivedT>			derived_traits;
    typedef typename derived_traits::value_type		value_type;
    typedef ScalarTraits<value_type>			value_traits;
    typedef typename value_traits::reference		reference;
    typedef typename value_traits::const_reference	const_reference;

  public:
    inline value_type operator()(size_t i) const {
      return this->exact().get(i);
    }
};



template<class LeftT, class RightT, class OpT> class VectorBinaryXpr;
template<class LeftT, class RightT, class OpT>
struct ExprTraits< VectorBinaryXpr<LeftT,RightT,OpT> >
{
  typedef VectorBinaryXpr<LeftT,RightT,OpT>		self;
  typedef typename OpT::value_type			value_type;
  typedef self						expr_reference;
  typedef self						expr_const_reference;
};

template<class LeftT, class RightT, class OpT>
class VectorBinaryXpr
: public OneD<
    VectorBinaryXpr<LeftT,RightT,OpT>
  >
{
  public:
    typedef ExprTraits<LeftT>				left_traits;
    typedef ExprTraits<RightT>				right_traits;

    typedef typename left_traits::expr_const_reference	left_reference;
    typedef typename right_traits::expr_const_reference	right_reference;

    typedef typename OpT::value_type			value_type;
    typedef typename OpT::reference			reference;
    typedef typename OpT::const_reference		const_reference;


  public:
    inline VectorBinaryXpr(left_reference left, right_reference right)
      : m_left(left), m_right(right) {}

  public:
    /* OneD Interface */
    inline value_type get(size_t i) const {
#if 0
      return OpT().apply(
	  left_traits().get(m_left, i),
	  right_traits().get(m_right, i));
#elif 1
      return OpT().apply(
	  AccessTraits<LeftT>().get(m_left, i),
	  AccessTraits<RightT>().get(m_right, i));
#elif 0
      //return OpT().apply(this->m_left(i), this->m_right(i));
      return OpT().apply(this->m_left(i), this->m_right(i));
#endif
    }


  protected:
    left_reference		m_left;
    right_reference		m_right;
};

#if 0
template<class LeftT, class RightT, class OpT>
struct AccessTraits< VectorBinaryXpr<LeftT,RightT,OpT> >
{
  typedef VectorBinaryXpr<LeftT,RightT,OpT>		self;
  typedef typename OpT::value_type			value_type;
  typedef self						expr_reference;
  typedef self						expr_const_reference;
  value_type get(const self& o, size_t i) const { return o(i); }
};
#endif

template<typename LeftT, typename RightT>
class ScalarAddOp
{
  public:
    typedef ScalarTraits<LeftT>				left_traits;
    typedef ScalarTraits<RightT>			right_traits;
    typedef typename left_traits::const_reference	left_reference;
    typedef typename right_traits::const_reference	right_reference;

    typedef double value_type;
    typedef double reference;
    typedef double const_reference;
    inline value_type apply(left_reference left, right_reference right) const {
      return left + right;
    }
};

/* For vc8, need the "inline" keyword here to force inlining: */
template<class LeftDerivedT, class RightDerivedT> inline
VectorBinaryXpr<
  LeftDerivedT,
  RightDerivedT,
  ScalarAddOp<
     typename LeftDerivedT::value_type,
     typename RightDerivedT::value_type
     >
>
operator+(
    const OneD<LeftDerivedT>&	left,
    const OneD<RightDerivedT>&	right
    )
{
  typedef ScalarAddOp<
    typename LeftDerivedT::value_type,
    typename RightDerivedT::value_type> OpT;
  return VectorBinaryXpr<LeftDerivedT, RightDerivedT, OpT>(
      left.exact(), right.exact());
}


class Fake1D;
template<> struct ExprTraits<Fake1D>
{
  typedef double					value_type;
  typedef Fake1D					expr_reference;
  typedef Fake1D					expr_const_reference;
};

class Fake1D
: public OneD<Fake1D>
{
  public:
    typedef double value_type;
  public:
    /* Interface: */
    inline double get(size_t i) const { return 1.; }
};

#if 0
template<> struct AccessTraits<Fake1D>
{
  typedef double					value_type;
  typedef const Fake1D&					expr_const_reference;
  value_type get(expr_const_reference o, size_t i) const { return o(i); }
};
#endif

int main()
{
  Fake1D a, b;
  double q = (a+b)(0);
  std::printf("%f\n", q);
  return 0;
}

// -------------------------------------------------------------------------
// vim:ft=cpp
