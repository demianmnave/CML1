/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#ifndef vector_ortho_h
#define vector_ortho_h

#include <cml/mathlib/vector_misc.h>
#include <cml/mathlib/misc.h>

/* Functions for orthonormalizing a set of basis vector in 3D or 2D, and for
 * constructing an orthonormal basis given various input parameters.
 */

namespace cml {

/* Orthonormalize 3 basis vectors in R3.
 *
 * Called with the default values, this function performs a single Gram-
 * Schmidt step to orthonormalize the input vectors. By default, the direction
 * of the 3rd basis vector is unchanged by this operation, but the unaffected
 * axis can be specified via the 'stable_axis' parameter.
 *
 * The arguments 'num_iter' and 's' can be specified to an iterative Gram-
 * Schmidt step. 'num_iter' is the number of iterations applied, and 's' is
 * the fraction applied towards orthonormality each step.
 *
 * In most cases, the default arguments can be ignored, leaving only the three
 * input vectors.
 */

//////////////////////////////////////////////////////////////////////////////
// Orthonormalization in 3D and 2D
//////////////////////////////////////////////////////////////////////////////

template < typename E, class A > void
orthonormalize(vector<E,A>& v0, vector<E,A>& v1, vector<E,A>& v2,
    size_t stable_axis = 2, size_t num_iter = 0, E s = E(1))
{
    /* Checking */
    detail::CheckVec3(v0);
    detail::CheckVec3(v1);
    detail::CheckVec3(v2);
    detail::CheckIndex3(stable_axis);

    typedef vector< E, fixed<3> > vector_type;
    typedef typename vector_type::value_type value_type;

    /* Iterative Gram-Schmidt; this step is skipped by default. */
    
    for (size_t i = 0; i < num_iter; ++i) {
        value_type dot01 = dot(v0,v1);
        value_type dot12 = dot(v1,v2);
        value_type dot20 = dot(v2,v0);
        value_type inv_dot00 = value_type(1) / dot(v0,v0);
        value_type inv_dot11 = value_type(1) / dot(v1,v1);
        value_type inv_dot22 = value_type(1) / dot(v2,v2);

        vector_type temp0 = v0 - s*dot01*inv_dot11*v1 - s*dot20*inv_dot22*v2;
        vector_type temp1 = v1 - s*dot12*inv_dot22*v2 - s*dot01*inv_dot00*v0;
        vector_type temp2 = v2 - s*dot20*inv_dot00*v0 - s*dot12*inv_dot11*v1;
        
        v0 = temp0;
        v1 = temp1;
        v2 = temp2;
    }

    /* Final Gram-Schmidt step to ensure orthonormality. If no iterations
     * have been requested (num_iter = 0), this is the only step. The step
     * is performed such that the direction of the axis indexed by
     * 'stable_axis' is unchanged.
     */

    size_t i, j, k;
    cyclic_permutation(stable_axis, i, j, k);
    vector_type v[] = { v0, v1, v2 };

    v[i].normalize();
    v[j] = normalize(project_to_hplane(v[j],v[i]));
    v[k] = normalize(project_to_hplane(project_to_hplane(v[k],v[i]),v[j]));
    
    v0 = v[0];
    v1 = v[1];
    v2 = v[2];
}

/* Orthonormalize 2 basis vectors in R2 */
template < typename E, class A > void
orthonormalize(vector<E,A>& v0, vector<E,A>& v1,
    size_t stable_axis = 2, size_t num_iter = 0, E s = E(1))
{
    typedef vector< E, fixed<2> > vector_type;
    typedef typename vector_type::value_type value_type;

    /* Checking */
    detail::CheckVec2(v0);
    detail::CheckVec2(v1);
    detail::CheckIndex2(stable_axis);

    /* Iterative Gram-Schmidt; this step is skipped by default. */
    
    for (size_t i = 0; i < num_iter; ++i) {
        value_type dot01 = dot(v0,v1);

        vector_type temp0 = v0 - (s * dot01 * v1) / dot(v1,v1);
        vector_type temp1 = v1 - (s * dot01 * v0) / dot(v0,v0);
        
        v0 = temp0;
        v1 = temp1;
    }

    /* Final Gram-Schmidt step to ensure orthonormality. If no iterations
     * have been requested (num_iter = 0), this is the only step. The step
     * is performed such that the direction of the axis indexed by
     * 'stable_axis' is unchanged.
     */

    size_t i, j;
    cyclic_permutation(stable_axis, i, j);
    vector_type v[] = { v0, v1 };

    v[i].normalize();
    v[j] = normalize(project_to_hplane(v[j],v[i]));
    
    v0 = v[0];
    v1 = v[1];
}

//////////////////////////////////////////////////////////////////////////////
// Orthonormal basis construction in 3D and 2D
//////////////////////////////////////////////////////////////////////////////

/* This version of orthonormal_basis() ultimately does the work for all
 * orthonormal_basis_*() functions. Given input vectors 'align' and
 * 'reference', and an order 'axis_order_<i><j><k>', it constructs an
 * orthonormal basis such that the i'th basis vector is aligned with (parallel
 * to and pointing in the same direction as) 'align', and the j'th basis
 * vector is maximally aligned with 'reference'. The k'th basis vector is
 * chosen such that the basis has a determinant of +1.
 *
 * Note that the algorithm fails when 'align' is nearly parallel to
 * 'reference'; this should be checked for and handled externally if it's a
 * case that may occur.
 */

/* Note: This is an example of the 'non-const argument modification
 * invalidates expression' gotcha. If x, y or z were to be assigned to before
 * we were 'done' with align and reference, and if one of them were the same
 * object as align or reference, then the algorithm could fail. As is the
 * basis vectors are assigned at the end of the function from a temporary
 * array, so all is well.
 */

template < class VecT_1, class VecT_2, typename E, class A > void
orthonormal_basis(
    const VecT_1& align,
    const VecT_2& reference,
    vector<E,A>& x,
    vector<E,A>& y,
    vector<E,A>& z,
    bool normalize_align = true,
    const axis_order& order = axis_order::zyx)
{
    typedef vector< E,fixed<3> > vector_type;
    typedef typename vector_type::value_type value_type;
    
    /* Checking handled by cross() and assignment to fixed<3>. */

    size_t i = order.i();
    size_t j = order.j();
    size_t k = order.k();
    
    vector_type axis[3];

    axis[i] = normalize_align ? normalize(align) : align;
    axis[k] = unit_cross(axis[i],reference);
    axis[j] = cross(axis[k],axis[i]);
    
    if (order.odd()) {
        axis[k] = -axis[k];
    }
    
    x = axis[0];
    y = axis[1];
    z = axis[2];
}

/* This version of orthonormal_basis() constructs in arbitrary basis given a
 * vector with which to align the i'th basis vector. To avoid the failure
 * case, the reference vector is always chosen so as to not be parallel to
 * 'align'. This means the algorithm will always generate a valid basis, which
 * can be useful in some circumstances; however, it should be noted that the
 * basis will likely 'pop' as the alignment vector changes, and so may not be
 * suitable for billboarding or other similar applications.
 */

template < class VecT, typename E, class A >
void orthonormal_basis(
    const VecT& align,
    vector<E,A>& x,
    vector<E,A>& y,
    vector<E,A>& z,
    bool normalize_align = true,
    const axis_order& order = axis_order::zyx)
{
    /* Checking (won't be necessary with index_of_min_abs() member function */
    detail::CheckVec3(align);

    /* @todo: vector member function index_of_min_abs() would clean this up */
    
    orthonormal_basis(
        align,
        axis_3D(cml::index_of_min_abs(align[0],align[1],align[2])),
        x, y, z, normalize_align, order
    );
}

/* orthonormal_basis_axial() generates a basis in which the j'th basis vector
 * is aligned with 'axis' and the i'th basis vector is maximally aligned (as
 * 'aligned as possible') with 'align'. This can be used for e.g. axial
 * billboarding for, say, trees or beam effects.
 *
 * Note that the implementation simply passes off to the 'reference' version
 * of orthonormal_basis(), with the parameters adjusted so that the alignment
 * is axial.
 *
 * With this algorithm the failure case is when 'align' and 'axis' are nearly
 * parallel; if this is likely, it should be checked for and handled
 * externally.
 */

template < class VecT_1, class VecT_2, typename E, class A >
void orthonormal_basis_axial(
    const VecT_1& align,
    const VecT_2& axis,
    vector<E,A>& x,
    vector<E,A>& y,
    vector<E,A>& z,
    bool normalize_align = true,
    const axis_order& order = axis_order::zyx)
{
    orthonormal_basis(axis,align,x,y,z,normalize_align,order.swapped());
}

/* orthonormal_basis_viewplane() builds a basis aligned with a viewplane, as
 * extracted from the input view matrix. The function takes into account the
 * handed of the input view matrix and orients the basis accordingly.
 *
 * The generated basis will always be valid.
 */
template < class MatT, typename E, class A >
void orthonormal_basis_viewplane(
    const MatT& view_matrix,
    vector<E,A>& x,
    vector<E,A>& y,
    vector<E,A>& z,
    const handedness& handed,
    const axis_order& order = axis_order::zyx)
{
    typedef MatT matrix_type;
    typedef typename matrix_type::value_type value_type;

    orthonormal_basis(
        -handed.sign<value_type>() *
            matrix_get_transposed_z_basis_vector(view_matrix),
        matrix_get_transposed_y_basis_vector(view_matrix),
        x, y, z, false, order
    );
}

/** Build a viewplane-oriented basis from a left-handed view matrix. */
template < class MatT, typename E, class A >
void orthonormal_basis_viewplane_LH(
    const MatT& view_matrix,
    vector<E,A>& x,
    vector<E,A>& y,
    vector<E,A>& z,
    const axis_order& order = axis_order::zyx)
{
    orthonormal_basis_viewplane(
        view_matrix,x,y,z,handedness::left_handed,order);
}

/** Build a viewplane-oriented basis from a right-handed view matrix. */
template < class MatT, typename E, class A >
void orthonormal_basis_viewplane_RH(
    const MatT& view_matrix,
    vector<E,A>& x,
    vector<E,A>& y,
    vector<E,A>& z,
    const axis_order& order = axis_order::zyx)
{
    orthonormal_basis_viewplane(
        view_matrix,x,y,z,handedness::right_handed,order);
}

/* Build a 2D orthonormal basis. */
template < class VecT, typename E, class A >
void orthonormal_basis_2D(
    const VecT& align,
    vector<E,A>& x,
    vector<E,A>& y,
    bool normalize_align = true,
    const axis_order_2D& order = axis_order_2D::xy)
{
    typedef vector< E,fixed<2> > vector_type;

    /* Checking handled by perp() and assignment to fixed<2>. */

    size_t i = order.i();
    size_t j = order.j();
    
    vector_type axis[2];

    axis[i] = normalize_align ? normalize(align) : align;
    axis[j] = perp(axis[i]);

    if (order.odd()) {
        axis[j] = -axis[j];
    }

    x = axis[0];
    y = axis[1];
}

} // namespace cml

#endif