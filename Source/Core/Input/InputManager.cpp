#include <Input/InputManager.h>

InputManager::InputManager() {
    this->logger = std::make_unique<Logger>("InputManager");
}

void InputManager::invokeIfMatch(const KeyBind &kb, const KeyBindMap &map) {

    for (auto const &[key, func]: map) {
        if (key == kb || (key.isModifierKey && key.key == kb.key)) {
            func();
        }
    }

}

void InputManager::update() const {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        
        const SDL_Keysym keySym = event.key.keysym;
        auto kb = KeyBind(0);

        switch (event.type) {
                
            case SDL_MOUSEMOTION:

                for (const auto& [axisType, func]: this->axisEvents) {

                    if (axisType == EAxisType::MouseX) {
                        func(event.motion.xrel);
                    }

                    if (axisType == EAxisType::MouseY) {
                        func(event.motion.yrel);
                    }

                }

                break;

            case SDL_MOUSEBUTTONDOWN:

                kb = KeyBind(event.button.button);
                invokeIfMatch(kb, keyDownEvents);
                logger->debug("Received mouse button press {}.", event.button.button);
                break;

            case SDL_MOUSEBUTTONUP:

                kb = KeyBind(event.button.button);
                invokeIfMatch(kb, keyUpEvents);
                logger->debug("Received mouse button release {}.", event.button.button);
                break;

            case SDL_KEYDOWN:

                kb = KeyBind(keySym.sym, keySym.mod);
                invokeIfMatch(kb, keyDownEvents);
                logger->debug("Received \"{}\" key press.", SDL_GetKeyName(keySym.sym));
                break;

            case SDL_KEYUP:

                kb = KeyBind(keySym.sym, keySym.mod);
                invokeIfMatch(kb, keyUpEvents);
                logger->debug("Received \"{}\" key release.", SDL_GetKeyName(keySym.sym));
                break;
                
            default:
                break;
                
        }

    }

}
