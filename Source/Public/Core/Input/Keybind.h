#pragma once

#include <Core/EngineMacros.h>
#include <Core/EngineTypes.h>

struct ENGINE_API KeyBind {
    
    explicit KeyBind(UInt8 button);

    explicit KeyBind(Int32 key);

    KeyBind(Int32 key, UInt16 mods);
    
    bool operator==(const KeyBind &other) const;

    bool operator<(const KeyBind &other) const;

    NODISCARD const char* getName() const;
    
    Int32 key;
    bool control;
    bool shift;
    bool alt;
    bool command;
    bool isMouseButton;
    bool isModifierKey;
    
};
