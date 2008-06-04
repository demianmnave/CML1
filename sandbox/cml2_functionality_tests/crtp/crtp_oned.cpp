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
 * - GCC3.4.5 (MinGW): -O3
 *
 * - GCC4.2.1 (MinGW-dw2): -O3
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

/* Marker for non-final base classes (base classes that can be
 * instantiated):
 */
struct Itself {};

/* Base class for all classes participating in a SCOOP static class
 * hierarchy:
 */
template<class ExactT>
class Any
{
  public:
    ExactT& exact() { return * (ExactT*) this; }
    const ExactT& exact() const { return * (ExactT*) this; }
};



/* Traits class for scalar values that can appear as operands in an
 * expression tree.  Using a traits class obviates the need for having a
 * distinct node type for scalar values (particularly constants) in the
 * tree.  By eliminating the Scalar node type (and consequently the
 * unnecessary copying of constants into a tree), we get a big win since
 * constants are typically heavily used by graphics-oriented vector and
 * matrix codes.  Instead, ScalarTraits defines an internal "Acessor"
 * template class (to delay instantiation until use) which contains
 * operators for accessing the scalar's value from within a OneD or TwoD
 * expression tree node:
 */
template<typename ScalarT>
struct ScalarTraits
{
  typedef ScalarT					value_type;
  typedef ScalarT					reference;
  typedef const ScalarT					const_reference;

  /* Use an internal template class to delay instantiation of the scalar
   * accessor function until it's actually used.
   */
  template<class U = void> struct Access {

    /* For OneD expressions: */
    value_type operator()(ScalarT o, size_t) const {
      return o;
    }

    /* For TwoD expressions: */
    value_type operator()(ScalarT o, size_t, size_t) const {
      return o;
    }
  };

  /* Define the accessor for easy access: */
  typedef Access<> Accessor;
};

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

/* Common traits class for a OneD-derived object.  This should be derived
 * from by the ExprTraits<> for T if the default access function is
 * acceptable.
 */
template<typename ValueT, class T>
struct OneDTraits
{
  typedef ValueT					value_type;

  /* Use an internal template class to delay instantiation of the 1D
   * accessor function (T::get()) until it's actually used.
   */
  template<class U = void> struct Access {
    value_type operator()(const T& o, size_t i) const {
      return o.get(i);
    }
  };

  /* Define the accessor for easy access: */
  typedef Access<> Accessor;
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
    value_type operator()(size_t i) const {
      return this->exact().get(i);
    }
};




/* Forward declare for use in defining the traits class: */
template<class LeftT, class RightT, class OpT> class VectorBinaryXpr;

template<class LeftT, class RightT, class OpT>
struct ExprTraits< VectorBinaryXpr<LeftT,RightT,OpT> >
: public OneDTraits<
   typename OpT::value_type,
   VectorBinaryXpr<LeftT,RightT,OpT>
   >
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
    typedef VectorBinaryXpr<LeftT,RightT,OpT>		self;
    typedef ExprTraits<self>				expr_traits;
    typedef ExprTraits<LeftT>				left_traits;
    typedef ExprTraits<RightT>				right_traits;

    typedef typename left_traits::expr_const_reference	left_reference;
    typedef typename right_traits::expr_const_reference	right_reference;

    typedef typename OpT::value_type			value_type;
    typedef typename OpT::reference			reference;
    typedef typename OpT::const_reference		const_reference;


  public:
    VectorBinaryXpr(left_reference left, right_reference right)
      : m_left(left), m_right(right) {}

  public:

    /* OneD Interface */
    value_type get(size_t i) const {
      typedef typename left_traits::Accessor left_access;
      typedef typename right_traits::Accessor right_access;
      return OpT().apply(
	  left_access()(m_left, i),
	  right_access()(m_right, i));
    }


  protected:
    left_reference		m_left;
    right_reference		m_right;
};

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
    value_type apply(left_reference left, right_reference right) const {
      return left + right;
    }
};

template<class LeftDerivedT, class RightDerivedT>
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
: public OneDTraits<double,Fake1D>
{
  typedef Fake1D					expr_reference;
  typedef const Fake1D					expr_const_reference;
};

class Fake1D
: public OneD<Fake1D>
{
  public:
    typedef double value_type;

  public:
    /* Interface: */
    double get(size_t i) const { return 1.; }
};

double f1()
{
  Fake1D a, b, c;
  return (a+b+c)(0);
}

int main()
{
  double q = f1();
  std::printf("%f\n", q);
  return 0;
}

// -------------------------------------------------------------------------
// vim:ft=cpp
