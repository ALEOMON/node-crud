// eventpp library
// Copyright (C) 2018 Wang Qi (wqking)
// Github: https://github.com/wqking/eventpp
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//   http://www.apache.org/licenses/LICENSE-2.0
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef CALLBACKLIST_H_588722158669
#define CALLBACKLIST_H_588722158669

#include "eventpolicies.h"
#include "internal/typeutil_i.h"

#include <atomic>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <utility>

namespace eventpp {

namespace internal_ {

template <
	typename Prototype,
	typename PoliciesType
>
class CallbackListBase;

template <
	typename PoliciesType,
	typename ReturnType, typename ...Args
>
class CallbackListBase<
	ReturnType (Args...),
	PoliciesType
>
{
private:
	using Policies = PoliciesType;

	using Threading = typename SelectThreading<Policies, HasTypeThreading<Policies>::value>::Type;

	using Callback_ = typename SelectCallback<
		Policies,
		HasTypeCallback<Policies>::value,
		std::function<ReturnType (Args...)>
	>::Type;

	using CanContinueInvoking = typename SelectCanContinueInvoking<
		Policies, HasFunctionCanContinueInvoking<Policies, Args...>::value
	>::Type;

	struct Node;
	using NodePtr = std::shared_ptr<Node>;

	struct Node
	{
		using Counter = unsigned int;

		Node(const Callback_ & callback, const Counter counter)
			: callback(callback), counter(counter)
		{
		}

		NodePtr previous;
		NodePtr next;
		Callback_ callback;
		Counter counter;
	};

	class Handle_ : public std::weak_ptr<Node>
	{
	private:
		using super = std::weak_ptr<Node>;

	public:
		using super::super;

		operator bool () const noexcept {
			return ! this->expired();
		}
	};

	using Counter = typename Node::Counter;
	enum : Counter {
		removedCounter = 0
	};

public:
	using Callback = Callback_;
	using Handle = Handle_;
	using Mutex = typename Threading::Mutex;

public:
	CallbackListBase() noexcept
		:
			head(),
			tail(),
			mutex(),
			currentCounter(0)
	{
	}

	CallbackListBase(const CallbackListBase & other)
		: CallbackListBase()
	{
		cloneFrom(other.head);
	}

	CallbackListBase(CallbackListBase && other) noexcept
		: CallbackListBase()
	{
		swap(other);
	}

	// If we use pass by value idiom and omit the 'this' check,
	// when assigning to self there is a deep copy which is inefficient.
	CallbackListBase & operator = (const CallbackListBase & other) {
		if(this != &other) {
			CallbackListBase copied(other);
			swap(copied);
		}
		return *this;
	}

	CallbackListBase & operator = (CallbackListBase && other) noexcept {
		if(this != &other) {
			head = std::move(other.head);
			tail = std::move(other.tail);
			currentCounter = other.currentCounter.load();
		}
		return *this;
	}

	~CallbackListBase()	{
		// Don't lock mutex here since it may throw exception

		NodePtr node = head;
		head.reset();
		while(node) {
			NodePtr next = node->next;
			node->previous.reset();
			node->next.reset();
			node = next;
		}
		node.reset();
	}
	
	void swap(CallbackListBase & other) noexcept {
		using std::swap;
		
		swap(head, other.head);
		swap(tail, other.tail);

		const auto value = currentCounter.load();
		currentCounter.exchange(other.currentCounter.load());
		other.currentCounter.exchange(value);
	}

	bool empty() const {
		// Don't lock the mutex for performance reason.
		// !head still works even when the underlying raw pointer is garbled (for other thread is writting to head)
		// And empty() doesn't guarantee the list is still empty after the function returned.
		//std::lock_guard<Mutex> lock