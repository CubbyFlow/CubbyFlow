/*************************************************************************
> File Name: Event.h
> Project Name: CubbyFlow
> This code is based on Jet Framework that was created by Doyub Kim.
> References: https://github.com/doyubkim/fluid-engine-dev
> Purpose: GEneric Event wrapper class
> Created Time: 2020/03/16
> Copyright (c) 2020, Ji-Hong snowapril
*************************************************************************/

#ifndef CUBBYFLOW_EVENT_H
#define CUBBYFLOW_EVENT_H

#include <functional>
#include <map>

namespace CubbyFlow {
namespace CubbyRender {

    //! Event token (handle) type.
    using EventToken = size_t;

    //! Empty event token.
    static const EventToken kEmptyEventToken = 0;

    //!
    //! \brief Generic event type.
    //!
    //! This class represents a generic event source that can send signals to
    //! multiple callback functions.
    //!
    template <typename... EventArgTypes>
    class Event {
     public:
        //! \brief Callback function type.
        //! The callback function type that can be attached to the event source.
        //! Returns true if an event is handled.
        using CallbackType = std::function<bool(EventArgTypes...)>;

        //!
        //! Invokes callback functions with given arguments.
        //!
        //! \param args Arguments to pass to the attached callback functions.
        //! \return True if one ore more callback functions handled the event.
        //!
        bool operator()(EventArgTypes... args);

        //!
        //! Attaches given callback function.
        //!
        //! \param callback Callback function to attach.
        //! \return Event token (handle) for the attached callback.
        //!
        EventToken operator+=(const CallbackType& callback);

        //!
        //! Detaches the callback function.
        //!
        //! \param token Event token for the callback function to detach.
        //!
        void operator-=(EventToken token);

     private:
        EventToken _lastToken = 0;
        std::map<EventToken, CallbackType> _callbacks;
    };
} 
}  

#include <Framework/Event-Impl.h>

#endif