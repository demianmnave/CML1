/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 *
 *  @internal This file isn't used right now, since vectors and matrices in
 *  an expression must automatically have consistent types (vectors must
 *  have the *same* type).
 */

#ifndef scalar_promotions_h
#define scalar_promotions_h

namespace cml {
namespace et {

/* Forward declare template: */
template<class E1, class E2> struct ScalarPromote;

/** Disambiguate double/any and any/double below. */
template<> struct ScalarPromote<double,double> {
    typedef double type;
};

template<typename E2> struct ScalarPromote<double,E2> {
    typedef double type;
};

template<typename E1> struct ScalarPromote<E1,double> {
    typedef double type;
};


} // namespace et
} // namespace cml

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
