/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#ifndef frustum_h
#define frustum_h

#include <cml/mathlib/matrix_concat.h>
#include <cml/mathlib/checking.h>

namespace cml {

/* @todo: plane class, and perhaps named arguments instead of an array. */

/* Extract the planes of a frustum given a modelview matrix and a projection
 * matrix with the given near z-clipping range. The planes are normalized by
 * default, but this can be turned off with the 'normalize' argument.
 *
 * The planes are in ax+by+cz+d = 0 form, and are in the order:
 *     left
 *     right
 *     bottom
 *     top
 *     near
 *     far
 */

template < class MatT, typename Real > void
extract_frustum_planes(
    const MatT& modelview,
    const MatT& projection,
    Real planes[6][4],
    ZClip z_clip,
    bool normalize = true)
{
    extract_frustum_planes(
        detail::matrix_concat_transforms_4x4(modelview,projection),
        planes,
        z_clip,
        normalize
    );
}

/* Extract the planes of a frustum from a single matrix assumed to contain any
 * model and view transforms followed by a projection transform with the given
 * near z-cliping range. The planes are normalized by default, but this can be
 * turned off with the 'normalize' argument.
 *
 * The planes are in ax+by+cz+d = 0 form, and are in the order:
 *     left
 *     right
 *     bottom
 *     top
 *     near
 *     far
 */

template < class MatT, typename Real > void
extract_frustum_planes(
    const MatT& m,
    Real planes[6][4],
    ZClip z_clip,
    bool normalize = true)
{
    detail::CheckMatHomogeneous3D(m);

    /* Left:   [03+00, 13+10, 23+20, 33+30] */
    
    planes[0][0] = m.basis_element(0,3) + m.basis_element(0,0);
    planes[0][1] = m.basis_element(1,3) + m.basis_element(1,0);
    planes[0][2] = m.basis_element(2,3) + m.basis_element(2,0);  
    planes[0][3] = m.basis_element(3,3) + m.basis_element(3,0);
    
    /* Right:  [03-00, 13-10, 23-20, 33-30] */
    
    planes[1][0] = m.basis_element(0,3) - m.basis_element(0,0);
    planes[1][1] = m.basis_element(1,3) - m.basis_element(1,0);
    planes[1][2] = m.basis_element(2,3) - m.basis_element(2,0);  
    planes[1][3] = m.basis_element(3,3) - m.basis_element(3,0);
    
    /* Bottom: [03+01, 13+11, 23+21, 33+31] */
    
    planes[2][0] = m.basis_element(0,3) + m.basis_element(0,1);
    planes[2][1] = m.basis_element(1,3) + m.basis_element(1,1);
    planes[2][2] = m.basis_element(2,3) + m.basis_element(2,1);  
    planes[2][3] = m.basis_element(3,3) + m.basis_element(3,1);
    
    /* Top:    [03-01, 13-11, 23-21, 33-31] */
    
    planes[3][0] = m.basis_element(0,3) - m.basis_element(0,1);
    planes[3][1] = m.basis_element(1,3) - m.basis_element(1,1);
    planes[3][2] = m.basis_element(2,3) - m.basis_element(2,1);  
    planes[3][3] = m.basis_element(3,3) - m.basis_element(3,1);
    
    /* Far:    [03-02, 13-12, 23-22, 33-32] */
    
    planes[5][0] = m.basis_element(0,3) - m.basis_element(0,2);
    planes[5][1] = m.basis_element(1,3) - m.basis_element(1,2);
    planes[5][2] = m.basis_element(2,3) - m.basis_element(2,2);  
    planes[5][3] = m.basis_element(3,3) - m.basis_element(3,2);
    
    /* Near:   [03+02, 13+12, 23+22, 33+32] : [02, 12, 22, 32] */
    
    if (z_clip == z_clip_neg_one) {       
        planes[4][0] = m.basis_element(0,3) + m.basis_element(0,2);
        planes[4][1] = m.basis_element(1,3) + m.basis_element(1,2);
        planes[4][2] = m.basis_element(2,3) + m.basis_element(2,2);  
        planes[4][3] = m.basis_element(3,3) + m.basis_element(3,2);
    } else { // z_clip == z_clip_zero
        planes[4][0] = m.basis_element(0,2);
        planes[4][1] = m.basis_element(1,2);
        planes[4][2] = m.basis_element(2,2);  
        planes[4][3] = m.basis_element(3,2);
    }

    /* @todo: This will be handled by the plane class */
    if (normalize) {
        for (size_t i = 0; i < 6; ++i) {
            Real invl = inv_sqrt(planes[i][0] * planes[i][0] +
                                 planes[i][1] * planes[i][1] +
                                 planes[i][2] * planes[i][2]);
                                
            planes[i][0] *= invl;
            planes[i][1] *= invl;
            planes[i][2] *= invl;
            planes[i][3] *= invl;
        }
    }
}

namespace detail {

/* This is currently only in support of finding the corners of a frustum.
 * The input planes are assumed to have a single unique intersection, so
 * no tolerance is used.
 */

template < typename Real > vector< Real, fixed<3> >
intersect_planes(Real p1[4], Real p2[4], Real p3[4])
{
    typedef vector< Real, fixed<3> > vector_type;
    typedef typename vector_type::value_type value_type;

    vector_type n1(p1[0],p1[1],p1[2]);
    vector_type n2(p2[0],p2[1],p2[2]);
    vector_type n3(p3[0],p3[1],p3[2]);
    
    value_type d1 = -p1[3];
    value_type d2 = -p2[3];
    value_type d3 = -p3[3];
    
    vector_type numer = d1*cross(n2,n3) + d2*cross(n3,n1) + d3*cross(n1,n2);
    value_type denom = triple_product(n1,n2,n3);
    return numer/denom;
}

} // namespace detail

/* Get the corners of a frustum defined by 6 planes. The planes are in
 * ax+by+cz+d = 0 form, and are in the order:
 *     left
 *     right
 *     bottom
 *     top
 *     near
 *     far
 *
 * The corners are in CCW order starting in the lower-left, first at the near
 * plane, then at the far plane.
 */

template < typename Real, typename E, class A > void
get_frustum_corners(Real planes[6][4], vector<E,A> corners[8])
{
    enum { LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR };

    corners[0] =
        detail::intersect_planes(planes[LEFT], planes[BOTTOM], planes[NEAR]);
    corners[1] =
        detail::intersect_planes(planes[RIGHT], planes[BOTTOM], planes[NEAR]);
    corners[2] =
        detail::intersect_planes(planes[RIGHT], planes[TOP], planes[NEAR]);
    corners[3] =
        detail::intersect_planes(planes[LEFT], planes[TOP], planes[NEAR]);
    corners[4] =
        detail::intersect_planes(planes[LEFT], planes[BOTTOM], planes[FAR]);
    corners[5] =
        detail::intersect_planes(planes[RIGHT], planes[BOTTOM], planes[FAR]);
    corners[6] =
        detail::intersect_planes(planes[RIGHT], planes[TOP], planes[FAR]);
    corners[7] =
        detail::intersect_planes(planes[LEFT], planes[TOP], planes[FAR]);
}

} // namespace cml

#endif