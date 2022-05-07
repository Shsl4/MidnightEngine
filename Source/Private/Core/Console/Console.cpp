#include <Console/Console.h>
#include <Core/Engine.h>

#include <iostream>

Console::Console(Engine* engine) : engine(engine){
    
    Logger::check(!instance, "Console has been built twice. Aborting.");
    
    Console::instance = this;
    
    this->logger = AutoReleasePointer<Logger>::make("Console");
    this->commandTree = AutoReleasePointer<CommandTree>::make();
    this->consoleThread = std::thread(&Console::consoleLoop, this);
    
    this->consoleThread.detach();

}

Console::~Console(){
    
    logger.release();
    Console::instance = nullptr;
    
}

void Console::registerCommand(CommandNode* node) const
{
    commandTree->registerNode(node);
}

void Console::consoleLoop(){
    
    while (engine->isRunning()) {

        std::cout << "> ";
        std::string input;
        std::getline(std::cin, input);
        
        String line = input;
        Array<String> args = line.split(' ');
        String name = args.getAt(0);
        args.removeAt(0);

        execute(name, args);
        
    }
    
}

void Console::execute(String const& command, Array<String> const& args)
{
    commandTree->execute(command, args);
}
