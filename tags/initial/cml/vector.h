/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 *
 *  The configurable vector<> class.
 */

#ifndef vector_h
#define vector_h

#include <cstddef>              // for size_t
#include <cml/common.h>

/* This is used to tell the enclosed headers that they are being included
 * from the vector.h header:
 */
#define cml_vector_h
#include <cml/et/vector_ops.h>
#include <cml/et/vector_unroller.h>
#undef cml_vector_h

namespace cml {

/** A configurable vector type.
 *
 * This class encapsulates the notion of a vector.  The ArrayType template
 * argument can be used to select the type of array to be used as internal
 * storage for a list of type Element.  The ArrayType is expected to
 * implement operator[]() to access Element's of the vector by index.
 *
 * @warning Vectors with different ArrayType's may not yet work as expected
 * in expressions, particularly those using dynamically-allocated memory.
 *
 * @sa cml::core::fixed
 * @sa cml::core::dynamic
 */
template<typename Element, class ArrayType>
class vector

/* Figure out (and inherit from) the selected base array type: */
: public ArrayType::template rebind<Element>::other
{
  public:

    /* Shorthand for the base array type: */
    typedef typename ArrayType::template rebind<Element>::other array_type;

    /* Shorthand for the type of this vector: */
    typedef vector<Element, ArrayType> vector_type;

    /* For integration into the expression template code: */
    typedef vector<Element, ArrayType> expr_type;

    /* Standard: */
    typedef typename array_type::value_type     value_type;
    typedef typename array_type::reference      reference;
    typedef typename array_type::const_reference const_reference;

    typedef vector_type& expr_reference;
    typedef const vector_type& expr_const_reference;

    /* For matching by storage type if necessary: */
    typedef typename array_type::memory_tag memory_tag;

    /* For matching by size type if necessary: */
    typedef typename array_type::size_tag size_tag;


  public:

    /** Default constructor.
     *
     * @throws same as the ArrayType constructor.
     */
    vector() : array_type() {}

    /** Constructor for dynamically-sized arrays.
     *
     * @param size specify the size of the array.
     *
     * @throws same as the ArrayType constructor.
     */
    explicit vector(size_t size) : array_type(size) {}

    /** Constructor for external arrays.
     *
     * @param size specify the size of the array.
     * @param ptr specify an external array pointer.
     *
     * @throws same as the ArrayType constructor.
     */
    explicit vector(size_t size, value_type* ptr) : array_type(size,ptr) {}

    /** Construct from a vector of the same type.
     *
     * @param v the vector to copy from.
     *
     * @throws only if ArrayType::resize() throws.
     */
    vector(const vector_type& v) {
        typedef et::OpAssign<Element,Element> OpT;
        this->resize(v.size());
        et::UnrollAssignment<OpT>(*this,v);
    }

    /** Construct from a vector expression.
     *
     * @param expr the expression to copy from.
     *
     * @throws only if ArrayType::resize() throws.
     *
     * @bug The number of elements in the expression needs to be checked
     * against the size of the vector, especially for dynamic vectors.
     */
    template<class XprT> vector(const et::VectorXpr<XprT>& expr) {
        typedef typename XprT::value_type src_value_type;
        typedef et::OpAssign<Element,src_value_type> OpT;
        this->resize(expr.size());
        et::UnrollAssignment<OpT>(*this,expr);
    }


  public:

    /** Declare a function to assign this vector from another.
     *
     * @param _op_ the operator (e.g. +=)
     * @param _op_name_ the op functor (e.g. et::OpAssign)
     */
#define CML_ASSIGN_FROM_VEC(_op_, _op_name_)                            \
    vector_type& operator _op_ (const vector_type& v) {                 \
        typedef _op_name_ <Element,Element> OpT;                        \
        this->resize(v.size());                                         \
        et::UnrollAssignment<OpT>(*this,v);                             \
        return *this;                                                   \
    }

    CML_ASSIGN_FROM_VEC(=, et::OpAssign)
    CML_ASSIGN_FROM_VEC(+=, et::OpAddAssign)
    CML_ASSIGN_FROM_VEC(-=, et::OpSubAssign)

#undef CML_ASSIGN_FROM_VEC


    /** Declare a function to assign this vector from a vector expression.
     *
     * @param _op_ the operator (e.g. +=)
     * @param _op_name_ the op functor (e.g. et::OpAssign)
     */
#define CML_ASSIGN_FROM_VECXPR(_op_, _op_name_)                         \
    template<class XprT> vector_type&                                   \
    operator _op_ (const et::VectorXpr<XprT>& expr) {                   \
        typedef typename XprT::value_type src_value_type;               \
        typedef _op_name_ <Element,src_value_type> OpT;                 \
        this->resize(expr.size());                                      \
        et::UnrollAssignment<OpT>(*this,expr);                          \
        return *this;                                                   \
    }

    CML_ASSIGN_FROM_VECXPR(=, et::OpAssign)
    CML_ASSIGN_FROM_VECXPR(+=, et::OpAddAssign)
    CML_ASSIGN_FROM_VECXPR(-=, et::OpSubAssign)

#undef CML_ASSIGN_FROM_VECXPR


    /** Declare a function to assign this vector from a scalar.
     *
     * @param _op_ the operator (e.g. +=)
     * @param _op_name_ the op functor (e.g. et::OpAssign)
     */
#define CML_ASSIGN_FROM_SCALAR(_op_, _op_name_)                         \
    vector_type& operator _op_ (const value_type& s) {                  \
        typedef _op_name_ <Element,value_type> OpT;                     \
        et::UnrollAssignment<OpT>(*this,s);                             \
        return *this;                                                   \
    }

    CML_ASSIGN_FROM_SCALAR(*=, et::OpMulAssign)
    CML_ASSIGN_FROM_SCALAR(/=, et::OpDivAssign)

#undef CML_ASSIGN_FROM_SCALAR
};

/* Specialize the expression traits for the vector class (has to happen
 * inside the et namespace):
 */
namespace et {

/** Expression traits for a vector<> type. */
template<typename E, class AT>
struct ExprTraits< cml::vector<E,AT> >
{
    typedef typename cml::vector<E,AT>::expr_type expr_type;
    typedef typename expr_type::value_type value_type;
    typedef typename expr_type::expr_reference reference;
    typedef typename expr_type::expr_const_reference const_reference;
    typedef vector_result_tag result_tag;
    typedef typename expr_type::size_tag size_tag;

    value_type get(const_reference v, size_t i) const { return v[i]; }
    size_t size(const expr_type& v) const { return v.size(); }
};

} // namespace et

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp