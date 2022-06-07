#include <Input/InputManager.h>
#include <SDL2/SDL.h>

#include "Console/Console.h"

void InputManager::invokeIfMatch(const KeyBind &kb, Array<KeyBindEntry> const& array) {

    for (auto const &e: array) {
        if (e.keyBind == kb || (e.keyBind.isModifierKey && e.keyBind.key == kb.key)) {
            e.function();
        }
    }
    
}

void InputManager::update() const {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        
        const auto [scancode, sym, mod, unused] = event.key.keysym;

        switch (event.type) {
                
            case SDL_MOUSEMOTION:

                for (const auto& e: this->axisEvents) {

                    if (e.type == EAxisType::MouseX) {
                        e.function(event.motion.xrel);
                    }
                    
                    if (e.type == EAxisType::MouseY) {
                        e.function(event.motion.yrel);
                    }

                }

                break;

            case SDL_MOUSEBUTTONDOWN:

                invokeIfMatch(KeyBind(event.button.button), keyDownEvents);
                break;

            case SDL_MOUSEBUTTONUP:

                invokeIfMatch(KeyBind(event.button.button), keyUpEvents);
                break;

        case SDL_KEYDOWN:
            
                invokeIfMatch(KeyBind(sym, mod), keyDownEvents);
                break;

            case SDL_KEYUP:

                invokeIfMatch(KeyBind(sym, mod), keyUpEvents);
                break;
                
            default:
                break;
                
        }

    }

}

void InputManager::unbindAll(void* target) {

    for (size_t i = 0; i < keyDownEvents.getSize();) {
            
        if (keyDownEvents[i].object == target) {
            keyDownEvents.removeAt(i);
            i = 0;
        }
        else {
            ++i;
        }
    }

    for (size_t i = 0; i < keyUpEvents.getSize();) {

        if (keyUpEvents[i].object == target) {
            keyUpEvents.removeAt(i);
            i = 0;
        }
        else {
            ++i;
        }
    }

    for (size_t i = 0; i < axisEvents.getSize();) {

        if (axisEvents[i].object == target) {
            axisEvents.removeAt(i);
            i = 0;
        }
        else{
            ++i;
        }

    }

}
