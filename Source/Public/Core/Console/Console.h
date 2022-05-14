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

    FORCEINLINE static void exec(String const& command) {
        instance->execute(command);
    }

    FORCEINLINE static void registerCommand(SharedPointer<CommandNode> const& node) {
        instance->registerNode(node);
    }
    
    FORCEINLINE static Console* getInstance(){
        return instance;
    }
    
    FORCEINLINE static WeakPointer<Logger> getLogger() {
        return instance->logger.weak();
    }
    
private:

    friend class Entry;
    
    void init();
    
    void consoleLoop() const;
    
    void consoleHelp(const CommandContext* context);

    void registerNode(SharedPointer<CommandNode> const& node) const;

    void execute(String const& command) const;
    
    inline static Console* instance = nullptr;
    
    SharedPointer<Logger> logger;
    
    UniquePointer<CommandTree> commandTree;
    
    std::thread consoleThread;
    
    Engine* engine;
    
};