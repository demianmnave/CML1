/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 *
 *  The configurable matrix<> class.
 */

#ifndef matrix_h
#define matrix_h

#include <cstddef>              // for size_t
#include <cml/common.h>

/* This is used to tell the enclosed headers that they are being included
 * from the matrix.h header:
 */
#define cml_matrix_h
#include <cml/et/matrix_ops.h>
#include <cml/et/matrix_unroller.h>
#undef cml_matrix_h

namespace cml {

/** A configurable matrix.
 *
 * This class encapsulates the notion of a matrix.  The ArrayType template
 * argument can be used to select the type of array to be used as internal
 * storage for a 2D arrayy of type Element.  The ArrayType is expected to
 * implement operator()(size_t) (linear) and operator()(size_t,size_t)
 * (matrix) to access Element's of the matrix.
 *
 * @warning Matrices with different ArrayType's may not yet work as
 * expected in expressions, particularly those using dynamically-allocated
 * memory.
 *
 * @sa cml::fixed
 * @sa cml::dynamic
 */
template<typename Element, class ArrayType, typename Orient = row_major>
class matrix

/* Figure out (and inherit from) the selected base array type: */
: public ArrayType::template rebind<Element,Orient>::other
{
  public:

    /* Shorthand for the base array type: */
    typedef typename ArrayType::template rebind<Element,Orient>::other
        array_type;

    /* Shorthand for the type of this matrix: */
    typedef matrix<Element,ArrayType,Orient> matrix_type;

    /* For integration into the expression template code: */
    typedef matrix<Element,ArrayType,Orient> expr_type;

    /* Standard: */
    typedef typename array_type::value_type     value_type;
    typedef typename array_type::reference      reference;
    typedef typename array_type::const_reference const_reference;

    typedef matrix_type& expr_reference;
    typedef const matrix_type& expr_const_reference;

    /* For matching by memory layout: */
    typedef typename array_type::orientation    orientation;

    /* For matching by storage type if necessary: */
    typedef typename array_type::memory_tag     memory_tag;

    /* For matching by size type if necessary: */
    typedef typename array_type::size_tag       size_tag;


  public:

    /** Default constructor.
     *
     * @throws same as the ArrayType constructor.
     * @sa cml::fixed
     * @sa cml::dynamic
     */
    matrix() {}

    /** Constructor for dynamically-sized arrays.
     *
     * @param rows specify the number of rows.
     * @param cols specify the number of cols.
     *
     * @throws same as the ArrayType constructor.
     * @sa cml::fixed
     * @sa cml::dynamic
     */
    explicit matrix(size_t rows, size_t cols)
        : array_type(rows,cols) {}

    /** Constructor for external arrays.
     *
     * @param rows specify the number of rows.
     * @param cols specify the number of cols.
     * @param ptr specify the external pointer.
     *
     * @throws same as the ArrayType constructor.
     * @sa cml::fixed
     * @sa cml::dynamic
     */
    explicit matrix(size_t rows, size_t cols, value_type* ptr)
        : array_type(rows,cols,ptr) {}

    /** Construct from another matrix. */
    matrix(const matrix_type& m) {
        typedef et::OpAssign<Element,Element> OpT;

        this->resize(m.rows(),m.cols());
        et::UnrollAssignment<OpT>(*this,m);
    }

    /** Construct from a matrix expression.
     *
     * @param expr the expression to copy from.
     *
     * @throws only if ArrayType::resize() throws.
     *
     * @bug The number of elements in the expression needs to be checked
     * against the size of the matrix, especially for dynamic matrices.
     */
    template<class XprT> matrix(const et::MatrixXpr<XprT>& expr) {
        typedef typename XprT::value_type src_value_type;
        typedef et::OpAssign<Element,src_value_type> OpT;

        this->resize(expr.rows(),expr.cols());
        et::UnrollAssignment<OpT>(*this,expr.expression());
    }


  public:

    /** Declare a function to assign this matrix from another.
     *
     * @param _op_ the operator (e.g. +=)
     * @param _op_name_ the op functor (e.g. et::OpAssign)
     */
#define CML_ASSIGN_FROM_MAT(_op_, _op_name_)                            \
    matrix_type& operator _op_ (const matrix_type& m) {                 \
        typedef _op_name_ <Element,Element> OpT;                        \
        this->resize(m.rows(),m.cols());                                \
        et::UnrollAssignment<OpT>(*this,m);                             \
        return *this;                                                   \
    }

    CML_ASSIGN_FROM_MAT(=, et::OpAssign)
    CML_ASSIGN_FROM_MAT(+=, et::OpAddAssign)
    CML_ASSIGN_FROM_MAT(-=, et::OpSubAssign)

#undef CML_ASSIGN_FROM_MAT


    /** Declare a function to assign this matrix from a matrix expression.
     *
     * @param _op_ the operator (e.g. +=)
     * @param _op_name_ the op functor (e.g. et::OpAssign)
     */
#define CML_ASSIGN_FROM_MATXPR(_op_, _op_name_)                         \
    template<class XprT> matrix_type&                                   \
    operator _op_ (const et::MatrixXpr<XprT>& expr) {                   \
        typedef typename XprT::value_type src_value_type;               \
        typedef _op_name_ <Element,src_value_type> OpT;                 \
        this->resize(expr.rows(),expr.cols());                          \
        et::UnrollAssignment<OpT>(*this,expr.expression());             \
        return *this;                                                   \
    }

    CML_ASSIGN_FROM_MATXPR(=, et::OpAssign)
    CML_ASSIGN_FROM_MATXPR(+=, et::OpAddAssign)
    CML_ASSIGN_FROM_MATXPR(-=, et::OpSubAssign)

#undef CML_ASSIGN_FROM_MATXPR


    /** Declare a function to assign this matrix from a scalar.
     *
     * @param _op_ the operator (e.g. +=)
     * @param _op_name_ the op functor (e.g. et::OpAssign)
     */
#define CML_ASSIGN_FROM_SCALAR(_op_, _op_name_)                         \
    matrix_type& operator _op_ (const value_type& s) {                  \
        typedef _op_name_ <Element,value_type> OpT;                     \
        et::UnrollAssignment<OpT>(*this,s);                             \
        return *this;                                                   \
    }

    CML_ASSIGN_FROM_SCALAR(*=, et::OpMulAssign)
    CML_ASSIGN_FROM_SCALAR(/=, et::OpDivAssign)

#undef CML_ASSIGN_FROM_SCALAR


#if defined(CML_ENABLE_MATRIX_BRACES)
  public:

    /* These classes and methods should only be used for testing: */

    /* Helper to use operator[] to access rows of the matrix: */
    template<class Matrix> struct row_ref {
        typedef typename Matrix::reference reference;
        reference operator[](size_t col) { return m(row,col); }
        Matrix& m;
        size_t row;
    };

    row_ref<matrix_type> operator[](size_t row) {
        row_ref<matrix_type> ref = { *this, row }; return ref;
    }

    /* Helper to use operator[] to access const rows of the matrix: */
    template<class Matrix> struct const_row_ref {
        typedef typename Matrix::const_reference const_reference;
        const_reference operator[](size_t col) const { return m(row,col); }
        const Matrix& m;
        size_t row;
    };

    const_row_ref<matrix_type> operator[](size_t row) const {
        const_row_ref<matrix_type> ref = { *this, row }; return ref;
    }
#endif
};

/* Specialize the expression traits for the matrix class (has to happen
 * inside the et namespace):
 */
namespace et {

template<typename E, class AT, typename O>
struct ExprTraits< cml::matrix<E,AT,O> >
{
    typedef typename cml::matrix<E,AT,O>::expr_type expr_type;
    typedef typename expr_type::value_type value_type;
    typedef typename expr_type::expr_reference reference;
    typedef typename expr_type::expr_const_reference const_reference;
    typedef matrix_result_tag result_tag;

    value_type get(const expr_type& m, size_t i) const {
        return m(i);
    }
    value_type get(const expr_type& m, size_t i, size_t j) const {
        return m(i,j);
    }

    size_t rows(const expr_type& m) const { return m.rows(); }
    size_t cols(const expr_type& m) const { return m.cols(); }
};

} // namespace et

} // namespace cml


#endif

// -------------------------------------------------------------------------
// vim:ft=cpp