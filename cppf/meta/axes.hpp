#if !defined(AXES_463A830B_0EDB_448A_BBC1_835763641AA8_INCLUDED)
# define AXES_463A830B_0EDB_448A_BBC1_835763641AA8_INCLUDED

# if defined(_MSC_VER) && (_MSC_VER >= 1020)
#   pragma once
# endif// defined(_MSC_VER) && (_MSC_VER >= 1020)

# include <boost/mpl/int.hpp>
# include <boost/mpl/next.hpp>
# include <boost/mpl/back.hpp>
# include <boost/mpl/push_back.hpp>
# include <boost/mpl/empty.hpp>
# include <boost/mpl/accumulate.hpp>
# include <boost/mpl/transform.hpp>
# include <boost/mpl/inherit.hpp>
# include <boost/mpl/inherit_linearly.hpp>
# include <boost/mpl/size.hpp>
# include <boost/mpl/vector_c.hpp>

namespace cppf
{
  namespace meta
  {
    namespace detail
    {
      template <bool=true>
      struct dimension
      {
        template <class SeqT>
        struct apply
        {
          typedef boost::mpl::int_<1>::type type;
        };
      };

      template <>
      struct dimension<false>
      {
        template <class SeqT>
        struct apply
        {
          typedef typename
            boost::mpl::next
            <
              typename boost::mpl::back<SeqT>::type
            >::type type;
        };
      };

      struct add_dimension
      {
        template <class T, class>
        struct apply
        {
          typedef typename
            boost::mpl::push_back
            <
               T
             , typename
                 boost::mpl::apply
                 <
                     dimension<boost::mpl::empty<T>::value>
                   , T
                 >::type
            >::type type;
          };
      };
            
      struct generate_dimensions
      {
        template <class SeqT>
        struct apply
        {
          typedef typename 
            boost::mpl::accumulate
            <
                SeqT
              , boost::mpl::vector_c<int>::type
              , add_dimension
            >::type type;
        };
      };
      
    }//namespace detail

    template <class List, class F>
    struct axes
    {
      typedef typename
        boost::mpl::transform
        <
            List
          , typename
              boost::mpl::apply
              <
                 detail::generate_dimensions
               , List
              >::type
          , F
        >::type axis_types;
    
      typedef typename
        boost::mpl::inherit_linearly
        <
            axis_types
          , boost::mpl::inherit<boost::mpl::_1, boost::mpl::_2>
        >::type type;
    
      enum { dimensions = boost::mpl::size<axis_types>::type::value };
    };
    
  }//namespace meta

}//namespace srlib

#endif//!defined(AXES_463A830B_0EDB_448A_BBC1_835763641AA8_INCLUDED)
