#pragma once

#include <SDL2/SDL.h>
#include <Core/EngineTypes.h>

struct KeyBind {
    
    explicit KeyBind(UInt8 button);

    explicit KeyBind(SDL_KeyCode key);

    KeyBind(SDL_Keycode key, UInt16 mods);
    
    bool operator==(const KeyBind &other) const;

    bool operator<(const KeyBind &other) const;
    
    Int32 key;
    bool control;
    bool shift;
    bool alt;
    bool command;
    bool isMouseButton;
    bool isModifierKey;
    
};
