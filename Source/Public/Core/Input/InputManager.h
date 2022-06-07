#pragma once

#include <Logging/Logger.h>

#include <vector>
#include <functional>

#include <Core/Object.h>
#include <Input/Keybind.h>
#include <Input/InputEnums.h>
#include <Memory/UniquePointer.h>

/*!
 * The InputManager allows binding functions to input events such as key presses or mouse movement.
 */
class ENGINE_API InputManager : public Object {

    struct KeyBindEntry {

        KeyBindEntry() = default;

        KeyBindEntry(void* object, KeyBind keyBind, std::function<void()> function, size_t id) : object(object), keyBind(keyBind),
        function(std::move(function)), id(id) {

        }

        bool operator==(KeyBindEntry const& other) const {
            return (object == other.object) && (keyBind == other.keyBind) && (id == other.id);
        }

        void* object = nullptr;
        KeyBind keyBind;
        std::function<void()> function;
        size_t id = 0;

    };    
    
    struct AxisBindEntry {

        AxisBindEntry() = default;
        
        AxisBindEntry(void* object, EAxisType type, std::function<void(Int32)> function, size_t id) : object(object),
            type(type), function(std::move(function)), id(id) {

        }

        bool operator==(AxisBindEntry const& other) const {
            return (object == other.object) && (type == other.type) && (id == other.id);
        }

        void* object = nullptr;
        EAxisType type;
        std::function<void(Int32)> function;
        size_t id = 0;

    };

    
public:

    InputManager() = default;

    void update() const;
    
    template<class T>
    void bindEvent(T *target, const KeyBind key, const EInputEvent type, void(T::*f)()) {
        const std::function<void()> function = std::bind(f, target);
        
        switch (type) {
            case EInputEvent::Pressed:
                keyDownEvents += (KeyBindEntry(target, key, function, nextId));
                break;
            case EInputEvent::Released:
                keyUpEvents += (KeyBindEntry(target, key, function, nextId));
                break;
        }
        
        ++nextId;

    }

    void unbindAll(void* target);

    template<class T>
    void bindAxis(T *target, EAxisType type, void(T::* f)(Int32)) {

        const auto function = std::bind(f, target, std::placeholders::_1);
        axisEvents += (AxisBindEntry(target, type, function, nextId));
        ++nextId;

    }

private:

    static void invokeIfMatch(const KeyBind &kb, Array<KeyBindEntry> const& array);
    
    size_t nextId = 0;

    Array<KeyBindEntry> keyDownEvents;
    Array<KeyBindEntry> keyUpEvents;
    Array<AxisBindEntry> axisEvents;
    
};
