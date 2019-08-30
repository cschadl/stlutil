#ifndef STLUTIL_CHRONO_H
#define STLUTIL_CHRONO_H

#include <chrono>
#include <functional>

namespace stlutil
{

#if __cplusplus >= 201402L
/// Breaks down duration into specified time intervals 
// ganked from stackoverflow
template<class...Durations, class DurationIn>
std::tuple<Durations...> break_down_durations( DurationIn d )
{
  std::tuple<Durations...> retval;
  using discard=int[];
  (void)discard{0,(void((
    (std::get<Durations>(retval) = std::chrono::duration_cast<Durations>(d)),
    (d -= std::chrono::duration_cast<DurationIn>(std::get<Durations>(retval)))
  )),0)...};
  return retval;
}
#else
template <class... Durations, class DurationIn>
std::tuple<Durations...> break_down_durations(DurationIn)
{
	static_assert(false, "break_down_durations requires C++14 or greater");
	return std::tuple<Durations...>();	// prevent compiler bitching about no return value 
}
#endif	// __cplusplus >= 201402L

template <typename ClockType = std::chrono::steady_clock>
class scoped_time_measure
{
private:
	using time_pt_t = std::chrono::time_point<ClockType>;
	using time_fn_t = std::function<void(const time_pt_t&, const time_pt_t&)>;	 

	time_fn_t m_fn;
	time_pt_t m_begin;

public:
	scoped_time_measure(const time_fn_t& fn) 
		: m_fn(fn)
		, m_begin(ClockType::now())
	{
		
	}

	~scoped_time_measure()
	{
		m_fn(m_begin, ClockType::now());
	}

	scoped_time_measure() = delete;
	scoped_time_measure(scoped_time_measure&) = delete;
	scoped_time_measure& operator=(scoped_time_measure&) = delete;
};

};	// namespace stlutil

#endif	// STLUTIL_CHRONO_H

