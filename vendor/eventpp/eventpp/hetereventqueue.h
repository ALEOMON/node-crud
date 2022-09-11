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

#ifndef HETEREVENTQUEUE_H_210892536510
#define HETEREVENTQUEUE_H_210892536510

#include "hetereventdispatcher.h"

// temp, should move the internal code in eventqueue to separated file
#include "eventqueue.h"

namespace eventpp {

namespace internal_ {

template <
	typename EventType_,
	typename PrototypeList_,
	typename Policies_
>
class HeterEventQueueBase : public HeterEventDispatcherBase<
		EventType_,
		PrototypeList_,
		Policies_,
		HeterEventQueueBase <
			EventType_,
			PrototypeList_,
			Policies_
		>
	>
{
private:
	using super = HeterEventDispatcherBase<
		EventType_,
		PrototypeList_,
		Policies_,
		HeterEventQueueBase <
			EventType_,
			PrototypeList_,
			Policies_
		>
	>;

	using Policies = typename super::Policies;
	using Threading = typename super::Threading;
	using ConditionVariable = typename Threading::ConditionVariable;

	struct QueuedItemBase;
	using ItemDispatcher = void (*)(const HeterEventQueueBase *, const QueuedItemBase &);

	struct QueuedItemBase
	{
		QueuedItemBase(const int callableIndex, const EventType_ & event, const ItemDispatcher dispatcher)
			: callableIndex(callableIndex), event(event), dispatcher(dispatcher)
		{
		}

		int callableIndex;
		EventType_ event;
		ItemDispatcher dispatcher;
	};

	template <typename T>
	struct QueuedItem : public QueuedItemBase