#include <Input/InputManager.h>

InputManager::InputManager() : lmbPressed(false), rmbPressed(false),
                               mmbPressed(false), mouseY(0), mouseX(0)
{
    this->logger = std::make_unique<Logger>("InputManager");
}

void InputManager::invokeIfMatch(KeyBind& kb, KeyBindMap& map){
    
    for (auto i : map) {
        if(i.first == kb){
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
            case SDL_MOUSEBUTTONDOWN:
                
                kb = KeyBind(event.button.button);
                invokeIfMatch(kb, keyDownEvents);
                
                if(event.button.button == SDL_BUTTON_LEFT){
                    
                    lmbPressed = true;
                    
                }
                else if(event.button.button == SDL_BUTTON_RIGHT){
                    
                    rmbPressed = true;
                    
                }
                else if(event.button.button == SDL_BUTTON_MIDDLE){
                    
                    mmbPressed = true;

                }
                
                break;
                
            case SDL_MOUSEBUTTONUP:
                
                kb = KeyBind(event.button.button);
                invokeIfMatch(kb, keyUpEvents);
                
                if(event.button.button == SDL_BUTTON_LEFT){
                    
                    lmbPressed = false;
                    
                }
                else if(event.button.button == SDL_BUTTON_RIGHT){
                    
                    rmbPressed = false;
                    
                }
                else if(event.button.button == SDL_BUTTON_MIDDLE){
                    
                    mmbPressed = false;

                }
                
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
        
        SDL_GetMouseState(&mouseX, &mouseY);
        
    }
    
}
