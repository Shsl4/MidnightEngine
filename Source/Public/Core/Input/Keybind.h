#pragma once

#include <Core/EngineMacros.h>
#include <Core/EngineTypes.h>

struct ENGINE_API KeyBind {

    KeyBind() = default;
    
    explicit KeyBind(UInt8 button);

    explicit KeyBind(Int32 key);

    KeyBind(Int32 key, UInt16 mods);

    KeyBind(Int32 key, bool shift, bool ctrl);
    
    bool operator==(const KeyBind &other) const;

    bool operator<(const KeyBind &other) const;

    NODISCARD const char* getName() const;
    
    Int32 key = 0;
    bool control = false;
    bool shift = false;
    bool alt = false;
    bool command = false;
    bool isMouseButton = false;
    bool isModifierKey = false;
    
};
