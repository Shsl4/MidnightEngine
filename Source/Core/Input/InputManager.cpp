#include <Input/InputManager.h>

InputManager::InputManager()
{
    this->logger = std::make_unique<Logger>("InputManager");
}

void InputManager::invokeIfMatch(KeyBind& kb, KeyBindMap& map){
    
    for (auto i : map) {
        if(i.first == kb || (i.first.isModifierKey && i.first.key == kb.key)){
            i.second();
        }
    }
    
}

void InputManager::update(){
    
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
       
        SDL_Keysym keySym = event.key.keysym;
        KeyBind kb = KeyBind(0);

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

                
        }
        
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
        
        for(auto func : mouseMotionEvents){
            
            func(mouseX, mouseY);
            
        }
        
    }
    
}
