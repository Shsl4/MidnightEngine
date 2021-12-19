#include <Input/InputManager.h>

InputManager::InputManager()
{
    this->logger = std::make_unique<Logger>("InputManager");
}

void InputManager::invokeIfMatch(const KeyBind& kb, const KeyBindMap& map){
    
    for (auto [key, func] : map) {
        if(key == kb || (key.isModifierKey && key.key == kb.key)){
            func();
        }
    }
    
}

void InputManager::update() const
{
    
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        const SDL_Keysym keySym = event.key.keysym;
        auto kb = KeyBind(0);

        switch (event.type)
        {
            case SDL_MOUSEMOTION:

                for (auto i : this->axisEvents) {

                    if (i.first == EAxisType::MouseX) {
                        i.second(event.motion.xrel);
                    }

                    if (i.first == EAxisType::MouseY) {
                        i.second(event.motion.yrel);
                    }

                }


                for (auto func : mouseMotionEvents) {

                    func(event.motion.xrel, event.motion.yrel);

                }

                break;

            case SDL_MOUSEBUTTONDOWN:
                
                kb = KeyBind(event.button.button);
                invokeIfMatch(kb, keyDownEvents);
                break;
                
            case SDL_MOUSEBUTTONUP:
                
                kb = KeyBind(event.button.button);
                invokeIfMatch(kb, keyUpEvents);
                break;
                
            case SDL_KEYDOWN:
                
                kb = KeyBind(keySym.sym, keySym.mod);
                invokeIfMatch(kb, keyDownEvents);
                break;
                                
            case SDL_KEYUP:
                
                kb = KeyBind(keySym.sym, keySym.mod);
                invokeIfMatch(kb, keyUpEvents);
                break;
            default:
                break;
        }
        
    }
    
}
