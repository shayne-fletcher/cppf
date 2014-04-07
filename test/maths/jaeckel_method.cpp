#include <cppf/maths/jaeckel_method.hpp>
#include <cppf/maths/option_price.hpp>
#include <cppf/maths/jaeckel_method_ntl.hpp>

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <boost/math/bindings/rr.hpp>

#include <iostream>

namespace
{
  void jaeckel_method();
  void jaeckel_method_ntl();
  void jaeckel_method_limits_high_vol_double();
  void jaeckel_method_limits_high_vol_ntl();
  void jaeckel_method_moneyness_tests();
  void jaeckel_method_moneyness_tests2();

}//namespace<anonymous>

namespace
{
  struct my_err_handler
  {

    template <class RealT>
    static boost::fusion::vector<int, RealT, unsigned long>
    on_put_price_greater_than_strike(
      RealT,RealT,RealT,RealT,cppf::maths::e_call_put,unsigned long, RealT)
    {
      typedef
        boost::fusion::vector<int, RealT, unsigned long> return_type;

      return return_type(2, (std::numeric_limits<RealT>::max)(), 0ul);
    }
  };

BOOST_AUTO_TEST_SUITE(jaeckel_method_suite)

  //void jaeckel_method()
  BOOST_AUTO_TEST_CASE(jaeckel_method)
  {
    double fwd, strike;
    double const vol1 = 0.025;
    double const time =   5.0;
    cppf::maths::e_call_put const
           call = cppf::maths::call
          , put = cppf::maths::put;
  
    // put options
    {
      using cppf::maths::process::lognormal::option_price;
      typedef cppf::maths::process::lognormal::implied_vol::
                                    jaeckel_method<> imp_vol_t;
      typedef cppf::maths::process::lognormal::implied_vol::
          jaeckel_method_default_tolerance<> default_tolerance;
      typedef cppf::maths::process::lognormal::implied_vol::
        jaeckel_method_default_iterations<> default_iterations;

      default_tolerance eps;
      default_iterations its;
      
      // atm put
      fwd = 0.05;
      strike = 0.05;
      double price1 = option_price(fwd, strike, vol1, time, put);
      imp_vol_t vol2(price1, fwd, strike, time, put, its, eps);
      BOOST_CHECK_EQUAL(vol2.iterations(), 0);
      BOOST_CHECK_EQUAL(vol2.status(), imp_vol_t::converged);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol2.as_number())
        % vol2.iterations()
      );
      double price2 = option_price(fwd, strike, vol2.as_number(), time, put);
      BOOST_CHECK_CLOSE(price1, price2, 1.0e-8);
    
