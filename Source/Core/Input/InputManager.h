#pragma once

#include <Core/Object.h>
#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <SDL2/SDL.h>
#include <map>
#include <vector>
#include <functional>

enum class EInputEvent {

    Pressed,
    Released,

};

enum class EAxisType {

    MouseX,
    MouseY

};

enum class EKeyModifier {

    Shift = 1,
    Control = 1 << 6,
    Alt = 1 << 8,
    Command = 1 << 10

};

#define bitCheck(a, p) (a & (1 << (p)))

struct KeyBind {

    Int32 key;
    bool control;
    bool shift;
    bool alt;
    bool command;
    bool isMouseButton;
    bool isModifierKey;

    bool operator==(const KeyBind &other) const {

        if (other.key != this->key) return false;
        if (other.shift != this->shift) return false;
        if (other.command != this->command) return false;
        if (other.control != this->control) return false;
        if (other.alt != this->alt) return false;
        if (other.isMouseButton != this->isMouseButton) return false;
        if (other.isModifierKey != this->isModifierKey) return false;

        return true;

    }

    bool operator<(const KeyBind &other) const {
        return this->key < other.key;
    }

    explicit KeyBind(const UInt8 button) {

        this->key = button;
        this->shift = false;
        this->control = false;
        this->alt = false;
        this->command = false;
        this->isMouseButton = true;
        this->isModifierKey = false;

    }

    explicit KeyBind(const SDL_KeyCode key) {

        this->key = key;
        this->shift = (key == SDLK_LSHIFT) || (key == SDLK_RSHIFT);
        this->control = (key == SDLK_LCTRL) || (key == SDLK_RCTRL);
        this->alt = (key == SDLK_LALT) || (key == SDLK_RALT);
        this->command = (key == SDLK_LGUI) || (key == SDLK_RGUI);;
        this->isMouseButton = false;
        this->isModifierKey = shift || control || command || alt;

    }

    KeyBind(const SDL_Keycode key, UInt16 mods) {

        this->key = key;
        this->shift = bitCheck(mods, 0);
        this->control = bitCheck(mods, 6);
        this->alt = bitCheck(mods, 8);
        this->command = bitCheck(mods, 10);
        this->isMouseButton = false;
        this->isModifierKey = shift || control || command || alt;

    }

};

using KeyBindMap = std::map<KeyBind, std::function<void()>>;
using AxisBindMap = std::map<EAxisType, std::function<void(Int32)>>;

/*!
 * The InputManager allows binding functions to input events such as key presses or mouse movement.
 */
class InputManager : public Object {
    
public:

    InputManager();

    void update() const;

    template<class T>
    void bindEvent(T *object, KeyBind key, EInputEvent type, void(T::*f)()) {

        std::function<void()> function = std::bind(f, object);

        switch (type) {
            case EInputEvent::Pressed:
                keyDownEvents.insert(std::make_pair(key, function));
                break;
            case EInputEvent::Released:
                keyUpEvents.insert(std::make_pair(key, function));
                break;
        }

    }

    template<class T>
    void bindAxis(T *object, EAxisType axisType, void(T::* f)(Int32)) {

        const auto function = std::bind(f, object, std::placeholders::_1);
        axisEvents.insert(std::make_pair(axisType, function));

    }

    template<class T>
    void bindMouseMovement(T *object, void(T::* f)(Int32, Int32)) {
        const auto function = std::bind(f, object, std::placeholders::_1, std::placeholders::_2);
        mouseMotionEvents.push_back(function);

    }

private:

    static void invokeIfMatch(const KeyBind &kb, const KeyBindMap &map);

    UniquePtr<Logger> logger;

    KeyBindMap keyDownEvents;
    KeyBindMap keyUpEvents;
    AxisBindMap axisEvents;

    std::vector<std::function<void(Int32, Int32)>> mouseMotionEvents;

};