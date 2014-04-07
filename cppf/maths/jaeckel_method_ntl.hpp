#if !defined(JAECKEL_METHOD_NTL_E2ED44B2_AFE1_49AF_9E85_F2A83E31FBF2_INCLUDED)
# define JAECKEL_METHOD_NTL_E2ED44B2_AFE1_49AF_9E85_F2A83E31FBF2_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

# include <boost/math/bindings/rr.hpp>

namespace cppf { namespace maths { namespace process { namespace lognormal {

namespace implied_vol
{
  template <class RealT>
  struct jaeckel_method_default_iterations;

  template <>
  struct jaeckel_method_default_iterations<boost::math::ntl::RR>
  {
    unsigned long operator()() const
    {
      using boost::math::tools::epsilon;
      using boost::math::tools::real_cast;
      typedef boost::math::ntl::RR real_type;

      real_type eps=epsilon<real_type>();
      
      return static_cast<unsigned long>(
         real_cast<double>(abs(log(eps)/log(real_type(2)))));
    }
  };

}//namespace implied_vol

}}}}//namespace cppf::maths::process::lognormal


#endif//!defined(JAECKEL_METHOD_NTL_E2ED44B2_AFE1_49AF_9E85_F2A83E31FBF2_INCLUDED)
