/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#ifndef fixed_2D_h
#define fixed_2D_h

#include <cml/core/common.h>
#include <cml/core/fixed_1D.h>

namespace cml {

/** The internal statically-allocated 2D-array implementation class.
 *
 * This uses an internal class to setup the data matrix with the proper
 * layout.  The alternative is to use a 1D array with size Rows*Cols and a
 * multiplication to dereference an element, but it seems that compilers
 * better optimize 2D array dereferences.  This is different from
 * dynamic_2D<>, which must use the 1D array method.
 *
 * @sa cml::fixed
 *
 * @note This class is designed to have the same size as a C array with the
 * same dimensions.  It's therefore possible (but not recommended!) to coerce
 * a normal C array into a fixed_2D<> like this:
 *
 * typedef fixed_2D<double,10,10,row_major> array;
 * double c_array[10][10];
 * array& array_object = *((array*)&c_array);
 * double e11 = array_object[1][1];
 *
 * It's also possible to do this with a pointer to an array of values (e.g. a
 * double*), whether or not it was actually declared as a fixed C array.  This
 * is HIGHLY DISCOURAGED, though, since it's relatively straightforward to
 * implement a separate class to take a C array (or pointer) and turn it into
 * an array object.
 *
 * @todo Implement external_fixed_2D<> to wrap up externally-defined
 * fixed-size arrays into a C++ object.
 *
 * @internal Do <em>not</em> add the empty constructor and destructor; at
 * least one compiler (Intel C++ 9.0) fails to optimize them away, and they
 * aren't needed anyway here.
 */
template<typename Element, int Rows, int Cols, typename Layout>
class fixed_2D
{
  public:

    /* Require Rows > 0, Cols > 0: */
    CML_STATIC_REQUIRE((Rows > 0) && (Cols > 0));

    /* Record the generator: */
    typedef fixed<Rows,Cols> generator_type;

    /* Standard: */
    typedef Element value_type;
    typedef Element& reference;
    typedef const Element& const_reference;

    /* For matching by memory layout: */
    typedef Layout layout;

    /* For matching by memory type: */
    typedef fixed_memory_tag memory_tag;

    /* For matching by size type: */
    typedef fixed_size_tag size_tag;

    /* For matching by dimensions: */
    typedef twod_tag dimension_tag;

    /* To simplify the matrix transpose operator: */
    typedef fixed_2D<Element,Cols,Rows,Layout> transposed_type;

    /* To simplify the matrix row and column operators: */
    typedef fixed_1D<Element,Rows> row_array_type;
    typedef fixed_1D<Element,Cols> col_array_type;


  public:

    enum { array_rows = Rows, array_cols = Cols };


  public:

    /** Return the number of rows in the array. */
    size_t rows() const { return size_t(array_rows); }

    /** Return the number of cols in the array. */
    size_t cols() const { return size_t(array_cols); }


  public:

    /** Access element (row,col) of the matrix.
     *
     * @param row row of element.
     * @param col column of element.
     * @returns mutable reference.
     *
     * @note This function does not range-check the arguments.
     */
    reference operator()(size_t row, size_t col) {
        /* Dispatch to the right function based on layout: */
        return get_element(row,col,layout());
    }

    /** Const access element (row,col) of the matrix.
     *
     * @param row row of element.
     * @param col column of element.
     * @returns const reference.
     *
     * @note This function does not range-check the arguments.
     */
    const_reference operator()(size_t row, size_t col) const {
        /* Dispatch to the right function based on layout: */
        return get_element(row,col,layout());
    }


  protected:

    reference get_element(size_t row, size_t col, row_major) {
        return m_data[row][col];
    }

    const_reference get_element(size_t row, size_t col, row_major) const {
        return m_data[row][col];
    }

    reference get_element(size_t row, size_t col, col_major) {
        return m_data[col][row];
    }

    const_reference get_element(size_t row, size_t col, col_major) const {
        return m_data[col][row];
    }


  protected:

    /* Typedef the possible layouts: */
    typedef Element row_major_array[Rows][Cols];
    typedef Element col_major_array[Cols][Rows];

    /* Now, select the right layout for the current matrix: */
    typedef typename select_switch<
        Layout, row_major, row_major_array,     /* Case 1 */
                col_major, col_major_array,     /* Case 2 */
                invalid_layout                  /* Error case */
        >::result array_data;

    /* Declare the data array: */
    array_data                  m_data;
};

} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
