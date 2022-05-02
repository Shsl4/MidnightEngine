#pragma once

#include <Logging/Logger.h>

#include <map>
#include <functional>

#include <Input/Keybind.h>
#include <Input/InputEnums.h>
#include <Memory/ARPointer.h>

/*!
 * The InputManager allows binding functions to input events such as key presses or mouse movement.
 */
class ENGINE_API InputManager : public Object {

    struct KeyBindEntry {

        KeyBindEntry(void* object, KeyBind keyBind, std::function<void()> function, size_t id) : object(object), keyBind(keyBind), function(function), id(id) {

        }

        bool operator==(KeyBindEntry const& other) {

            return (object == other.object) && (keyBind == other.keyBind) && (id == other.id);

        }

        void* object = nullptr;
        KeyBind keyBind;
        std::function<void()> function;
        size_t id;

    };    
    
    struct AxisBindEntry {

        AxisBindEntry(void* object, EAxisType type, std::function<void(Int32)> function, size_t id) : object(object), type(type), function(function), id(id) {

        }

        bool operator==(AxisBindEntry const& other) {

            return (object == other.object) && (type == other.type) && (id == other.id);

        }

        void* object = nullptr;
        EAxisType type;
        std::function<void(Int32)> function;
        size_t id;

    };

    
public:

    InputManager();

    void update() const;

    size_t nextId = 0;

    template<class T>
    void bindEvent(T *target, KeyBind key, EInputEvent type, void(T::*f)()) {

        std::function<void()> function = std::bind(f, target);
        
        switch (type) {
            case EInputEvent::Pressed:
                keyDownEvents.push_back(KeyBindEntry(target, key, function, nextId));
                break;
            case EInputEvent::Released:
                keyUpEvents.push_back(KeyBindEntry(target, key, function, nextId));
                break;
        }

        if (Object::isObject(target))
        {
            const Object* obj = target;
            logger->debug("Registered event binding of type \"{}\" and key \"{}\" from {}",
                IEName::toString(type),
                key.getName(),
                obj->getDescription());
        }
        else
        {
            logger->debug("Registered event binding of type \"{}\" and key \"{}\" from non Object class.",
                IEName::toString(type),
                key.getName());
        }
        
        ++nextId;

    }

    template<class T>
    void unbindAll(T* target) {

        for (int i = 0; i < keyDownEvents.size();) {
            
            if (keyDownEvents[i].object == target) {
                keyDownEvents.erase(keyDownEvents.begin() + i);
                i = 0;
            }
            else {
                ++i;
            }
        }

        for (int i = 0; i < keyUpEvents.size();) {

            if (keyUpEvents[i].object == target) {
                keyUpEvents.erase(keyUpEvents.begin() + i);
                i = 0;
            }
            else {
                ++i;
            }
        }

        for (int i = 0; i < axisEvents.size();) {

            if (axisEvents[i].object == target) {
                axisEvents.erase(axisEvents.begin() + i);
                i = 0;
            }
            else{
                ++i;
            }

        }

    }

    template<class T>
    void bindAxis(T *target, EAxisType type, void(T::* f)(Int32)) {

        const auto function = std::bind(f, target, std::placeholders::_1);
        axisEvents.push_back(AxisBindEntry(target, type, function, nextId));

        if (Object::isObject(target))
        {
            const Object* obj = target;
            logger->debug("Registered axis binding of type \"{}\" from {}", IEName::toString(type), obj->getDescription());
        }
        else
        {
            logger->debug("Registered axis binding of type \"{}\" from non Object class.", IEName::toString(type));
        }

        ++nextId;

    }

private:

    static void invokeIfMatch(const KeyBind &kb, const std::vector<KeyBindEntry>&);

    ARPointer<Logger> logger;

    std::vector<KeyBindEntry> keyDownEvents;
    std::vector<KeyBindEntry> keyUpEvents;
    std::vector<AxisBindEntry> axisEvents;
    
};
