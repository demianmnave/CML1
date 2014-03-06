/* -*- C++ -*- ------------------------------------------------------------
 
Copyright (c) 2007 Jesse Anders and Demian Nave http://cmldev.net/

The Configurable Math Library (CML) is distributed under the terms of the
Boost Software License, v1.0 (see cml/LICENSE for details).

 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */

#ifndef vector_traits_h
#define vector_traits_h

#include <cml/core/common.h>
#include <cml/et/traits.h>
#include <cml/fixed.h>

namespace cml {
namespace et {

/** Expression traits for a general vector<> type. */
template<typename E, class AT>
struct ExprTraits< cml::vector<E,AT> >
{
    typedef typename cml::vector<E,AT>::expr_type       expr_type;
    typedef typename expr_type::value_type              value_type;
    typedef typename expr_type::expr_reference          reference;
    typedef typename expr_type::expr_const_reference    const_reference;
    typedef typename expr_type::result_tag              result_tag;
    typedef typename expr_type::size_tag                size_tag;
    typedef typename expr_type::resizing_tag            resizing_tag;
    typedef typename expr_type::assignable_tag          assignable_tag;
    typedef expr_type                                   result_type;
    typedef expr_leaf_tag                               node_tag;

    enum { array_size = expr_type::array_size };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& v) const { return v.size(); }
};

/** Expression traits for a C-array type. */
template<typename E, size_t N>
struct ExprTraits< E[N] >
{
    typedef E                                           expr_type[N];
    typedef E                                           value_type;
    typedef E&                                          reference;
    typedef E const&                                    const_reference;
    typedef cml::et::vector_result_tag                  result_tag;
    typedef cml::fixed_size_tag                         size_tag;
    typedef cml::not_resizable_tag                      resizing_tag;
    typedef cml::et::assignable_tag                     assignable_tag;
    typedef cml::vector<E, cml::fixed<N> >              result_type;
    typedef expr_leaf_tag                               node_tag;

    enum { array_size = N };

    value_type get(const expr_type& v, size_t i) const { return v[i]; }
    size_t size(const expr_type& v) const { return N; }
};

} // namespace et
} // namespace cml


#endif

// -------------------------------------------------------------------------
// vim:ft=cpp
