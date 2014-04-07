#if !defined(POLICY_SELECTOR_36B5033E_EBA0_44EA_98D4_73E8A663B372_INCLUDED)
# define POLICY_SELECTOR_36B5033E_EBA0_44EA_98D4_73E8A663B372_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

# include <cppf/meta/axes.hpp>
# include <cppf/meta/discriminator.hpp>

namespace cppf
{
  namespace meta
  {
    template <class Policies>
    struct policy_selector
      : axes
        <
            Policies
          , generate_discriminator<boost::mpl::_1, boost::mpl::_2>
      >::type
    {
    };

  }//namespace meta

}//namespace cppf

#endif//!defined(POLICY_SELECTOR<2>_36B5033E_EBA0_44EA_98D4_73E8A663B372_INCLUDED)
