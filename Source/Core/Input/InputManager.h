#pragma once

#include <Core/Object.h>
#include <Core/EngineTypes.h>
#include <Core/Logging/Logger.h>
#include <Memory/Array.h>
#include <SDL2/SDL.h>
#include <map>
#include <functional>

enum class EInputEvent{
    
    Pressed,
    Released,
    
};

enum class EAxisType {

    MouseX,
    MouseY

};

enum class EKeyModifier{
    
    Shift = 1,
    Control = 1 << 6,
    Alt = 1 << 8,
    Command = 1 << 10
    
};

#define bitCheck(a, p) (a & (1 << (p)))

struct KeyBind{
    
    int32_t key;
    bool control;
    bool shift;
    bool alt;
    bool command;
    bool isMouseButton;
    
    bool operator ==(const KeyBind& other) const{
        
        if(other.key != this->key) return false;
        if(other.shift != this->shift) return false;
        if(other.command != this->command) return false;
        if(other.control != this->control) return false;
        if(other.alt != this->alt) return false;

        return true;
        
    }
    
    bool operator <(const KeyBind& other) const{
        return this->key < other.key;
    }
    
    KeyBind(uint8_t button){
        
        this->key = button;
        this->shift = false;
        this->control = false;
        this->alt = false;
        this->command = false;
        this->isMouseButton = true;
        
    }
    
    KeyBind(SDL_KeyCode key){
        
        this->key = key;
        this->shift = false;
        this->control = false;
        this->alt = false;
        this->command = false;
        this->isMouseButton = false;
        
    }
    
    KeyBind(SDL_Keycode key, uint16_t mods){
        
        this->key = key;
        this->shift = bitCheck(mods, 0);
        this->control = bitCheck(mods, 6);
        this->alt = bitCheck(mods, 8);
        this->command = bitCheck(mods, 10);
        this->isMouseButton = false;

    }
    
};

typedef std::map<KeyBind, std::function<void()>> KeyBindMap;
typedef std::map<EAxisType, std::function<void(int)>> AxisBindMap;

class InputManager
{
public:
    
    InputManager();
    
    void update();
    
    template<class T>
    void bindEvent(T* object, KeyBind key, EInputEvent type, void(T::*f)()){
        
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
    void bindAxis(T* object, EAxisType axisType, void(T::* f)(int)) {
        
        std::function<void(int)> function = std::bind(f, object, std::placeholders::_1);
        axisEvents.insert(std::make_pair(axisType, function));

    }
    
    FORCEINLINE bool leftMousePressed() { return lmbPressed; }
    FORCEINLINE bool rightMousePressed() { return rmbPressed; }
    FORCEINLINE bool middleMousePressed() { return mmbPressed; }
    FORCEINLINE int getMouseX() { return this->mouseX; }
    FORCEINLINE int getMouseY() { return this->mouseY; }

private:
    
    void invokeIfMatch(KeyBind& kb, KeyBindMap& map);
    
    UniquePtr<Logger> logger;
    KeyBindMap keyDownEvents;
    KeyBindMap keyUpEvents;
    AxisBindMap axisEvents;

    int mouseX;
    int mouseY;
    
    bool lmbPressed;
    bool rmbPressed;
    bool mmbPressed;

};
