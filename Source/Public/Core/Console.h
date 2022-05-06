#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>
#include <Core/EngineTypes.h>

#include <Logging/Logger.h>
#include <Memory/AutoReleasePointer.h>

#include <thread>

class Console final : public Object{
    
public:
    
    Console(class Engine* engine);
    
    ~Console();
    
    FORCEINLINE static Console* getInstance(){
        return instance;
    }
    
private:
    
    void consoleLoop();
    
    inline static Console* instance = nullptr;
    
    AutoReleasePointer<Logger> logger;
    
    std::thread consoleThread;
    
    Engine* engine;
    
};
