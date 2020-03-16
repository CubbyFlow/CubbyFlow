/*************************************************************************
> File Name: Event.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GEneric Event wrapper class
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_EVENT_IMPL_H
#define CUBBYFLOW_EVENT_IMPL_H

namespace CubbyFlow  {
namespace CubbyRender {

    template <typename... EventArgTypes>
    bool Event<EventArgTypes...>::operator()(EventArgTypes... args) 
    {
        bool handled = false;
        for (auto& callback : _callbacks) {
            handled |= callback.second(args...);
        }
        return handled;
    }

    template <typename... EventArgTypes>
    EventToken Event<EventArgTypes...>::operator+=(const CallbackType& callback) 
    {
        _callbacks[++_lastToken] = callback;
        return _lastToken;
    }

    template <typename... EventArgTypes>
    void Event<EventArgTypes...>::operator-=(EventToken token) 
    {
        _callbacks.erase(token);
    }

} 
} 

#endif 