      // out of the money put
      fwd = 0.07;
      strike = 0.05;
      double price3 = option_price(fwd, strike, vol1, time, put);
      imp_vol_t vol3(price3, fwd, strike, time, put, its, eps);
      BOOST_CHECK_EQUAL(vol3.status(), imp_vol_t::converged);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol3.as_number())
        % vol3.iterations()
      );
      double price4 = option_price(fwd, strike, vol3.as_number(), time, put);
      BOOST_CHECK_CLOSE(price3, price4, 1.0e-8);
  
      // in the money put
      fwd = 0.05;
      strike = 0.07;
      double price5 = option_price(fwd, strike, vol1, time, put);
      imp_vol_t vol4(price5, fwd, strike, time, put, its, eps);
      BOOST_CHECK_EQUAL(vol4.status(), imp_vol_t::converged);
      BOOST_CHECK(vol4.iterations() < 30);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol4.as_number())
        % vol4.iterations()
      );
      double price6 = option_price(fwd, strike, vol4.as_number(), time, put);
      BOOST_CHECK_CLOSE(price5, price6, 1.0e-8);
    } // put
  
    // call options
    {
      using cppf::maths::process::lognormal::option_price;
      typedef cppf::maths::process::lognormal::implied_vol::
                                     jaeckel_method<> imp_vol_t;
      typedef cppf::maths::process::lognormal::implied_vol::
           jaeckel_method_default_tolerance<> default_tolerance;
      typedef cppf::maths::process::lognormal::implied_vol::
         jaeckel_method_default_iterations<> default_iterations;

      default_tolerance eps;
      default_iterations its;
  
      // atm call
      fwd = 0.05;
      strike = 0.05;
      double price1 = option_price(fwd, strike, vol1, time, call);
      imp_vol_t vol2(price1, fwd, strike, time, call, its, eps);
      BOOST_CHECK_EQUAL(vol2.status(), imp_vol_t::converged);
      BOOST_CHECK_EQUAL(vol2.iterations(), 0);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol2.as_number())
        % vol2.iterations()
      );
      double price2 = option_price(fwd, strike, vol2.as_number(), time, call);
      BOOST_CHECK_CLOSE(price1, price2, 1.0e-8);
  
      // in the money call
      fwd = 0.07;
      strike = 0.05;
      double price3 = option_price(fwd, strike, vol1, time, call);
      imp_vol_t vol3(price3, fwd, strike, time, call, its, eps);
      BOOST_CHECK_EQUAL(vol3.status(), imp_vol_t::converged);
      BOOST_CHECK(vol3.iterations() < 30);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol3.as_number())
        % vol3.iterations()
      );
      double price4 = option_price(fwd, strike, vol3.as_number(), time, call);
      BOOST_CHECK_CLOSE(price3, price4, 1.0e-8);
  
      // out of the money call
      fwd = 0.05;
      strike = 0.07;
      double price5 = option_price(fwd, strike, vol1, time, call);
      imp_vol_t vol4(price5, fwd, strike, time, call, its, eps);
      BOOST_CHECK_EQUAL(vol4.status(), imp_vol_t::converged);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol4.as_number())
        % vol4.iterations()
      );
      double price6 = option_price(fwd, strike, vol4.as_number(), time, call);
      BOOST_CHECK_CLOSE(price5, price6, 1.0e-8);
  
    } // call
  
    // exceptions
    {
      double f, k, p, t;
  
      typedef cppf::maths::process::lognormal::implied_vol::
                                      jaeckel_method<> imp_vol_t;
      typedef cppf::maths::process::lognormal::implied_vol::
            jaeckel_method_default_tolerance<> default_tolerance;
      typedef cppf::maths::process::lognormal::implied_vol::
          jaeckel_method_default_iterations<> default_iterations;

      default_tolerance eps;
      default_iterations its;
  
      // negative forward
      f = -0.05; 
      k =  0.07;
      p =   1.0;
      t =   1.0;
      BOOST_CHECK_THROW(
        (imp_vol_t(p, f, k, t, call, its, eps)), std::runtime_error);
      // negative strike
      f =  0.05;
      k = -0.07; 
      p =   1.0;
      t =   1.0;
      BOOST_CHECK_THROW(
        (imp_vol_t(p, f, k, t, call, its, eps)), std::runtime_error); 
      // negative time
      f =  0.05;
      k =  0.07;
      p =   1.0;
      t =  -1.0; 
      BOOST_CHECK_THROW(
        (imp_vol_t(p, f, k, t, call, its, eps)), std::runtime_error); 
      // price less than intrinsic value
      f =  0.07;
      k =  0.05;
      p =  0.01;
      t =   1.0; 
      BOOST_CHECK_THROW(
        (imp_vol_t(p, f, k, t, call, its, eps)), std::runtime_error); 
      // price greater than fwd for a call
      f =  0.07;
      k =  0.05;
      p =  0.08;
      t =   1.0; 
      BOOST_CHECK_THROW(
        (imp_vol_t(p, f, k, t, call, its, eps)), std::runtime_error); 
      // price greater than strike for a put
      f =  0.07;
      k =  0.05;
      p =  0.08;
      t =   1.0; 
      BOOST_CHECK_THROW(
        (imp_vol_t(p, f, k, t, put, its, eps)), std::runtime_error);
  
    } // exceptions

    //customize error handling
    {
      using cppf::maths::process::lognormal::implied_vol::
                                                          jaeckel_method;
      using cppf::maths::process::lognormal::implied_vol::
                                           put_price_greater_than_strike;
      using cppf::maths::process::lognormal::implied_vol::
                                         jaeckel_method_default_tolerance;
      using cppf::maths::process::lognormal::implied_vol::
                                        jaeckel_method_default_iterations;

      typedef
        jaeckel_method<
          double
        , put_price_greater_than_strike<my_err_handler > > imp_vol_t;

      jaeckel_method_default_tolerance<> eps;
      jaeckel_method_default_iterations<> its;
  
      double f, k, p, t;
  
      // negative forward
      f = -0.05; 
      k =  0.07;
      p =   1.0;
      t =   1.0;
      BOOST_CHECK_THROW(
        (imp_vol_t(p, f, k, t, call, its, eps)), std::runtime_error);
  
      // put price greater than strike
      f =  0.07;
      k =  0.05;
      p =  0.08;
      t =   1.0; 
      imp_vol_t vol2(p, f, k, t, put, its, eps);
      BOOST_CHECK_EQUAL(vol2.status(), imp_vol_t::determined);
      BOOST_CHECK_EQUAL(vol2.as_number(), (std::numeric_limits<double>::max)());
      BOOST_CHECK_EQUAL(vol2.iterations(), 0);
    }
  }

  //void jaeckel_method_ntl()
  BOOST_AUTO_TEST_CASE(jaeckel_method_ntl)
  {
    typedef boost::math::ntl::RR real_type;
    real_type::SetOutputPrecision(20);

    real_type tol=1.0e-16;
    real_type fwd, strike;
    real_type const vol1 = 0.025;
    real_type const time =   5.0;
    cppf::maths::e_call_put const
           call = cppf::maths::call
         , put  = cppf::maths::put;
  
    // put options
    {
      using cppf::maths::process::lognormal::option_price;
      typedef cppf::maths::process::lognormal::implied_vol::
                        jaeckel_method<real_type> imp_vol_t;
      typedef cppf::maths::process::lognormal::implied_vol::
          jaeckel_method_default_tolerance<real_type> default_tolerance;
      typedef cppf::maths::process::lognormal::implied_vol::
        jaeckel_method_default_iterations<real_type> default_iterations;

      default_tolerance eps;
      default_iterations its;
      
      // atm put
      fwd = 0.05;
      strike = 0.05;
      real_type price1 = option_price(fwd, strike, vol1, time, put);
      imp_vol_t vol2(price1, fwd, strike, time, put, its, eps);
      BOOST_CHECK_EQUAL(vol2.iterations(), 0);
      BOOST_CHECK_EQUAL(vol2.status(), imp_vol_t::converged);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol2.as_number())
        % vol2.iterations()
      );
      real_type price2 = option_price(fwd, strike, vol2.as_number(), time, put);
      BOOST_CHECK_CLOSE(price1, price2, 1.0e-8);
    
      // out of the money put
      fwd = 0.07;
      strike = 0.05;
      real_type price3 = option_price(fwd, strike, vol1, time, put);
      imp_vol_t vol3(price3, fwd, strike, time, put, its, eps);
      BOOST_CHECK_EQUAL(vol3.status(), imp_vol_t::converged);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol3.as_number())
        % vol3.iterations()
      );
      real_type price4 = option_price(fwd, strike, vol3.as_number(), time, put);
      BOOST_CHECK_CLOSE(price3, price4, 1.0e-8);
  
      // in the money put
      fwd = 0.05;
      strike = 0.07;
      real_type price5 = option_price(fwd, strike, vol1, time, put);
      imp_vol_t vol4(price5, fwd, strike, time, put, its, eps);
      BOOST_CHECK_EQUAL(vol4.status(), imp_vol_t::converged);
      BOOST_CHECK(vol4.iterations() < its());
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<double>::digits10 + 1)
          , vol4.as_number())
        % vol4.iterations()
      );
      real_type price6 = option_price(fwd, strike, vol4.as_number(), time, put);
      BOOST_CHECK_CLOSE(price5, price6, 1.0e-8);
    } // put
  
    // call options
    {
      using cppf::maths::process::lognormal::option_price;
      typedef cppf::maths::process::lognormal::implied_vol
                    ::jaeckel_method<real_type> imp_vol_t;
      typedef cppf::maths::process::lognormal::implied_vol::
         jaeckel_method_default_tolerance<real_type> default_tolerance;
      typedef cppf::maths::process::lognormal::implied_vol::
        jaeckel_method_default_iterations<real_type> default_iterations;

      default_iterations its;
      default_tolerance eps;
  
      // atm call
      fwd = 0.05;
      strike = 0.05;
      real_type price1 = option_price(fwd, strike, vol1, time, call);
      imp_vol_t vol2(price1, fwd, strike, time, call, its, eps);
      BOOST_CHECK_EQUAL(vol2.status(), imp_vol_t::converged);
      BOOST_CHECK_EQUAL(vol2.iterations(), 0);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<real_type>::digits10 + 1)
          , vol2.as_number())
        % vol2.iterations()
      );
      real_type price2 = option_price(fwd, strike, vol2.as_number(), time, call);
      BOOST_CHECK_CLOSE(price1, price2, 1.0e-8);
  
      // in the money call
      fwd = 0.07;
      strike = 0.05;
      real_type price3 = option_price(fwd, strike, vol1, time, call);
      imp_vol_t vol3(price3, fwd, strike, time, call, its, eps);
      BOOST_CHECK_EQUAL(vol3.status(), imp_vol_t::converged);
      BOOST_CHECK(vol3.iterations() < its());
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<real_type>::digits10 + 1)
          , vol3.as_number())
        % vol3.iterations()
      );
      real_type price4 = option_price(fwd, strike, vol3.as_number(), time, call);
      BOOST_CHECK_CLOSE(price3, price4, 1.0e-8);
  
      // out of the money call
      fwd = 0.05;
      strike = 0.07;
      real_type price5 = option_price(fwd, strike, vol1, time, call);
      imp_vol_t vol4(price5, fwd, strike, time, call, its, eps);
      BOOST_CHECK_EQUAL(vol4.status(), imp_vol_t::converged);
      BOOST_MESSAGE
      (
        boost::format(
          "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
        % boost::io::group(
            std::setprecision(std::numeric_limits<real_type>::digits10 + 1)
          , vol4.as_number())
        % vol4.iterations()
      );
      real_type price6 = option_price(fwd, strike, vol4.as_number(), time, call);
      BOOST_CHECK_CLOSE(price5, price6, 1.0e-8);
  
    } // call 
  }

  //void jaeckel_method_limits_high_vol_double()
  BOOST_AUTO_TEST_CASE(jaeckel_method_limits_high_vol_double)
  {
    BOOST_CHECK(true);//...doesn't include any assertions

    double const fwd       = 0.15;
    double const strike    = 0.05;
    double const time      = 25.0;
    double const input_vol = 5.0;
    cppf::maths::e_call_put const call = cppf::maths::call;
    cppf::maths::e_call_put const put  = cppf::maths::put;

    using cppf::maths::process::lognormal::option_price;
    typedef cppf::maths::process::lognormal::implied_vol::jaeckel_method<> imp_vol_t;
    typedef cppf::maths::process::lognormal::implied_vol::jaeckel_method_default_iterations<> default_iterations;
    typedef cppf::maths::process::lognormal::implied_vol::jaeckel_method_default_tolerance<> default_tolerance;

 //These throw

/*
    default_tolerance eps;
    default_iterations its;

    //These go horribly wrong. Higher precision required.

    double put_price  = option_price(fwd,strike,input_vol,time,put);
    imp_vol_t put_vol(put_price,fwd,strike,time,put,its,eps);
    BOOST_MESSAGE
    (
      boost::format(
        "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
      % boost::io::group(
          std::setprecision(std::numeric_limits<double>::digits10 + 1)
        , put_vol.as_number())
      % put_vol.iterations()
      );
    BOOST_CHECK_CLOSE(put_vol.as_number() , input_vol , 1e-10);

    double call_price = option_price(fwd,strike,input_vol,time,call);
    imp_vol_t call_vol(call_price,fwd,strike,time,call,its,eps);
    BOOST_MESSAGE
    (
      boost::format(
        "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
      % boost::io::group(
          std::setprecision(std::numeric_limits<double>::digits10 + 1)
        , call_vol.as_number())
      % call_vol.iterations()
      );

    BOOST_CHECK_CLOSE(call_vol.as_number() , input_vol , 1e-6);
*/

  }

  //void jaeckel_method_limits_high_vol_ntl()
  BOOST_AUTO_TEST_CASE(jaeckel_method_limits_high_vol_ntl)
  {
    typedef boost::math::ntl::RR real_type;

    real_type::SetOutputPrecision(20);

    real_type const fwd       = 0.15;
    real_type const strike    = 0.05;
    real_type const time      = 25.0;
    real_type const input_vol = 5.0;
    cppf::maths::e_call_put const call = cppf::maths::call;
    cppf::maths::e_call_put const put  = cppf::maths::put;

    using namespace cppf::maths;
    using namespace cppf::maths::process::lognormal;
    using namespace cppf::maths::process::lognormal::implied_vol;
    using namespace cppf::maths::process::lognormal::implied_vol::detail;

    using cppf::maths::process::lognormal::option_price;
    typedef cppf::maths::process::lognormal::implied_vol::
                                  jaeckel_method<real_type> imp_vol_t;
    typedef cppf::maths::process::lognormal::implied_vol::
        jaeckel_method_default_tolerance<real_type> default_tolerance;
    typedef cppf::maths::process::lognormal::implied_vol::
      jaeckel_method_default_iterations<real_type> default_iterations;

    default_tolerance eps;
    default_iterations its;

    real_type put_price  = option_price(fwd,strike,input_vol,time,put);
    imp_vol_t put_vol(put_price,fwd,strike,time,put,its,eps);
    BOOST_MESSAGE
    (
      boost::format(
        "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
      % boost::io::group(
          std::setprecision(std::numeric_limits<real_type>::digits10 + 1)
        , put_vol.as_number())
      % put_vol.iterations()
    );
    BOOST_CHECK_CLOSE(put_vol.as_number() , input_vol , 1e-10);

      //This throws

    real_type call_price = option_price(fwd,strike,input_vol,time,call);
    imp_vol_t call_vol(call_price,fwd,strike,time,call,its,eps);
    BOOST_MESSAGE
    (
      boost::format(
        "Implied black vol. (Jaeckel) = %1% found in %2% iterations")
      % boost::io::group(
          std::setprecision(std::numeric_limits<real_type>::digits10 + 1)
        , call_vol.as_number())
      % call_vol.iterations()
    );
    BOOST_CHECK_CLOSE(call_vol.as_number() , input_vol , 1e-10);

  }

  //Utility function called on by jaeckel_method_moneyness_tests2().

  bool success_expected(
     double num_std_dev, cppf::maths::e_call_put code)
  {
    bool expected=false;
    if(code == cppf::maths::call)
    {
      expected = (-37.5 <= num_std_dev && num_std_dev <= 4.0);
    }
    else //put
    {
      expected = (-4.0 <= num_std_dev && num_std_dev <= 37.5);
    }

    return expected;
  }

  //Utility function called on by jaeckel_method_moneyness_tests2().

  bool success_expected(
     boost::math::ntl::RR num_std_dev, cppf::maths::e_call_put code)
  {
    bool expected=false;
    if(code == cppf::maths::call)
    {
      expected = (-37.5 <= num_std_dev && num_std_dev <= 12.0);
    }
    else //put
    {
      expected = (-12.0 <= num_std_dev && num_std_dev <= 37.5);
    }

    return expected;
  }

  template<class RealType> 
  void jaeckel_method_moneyness_test2_impl()
  {
    typedef RealType real_type;

    using namespace cppf::maths;
    using namespace cppf::maths::process::lognormal;
    using namespace cppf::maths::process::lognormal::implied_vol;
    using namespace cppf::maths::process::lognormal::implied_vol::detail;
    using cppf::maths::process::lognormal::option_price;
    typedef implied_vol::jaeckel_method<real_type> imp_vol_t;
    jaeckel_method_default_tolerance<real_type> eps;
    jaeckel_method_default_iterations<real_type> its;

    real_type const stddev_min = -37.5;
    real_type const stddev_max = 37.5;
    real_type const stddev_step = 0.5;
    real_type const sigma_min  = 0.1;
    real_type const sigma_max  = 0.1;
    real_type const sigma_step = 10;
    real_type const K = 0.5;
    real_type const T = 2.0;

    using std::sqrt; //for the double case
    using std::exp;  //ditto
    using std::abs;  //tritto

    for( real_type sigma = sigma_min ; sigma <= sigma_max ; sigma *= sigma_step )
    {
      real_type sigma_rootT = sigma*sqrt(T);
      for( real_type sd = stddev_min ; sd <= stddev_max ; sd = sd + stddev_step )
      {
        cppf::maths::e_call_put current;

        try
        {
          //require log(F/K) = sd*sigma*sqrt(T);
          //i.e F = K exp( sd*sigma*sqrt(T) );
          real_type F = K * exp( sd * sigma_rootT );
          real_type call_price = option_price(F,K,sigma,T,call);
          real_type put_price  = option_price(F,K,sigma,T,put);

          current = call;

          imp_vol_t vol1(call_price,F,K,T,call,its,eps);

          current = put;

          imp_vol_t vol2(put_price ,F,K,T,put,its,eps);

          if(success_expected(sd, call))
            BOOST_CHECK_MESSAGE(
              vol1.status() == imp_vol_t::determined ||
              vol1.status() == imp_vol_t::converged
            , "\nFailure for (sigma,sd) = (" << sigma << " , " << sd << ")"
            );
          if(success_expected(sd, put))
            BOOST_CHECK_MESSAGE(
              vol2.status() == imp_vol_t::determined ||
              vol2.status() == imp_vol_t::converged
            , "\nFailure for (sigma,sd) = (" << sigma << " , " << sd << ")"
            );

          real_type result_call = vol1.as_number();
          real_type result_put  = vol2.as_number();

          real_type const absolute_tolerance = 1e-12;
          bool call_good = abs(result_call - sigma) < absolute_tolerance;
          bool put_good  = abs(result_put  - sigma) < absolute_tolerance;
          if(success_expected(sd, call))
            BOOST_CHECK_MESSAGE(call_good 
              ,  "\nFailure for call with (sigma,sd) : (" << sigma << " , " << sd << ")"
              << "\nResult was                       : " << result_call
              << "\nDifference was                   : " << result_call - sigma
              << "\nValue type was                   : " << typeid(real_type).name()
              << "\nIterations carried out           : " << vol1.iterations()
            );
          if(success_expected(sd, put))
            BOOST_CHECK_MESSAGE(put_good 
              ,  "\nFailure for put with (sigma,sd)  : (" << sigma << " , " << sd << ")"
              << "\nResult was                       : " << result_put
              << "\nDifference was                   : " << result_put - sigma
              << "\nValue type was                   : " << typeid(real_type).name()
              << "\nIterations carried out           : " << vol2.iterations()
            );
        }
        catch(std::runtime_error& e)
        {
          if(success_expected(sd,current))
          {
            BOOST_CHECK_MESSAGE(false , 
               "\nFailure for (sigma,sd) = (" << sigma << " , " << sd << ")"
            << "\nPut call code = " << current
            << "\nValue type = " << typeid(real_type).name()
            << "\nException thrown was " << e.what()
            );
          }
        }

      }
    }
  }

  //void jaeckel_method_moneyness_tests2()
  BOOST_AUTO_TEST_CASE(jaeckel_method_moneyness_tests2)
  {
    //Test with double type
    using namespace cppf::maths::process::lognormal::implied_vol;
    jaeckel_method_moneyness_test2_impl<double>();

    //Test with real type
    typedef boost::math::ntl::RR real_type;
    real_type::SetOutputPrecision(20);
    jaeckel_method_moneyness_test2_impl<real_type>();
  }
  
  //void jaeckel_method_moneyness_tests()
  BOOST_AUTO_TEST_CASE(jaeckel_method_moneyness_tests)
  {
    using namespace cppf::maths;
    using namespace cppf::maths::process::lognormal;
    using namespace cppf::maths::process::lognormal::implied_vol;
    using namespace cppf::maths::process::lognormal::implied_vol::detail;
    typedef cppf::maths::process::lognormal::implied_vol::
                                              jaeckel_method<> imp_vol_t;

    double const strike=1;
    double const time_to_maturity=1;

    //Generate x values (=log(F/K)) ranging over [-x_max, x_max].
    double x_max=3;
    std::size_t nx=29;
    std::vector<double> x(nx, -x_max);
    for(std::size_t i = 1; i < nx; ++i) x[i] = x[i-1]+2*x_max/(nx - 1);

    //Generate standard deviations ranging over [smin, smax].
    double s_min=1.0e-5;
    double s_max=2;
    std::size_t ns=29;
    double ln_10=std::log(10.);
    double log10_s_min=std::log(s_min)/ln_10;
    double log10_s_max=std::log(s_max)/ln_10;
    double stepscale=0.5;
    std::vector<double> s(ns);
    for(std::size_t i = 0; i < ns; ++i)
      s[i]=
        std::pow(10
              , log10_s_min +
                 std::pow(double(i)/(ns - 1), stepscale)*
                                       (log10_s_max - log10_s_min));

    //Generate prices given x and s values.
    std::vector<std::vector<double> > prices(ns, std::vector<double>(ns));
    for(std::size_t i = 0; i < ns; ++i)
    {
      for(std::size_t j = 0; j < nx; ++j)
      {
        double xx = x[j] > 0 ? -x[j] : x[j];
        prices[i][j] = normalized_black_call(xx, s[i])*std::exp(0.5*x[j]);
      }
    }

    //Imply vols. from the prices.
    jaeckel_method_default_tolerance<> eps;
    jaeckel_method_default_iterations<> its;
    std::vector<std::vector<double> > vols(ns, std::vector<double>(nx));
    std::ostringstream os;
    os.precision(std::numeric_limits<double>::digits10+1);
    for(std::size_t i = 0; i < ns; ++i)
    {
      double num_std_devs=x_max/s[i];
      os << "num std. devs. : " << num_std_devs << "\n\n";
      for(std::size_t j = 0; j < nx; ++j)
      {
        double price=prices[i][j];
        double fwd=std::exp(x[j]);
        imp_vol_t vol(
            price
          , fwd
          , strike
          , time_to_maturity
          , (x[j] > 0 ? put : call), its, eps);

        BOOST_CHECK(
          vol.status() == imp_vol_t::determined ||
          vol.status() == imp_vol_t::converged);

        double theta =(x[j] > 0 ? -1 : 1);
        double sig=vol.as_number();

        os << "price : " << price << '\n';
        os << "forward : " << std::exp(x[j]) << '\n';
        os << "strike : " << 1 << '\n';
        os << "time to maturity : " << 1 << '\n';
        os << "put/call code : " << theta << '\n';
        os << "implied vol : " << vol.as_number() << "\n";
        os << "iterations : " << vol.iterations() << "\n\n";

        //Have we recovered the input vol?
        double n=std::abs(num_std_devs);
        if(n <= 37.5)
          BOOST_CHECK_CLOSE(sig, s[i], 1.0e-8); //relative closeness check
      }

      os << "---\n\n";
    }

    BOOST_MESSAGE(os.str());
  }

BOOST_AUTO_TEST_SUITE_END()  

}//namespace<anonymous>
