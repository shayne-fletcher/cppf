#if !defined(JAECKEL_METHOD_DDAE8974_C6E8_40B3_AD3A_43417A3B1CAC_INCLUDED)
# define JAECKEL_METHOD_DDAE8974_C6E8_40B3_AD3A_43417A3B1CAC_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

# include <cppf/maths/config.hpp>
# include <cppf/maths/norm_cdf.hpp>
# include <cppf/maths/inverse_norm_cdf.hpp>
# include <cppf/maths/e_call_put.hpp>
# include <cppf/maths/heaviside.hpp>
# include <cppf/meta/policy_selector.hpp>

# include <boost/fusion/container/vector.hpp>
# include <boost/fusion/container/generation/make_vector.hpp>
# include <boost/fusion/container/generation/vector_tie.hpp>
# include <boost/fusion/tuple/tuple_tie.hpp>
# include <boost/numeric/conversion/cast.hpp>

# include <limits>
# include <cmath>
# include <stdexcept>
# include <cassert>

namespace cppf { namespace maths { namespace process { namespace lognormal {

namespace implied_vol
{
  namespace detail
  {
    struct jaeckel_method_err_defaults;

  } // namespace detail

  template <class RealT=double>
  struct jaeckel_method_default_iterations
  {
    unsigned long operator()() const
    {
      using std::abs;
      using std::log;

      RealT eps=boost::math::tools::epsilon<RealT>();
      
      return boost::numeric_cast<unsigned long>(abs(log(eps)/log(RealT(2))));
    }
  };

  template <class RealT=double>
  struct jaeckel_method_default_tolerance
  {
    RealT operator()() const
    {
      using std::sqrt;

      RealT eps=boost::math::tools::epsilon<RealT>();
      RealT sqrt_eps=sqrt(eps);
 
      return sqrt_eps*sqrt(sqrt_eps); //eps^(3/4)
    }
  };

  template
  <
     class RealT=double
   , class E1 = detail::jaeckel_method_err_defaults //negative fwd.
   , class E2 = detail::jaeckel_method_err_defaults //negative strike
   , class E3 = detail::jaeckel_method_err_defaults //negative time
   , class E4 = detail::jaeckel_method_err_defaults //put price > strike
   , class E5 = detail::jaeckel_method_err_defaults //call price > fwd.
   , class E6 = detail::jaeckel_method_err_defaults //price < intrinsic
  >
  class jaeckel_method
  {
 public:
    enum status_t
    {
        undetermined=1
      , determined
      , converged
      , max_iterations_exceeded
    };

 public:
    template <class ItsF, class EpsF>
    jaeckel_method
    (
        RealT price
      , RealT forward
      , RealT strike
      , RealT time_to_maturity
      , e_call_put call_put_code
      , ItsF max_its_f
      , EpsF eps
    );
    RealT as_number() const { return vol_; }
    int status() const { return status_; }
    unsigned long iterations() const { return iterations_; }

 private:
    int status_;
    RealT vol_;
    unsigned long iterations_;
  };

  namespace detail
  {
    namespace result_of
    {
      template <class RealT>
      struct jaeckel_method_err
      {
        typedef
          boost::fusion::vector
          <
              int
            , RealT
            , unsigned long
          > type;
      };

    }//namespace result_of

    struct jaeckel_method_default_err_handler_impl
    {
      template<class RealT>
      static typename result_of::jaeckel_method_err<RealT>::type
      on_negative_forward(
          RealT price
        , RealT fwd
        , RealT strike
        , RealT t
        , e_call_put cp
        , unsigned long its
        , RealT eps)
      {
        typedef typename
          result_of::jaeckel_method_err<RealT>::type return_type;

        throw std::runtime_error("negative forward");

        return return_type();
      }
  
      template<class RealT>
      static typename result_of::jaeckel_method_err<RealT>::type
      on_negative_strike(
          RealT price
        , RealT fwd
        , RealT strike
        , RealT t
        , e_call_put cp
        , unsigned long its
        , RealT eps
      )
      {
        typedef typename
          result_of::jaeckel_method_err<RealT>::type return_type;
    
        throw std::runtime_error("negative strike");

        return return_type();
      }
  
