/* -*- C++ -*- ------------------------------------------------------------
 @@COPYRIGHT@@
 *-----------------------------------------------------------------------*/
/** @file
 *  @brief
 */


/* Note: this has to have the same template params as cml::vector<>: */
template<typename E, class AT, class O> std::ostream&
operator<<(std::ostream& os, const cml::vector<E,AT,O>& v)
{
    os << "[";
    for(size_t i = 0; i < v.size(); ++i) {
        os << " " << v[i];
    }
    os << " ]";
    return os;
}

// -------------------------------------------------------------------------
// vim:ft=cpp
