#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>

#include <Logging/Logger.h>
#include <Memory/UniquePointer.h>
#include <Console/CommandTree.h>

#include <thread>

class ENGINE_API Console final : public Object {
    
public:
    
    explicit Console(class Engine* engine);
        
    ~Console() override;

    void registerCommand(SharedPointer<CommandNode> const& node) const;

    void execute(String const& command) const;
    
    FORCEINLINE static Console* getInstance(){
        return instance;
    }
    
    FORCEINLINE static const Logger* getLogger() {
        return instance->logger.raw();
    }
    
private:

    friend class Engine;
    
    void init();
    
    void consoleLoop();
    
    void consoleHelp(const CommandContext* context);
    
    inline static Console* instance = nullptr;
    
    UniquePointer<Logger> logger;
    UniquePointer<CommandTree> commandTree;
    
    std::thread consoleThread;
    
    Engine* engine;
    
};