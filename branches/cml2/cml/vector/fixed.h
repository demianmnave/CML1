/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 */

#pragma once

#ifndef	cml_vector_fixed_h
#define	cml_vector_fixed_h

#include <cml/common/fixed_selector.h>
#include <cml/vector/writable_vector.h>

namespace cml {

template<class Element, int Size>
struct vector_traits< vector<Element, fixed<Size>> >
{
  typedef Element					value_type;
  typedef value_type*					pointer;
  typedef value_type&					reference;
  typedef value_type const*				const_pointer;
  typedef value_type const&				const_reference;
  typedef value_type&					mutable_value;
  typedef value_type const&				immutable_value;
};

template<class Element, int Size>
class vector<Element, fixed<Size>>
: public writable_vector< vector<Element, fixed<Size>> >
{
  public:

    typedef vector<Element, fixed<Size>>		vector_type;
    typedef writable_vector<vector_type>		writable_type;
    typedef vector_traits<vector_type>			traits_type;
    typedef typename traits_type::value_type		value_type;
    typedef typename traits_type::pointer		pointer;
    typedef typename traits_type::reference		reference;
    typedef typename traits_type::const_pointer		const_pointer;
    typedef typename traits_type::const_reference	const_reference;
    typedef typename traits_type::mutable_value		mutable_value;
    typedef typename traits_type::immutable_value	immutable_value;


  public:

    using writable_type::operator=;


  public:

    /** Default constructor.
     *
     * @note The vector elements are uninitialized.
     */
    vector();

    /** Construct from a C-style array. */
    vector(const value_type data[Size]);

    /** Construct from std::initialize_list. */
    template<class Other> vector(std::initializer_list<Other> l);


  public:

    /** Return the length of the vector. */
    int size() const;

    /** Return vector element @c i. */
    mutable_value get(int i);

    /** Return vector const element @c i. */
    immutable_value get(int i) const;

    /** Set vector element @c i. */
    vector_type& set(int i, mutable_value v);

    /** Return access to the vector data as a raw pointer. */
    pointer data();

    /** Return const access to the vector data as a raw pointer. */
    const_pointer data() const;


  protected:

    /** Fixed-length array. */
    value_type			m_data[Size];
};

} // namespace cml

#define __CML_VECTOR_FIXED_TPP
#include <cml/vector/fixed.tpp>
#undef __CML_VECTOR_FIXED_TPP

#endif

// -------------------------------------------------------------------------
// vim:ft=cpp:sw=2
