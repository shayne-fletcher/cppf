#if !defined(HEAVISIDE_1B7F22AF_26EF_40F7_BF9D_3E281E6403E1_INCLUDED)
# define HEAVISIDE_1B7F22AF_26EF_40F7_BF9D_3E281E6403E1_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

namespace cppf
{
  namespace maths
  {
    template <class T>
    inline T heaviside(T x)
    {
      return x < T(0) ? T(0) : (x > T(0) ? T(1) : T(0.5));
    }

  }//namespace maths

}//namespace cppf

#endif//!defined(HEAVISIDE_1B7F22AF_26EF_40F7_BF9D_3E281E6403E1_INCLUDED)

