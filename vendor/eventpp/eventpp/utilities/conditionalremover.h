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

		template <typ