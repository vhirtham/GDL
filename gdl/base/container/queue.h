#pragma once

#include "gdl/base/container/deque.h"

#include <queue>



namespace GDL
{

template <typename _type>
using Queue = std::queue<_type, Deque<_type>>;

template <typename _type>
using QueueS = std::queue<_type, DequeS<_type>>;

template <typename _type>
using QueueTPS = std::queue<_type, DequeTPS<_type>>;
}
