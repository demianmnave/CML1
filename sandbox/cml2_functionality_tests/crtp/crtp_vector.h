/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#ifndef	vector_h
#define	vector_h

/* Marker for non-final base classes (base classes that can be
 * instantiated):
 */
struct Itself {};

/* Marker for arrays sized at compile time: */
struct static_size_tag {};

/* Marker for arrays sized at runtime: */
struct runtime_size_tag {};


template<class SelfT, class DerivedT>
struct ItselfOrDerived
{
  typedef DerivedT type;
};

template<class SelfT>
struct ItselfOrDerived<SelfT,Itself>
{
  typedef SelfT type;
};


/* Base class for all classes participating in a SCOOP static class
 * hierarchy:
 */
template<class ExactT>
class Any
{
  // XXX STATIC_REQUIRE(!is_same(DerivedT,Itself))
  public:
    ExactT& exact() { return (ExactT&) *this; }
    const ExactT& exact() const { return (ExactT&) *this; }
};



/** Traits class for scalar values that can appear as operands in an
 * expression tree. 
 *
 * @rationale Using a traits class obviates the need for having a distinct
 * node type for scalar values (particularly constants) in the tree. By
 * eliminating the Scalar node type (and consequently the unnecessary
 * copying of constants into a tree), we get a big win since constants are
 * typically heavily used by graphics-oriented vector and matrix codes.
 * Instead, ScalarTraits defines an internal "Acessor" template class (to
 * delay instantiation until use) which contains operators for accessing
 * the scalar's value from within a OneD or TwoD expression tree node:
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


/** Traits for nodes in an expression tree.  T must be completely resolved
 * at the point that ExprTraits<> is required; e.g.
 *
 * @code
 *
 * class A;                                     // Forward declare.
 *
 * template<> struct ExprTraits<A> {
 *    typedef double value_type;                // Ok. double is builtin.
 *    typedef typename A::int_type int_type;    // Bad. A is not resolved.
 * };
 *
 * class A : public VectorXpr<A> {
 *    public: typedef int int_type;
 *    public: double get(size_t) { return 1.; }
 * };
 *
 *
 * class B {                                    // Resolve B completely.
 *    typedef short int_type;
 * };
 *
 * template<> struct ExprTraits<B> {
 *    typedef typename B::int_type int_type;    // Ok. B is resolved.
 * };
 *
 * @endcode
 *
 * There are also a number of default types based upon T:
 *
 * @code
 * typedef typename T::value_type        value_type;
 * typedef T&                            expr_reference;
 * typedef const T&                      expr_const_reference;
 * @endcode
 */
template<typename T> struct ExprTraits
{
  typedef typename T::value_type			value_type;
  typedef T&						expr_reference;
  typedef const T&					expr_const_reference;
};


/** Base class for vector instances in the expression tree.  VectorXpr<>
 * uses methods in the derived class to implement a common, read-only
 * vector node instance in an expression tree.  Derived classes must
 * implement the ReadOnlyArray1D interface.
 *
 * @sa VectorNode
 */
template<class DerivedT>
class VectorXpr
: public Any<DerivedT> 
{
  // XXX STATIC_REQUIRE(!is_same(VectorXpr<DerivedT>, DerivedT)
  public:

    typedef ExprTraits<DerivedT>			expr_traits;
    typedef typename expr_traits::value_type		value_type;
    typedef typename expr_traits::expr_reference	expr_reference;
    typedef typename expr_traits::expr_const_reference	expr_const_reference;

    typedef ScalarTraits<value_type>			value_traits;
    typedef typename value_traits::reference		reference;
    typedef typename value_traits::const_reference	const_reference;

  public:

    size_t size() const {
      return this->exact(). _size();
    }

    value_type operator[](size_t i) const {
      return this->exact(). _get(i);
    }
};

/** Assignable vector node base class. VectorNode<> uses methods in the
 * derived class to support vector assignment in the expression tree.
 * Derived classes must implement the ReadWriteVector interface (which
 * includes the ReadWriteArray1D interface).
 */
