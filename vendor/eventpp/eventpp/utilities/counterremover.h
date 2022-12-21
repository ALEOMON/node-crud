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

#ifndef COUNTERREMOVER_H_211532679833
#define COUNTERREMOVER_H_211532679833

#include "../eventpolicies.h"

#include <memory>

namespace eventpp {

template <typename DispatcherType, typename Enabled = void>
class CounterRemover;

template <typename DispatcherType>
class CounterRemover <
		DispatcherType,
		typename std::enable_if<std::is_base_of<TagEventDispatcher, DispatcherType>::value>::type
	>
{
private:
	template <typename Callback>
	struct Wrapper
	{
		struct Data
		{
			int triggerCount;
			DispatcherType & dispatcher;
			typename DispatcherType::Event event;
			Callback listener;
			typename DispatcherType::Handle handle;
		};

		template <typename ...Args>
		void operator() (Args && ...args) const {
			if(--data->triggerCount <= 0) {
				data->dispatcher.removeListener(data->event, data->handle);
			}
			data->listener(std::forward(args)...);
		}
		
		std::shared_ptr<Data> data;
	};
	
public:
	explicit CounterRemover(DispatcherType & dispatcher)
		: dispatcher(dispatcher)
	{
	}
	
	template <typename Callback>
	typename DispatcherType::Handle appendListener(
			const typename DispatcherType::Event & event,
			const Callback & listener,
			const int triggerCount = 1
		)
	{
		auto data = std::make_shared<typename Wrapper<Callback>::Data>(type