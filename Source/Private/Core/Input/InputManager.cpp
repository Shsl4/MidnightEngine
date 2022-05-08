#include <Input/InputManager.h>
#include <SDL2/SDL.h>

void InputManager::invokeIfMatch(const KeyBind &kb, const std::vector<KeyBindEntry>& arr) {

    for (auto const &e: arr) {
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
