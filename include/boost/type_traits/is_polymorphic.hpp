//  (C) Copyright John Maddock 2000. Permission to copy, use, modify, sell and   
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

#ifndef BOOST_TT_IS_POLYMORPHIC_HPP
#define BOOST_TT_IS_POLYMORPHIC_HPP

#include <boost/type_traits/is_class.hpp>
// should be the last #include
#include "boost/type_traits/detail/bool_trait_def.hpp"

namespace boost{
namespace detail{

template <class T>
struct is_polymorphic_imp1
{
   struct d1 : public T
   {
      d1();
      ~d1();
      char padding[256];
   };
   struct d2 : public T
   {
      d2();
      virtual ~d2();
#ifndef BOOST_MSVC
      // for some reason this messes up VC++ when T has virtual bases:
      virtual void foo();
#endif
      char padding[256];
   };
   BOOST_STATIC_CONSTANT(bool, value = (sizeof(d2) == sizeof(d1)));
};

template <class T>
struct is_polymorphic_imp2
{
   BOOST_STATIC_CONSTANT(bool, value = false);
};

template <bool is_class>
struct is_polymorphic_selector
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp2<T> type;
   };
};

template <>
struct is_polymorphic_selector<true>
{
   template <class T>
   struct rebind
   {
      typedef is_polymorphic_imp1<T> type;
   };
};

template <class T>
struct is_polymorphic_imp
{
   typedef is_polymorphic_selector< ::boost::is_class<T>::value> selector;
   typedef typename selector::template rebind<T> binder;
   typedef typename binder::type imp_type;
   BOOST_STATIC_CONSTANT(bool, value = imp_type::value);
};

} // namespace detail

BOOST_TT_AUX_BOOL_TRAIT_DEF1(is_polymorphic,T,::boost::detail::is_polymorphic_imp<T>::value)

} // namespace boost

#include "boost/type_traits/detail/bool_trait_undef.hpp"

#endif

