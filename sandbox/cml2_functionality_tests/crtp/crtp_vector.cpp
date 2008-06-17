/* -*- C++ -*- ------------------------------------------------------------
   @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#include <iostream>

/* Marker for non-final base classes (base classes that can be
 * instantiated):
 */
struct Itself {};

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

#define CML_EXACT_TYPE(_Self, _Derived)					\
  typename ItselfOrDerived<_Self, _Derived>::type


/* Base class for all classes participating in a SCOOP static class
 * hierarchy:
 */
template<class ExactT>
class Any
{
  // XXX STATIC_REQUIRE(!is_same(DerivedT,Itself))
  public:
    ExactT& exact() { return * (ExactT*) this; }
    const ExactT& exact() const { return * (ExactT*) this; }
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
 * class A : public VectorXprNode<A> {
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


/** Base class for vector instances in the expression tree.
 * VectorXprNode<DerivedT> uses methods in the DerivedT class to implement
 * a common, read-only vector node instance in an expression tree.
 *
 * @note Derived classes may implement read-write methods to support
 * assignment to nodes in an expression tree.
 */
template<class DerivedT>
class VectorXprNode
: public Any<DerivedT> 
{
  // XXX STATIC_REQUIRE(!is_same(VectorXprNode<DerivedT>, DerivedT)
  public:
    typedef ExprTraits<DerivedT>			node_traits;
    typedef typename node_traits::value_type		value_type;
    typedef typename node_traits::expr_type		expr_type;
    typedef typename node_traits::expr_reference	expr_reference;
    typedef typename node_traits::expr_const_reference	expr_const_reference;

    typedef ScalarTraits<value_type>			value_traits;
    typedef typename value_traits::reference		reference;
    typedef typename value_traits::const_reference	const_reference;

  public:
    size_t size() const {
      return this->exact().size();
      /* Note: this is safe since DerivedT cannot be typeof(*this). */
    }

    const value_type& operator[](size_t i) const {
      return this->exact()[i];
      /* Note: this is safe since DerivedT cannot be typeof(*this). */
    }
};

/** Assignable vector node base class. */
template<class DerivedT>
class VectorNode
: public VectorXprNode<DerivedT> 
{
  // XXX STATIC_REQUIRE(!is_same(VectorNode<DerivedT>, DerivedT)
  public:
    typedef ExprTraits<DerivedT>			node_traits;
    typedef typename node_traits::value_type		value_type;
    typedef typename node_traits::array_type		array_type;
    typedef typename node_traits::expr_type		expr_type;
    typedef typename node_traits::expr_reference	expr_reference;
    typedef typename node_traits::expr_const_reference	expr_const_reference;

    typedef ScalarTraits<value_type>			value_traits;
    typedef typename value_traits::reference		reference;
    typedef typename value_traits::const_reference	const_reference;

    // XXX CML_STATIC_ASSERT(node_traits::is_assignable)

  protected:

    /** Roll copy functionality into a single method. */
    template</*class OpT, */ class OtherT> expr_reference
      copy(const VectorXprNode<OtherT>& other) {
	// XXX RequireSameSize(*this, other);
	// XXX this->resize(other.size());
	for(size_t i = 0; i < other.exact().size(); ++ i)
	  // XXX OpT()(this->exact()[i], other[i])
	  this->exact()[i] = other[i];
	  //this->exact().get(i) = other.exact().get(i);
	return this->exact();
      }
};



template<typename Element, class ArrayType, class DerivedT = Itself>
class vector;

template<int Dim> struct fixed {};
template<int Dim = -1> struct external {};

template<typename Element, class ArrayType>
struct ExprTraits< vector<Element, ArrayType, Itself> >
{
  typedef Element					value_type;
  typedef ArrayType					array_type;
  typedef vector<Element, ArrayType>			expr_type;
  typedef expr_type&					expr_reference;
  typedef const expr_type&				expr_const_reference;
};

template<typename Element, int Size, class DerivedT>
class FixedArrayVector
: public VectorNode<DerivedT>
{
  public:
    typedef Element					value_type;

  public:
    /** @name VectorNode Interface */
    /*@{*/
    size_t size() const { return size_t(Size); }
    const value_type& operator[](size_t i) const { return m_data[i]; }
    value_type& operator[](size_t i) { return m_data[i]; }
    /*@}*/

  protected:
    value_type m_data[Size];
};

template<typename Element, int Size, class DerivedT>
class FixedExternalVector
: public VectorNode<DerivedT>
{
  public:
    typedef Element					value_type;

  public:
    /** @name VectorNode Interface */
    /*@{*/
    size_t size() const { return size_t(Size); }
    const value_type& operator[](size_t i) const { return m_data[i]; }
    value_type& operator[](size_t i) { return m_data[i]; }
    /*@}*/
    // XXX These should all verify that m_data != NULL

  protected:
    FixedExternalVector(value_type data[Size]) : m_data(&data[0]) {}

  protected:
    value_type* m_data;
};

template<typename Element, class DerivedT>
class DynamicExternalVector
: public VectorNode<DerivedT>
{
  public:
    typedef Element					value_type;

  public:
    /** @name VectorNode Interface */
    /*@{*/
    size_t size() const { return m_size; }
    const value_type& operator[](size_t i) const { return m_data[i]; }
    value_type& operator[](size_t i) { return m_data[i]; }
    /*@}*/
    // XXX These should all verify that m_data != NULL

  protected:
    DynamicExternalVector(value_type* data, size_t size)
      : m_data(data), m_size(size) {}

  protected:
    value_type* m_data;
    size_t m_size;
};

template<typename Element, int Size, class DerivedT>
class vector< Element, fixed<Size>, DerivedT >
: public FixedArrayVector<Element, Size,
  typename ItselfOrDerived<vector< Element, fixed<Size> >, DerivedT>::type
  >
{
  // XXX BOOST_STATIC_REQUIRE(Size > 0)
  public:
    typedef vector<Element, fixed<Size>, DerivedT>	self;
    typedef FixedArrayVector<Element, Size,
	    typename ItselfOrDerived<self, DerivedT>::type
    >							super;
    typedef vector< Element, fixed<Size> >		expr_type;
    typedef VectorNode<expr_type>			node_type;
    typedef typename node_type::value_type		value_type;
    typedef typename node_type::expr_reference		expr_reference;
    typedef typename node_type::expr_const_reference	expr_const_reference;

  public:

    /** Default constructor, does not initialize values. */
    vector() {}

    /** Copy-from-same constructor. */
    vector(expr_const_reference other) {
      std::clog << "copy-vector-same!\n";
      this->copy/*<OpAssign...>*/(other);
    }

    /** Copy-from-expression constructor. */
    template<class OtherT> vector(const VectorXprNode<OtherT>& other) {
      std::clog << "copy-vector-expr!\n";
      this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-same constructor. */
    expr_reference operator=(expr_const_reference other) {
      std::clog << "assign-vector-same!\n";
      return this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-expression constructor. */
    template<class OtherT> expr_reference
      operator=(const VectorXprNode<OtherT>& other) {
	std::clog << "assign-vector-expr!\n";
	return this->copy/*<OpAssign...>*/(other);
      }
};

template<typename Element, int Size, class DerivedT>
class vector< Element, external<Size>, DerivedT >
: public FixedExternalVector<Element, Size,
  typename ItselfOrDerived<vector< Element, external<Size> >, DerivedT >::type
  >
{
  // XXX BOOST_STATIC_REQUIRE(Size > 0)
  public:
    typedef vector<Element, external<Size>, DerivedT>	self;
    typedef FixedExternalVector<Element, Size,
	    typename ItselfOrDerived<self, DerivedT>::type
    >							super;
    typedef vector< Element, external<Size> >		expr_type;
    typedef VectorNode<expr_type>			node_type;
    typedef typename node_type::value_type		value_type;
    typedef typename node_type::expr_reference		expr_reference;
    typedef typename node_type::expr_const_reference	expr_const_reference;

  public:

    /** Construct from a pointer to the external array. */
    vector(value_type data[Size]) : super(data) {}

    /** Copy-from-same constructor. */
    vector(expr_const_reference other) {
      std::clog << "copy-vector-same!\n";
      this->copy/*<OpAssign...>*/(other);
    }

    /** Copy-from-expression constructor. */
    template<class OtherT> vector(const VectorXprNode<OtherT>& other) {
      std::clog << "copy-vector-expr!\n";
      this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-same constructor. */
    expr_reference operator=(expr_const_reference other) {
      std::clog << "assign-vector-same!\n";
      return this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-expression constructor. */
    template<class OtherT> expr_reference
      operator=(const VectorNode<OtherT>& other) {
	std::clog << "assign-vector-expr!\n";
	return this->copy/*<OpAssign...>*/(other);
      }
};

template<typename Element, class DerivedT>
class vector< Element, external<>, DerivedT >
: public DynamicExternalVector<Element,
  typename ItselfOrDerived<vector< Element, external<> >, DerivedT >::type
  >
{
  // XXX BOOST_STATIC_REQUIRE(Size > 0)
  public:
    typedef vector<Element, external<>, DerivedT>	self;
    typedef DynamicExternalVector<Element,
	    typename ItselfOrDerived<self, DerivedT>::type
    >							super;
    typedef vector< Element, external<> >		expr_type;
    typedef VectorNode<expr_type>			node_type;
    typedef typename node_type::value_type		value_type;
    typedef typename node_type::expr_reference		expr_reference;
    typedef typename node_type::expr_const_reference	expr_const_reference;

  public:

    /** Construct from a pointer to the external array. */
    vector(size_t size, value_type* data) : super(size, data) {}

    /** Copy-from-same constructor. */
    vector(expr_const_reference other) {
      std::clog << "copy-vector-same!\n";
      this->copy/*<OpAssign...>*/(other);
    }

    /** Copy-from-expression constructor. */
    template<class OtherT> vector(const VectorXprNode<OtherT>& other) {
      std::clog << "copy-vector-expr!\n";
      this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-same constructor. */
    expr_reference operator=(expr_const_reference other) {
      std::clog << "assign-vector-same!\n";
      return this->copy/*<OpAssign...>*/(other);
    }

    /** Assign-from-expression constructor. */
    template<class OtherT> expr_reference
      operator=(const VectorXprNode<OtherT>& other) {
	std::clog << "assign-vector-expr!\n";
	return this->copy/*<OpAssign...>*/(other);
      }
};

int main()
{
  vector< double, fixed<4> > v;
  vector< double, fixed<4> > w;
  vector< double, fixed<4> > y(v);
  vector< double, fixed<4> > z(w);
  double some_data[4];
  vector< double, external<4> > x(some_data);
  x = v;
  v = w;
  for(int i = 0; i < 4; ++ i) std::clog << x[i] << "\n";
  return 0;
}

// -------------------------------------------------------------------------
// vim:ft=cpp
