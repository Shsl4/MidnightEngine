#pragma once

#include <Logging/Logger.h>

#include <map>
#include <functional>

#include <Input/Keybind.h>
#include <Input/InputEnums.h>
#include <Memory/UniquePtr.h>

/*!
 * The InputManager allows binding functions to input events such as key presses or mouse movement.
 */
class ENGINE_API InputManager : public Object {

    using KeyBindMap = std::map<KeyBind, std::function<void()>>;
    using AxisBindMap = std::map<EAxisType, std::function<void(Int32)>>;
    
public:

    InputManager();

    void update() const;

    template<class T>
    void bindEvent(T *target, KeyBind key, EInputEvent type, void(T::*f)()) {

        std::function<void()> function = std::bind(f, target);
        
        switch (type) {
            case EInputEvent::Pressed:
                keyDownEvents.insert(std::make_pair(key, function));
                break;
            case EInputEvent::Released:
                keyUpEvents.insert(std::make_pair(key, function));
                break;
        }

        if (Object::isObject(target))
        {
            const Object* obj = target;
            logger->debug("Registered event binding of type \"{}\" and key \"{}\" from {}",
                IEName::toString(type),
                SDL_GetKeyName(key.key),
                obj->getDescription());
        }
        else
        {
            logger->debug("Registered event binding of type \"{}\" and key \"{}\" from non Object class.",
                IEName::toString(type),
                SDL_GetKeyName(key.key));
        }
        
    }

    template<class T>
    void bindAxis(T *target, EAxisType type, void(T::* f)(Int32)) {

        const auto function = std::bind(f, target, std::placeholders::_1);
        axisEvents.insert(std::make_pair(type, function));

        if (Object::isObject(target))
        {
            const Object* obj = target;
            logger->debug("Registered axis binding of type \"{}\" from {}", IEName::toString(type), obj->getDescription());
        }
        else
        {
            logger->debug("Registered axis binding of type \"{}\" from non Object class.", IEName::toString(type));
        }

    }

private:

    static void invokeIfMatch(const KeyBind &kb, const KeyBindMap &map);

    UniquePtr<Logger> logger;

    KeyBindMap keyDownEvents;
    KeyBindMap keyUpEvents;
    AxisBindMap axisEvents;
    
};
