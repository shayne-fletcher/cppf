// g++ -fPIC -O3 -ffast-math -shared norm_cdf.cpp inverse_norm_cdf.cpp by_implication_by_implementation.cpp  -o by_implication_by_implementation.so

// i586-mingw32msvc-g++ -DNDEBUG -O3 -ffast-math -shared norm_cdf.cpp inverse_norm_cdf.cpp by_implication_by_implementation.cpp  -o by_implication_by_implementation.dll -Wl,--kill-at -s

#include <limits>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include <float.h>
#include <string.h>

#ifdef WIN32

#include <windows.h>

namespace {
  HMODULE & dll() {
    static HMODULE hmodule = 0;
    return hmodule;
  }
}
BOOL WINAPI DllMain(
		    HINSTANCE hinstDLL,  // handle to DLL module
		    DWORD fdwReason,     // reason for calling function
		    LPVOID lpvReserved   // reserved
		    ) {
  switch (fdwReason) {
  case DLL_PROCESS_ATTACH:
    dll() = hinstDLL;
    break;
  default:;
  }
  return true;
}
#define API_EXPORT( TYPE ) extern "C" __declspec(dllexport) TYPE WINAPI
#else
#define API_EXPORT( TYPE ) extern "C" TYPE
#endif

#include <cppf/maths/norm_cdf.hpp>
#include <cppf/maths/inverse_norm_cdf.hpp>

//double norm_cdf(double);
//double inverse_norm_cdf(double);

using cppf::maths::norm_cdf;
using cppf::maths::inverse_norm_cdf;

namespace {

  typedef double  RealT;
  RealT const zero  =0;
  RealT const one   =1;
  RealT const two   =2;
  RealT const three =3;
  RealT const four  =4;
  RealT const six   =6;
  RealT const eight =8;
  RealT const machine_precision=DBL_EPSILON;
  RealT const pi=3.14159265358979323846264;
  RealT const one_div_sqrt_two_pi=one/sqrt(2*pi);
  RealT const volatility_precision=sqrt(machine_precision)*sqrt(sqrt(machine_precision));

  inline RealT cube(RealT x){ return x*x*x; }

  inline RealT sig_lo(RealT x, RealT beta, RealT b_c)
  {
    using std::abs;
    using std::log;
    using std::sqrt;

    return sqrt(2.0*x*x/(abs(x) - 4.0*log((beta)/(b_c))));
  }
      
  inline RealT sig_hi(RealT x, RealT beta, RealT b_c)
  {
    using std::exp;
    using std::sqrt;
    using std::abs;
      
    RealT e = exp(0.5*x);
    return -2.0*inverse_norm_cdf(((e - beta)/(e - b_c))*
				 norm_cdf(-sqrt(0.5*abs(x))));
  }
      
  inline RealT w(RealT xi, RealT gamma)
  {
    using std::pow;
      
    return (std::min)(pow(xi, gamma), RealT(1.0));
  }


