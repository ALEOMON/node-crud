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

#ifndef CONDITIONALREMOVER_H_882115092280
#define CONDITIONALREMOVER_H_882115092280

#include "../eventpolicies.h"

#include <memory>

namespace eventpp {

template <typename DispatcherType, typename Enabled = void>
class ConditionalRemover;

template <typename DispatcherType>
class ConditionalRemover <
		DispatcherType,
		typename std::enable_if<std::is_base_of<TagEventDispatcher, DispatcherType>::value>::type
	>
{
private:
	template <typename Callback, typename Condition>
	struct ItemByCondition
	{
		struct Data
		{
			Condition shouldRemove;
			DispatcherType & dispatcher;
			typename DispatcherType::Event event;
			Callback listener;
			typename DispatcherType::Handle handle;
		};

		template <typename ...Args>
		void operator() (Args && ...args) const {
			if(data->shouldRemove()) {
				data->dispatcher.removeListener(data->event, data->handle);
			}
			data->listener(std::forward(args)...);
		}
		
		std::shared_ptr<Data> data;
	};

public:
	explicit ConditionalRemover(DispatcherType & dispatcher)
		: dispatcher(dispatcher)
	{
	}
	
	template <typename Callback, typename Condition>
	typename DispatcherType::Handle appendListener(
			const typename DispatcherType::Event & event,
			const Callback & listener,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Callback, Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.appendListener(event, Wrapper{data});
		return data->handle;
	}

	template <typename Callback, typename Condition>
	typename DispatcherType::Handle prependListener(
			const typename DispatcherType::Event & event,
			const Callback & listener,
			const Condition & condition
		)
	{
		using Wrapper = ItemByCondition<Callback, Condition>;
		auto data = std::make_shared<typename Wrapper::Data>(typename Wrapper::Data {
			condition, dispatcher, event, listener, typename DispatcherType::Handle()
		});
		data->handle = dispatcher.prependListener(event, Wrapper{data});
		return data->handle;
	}

	template <typename Callback, typena