      template<class RealT>
      static typename result_of::jaeckel_method_err<RealT>::type
      on_negative_time_to_maturity(
          RealT price
        , RealT fwd
        , RealT strike
        , RealT t
        , e_call_put cp
        , unsigned long its
        , RealT eps
      )
      {
        typedef typename
          result_of::jaeckel_method_err<RealT>::type return_type;
    
        throw std::runtime_error("negative time to maturity");
    
        return return_type();
      }
      
      template<class RealT>
      static typename result_of::jaeckel_method_err<RealT>::type
      on_put_price_greater_than_strike(
          RealT price
        , RealT fwd
        , RealT strike
        , RealT t
        , e_call_put cp
        , unsigned long its
        , RealT eps
      )
      {
        typedef typename
          result_of::jaeckel_method_err<RealT>::type return_type;

        throw std::runtime_error("put price greater than strike");

        return return_type();
      }

      template<class RealT>
      static typename result_of::jaeckel_method_err<RealT>::type
      on_call_price_greater_than_forward(
          RealT price
        , RealT fwd
        , RealT strike
        , RealT t
        , e_call_put cp
        , unsigned long its
        , RealT eps
      )
      {
        typedef typename
          result_of::jaeckel_method_err<RealT>::type return_type;

        throw std::runtime_error("call price greater than forward");

        return return_type();
      }

      template<class RealT>
      static typename result_of::jaeckel_method_err<RealT>::type
      on_price_less_than_intrinsic(
          RealT price
        , RealT fwd
        , RealT strike
        , RealT t
        , e_call_put cp
        , unsigned long its
        , RealT eps
      )
      {
        typedef typename
          result_of::jaeckel_method_err<RealT>::type return_type;

        throw std::runtime_error("price greater than intrinsic");

        return return_type();
      }
      
    };

    struct jaeckel_method_default_err_handlers
      : jaeckel_method_default_err_handler_impl
    {
      typedef jaeckel_method_default_err_handler_impl base_t;

      typedef base_t err_hnd1_t; //negative fwd.
      typedef base_t err_hnd2_t; //negative strike
      typedef base_t err_hnd3_t; //negative time
      typedef base_t err_hnd4_t; //put price > strike
      typedef base_t err_hnd5_t; //call price > fwd.
      typedef base_t err_hnd6_t; //price < intrinsic
    };

    struct jaeckel_method_err_defaults
      : virtual jaeckel_method_default_err_handlers
    {};

  }//namespace detail

  template <class P>
  struct negative_forward
    : virtual detail::jaeckel_method_default_err_handlers
  {
    typedef P err_hnd1_t;
  };

  template <class P>
  struct negative_strike
    : virtual detail::jaeckel_method_default_err_handlers
  {
    typedef P err_hnd2_t;
  };

  template <class P>
  struct negative_time_to_maturity
    : virtual detail::jaeckel_method_default_err_handlers
  {
    typedef P err_hnd3_t;
  };

  template <class P>
  struct put_price_greater_than_strike
    : virtual detail::jaeckel_method_default_err_handlers
  {
    typedef P err_hnd4_t;
  };

  template <class P>
  struct call_price_greater_than_forward
    : virtual detail::jaeckel_method_default_err_handlers
  {
    typedef P err_hnd5_t;
  };

  template <class P>
  struct price_less_than_intrinsic
    : virtual detail::jaeckel_method_default_err_handlers
  {
    typedef P err_hnd6_t;
  };

  namespace detail
  {
    template <class RealT>
    RealT normalized_black_call(RealT x, RealT sig)
    {
      using std::exp;
      using std::abs;
      using std::pow;

      RealT zero  =0;
      RealT one   =1;
      RealT two   =2;
      RealT three =3;
      RealT four  =4;
      RealT six   =6;
      RealT eight =8;
      RealT eps=boost::math::tools::epsilon<RealT>();
      RealT pi=boost::math::constants::pi<RealT>();
      RealT one_div_sqrt_two_pi=one/sqrt(2*pi);

      RealT x2=x*x;
      RealT s2=sig*sig;
      RealT b_max=exp(0.5*x);
      RealT one_over_b_max=one/b_max;

      if((x2 < eps*s2) || ((x2 + s2) < eps))
      {
        RealT b0 = (s2*s2 > eps)
          ? one - two*norm_cdf(-0.5*sig)
          : one_div_sqrt_two_pi*sig*(
                one - s2*(one/RealT(24)
                             - s2*(one/RealT(640)
                                    - s2*(one/RealT(21504)
                                            - s2/RealT(884736)))));
        return std::max(b0 + 0.5*x, zero);
      }

      RealT xi=x/sig;
      if(s2 < eps*x2)
      {
        RealT xi2=xi*xi;
        RealT phi0=exp(-0.5*xi2)*one_div_sqrt_two_pi;

        return std::max(
            phi0*exp(-0.125*s2)*four*
                sig/pow(4*xi2 - s2, three)*
                  (eight*xi2*(two*xi2 - s2 - six) + s2*(s2 - four))
          , zero);
      }

      return std::max(
           norm_cdf(xi + 0.5*sig)*b_max -
             norm_cdf(xi - 0.5*sig)*one_over_b_max
         , zero);
    }

