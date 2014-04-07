#if !defined(DISCRIMINATOR_ECD35FEE_E550_4D2C_97BC_E3B11D5F5159_INCLUDED)
# define DISCRIMINATOR_ECD35FEE_E550_4D2C_97BC_E3B11D5F5159_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

namespace cppf
{
  namespace meta
  {
    template <class BaseT, int D>
    struct discriminator : BaseT
    {};

    template <class T, class D>
    struct generate_discriminator
    {
      typedef discriminator<T, D::value> type;
    };

  }//namespace meta

}//namespace cppf

#endif//!defined(DISCRIMINATOR_ECD35FEE_E550_4D2C_97BC_E3B11D5F5159_INCLUDED)
