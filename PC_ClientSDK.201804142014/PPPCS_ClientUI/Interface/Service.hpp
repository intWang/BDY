#pragma once

#include <vector>
#include <algorithm>
#include <functional>

namespace ls
{
    template<class T, class CallbackType>
    class Service : public T
    {
    public:
        /**
         * @brief
         * Real Callback Type
         */
        using CallbackWeakPtr = std::weak_ptr<CallbackType>;

        /**
         * @brief
         * addCallback to list.
         * Before that it need cast from abstract ICallback to I???Callback
         */
        virtual bool addCallback(ICallbackWeakPtr callback, ReceivePriority priority = ReceivePriority::Normal) override
        {
            if (auto strongPtr = callback.lock())
            {
                if (auto realCallback = std::dynamic_pointer_cast<CallbackType>(strongPtr))
                {
                    mCallbacks.emplace_back(CallbackInfo(realCallback, priority));

                    // sort callbacks with priority from high to low.
                    std::sort(mCallbacks.begin(), mCallbacks.end(), [](const CallbackInfo& item1, const CallbackInfo& item2) -> bool
                    {
                        return (int)item1.priority < (int)item2.priority;
                    });
                }
            }
            return true;
        }

        /**
         * @brief
         * removeCallback, before that it need try to cast to I???Callback
         */
        virtual bool removeCallback(ICallbackWeakPtr callback) override
        {
            if (auto strongPtr = callback.lock())
            {
                if (auto realCallback = std::dynamic_pointer_cast<CallbackType>(strongPtr))
                {
                    auto iter = find_if(mCallbacks.begin(), mCallbacks.end(), [realCallback](const auto& item)
                    {
                        if (auto strongCallback = item.callback.lock())
                        {
                            return strongCallback.get() == realCallback.get();
                        }
                        return false;
                    });

                    if (iter != mCallbacks.end())
                        mCallbacks.erase(iter);
                }
            }

            return true;
        }

        using CallbackStrongPtr = std::shared_ptr<CallbackType>;
        using Closure = std::function<void(CallbackStrongPtr)>;
        void fireNotification(Closure closure)
        {
            //Won Zhou,2019/06/20, we MUSR make a copy of callbacks
            //because we don't know what kind of job will happen in this callback
            //someone could addCallback / removeCallback while interaction is in progress
            //this would cause crash
            auto copyOfCallbacks = mCallbacks;
            for (auto& callbackInfo : copyOfCallbacks)
            {
                if (auto strongCallback = std::dynamic_pointer_cast<CallbackType>(callbackInfo.callback.lock()))
                    closure(strongCallback);
            }
        }

    protected:
        struct CallbackInfo
        {
            CallbackWeakPtr callback;
            ReceivePriority priority{ ReceivePriority::Normal };

            CallbackInfo(CallbackWeakPtr _callback, ReceivePriority _priority)
                : callback(_callback)
                , priority(_priority)
            {
            }
        };

        /**
         * @brief
         * List holds the weak-ptr of real callback type
         */
        std::vector<CallbackInfo> mCallbacks;
    };

}
