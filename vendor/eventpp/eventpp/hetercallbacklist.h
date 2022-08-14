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

#ifndef HETERCALLBACKLIST_H_966730936077
#define HETERCALLBACKLIST_H_966730936077

#include "internal/hetercallbacklist_i.h"
#include "callbacklist.h"

#include <array>
#include <functional>
#include <type_traits>
#include <mutex>
#include <memory>
#include <tuple>

namespace eventpp {

namespace internal_ {

template <
	typename PrototypeList_,
	typename Policies_
>
class HeterCallbackListBase
{
protected:
	struct HeterHandle_
	{
		int index;
		std::weak_ptr<void> homoHandle;

		operator bool () const noexcept {
			return (bool)homoHandle;
		}
	};

	struct UnderlyingPoliciesType_
	{
	};

	class HomoCallbackListTypeBase
	{
	public:
		virtual bool empty() = 0;
		virtual bool doRemove(const HeterHandle_ & handle) = 0;
		virtual std::shared_ptr<HomoCallbackListTypeBase> doClone() = 0;
	};

	template <typename T>
	class HomoCallbackListType : public CallbackList<T, UnderlyingPoliciesType_>, public HomoCallbackListTypeBase
	{
	private:
		using super = CallbackList<T, UnderlyingPoliciesType_>;

	public:
		virtual bool empty() override {
			return super::empty();
		}

		virtual bool doRemove(const HeterHandle_ & handle) override {
			auto sp = handle.homoHandle.lock();
			if(! sp) {
				return false;
			}
			return this->remove(typename super::Handle(std::static_pointer_cast<typename super::Handle::element_type>(sp)));
		}

		virtual std::shared_ptr<HomoCallbackListTypeBase> doClone() override {
			return std::make_shared<HomoCallbackListType<T> >(*this);
		}
	};

	using Policies = Policies_;
	using Threading = typename SelectThreading<Policies, HasTypeThreading<Policies>::value>::Type;

	using PrototypeList = PrototypeList_;

public:
	using Handle = HeterHandle_;
	using Mutex = typename Threading::Mutex;

public:
	HeterCallbackListBase()
		:
			callbackListList(),
			callbackListListMutex()
	{
	}

	HeterCallbackListBase(const HeterCallbackListBase & other)
		:
			callbac