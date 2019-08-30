#ifndef _STLUTIL_TYPE_TRAITS_H
#define _STLUTIL_TYPE_TRAITS_H

#include <type_traits>
#include <utility>

namespace stlutil
{
	// I thnk a lot of this stuff is in the C++17 spec, just not in my implementation of GCC, yet...
	// Most of this is ganked from the "possible implementations" on cppreference.com
	template<class> struct is_ref_wrapper : std::false_type {};
	template<class T> struct is_ref_wrapper<std::reference_wrapper<T>> : std::true_type {};

	template<class B>
	struct negation : std::integral_constant<bool, !bool(B::value)> { };

	template<class T>
	using not_ref_wrapper = negation<is_ref_wrapper<std::decay_t<T>>>;

	// Perform a logical AND on a parameter pack
	template<class...> struct conjunction : std::true_type { };
	template<class B1> struct conjunction<B1> : B1 { };
	template<class B1, class... Bn>
	struct conjunction<B1, Bn...>
	    : std::conditional_t<bool(B1::value), conjunction<Bn...>, B1> {};

	// GCC gets pissy about this for some reason but it's not a big deal
	//template<class... B>
	//inline constexpr bool conjunction_v = conjunction<B...>::value;
};	// namespace stlutil
#endif // #define _STLUTIL_TYPE_TRAITS_H
