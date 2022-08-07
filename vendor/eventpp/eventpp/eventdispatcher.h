
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

#ifndef EVENTDISPATCHER_H_319010983013
#define EVENTDISPATCHER_H_319010983013

#include "callbacklist.h"

#include <string>
#include <functional>
#include <type_traits>
#include <mutex>
#include <memory>

namespace eventpp {

namespace internal_ {

template <
	typename EventType_,
	typename Prototype_,
	typename Policies_,
	typename MixinRoot_
>
class EventDispatcherBase;

template <
	typename EventType_,
	typename Policies_,
	typename MixinRoot_,
	typename ReturnType, typename ...Args
>
class EventDispatcherBase <
	EventType_,
	ReturnType (Args...),
	Policies_,
	MixinRoot_
>
{
protected:
	using ThisType = EventDispatcherBase<
		EventType_,
		ReturnType (Args...),
		Policies_,
		MixinRoot_
	>;
	using MixinRoot = typename std::conditional<
		std::is_same<MixinRoot_, void>::value,
		ThisType,
		MixinRoot_
	>::type;
	using Policies = Policies_;

	using Threading = typename SelectThreading<Policies_, HasTypeThreading<Policies_>::value>::Type;

	using ArgumentPassingMode = typename SelectArgumentPassingMode<
		Policies_,
		HasTypeArgumentPassingMode<Policies_>::value,
		ArgumentPassingAutoDetect
	>::Type;

	using Callback_ = typename SelectCallback<
		Policies_,
		HasTypeCallback<Policies_>::value,
		std::function<ReturnType (Args...)>
	>::Type;
	using CallbackList_ = CallbackList<ReturnType (Args...), Policies_>;

	using Prototype = ReturnType (Args...);

	using Map = typename SelectMap<
		EventType_,
		CallbackList_,
		Policies_,
		HasTemplateMap<Policies_>::value
	>::Type;

	using Mixins = typename internal_::SelectMixins<
		Policies_,
		internal_::HasTypeMixins<Policies_>::value
	>::Type;

public:
	using Handle = typename CallbackList_::Handle;
	using Callback = Callback_;
	using Event = EventType_;
	using Mutex = typename Threading::Mutex;

public:
	EventDispatcherBase()
		:
			eventCallbackListMap(),
			listenerMutex()
	{
	}

	EventDispatcherBase(const EventDispatcherBase & other)
		:
			eventCallbackListMap(other.eventCallbackListMap),
			listenerMutex()
	{
	}

	EventDispatcherBase(EventDispatcherBase && other) noexcept
		:
			eventCallbackListMap(std::move(other.eventCallbackListMap)),
			listenerMutex()
	{
	}

	EventDispatcherBase & operator = (const EventDispatcherBase & other)
	{
		eventCallbackListMap = other.eventCallbackListMap;
		return *this;
	}

	EventDispatcherBase & operator = (EventDispatcherBase && other) noexcept
	{
		eventCallbackListMap = std::move(other.eventCallbackListMap);
		return *this;
	}

	void swap(EventDispatcherBase & other) noexcept {
		using std::swap;
		
		swap(eventCallbackListMap, other.eventCallbackListMap);
	}

	Handle appendListener(const Event & event, const Callback & callback)
	{
		std::lock_guard<Mutex> lockGuard(listenerMutex);

		return eventCallbackListMap[event].append(callback);
	}

	Handle prependListener(const Event & event, const Callback & callback)
	{
		std::lock_guard<Mutex> lockGuard(listenerMutex);

		return eventCallbackListMap[event].prepend(callback);
	}

	Handle insertListener(const Event & event, const Callback & callback, const Handle & before)
	{
		std::lock_guard<Mutex> lockGuard(listenerMutex);

		return eventCallbackListMap[event].insert(callback, before);
	}

	bool removeListener(const Event & event, const Handle handle)
	{
		CallbackList_ * callableList = doFindCallableList(event);
		if(callableList) {
			return callableList->remove(handle);
		}

		return false;
	}