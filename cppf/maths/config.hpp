#if !defined(CONFIG_7C7FF300_0C04_4F9B_AB61_0ECEE4757D26_INCLUDED)
# define CONFIG_7C7FF300_0C04_4F9B_AB61_0ECEE4757D26_INCLUDED

#  if defined(_MSC_VER) && (_MSC_VER >= 1020)
#    pragma once
#  endif // defined(_MSC_VER) && (_MSC_VER >= 1020)

#  if defined(CPPF_MATHS_DYNAMIC_LIB)
#    if defined(_WIN32) || defined(__CYGWIN__)
#      if defined(CPPF_MATHS_SOURCE)
#        define CPPF_MATHS_DECL __declspec(dllexport)
#      else
#        define CPPF_MATHS_DECL __declspec(dllimport)
#      endif // defined(CPPF_MATHS_SOURCE)
#    endif // defined(_WIN32)
#  endif // defined(CPPF_MATH_DYNAMIC_LIB)

#  if !defined(CPPF_MATHS_DECL)
#    define CPPF_MATHS_DECL
#  endif // !defined(CPPF_MATHS_DECL)

#endif//!defined(CONFIG_7C7FF300_0C04_4F9B_AB61_0ECEE4757D26_INCLUDED)
