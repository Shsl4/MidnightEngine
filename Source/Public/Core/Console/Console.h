#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>

#include <Logging/Logger.h>
#include <Memory/AutoReleasePointer.h>
#include <Console/CommandTree.h>

#include <thread>

class ENGINE_API Console final : public Object {
    
public:
    
    explicit Console(class Engine* engine);
    
    ~Console() override;
    
    FORCEINLINE static Console* getInstance(){
        return instance;
    }

    void registerCommand(CommandNode* node) const;
    
private:
    
    void consoleLoop();

    void execute(String const& command, Array<String> const& args);
    
    inline static Console* instance = nullptr;
    
    AutoReleasePointer<Logger> logger;
    AutoReleasePointer<CommandTree> commandTree;
    
    std::thread consoleThread;
    
    Engine* engine;
    
};