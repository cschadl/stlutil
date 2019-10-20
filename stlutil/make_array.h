#ifndef _STLUTIL_INCLUDE_MAKE_ARRAY_H
#define _STLUTIL_INCLUDE_MAKE_ARRAY_H

#include <array>
#include <functional>

#include <stlutil/type_traits.h>

namespace stlutil 
{

namespace detail_
{
	template <typename U, size_t... Is>
	constexpr std::array<U, sizeof...(Is)> make_array_val(U const& value, std::index_sequence<Is...>)
	{
		// The static_cast<void> is in case U has overloaded operator, (who does this?)
		// Also it prevents an annoying compiler warning in GCC
		// warning: left operand of comma operator has no effect [-Wunused-value]
		return {{(static_cast<void>(Is), value)...}};
	}

	template <class D, class...> struct return_type_helper { using type = D; };

	template <class... Types>
	struct return_type_helper<void, Types...> : std::common_type<Types...>
	{
		static_assert(	stlutil::conjunction<not_ref_wrapper<Types>...>::value, 
							"Types cannot contain reference_wrappers when D is void");
	};

	template <class D, class... Types>
	using return_type = std::array<typename return_type_helper<D, Types...>::type, sizeof...(Types)>;

	template <typename T, size_t N, size_t... Ns>
	constexpr std::array<T, N> make_array_from_init_list(std::initializer_list<T> t, std::index_sequence<Ns...>)
	{
		static_assert(sizeof...(Ns) == N, "Number of elements must be <= N");
		return std::array<T, N>{ *(t.begin() + Ns)... };
	}
};

template <class D = void, class... Types>
constexpr detail_::return_type<D, Types...> make_array(Types&&... t) { return {std::forward<Types>(t)... }; }

template <typename T, size_t N>
constexpr std::array<T, N> make_array_from_init_list(std::initializer_list<T> t)
{
	static_assert(N == t.size(), "Array size / num elements mismatch");
	return detail_::make_array_from_init_list<T, N>(t, std::make_index_sequence<N>{});
}

template <typename T, size_t N>
constexpr std::array<T, N> make_array_val(T const& value)
{
	return detail_::make_array_val(value, std::make_index_sequence<N>{});
}

};

#endif // _STLUTIL_INCLUDE_MAKE_ARRAY_H
