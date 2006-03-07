/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 *
 * Defines vector unrollers.
 *
 * @todo Add unrolling for dynamic vectors, and for vectors longer than
 * CML_VECTOR_UNROLL_LIMIT.
 *
 * @todo Does it make sense to unroll an assignment if either side of the
 * assignment has a fixed size, or just when the target vector is fixed
 * size?
 */

#ifndef vector_unroller_h
#define vector_unroller_h

#include <cml/core/fwd.h>
#include <cml/et/traits.h>
#include <cml/et/size_checking.h>
#include <cml/et/scalar_promotions.h>

#if !defined(CML_VECTOR_UNROLL_LIMIT)
#error "CML_VECTOR_UNROLL_LIMIT is undefined."
#endif

namespace cml {
namespace et {

/** Detail for the vector unroller code. */
namespace detail {

/** Unroll a binary assignment operator on a fixed-size vector.
 *
 * This uses forward iteration to make efficient use of the cache.
 *
 * @sa cml::vector
 * @sa cml::et::OpAssign
 *
 * @bug Need to verify that OpT is actually an assignment operator.
 */
template<class OpT, typename E, class AT, class O, class SrcT>
class VectorAssignmentUnroller
{
  protected:

    /* Forward declare: */
    template<int N, int Last, bool can_unroll> struct Eval;

    /* The vector type being assigned to: */
    typedef cml::vector<E,AT,O> vector_type;

    /* Record traits for the arguments: */
    typedef ExprTraits<vector_type> dest_traits;
    typedef ExprTraits<SrcT> src_traits;

    /* Get argument reference types: */
    typedef typename dest_traits::reference dest_reference;
    typedef typename src_traits::const_reference src_reference;

    /** Evaluate the binary operator for the first Len-1 elements. */
    template<int N, int Last> struct Eval<N,Last,true> {
        void operator()(dest_reference dest, src_reference src) const {

            /* Apply to current N: */
            OpT().apply(dest[N], src_traits().get(src,N));
            /* Note: we don't need get(), since we know dest is a vector. */

            /* Apply to N+1: */
            Eval<N+1,Last,true>()(dest, src);
        }
    };

    /** Evaluate the binary operator at element Last. */
    template<int Last> struct Eval<Last,Last,true> {
        void operator()(dest_reference dest, src_reference src) const {

            /* Apply to last element: */
            OpT().apply(dest[Last], src_traits().get(src,Last));
            /* Note: we don't need get(), since we know dest is a vector. */
        }
    };


    /** Evaluate the binary operator using a loop.
     *
     * This is used when the vector's length is longer than
     * CML_VECTOR_UNROLL_LIMIT
     */
    template<int N, int Last> struct Eval<N,Last,false> {
        void operator()(dest_reference dest, src_reference src) const {
            for(size_t i = 0; i <= Last; ++i) {
                OpT().apply(dest[i], src_traits().get(src,i));
                /* Note: we don't need get(), since we know dest is a vector. */
            }
        }
    };


  public:

    /** Unroll assignment to a fixed-sized vector. */
    void operator()(vector_type& dest, const SrcT& src, cml::fixed_size_tag)
    {
        typedef cml::vector<E,AT,O> vector_type;
        enum { Len = vector_type::array_size };
        typedef typename VectorAssignmentUnroller<OpT,E,AT,O,SrcT>::template
            Eval<0, Len-1, (Len <= CML_VECTOR_UNROLL_LIMIT)> Unroller;
        /* Note: Len is the array size, so Len-1 is the last element. */

        /* Use a run-time check if src is a run-time sized expression: */
        typedef typename ExprTraits<SrcT>::size_tag src_size;
        typedef typename select_if<
            same_type<src_size,dynamic_size_tag>::is_true,
            dynamic_size_tag, fixed_size_tag>::result size_tag;

        /* Check the expression size (the returned size isn't needed): */
        CheckedSize(dest,src,size_tag());
        /* Note: for two fixed-size expressions, the if-statements and
         * comparisons should be completely eliminated as dead code.  If src
         * is a dynamic-sized expression, the check will still happen.
         */

        /* Now, call the unroller: */
        Unroller()(dest,src);
    }