    template <class RealT>
    inline RealT sig_lo(RealT x, RealT beta, RealT b_c)
    {
      using std::abs;
      using std::log;
      using std::sqrt;

      return sqrt(2.0*x*x/(abs(x) - 4.0*log((beta)/(b_c))));
    }
      
    template <class RealT>
    inline RealT sig_hi(RealT x, RealT beta, RealT b_c)
    {
      using std::exp;
      using std::sqrt;
      using std::abs;
      
      RealT e = exp(0.5*x);
      return -2.0*inverse_norm_cdf(((e - beta)/(e - b_c))*
                                             norm_cdf(-sqrt(0.5*abs(x))));
    }
      
    template <class RealT>
    inline RealT w(RealT xi, RealT gamma)
    {
      using std::pow;
      
      return (std::min)(pow(xi, gamma), RealT(1.0));
    }

  }//namespace detail

//avoid local code repetition
# define CPPF_JAECKEL_METHOD_ENFORCE(cond, handler, which) \
  if(!(cond))\
    {\
      boost::fusion::tie(status_, vol_, iterations_ ) = \
        handler:: which(\
                    price\
                  , forward\
                  , strike\
                  , time_to_maturity\
                  , call_put_code\
                  , max_its\
                  , eps);\
      return;\
    }\
/**/

