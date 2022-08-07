
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

#ifndef EVENTQUEUE_H_705786053037
#define EVENTQUEUE_H_705786053037

#include "eventdispatcher.h"
#include "internal/eventqueue_i.h"

#include <list>
#include <tuple>
#include <chrono>
#include <mutex>
#include <array>
#include <cassert>

namespace eventpp {

namespace internal_ {

template <
	typename EventType_,
	typename Prototype_,
	typename Policies_
>
class EventQueueBase;

template <
	typename EventType_,
	typename Policies_,
	typename ReturnType, typename ...Args
>
class EventQueueBase <
		EventType_,
		ReturnType (Args...),
		Policies_
	> : public EventDispatcherBase<
		EventType_,
		ReturnType (Args...),
		Policies_,
		EventQueueBase <
			EventType_,
			ReturnType (Args...),
			Policies_
		>
	>
{
private:
	using super = EventDispatcherBase<
		EventType_,
		ReturnType (Args...),
		Policies_,
		EventQueueBase <
			EventType_,
			ReturnType (Args...),
			Policies_
		>
	>;

	using Policies = typename super::Policies;
	using Threading = typename super::Threading;
	using ConditionVariable = typename Threading::ConditionVariable;

	struct QueuedEvent_
	{
		typename std::remove_cv<typename std::remove_reference<typename super::Event>::type>::type event;
		std::tuple<typename std::remove_cv<typename std::remove_reference<Args>::type>::type...> arguments;
	};

	using BufferedItemList = std::list<BufferedItem<sizeof(QueuedEvent_)> >;

public:
	using QueuedEvent = QueuedEvent_;
	using super::Event;
	using super::Handle;
	using super::Callback;
	using Mutex = typename super::Mutex;

	struct DisableQueueNotify
	{
		DisableQueueNotify(EventQueueBase * queue)
			: queue(queue)
		{
			++queue->queueNotifyCounter;
		}

		~DisableQueueNotify()
		{
			--queue->queueNotifyCounter;

			if(queue->doCanNotifyQueueAvailable() && ! queue->emptyQueue()) {
				queue->queueListConditionVariable.notify_one();
			}
		}

		EventQueueBase * queue;
	};

public:
	EventQueueBase()
		:
			super(),
			queueListConditionVariable(),
			queueEmptyCounter(0),
			queueNotifyCounter(0),
			queueListMutex(),
			queueList(),
			freeListMutex(),
			freeList()
	{
	}

	EventQueueBase(const EventQueueBase & other)
		: super(other)
	{
	}

	EventQueueBase(EventQueueBase && other) noexcept
		: super(std::move(other))
	{
	}

	EventQueueBase & operator = (const EventQueueBase & other)
	{
		super::operator = (other);
		return *this;
	}
	
	EventQueueBase & operator = (EventQueueBase && other) noexcept
	{
		super::operator = (std::move(other));
		return *this;
	}