    /** Just use a loop to assign to a resizable vector. */
    void operator()(vector_type& dest, const SrcT& src, cml::dynamic_size_tag)
    {
        /* Shorthand: */
        typedef ExprTraits<SrcT> src_traits;

#if defined(CML_AUTOMATIC_VECTOR_RESIZE_ON_ASSIGNMENT)
        /* Get the size of src.  This also causes src to check its size: */
        size_t N = src_traits().size(src);

        /* Set the destination vector's size: */
        dest.resize(N);
#else
        size_t N = CheckedSize(dest,src,dynamic_size_tag());
#endif

        for(size_t i = 0; i < N; ++i) {
            OpT().apply(dest[i], src_traits().get(src,i));
            /* Note: we don't need get(), since we know dest is a vector. */
        }
    }

};

/** Unroll a vector accumulation/reduction operator.
 *
 * This uses forward iteration to make efficient use of the cache.
 */
template<class AccumT, class OpT, class LeftT, class RightT>
struct VectorAccumulateUnroller
{
    /* Forward declare: */
    template<int N, int Last, bool can_unroll> struct Eval;

    /* Record traits for the arguments: */
    typedef ExprTraits<LeftT> left_traits;
    typedef ExprTraits<RightT> right_traits;

    /* Get argument reference types: */
    typedef typename left_traits::const_reference left_reference;
    typedef typename right_traits::const_reference right_reference;

    /* Figure out the return type: */
    typedef typename AccumT::value_type result_type; 

    /** Evaluate for the first Len-1 elements. */
    template<int N, int Last> struct Eval<N,Last,true> {
        result_type operator()(
                left_reference left, right_reference right) const
        {

            /* Apply to last value: */
            return AccumT().apply(
                    OpT().apply(left[N], right[N]),
                    Eval<N+1,Last,true>()(left, right));
            /* Note: we don't need get(), since both arguments are required to
             * be vector expressions.
             */
        }
    };

    /** Evaluate the binary operator at element Last. */
    template<int Last> struct Eval<Last,Last,true> {
        result_type operator()(
                left_reference left, right_reference right) const
        {
            return OpT().apply(left[Last],right[Last]);
            /* Note: we don't need get(), since both arguments are required to
             * be vector expressions.
             */
        }
    };

    /** Evaluate using a loop. */
    template<int N, int Last> struct Eval<N,Last,false> {
        result_type operator()(
                left_reference left, right_reference right) const
        {
            result_type accum = OpT().apply(left[0],right[0]);
            for(size_t i = 1; i <= Last; ++i) {
                /* XXX This might not be optimized properly by some compilers,
                 * but to do anything else requires changing the requirements
                 * of a scalar operator.
                 */
                accum = AccumT().apply(accum, OpT().apply(left[i],right[i]));
                /* Note: we don't need get(), since both arguments are
                 * required to be vector expressions.
                 */
            }
        }
    };
};

}

/** Construct an assignment unroller.
 *
 * The operator must be an assignment op, otherwise, this doesn't make any
 * sense.
 *
 * @bug Need to verify that OpT is actually an assignment operator.
 */
template<class OpT, class SrcT, typename E, class AT, class O>
void UnrollAssignment(cml::vector<E,AT,O>& dest, const SrcT& src)
{
    /* Record the destination vector type, and the expression traits: */
    typedef cml::vector<E,AT,O> vector_type;

    /* Record the type of the unroller: */
    typedef detail::VectorAssignmentUnroller<OpT,E,AT,O,SrcT> unroller;

    /* Do the unroll call: */
    unroller()(dest, src, typename vector_type::size_tag());
    /* XXX It may make sense to unroll if either side is a fixed size. */
}

} // namespace et
} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
