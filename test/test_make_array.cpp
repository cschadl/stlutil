#include <iostream>

#include <stlutil/type_traits.h>
#include <stlutil/make_array.h>

template <typename T, size_t N>
class blah
{
private:
	std::array<T, N> m_v;
public:
	// holy fucking shit
	// this makes v5 work
	template<typename...U,
				std::enable_if_t<	stlutil::conjunction<
											std::is_reference<U>..., 
											std::is_same<T, std::decay_t<U>>...>::value, void*> = nullptr>
	blah(U&& ... args)
		: m_v{std::forward<U>(args)...}
	{
		// We'll still get a failure if the # of arguments is > N
		static_assert(sizeof...(args) >= N || sizeof...(args)==1, "Too few arguments");
	}

	// This makes v3, v4 work 
	template<typename...U, 
				std::enable_if_t<stlutil::conjunction<std::is_convertible<T, U>...>::value, void*> = nullptr>
	blah(U&& ... args)
		: m_v{static_cast<T>(std::forward<U>(args))...}	// the static_cast<T> prevents 'narrowing int -> double' warning
	{
		// We'll still get a failure if the # of arguments is > N
		static_assert(sizeof...(args) >= N || sizeof...(args)==1, "Too few arguments");
	}

	// SFINAE failed, so look at these constructors
	blah()
		: m_v(stlutil::make_array_val<T, N>(T(0)))
	{
	}

	blah(blah const&) = default;

	T const& operator()(size_t n) const { return m_v[n]; }
};

int main()
{
	auto arr = stlutil::make_array(1, 2, 3, 4, 5);
	bool const is_5_int_array = std::is_same<decltype(arr), std::array<int, 5>>::value;
	std::cout << "Is an array of 5 ints? " << (is_5_int_array ? "true" : "FALSE") << std::endl;

	std::array<int, 3> ints = stlutil::make_array(1, 2, 3);
	std::cout << "ints is (" << ints[0] << ", " << ints[1] << ", " << ints[2] << ")" << std::endl;

	using blah3d_t = blah<double, 3>;

	blah3d_t v3(1.5, 2.4, 8.7);
	std::cout << "v3 is (" << v3(0) << ", " << v3(1) << ", " << v3(2) << ")" << std::endl;

	blah3d_t v0;
	std::cout << "v0 is (" << v0(0) << ", " << v0(1) << ", " << v0(2) << ")" << std::endl;

	blah3d_t v1(v3);
	std::cout << "v1 is (" << v1(0) << ", " << v1(1) << ", " << v1(2) << ")" << std::endl;

	// These should fail
	//blah3d_t v_bad1(2.3, 1.5);
	//blah3d_t v_bad2(1.8, 84.2, 9.9, 6.3);
	//
	// What about this
	// I get 'warning: narrowing conversion of int to double'
	// (when using std::is_convertible in the std::enable if for blah(...))
	// but is that right?  The standard says that a narrowing conversion
	// doesn't apply if the source is a constant expression...
	blah3d_t v4(1, 2, 3);
	std::cout << "v4 is (" << v4(0) << ", " << v4(1) << ", " << v4(2) << ")" << std::endl;

	double d1 = 5.2;
	double d2 = 2.8;
	double d3 = 1.7;
	blah3d_t v5(d1, d2, d3);
	std::cout << "v5 is (" << v5(0) << ", " << v5(1) << ", " << v5(2) << ")" << std::endl;

	return 0;
}