  RealT normalized_black_call(RealT x, RealT sig)
  {
    using std::exp;
    using std::abs;
    using std::pow;


    RealT const x2=x*x;
    RealT const s2=sig*sig;
    RealT const b_max=exp(0.5*x);
    RealT const one_over_b_max=one/b_max;

    if((x2 < machine_precision*s2) || ((x2 + s2) < machine_precision))
      {
	RealT b0 = (s2*s2 > machine_precision)
	  ? one - two*norm_cdf(-0.5*sig)
	  : one_div_sqrt_two_pi*sig*(
				     one - s2*(one/RealT(24)
					       - s2*(one/RealT(640)
						     - s2*(one/RealT(21504)
							   - s2/RealT(884736)))));
	return std::max(b0 + 0.5*x, zero);
      }

    RealT xi=x/sig;
    if(s2 < machine_precision*x2)
      {
	RealT const xi2=xi*xi;
	RealT const phi0=exp(-0.5*xi2)*one_div_sqrt_two_pi;

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

}

#ifdef WIN32
#define TRY try
#ifdef REPORT_ERRORS_WITH_MESSAGE_BOXES
#define CATCH catch (std::exception &e){ MessageBox(0,e.what(),"Error",MB_OK); } catch (...){ MessageBox(0,"of unknown type","Error",MB_OK); }
#else
#define CATCH catch (std::exception &e){ fprintf(stderr,"Error: %s\n",e.what()); } catch (...){ fprintf(stderr,"Unknown error\n"); }
#endif
#endif

#define REQUIRE( condition , message ) if ( !bool(condition) ) throw std::runtime_error( message );

#ifndef TRY
#define TRY
#endif

#ifndef CATCH
#define CATCH
#endif

API_EXPORT(RealT) normalized_black(RealT x, RealT sig, RealT theta){
  TRY {
    return normalized_black_call(theta*x,sig);
  } CATCH;
  return std::numeric_limits<double>::quiet_NaN();
}

API_EXPORT(RealT) jaeckel_method(RealT p, RealT F, RealT K, RealT T, RealT theta)
{
  TRY {
    const unsigned long max_its = DBL_DIG*10/3;
    RealT const zero = 0;

    REQUIRE(zero!=theta,"The call/put flag theta should be +1 or -1.");
    REQUIRE(F>zero,"Non-positive forwards are not permissible.");
    REQUIRE(K>zero,"Non-positive strikes are not permissible.");
    REQUIRE(T>zero,"Non-positive times to maturity are not permissible.");
    REQUIRE(theta>zero||p<K,"Undiscounted put price must be less than strike.");
    REQUIRE(theta<zero||p<F,"Undiscounted call price must be less than forward.");

    using std::log;
    using std::sqrt;
    using std::exp;
    using std::abs;

    RealT const intrinsic = (std::max)(theta*(F-K),zero);
    if (p==intrinsic) return zero;

    RealT beta = (p-intrinsic)/(sqrt(F*K));

    REQUIRE(beta>0,"Option price must be greater than intrinsic value");

    //operate on out-of-the-money calls from here
    beta /= abs(theta);
    RealT const x = -abs(theta*log(F/K));

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
	   <  DBL_MIN)
	  {
	    sig0 += sigstar;
	    sig0 *= 0.5;
	    if(normalized_black_call(x, sig0)
	       < DBL_MIN)
	      {
		sig0 += sig_c;
		sig0 *= 0.5;
	      }
	  }
      }
    else
      sig0 = sig_hi(x, beta, b_c);

    unsigned long iterations=0;

    //halley's method
    while(iterations < max_its)
      {
	RealT b = normalized_black_call(x, sig0);
	RealT xdivsig2 = (x/sig0)*(x/sig0);
	RealT sigdiv2 = 0.5*sig0;
	RealT sigd2s = (sigdiv2)*(sigdiv2);
	RealT bp = exp(-0.5*xdivsig2 - 0.5*sigd2s)/sqrt(2.0*pi);
  
	RealT vn = 0;
	if(beta < b_c)
	  {
	    vn = log(beta/b)*(log(b)/log(beta))*(b/bp);
	  }
	else
	  {
	    vn = (beta - b)/bp;
	  }
	RealT b2divb1 = x*x/cube(sig0) - 0.25*sig0;
	RealT f2divf1 = b2divb1 -
	  (((2 + log(b))/log(b))*bp/b)*((beta < b_c) ? 1 : 0);
	RealT vhatn = (std::max)(vn, -0.5*sig0);
	RealT nhatn = (std::max)(0.5*vhatn*f2divf1, RealT(-0.75));
	RealT sig1 = sig0 + (std::max)(vhatn/(1 + nhatn), -0.5*sig0);
  
	if(abs(((sig1/sig0 - 1))) <= volatility_precision)
	  {
	    break;
	  }

	sig0 = sig1;
	++iterations;
      }

    return sig0/sqrt(T);
  } CATCH;
  return std::numeric_limits<double>::quiet_NaN();
}
