#pragma once

#include <Core/EngineMacros.h>

enum class ENGINE_API EInputEvent {

    Pressed,
    Released

};

enum class ENGINE_API EAxisType {

    MouseX,
    MouseY

};

enum class ENGINE_API EKeyModifier {

    Shift = 1,
    Control = 1 << 6,
    Alt = 1 << 8,
    Command = 1 << 10

};

class ENGINE_API IEName final {

public:
    
    static const char* toString(const EInputEvent e)
    {
        switch (e) {
            case EInputEvent::Pressed: return "Pressed";
            case EInputEvent::Released: return "Released";
        }

        return "";        
    }

    static const char* toString(const EAxisType e)
    {
        switch (e) {
            case EAxisType::MouseX: return "MouseX";
            case EAxisType::MouseY: return "MouseY";
        }

        return "";        
    }

    static const char* toString(const EKeyModifier e)
    {
        switch (e) {
            case EKeyModifier::Shift: return "Shift";
            case EKeyModifier::Control: return "Control";
            case EKeyModifier::Alt: return "Alt";
            case EKeyModifier::Command: return "Command";
        }

        return "";        
    }
        
};