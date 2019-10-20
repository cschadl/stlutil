// STL Utilities

#ifndef _STLUTIL_FINALLY_H
#define _STLUTIL_FINALLY_H

#include <functional>

namespace stlutil
{

/// Executes a function when the object goes out of scope.
class finally
{
private:
	std::function<void()> m_func;

public:
	finally(const std::function<void()> & func) : m_func(func) { }
	~finally() { m_func(); }

	finally(const finally&) = delete;
	finally& operator=(const finally&) = delete;
};

}; // namespace stlutil

#endif // _STLUTIL_FINALLY_H
