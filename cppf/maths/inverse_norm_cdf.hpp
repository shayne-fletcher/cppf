#if !defined(INVERSE_NORM_CDF_C1876A35_FAF9_4F2E_8710_1575219C59AD_INCLUDED)
# define INVERSE_NORM_CDF_C1876A35_FAF9_4F2E_8710_1575219C59AD_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

namespace cppf
{
  namespace maths
  {
    template <class T>
    inline T inverse_norm_cdf(T x)
    {
      return boost::math::quantile(
         boost::math::normal_distribution<T>(T(0), T(1)), x);
    }

  }//namespace maths

}//namespace cppf

#endif//!defined(INVERSE_NORM_CDF_C1876A35_FAF9_4F2E_8710_1575219C59AD_INCLUDED)
