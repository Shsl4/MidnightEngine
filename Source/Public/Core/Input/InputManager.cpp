#include <Input/InputManager.h>
#include <SDL2/SDL.h>

InputManager::InputManager() : logger(ARPointer<Logger>::make("InputManager")) {

}

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
        
        const SDL_Keysym keySym = event.key.keysym;

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
                //logger->debug("Received mouse button press {}.", event.button.button);
                break;

            case SDL_MOUSEBUTTONUP:

                invokeIfMatch(KeyBind(event.button.button), keyUpEvents);
                //logger->debug("Received mouse button release {}.", event.button.button);
                break;

            case SDL_KEYDOWN:

                invokeIfMatch(KeyBind(keySym.sym, keySym.mod), keyDownEvents);
                //logger->debug("Received \"{}\" key press.", SDL_GetKeyName(keySym.sym));
                break;

            case SDL_KEYUP:

                invokeIfMatch(KeyBind(keySym.sym, keySym.mod), keyUpEvents);
                //logger->debug("Received \"{}\" key release.", SDL_GetKeyName(keySym.sym));
                break;
                
            default:
                break;
                
        }

    }

}