template<class DerivedT>
class VectorNode
: public VectorXpr<DerivedT> 
{
  // XXX STATIC_REQUIRE(!is_same(VectorNode<DerivedT>, DerivedT)
  public:
    typedef ExprTraits<DerivedT>			expr_traits;
    typedef typename expr_traits::value_type		value_type;
    typedef typename expr_traits::array_type		array_type;
    typedef typename expr_traits::expr_reference	expr_reference;
    typedef typename expr_traits::expr_const_reference	expr_const_reference;

    typedef ScalarTraits<value_type>			value_traits;
    typedef typename value_traits::reference		reference;
    typedef typename value_traits::const_reference	const_reference;


  public:

    value_type operator[](size_t i) const {
      return this->exact(). _get(i);
      /* Note: this is safe since DerivedT cannot be typeof(*this). */
    }

    value_type& operator[](size_t i) {
      return this->exact(). _get(i);
      /* Note: this is safe since DerivedT cannot be typeof(*this). */
    }


  protected:

    /** Roll copy functionality into a single method. */
    template</*class OpT, */ class OtherT> expr_reference
      copy(const VectorXpr<OtherT>& other) {
	// XXX VectorRequireSameSize(*this, other);
	// XXX if(dynamic) this->resize(other.size());
	for(size_t i = 0; i < other.exact().size(); ++ i)
	  // XXX OpT()(this->exact()[i], other[i])
	  // this->exact()[i] = other[i];
	  ((DerivedT*)this)->_get(i) = other[i];
	  //this->exact().get(i) = other.exact().get(i);
	return this->exact();
      }
};

template<typename Element, int Size>
class DenseFixed1D
{
  public:

    typedef Element					value_type;

  public:

    /** @name ReadWriteArray1D Interface */
    /*@{*/

    size_t _size() const { return size_t(Size); }
    const value_type& _get(size_t i) const {
      return m_data[i];
    }
    value_type& _get(size_t i) { return m_data[i]; }

    /*@}*/

  public:

    DenseFixed1D() {}

  protected:

    value_type m_data[Size];
};

template<typename Element, int Size>
class ExternalDenseFixed1D
{
  public:

    typedef Element					value_type;

  public:

    /** @name ReadWriteArray1D Interface */
    /*@{*/

    size_t _size() const { return size_t(Size); }
    const value_type& _get(size_t i) const { return m_data[i]; }
    value_type& _get(size_t i) { return m_data[i]; }

    /*@}*/

  protected:

    ExternalDenseFixed1D(value_type* data) : m_data(data) {}

  protected:

    value_type* m_data;
};

template<typename Element>
class ExternalDenseDynamic1D
{
  public:

    typedef Element					value_type;

  public:

    /** @name ReadWriteArray1D Interface */
    /*@{*/

    size_t _size() const { return m_size; }
    const value_type& _get(size_t i) const { return m_data[i]; }
    value_type& _get(size_t i) { return m_data[i]; }

    /*@}*/

  protected:

    ExternalDenseDynamic1D(size_t size, value_type* data)
      : m_size(size), m_data(data) {}

  protected:

    size_t	m_size;
    value_type* m_data;
};


template<typename Element, class ArrayType, class DerivedT = Itself>
class vector;

template<int Dim> struct fixed {
  typedef static_size_tag				size_tag;
};

template<int Dim = -1> struct external {
  typedef static_size_tag				size_tag;
};

template<> struct external<-1> {
  typedef runtime_size_tag				size_tag;
};

template<typename Element, class ArrayType>
struct ExprTraits< vector<Element, ArrayType, Itself> >
{
  typedef Element					value_type;
  typedef ArrayType					array_type;
  typedef vector<Element, ArrayType, Itself>		expr_type;
  typedef expr_type&					expr_reference;
  typedef const expr_type&				expr_const_reference;
  typedef typename array_type::size_tag			size_tag;
};


template<int I, int N, class V1, class V2>
struct FixedAssignmentUnroller;

template<int I, int N, class V1, class V2>
struct FixedAssignmentUnroller
{
  void operator()(V1& v1, const V2& v2) const {
    v1. _get(I) = v2[I];
    FixedAssignmentUnroller<I+1,N,V1,V2>()(v1,v2);
  }
};

template<int N, class V1, class V2>
struct FixedAssignmentUnroller<N,N,V1,V2>
{
  void operator()(V1& v1, const V2& v2) const {
    v1. _get(N) = v2[N];
  }
};