  template<
     class RealT
   , class E1
   , class E2
   , class E3
   , class E4
   , class E5
   , class E6
  >
  template <class ItsF, class EpsF>
  jaeckel_method<RealT, E1, E2, E3, E4, E5, E6>::jaeckel_method(
      RealT price
    , RealT forward
    , RealT strike
    , RealT time_to_maturity
    , e_call_put call_put_code
    , ItsF its_f
    , EpsF eps_f)
    : status_(undetermined)
    , vol_(boost::math::tools::max_value<RealT>())
    , iterations_(0ul)
  {
    unsigned long max_its = its_f();
    RealT eps = eps_f();

    //'By Implication', Peter Jaeckel, Oct. 2006

    typedef boost::mpl::vector<E1,E2,E3,E4,E5,E6> policies_t;
    typedef cppf::meta::policy_selector<policies_t> err_hnds_t;
    typedef typename err_hnds_t::err_hnd1_t err_hnd1_t; //negative fwd.
    typedef typename err_hnds_t::err_hnd2_t err_hnd2_t; //negative strike
    typedef typename err_hnds_t::err_hnd3_t err_hnd3_t; //negative time
    typedef typename err_hnds_t::err_hnd4_t err_hnd4_t; //put price > strike
    typedef typename err_hnds_t::err_hnd5_t err_hnd5_t; //call price > fwd.
    typedef typename err_hnds_t::err_hnd6_t err_hnd6_t; //price < intrinsic
    
    using namespace ::cppf::maths::process::lognormal::implied_vol::detail;
          
    RealT p = price;
    RealT F = forward;
    RealT K = strike;
    RealT T = time_to_maturity;
    RealT theta = call_put_code;
    RealT pi = boost::math::constants::pi<RealT>();
    RealT zero=0;

    CPPF_JAECKEL_METHOD_ENFORCE(
        F > 0
      , err_hnd1_t, on_negative_forward);
    CPPF_JAECKEL_METHOD_ENFORCE(
        K > 0
      , err_hnd2_t, on_negative_strike);
    CPPF_JAECKEL_METHOD_ENFORCE(
        T > 0
      , err_hnd3_t, on_negative_time_to_maturity);
    CPPF_JAECKEL_METHOD_ENFORCE(
        theta ==  1 || price < strike
      , err_hnd4_t, on_put_price_greater_than_strike);
    CPPF_JAECKEL_METHOD_ENFORCE(
        theta ==  -1 || price <= forward
      , err_hnd5_t, on_call_price_greater_than_forward);

    RealT intrinsic=(std::max)(theta*(F - K), zero);
    if(p == intrinsic)
    {
      boost::fusion::vector_tie(status_, vol_) =
        boost::fusion::make_vector(determined, zero);

      return;
    }

    RealT beta=(p - intrinsic)/sqrt(F*K);
    CPPF_JAECKEL_METHOD_ENFORCE(
         beta >= 0
      , err_hnd6_t, on_price_less_than_intrinsic);

    using std::log;
    using std::sqrt;
    using std::exp;
    using std::abs;

    //operate on out-of-the-money calls from here
    RealT x = -abs(theta*log(F/K));
    RealT xdiv2 = 0.5*x;

    //initial guess
    RealT sig_c = sqrt(2*abs(x));
    RealT b_c = normalized_black_call(x, sig_c);
    RealT sig0 = 0;
    if(beta < b_c)
    {
      //get hi and lo and do the interpolation
      RealT siglo = sig_lo(x, beta, b_c);
      RealT sighi = sig_hi(x, beta, b_c);
      RealT sigstar = sig_hi(x, RealT(0), b_c);
      RealT bstar = normalized_black_call(x, sigstar);
      RealT siglostar = sig_lo(x, bstar, b_c);
      RealT sighistar = sig_hi(x, bstar, b_c);

      RealT log_arg1 = (sigstar - siglostar)/(sighistar - siglostar);
      assert(log_arg1 > 0.0);
      RealT log_arg2 = bstar/b_c;
      assert(log_arg2 > 0.0);

      RealT gamma = log(log_arg1)/log(log_arg2);
      RealT t = w(beta/b_c, gamma);
      sig0 = (1.0 - t)*siglo + t*sighi;
      if(normalized_black_call(x, sig0)
           <  boost::math::tools::min_value<RealT>())
      {
        sig0 += sigstar;
        sig0 *= 0.5;
        if(normalized_black_call(x, sig0)
            < boost::math::tools::min_value<RealT>())
        {
          sig0 += sig_c;
          sig0 *= 0.5;
        }
      }
    }
    else
      sig0 = sig_hi(x, beta, b_c);

    RealT sqrt_two_pi = sqrt(2*pi);
    
    //halley's method
    while(iterations_ < max_its)
    {
      RealT b = normalized_black_call(x, sig0);
      RealT xdivsig2 = (x/sig0)*(x/sig0);
      RealT sigdiv2 = 0.5*sig0;
      RealT sigd2s = (sigdiv2)*(sigdiv2);
      RealT bp = exp(-0.5*xdivsig2 - 0.5*sigd2s)/sqrt_two_pi;
  
      RealT vn = 0;
      if(beta < b_c)
      {
        vn = log(beta/b)*(log(b)/log(beta))*(b/bp);
      }
      else
      {
        vn = (beta - b)/bp;
      }
      RealT b2divb1 = x*x/pow(sig0, 3) - 0.25*sig0;
      RealT f2divf1 = b2divb1 -
        (((2 + log(b))/log(b))*bp/b)*((beta < b_c) ? 1 : 0);
      RealT vhatn = (std::max)(vn, -0.5*sig0);
      RealT nhatn = (std::max)(0.5*vhatn*f2divf1, RealT(-0.75));
      RealT sig1 = sig0 + (std::max)(vhatn/(1 + nhatn), -0.5*sig0);
  
      assert(boost::math::isfinite(sig1));

      if(abs(((sig1/sig0 - 1))) <= eps)
      {
        break;
      }

      sig0 = sig1;
      ++iterations_;
    }

    boost::fusion::vector_tie(status_, vol_) =
      boost::fusion::make_vector(
          iterations_ < max_its ?
          converged : max_iterations_exceeded
        , sig0/sqrt(T)
      );

    return;
  }

#undef CPPF_JAECKEL_METHOD_ENFORCE

}//namespace implied_vol

}}}}//namespace cppf::maths::process::lognormal

#endif//!defined(JAECKEL_METHOD_DDAE8974_C6E8_40B3_AD3A_43417A3B1CAC_INCLUDED)
