#if !defined(OPTION_PRICE_839C25F9_A71E_4E4E_81DA_0FEE7ACE588F_INCLUDED)
# define OPTION_PRICE_839C25F9_A71E_4E4E_81DA_0FEE7ACE588F_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

# include <cppf/maths/config.hpp>
# include <cppf/maths/e_call_put.hpp>
# include <cppf/maths/norm_cdf.hpp>
# include <cppf/maths/inverse_norm_cdf.hpp>

# include <boost/math/tools/precision.hpp>

namespace cppf { namespace maths { namespace process {

namespace lognormal
{
  template <class RealT>
  RealT option_price(
      RealT fwd
    , RealT strike
    , RealT vol
    , RealT time
    , e_call_put cp = cppf::maths::call)
    {       
      using std::sqrt;
      using std::log;
      
      if (time < 0.0)
        throw std::out_of_range("trying to value option in the past");

      RealT volt = vol*sqrt(time);
      RealT res;
      RealT TINY = boost::math::tools::epsilon<RealT>();
      if (volt <= 0.0 ||
          (strike < TINY && fwd > TINY) ||
          (strike > TINY && fwd < TINY))
      {
        res = (std::max)(cp*(fwd-strike), RealT(0.0));
      }
      else
      {
        if ((strike < TINY && fwd < TINY))
        { 
          fwd *= -1.; strike *= -1.;
          cp = (cp == cppf::maths::call ? cppf::maths::put : cppf::maths::call); 
        }
        RealT d1 = log(fwd/strike)/volt+0.5*volt;
        RealT d2 = d1-volt;
        res = cp*(fwd*norm_cdf(cp*d1)-strike*norm_cdf(cp*d2));
      }

      res = (std::max)(res, RealT(0.0)); //cut-off numeric effects

      return res;
    }    
  
}//namespace lognormal

}}}//namespace cppf::maths::process

#endif//!defined(OPTION_PRICE_839C25F9_A71E_4E4E_81DA_0FEE7ACE588F_INCLUDED)
