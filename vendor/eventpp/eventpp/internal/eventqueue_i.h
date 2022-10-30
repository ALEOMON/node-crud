#ifndef EVENTQUEUE_I_H
#define EVENTQUEUE_I_H

#include <array>
#include <cassert>

namespace eventpp {

namespace internal_ {

template <size_t ...Indexes>
struct IndexSequence
{
};

template <size_t N, size_t ...Indexes>
struct MakeIndexSequence : MakeIndexSequence <N - 1, N - 1, Indexes...>
{
};

template <std::size_t ...Indexes>
struct MakeIndexSequence<0, Indexes...>
{
	using Type = IndexSequence<Indexes...>;
};

template <typename T>
struct CounterGuard
{
	explicit CounterGuard(T & v) : value(v) {
		++value;
	}

	~CounterGuard() {
		--value;
	}

	T & value;
};

using DtorFunc = void (*)(void *);

template <typename T>
void commonDtor(void * instance)
{
	reinterpret_cast<T *>(instance)->~T();
}

template <size_t Size>
class BufferedItem
{
public:
	explicit BufferedItem() : buffer(), dtor(nullptr)
	{
	}

	~BufferedItem()
	{
		if(dtor != nullptr) {
			clear();
		}
	}

	BufferedItem(BufferedItem &&) = delete;
	BufferedIt