template<int N, class V1, class V2> void
UnrollFixedVector(V1& v1, const V2& v2) {
  FixedAssignmentUnroller<0,N-1,V1,V2>()(v1,v2);
}

template<typename Element, int Size>
class vector< Element, fixed<Size> >
: public VectorNode< vector< Element, fixed<Size> > >
, public DenseFixed1D<Element, Size>	// ReadWriteArray1D interface
{
  // XXX BOOST_STATIC_REQUIRE(Size > 0)
  public:

    typedef DenseFixed1D<Element, Size>			array_base;
    typedef VectorNode<vector>				super;
    typedef vector					self;

    typedef ExprTraits<self>				expr_traits;
    typedef typename expr_traits::value_type		value_type;
    typedef typename expr_traits::array_type		array_type;
    typedef typename expr_traits::expr_reference	expr_reference;
    typedef typename expr_traits::expr_const_reference	expr_const_reference;
    typedef typename expr_traits::size_tag		size_tag;

  public:

    /** Default constructor, does not initialize values. */
    vector() : array_base() {}

    /** @name Interface */
    /*@{*/

    /** Copy-from-same constructor. */
    vector(expr_const_reference other) : array_base() {
      // this->copy/*<OpAssign...>*/(other);
      UnrollFixedVector<Size>(*this,other);
    }

    /** Copy-from-expression constructor. */
    template<class OtherT> vector(const VectorXpr<OtherT>& other) {
      this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-same constructor. */
    expr_reference operator=(expr_const_reference other) {
      UnrollFixedVector<Size>(*this,other);
      return *this;
      //return this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-expression constructor. */
    template<class OtherT> expr_reference
      operator=(const VectorXpr<OtherT>& other) {
      UnrollFixedVector<Size>(*this,other);
      return *this;
	//return this->copy/*<OpAssign...>*/(other);
      }

    /*@}*/
};

template<typename Element, int Size>
class vector< Element, external<Size> >
: public VectorNode< vector< Element, external<Size> > >
, public ExternalDenseFixed1D<Element, Size>	// ReadWriteArray1D interface
{
  // XXX BOOST_STATIC_REQUIRE(Size > 0)
  public:

    typedef ExternalDenseFixed1D<Element, Size>		array_base;
    typedef VectorNode<vector>				super;
    typedef vector					self;

    typedef ExprTraits<self>				expr_traits;
    typedef typename expr_traits::value_type		value_type;
    typedef typename expr_traits::array_type		array_type;
    typedef typename expr_traits::expr_reference	expr_reference;
    typedef typename expr_traits::expr_const_reference	expr_const_reference;
    typedef typename expr_traits::size_tag		size_tag;

  public:

    /** Initialize from an external array of values. */
    vector(value_type data[Size]) : array_base(data) {}

    /** @name Vector Interface */
    /*@{*/

    /** Assign-from-same constructor. */
    expr_reference operator=(expr_const_reference other) {
      return this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-expression constructor. */
    template<class OtherT> expr_reference
      operator=(const VectorXpr<OtherT>& other) {
	return this->copy/*<OpAssign...>*/(other);
      }

    /*@}*/
};

template<typename Element>
class vector< Element, external<-1> >
: public VectorNode< vector< Element, external<-1> > >
, public ExternalDenseDynamic1D<Element>	// ReadWriteArray1D interface
{
  public:

    typedef ExternalDenseDynamic1D<Element>		array_base;
    typedef VectorNode<vector>				super;
    typedef vector					self;

    typedef ExprTraits<self>				expr_traits;
    typedef typename expr_traits::value_type		value_type;
    typedef typename expr_traits::array_type		array_type;
    typedef typename expr_traits::expr_reference	expr_reference;
    typedef typename expr_traits::expr_const_reference	expr_const_reference;
    typedef typename expr_traits::size_tag		size_tag;

  public:

    /** Initialize from an external array of values. */
    vector(size_t size, value_type* data) : array_base(size, data) {}

    /** @name ReadWriteVector Interface */
    /*@{*/

    /** Assign-from-same constructor. */
    expr_reference operator=(expr_const_reference other) {
      return this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-expression constructor. */
    template<class OtherT> expr_reference
      operator=(const VectorXpr<OtherT>& other) {
	return this->copy/*<OpAssign...>*/(other);
      }

    /*@}*/
};

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
