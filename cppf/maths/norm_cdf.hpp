#if !defined(NORM_CDF_48D8E872_908A_41F5_88B8_08B12DC42E26_INCLUDED)
# define NORM_CDF_48D8E872_908A_41F5_88B8_08B12DC42E26_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

# include <boost/math/distributions/normal.hpp>

namespace cppf
{
  namespace maths
  {
    template <class T>
    inline T norm_cdf(T x)
    {
      return boost::math::cdf(
         boost::math::normal_distribution<T>(T(0), T(1)), x);
    }

  }//namespace maths

}//namespace cppf

#endif//!defined(NORM_CDF_48D8E872_908A_41F5_88B8_08B12DC42E26_INCLUDED)
