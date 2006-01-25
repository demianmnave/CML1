/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#ifndef vector_unroller_h
#define vector_unroller_h

#if !defined(CML_VECTOR_UNROLL_LIMIT)
#error "CML_VECTOR_UNROLL_LIMIT is undefined."
#endif

#include <cml/et/traits.h>

namespace cml {
namespace et {

/** Detail for the vector unroller code. */
namespace detail {

/** Unroll a binary assignment operator on a fixed-size vector.
 *
 * This uses forward iteration to make better use of the cache.
 *
 * @sa cml::vector
 * @sa cml::et::OpAssign
 *
 * @bug Need to verify that OpT is actually an assignment operator.
 * @bug Need to verify that the vector sizes match.
 */
template<class OpT, typename E, class AT,  class SrcT>
struct VectorAssignmentUnroller
{
    /* Forward declare: */
    template<int N, int Last, bool can_unroll> struct Eval;

    /* The vector type being assigned to: */
    typedef cml::vector<E,AT> vector_type;

    /* Record traits for the arguments: */
    typedef ExprTraits<vector_type> dest_traits;
    typedef ExprTraits<SrcT> src_traits;

    /* Get argument reference types: */
    typedef typename dest_traits::reference dest_reference;
    typedef typename src_traits::const_reference src_reference;

    /** Evaluate the binary operator at element Last. */
    template<int Last> struct Eval<Last,Last,true> {
        void operator()(dest_reference dest, src_reference src) const {

            /* Apply to last element: */
            OpT().apply(dest[Last], src_traits().get(src,Last));
            /* Note: we don't need get(), since we know dest is a vector. */
        }
    };

    /** Evaluate the binary operator for the first Max-1 elements. */
    template<int N, int Last> struct Eval<N,Last,true> {
        void operator()(dest_reference dest, src_reference src) const {

            /* Apply to current N: */
            OpT().apply(dest[N], src_traits().get(src,N));
            /* Note: we don't need get(), since we know dest is a vector. */

            /* Apply to next N: */
            Eval<N+1,Last,true>()(dest, src);
        }
    };

    /** Evaluate the binary operator using a loop. */
    template<int N, int Last> struct Eval<N,Last,false> {
        void operator()(dest_reference dest, src_reference src) const {
            for(size_t i = 0; i <= Last; ++i) {
                OpT().apply(dest[i], src_traits().get(src,i));
                /* Note: we don't need get(), since we know dest is a vector. */
            }
        }
    };
};

/** Unroll assignment for a fixed-sized vector. */
template<class OpT, typename E, class AT, class SrcT>
void UnrollAssignment(
    cml::vector<E,AT>& dest, const SrcT& src, cml::fixed_size_tag)
{
    typedef cml::vector<E,AT> vector_type;
    enum { Len = vector_type::array_size };
    typedef typename VectorAssignmentUnroller<OpT,E,AT,SrcT>
        ::template Eval<0, Len-1, (Len <= CML_VECTOR_UNROLL_LIMIT)> Unroller;
    /* Note: Max is the array size, so Len-1 is the last element. */

    Unroller()(dest,src);
}

/** Just use a loop for dynamic vector assignment. */
template<class OpT, typename E, class AT, class SrcT>
void UnrollAssignment(
        cml::vector<E,AT>& dest, const SrcT& src, cml::dynamic_size_tag)
{
    typedef ExprTraits<SrcT> src_traits;
    for(size_t i = 0; i < dest.size(); ++i) {
        OpT().apply(dest[i], src_traits().get(src,i));
        /* Note: we don't need get(), since we know dest is a vector. */
    }
}

}

/** This constructs an assignment unroller for fixed-size arrays.
 *
 * The operator must be an assignment op (otherwise, this doesn't make any
 * sense).  Also, DestT must a fixed array type.
 *
 * @sa cml::vector
 * @sa cml::et::OpAssign
 *
 * @bug Need to verify that OpT is actually an assignment operator.
 */
template<class OpT, typename E, class AT, class SrcT>
void UnrollAssignment(cml::vector<E,AT>& dest, const SrcT& src) {
    typedef cml::vector<E,AT> vector_type;
    detail::UnrollAssignment<OpT>(dest, src, typename vector_type::size_tag());
}

} // namespace et
} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp