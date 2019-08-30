#ifndef _MOVE_ACCUMULATE_H
#define _MOVE_ACCUMULATE_H

#include <utility>

namespace stlutil
{

/// Like std::accumulate, but aware of move semantics.
/// accumulating into init when T has a move assignment
/// and move constructor will result in no copies.
template <class InputIt, class T, class BinaryOp>
T&& move_accumulate(InputIt begin, InputIt end, T&& init, BinaryOp op)
{
	for ( ; begin != end ; ++begin)
		init = op(std::forward<T>(init), *begin);

	return std::forward<T>(init);
}

};

#endif
