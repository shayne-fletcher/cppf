#include <cppf/meta/policy_selector.hpp>

#define BOOST_TEST_MODULE cppf_meta_tests
#include <boost/test/unit_test.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/type_traits.hpp>

#include <vector>

namespace
{
  //strictly compile time tests

  void generate_dimensions_test()
  {
    typedef
      boost::mpl::apply
      <
          cppf::meta::detail::generate_dimensions
        , boost::mpl::vector<char, int, long, double>
      >::type dimensions_type;

    typedef
      boost::mpl::vector4
      <
          boost::mpl::int_<1>
        , boost::mpl::int_<2>
        , boost::mpl::int_<3>
        , boost::mpl::int_<4>
     > expected_type;

    BOOST_MPL_ASSERT(( boost::mpl::equal<dimensions_type, expected_type> ));
  }

  struct policy1 {};
  struct policy2 {};

  void policy_selector_test()
  {
    typedef boost::mpl::vector<policy1, policy2> policies;
    typedef cppf::meta::policy_selector<policies> selector;

    BOOST_MPL_ASSERT((
      boost::is_base_of<cppf::meta::discriminator<policy1, 1>, selector> ));
    BOOST_MPL_ASSERT((
      boost::is_base_of<cppf::meta::discriminator<policy2, 2>, selector> ));
  }

}//namespace<anonymous>
