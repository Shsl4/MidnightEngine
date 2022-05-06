#include <Core/Console.h>
#include <Core/Engine.h>

#include <iostream>

Console::Console(Engine* engine) : engine(engine){
    
    Logger::check(!instance, "Console has been built twice. Aborting.");
    
    Console::instance = this;
    
    this->logger = AutoReleasePointer<Logger>::make("Console");
    this->consoleThread = std::thread(&Console::consoleLoop, this);
    
    this->consoleThread.detach();
    
}

Console::~Console(){
    
    logger.release();
    Console::instance = nullptr;
    
}

void Console::consoleLoop(){
    
    while (engine->isRunning()) {
        
        std::string input;
        std::getline(std::cin, input);
        
        String line = input;
        Array<String> args = line.split(' ');
        
        logger->debug("{}", args.getSize());
        
        for(auto& e : args){
            std::cout << e;
        }
        
        String& name = args.getAt(0);
        
        logger->debug("{}", line);
        
        if (name == "scene.unload") {
            engine->unloadScene();
        }
        else{
            logger->error("Unknown command.");
        }
        
        
    }
    
}
