#include <Console/Console.h>
#include <Core/Engine.h>

#include <Exception/Exception.h>

Console::Console(Engine* engine) : engine(engine){
    
    Logger::check(!instance, "Console has been built twice. Aborting.");
    
    Console::instance = this;
    
    this->logger = UniquePointer<Logger>::make("Engine");
    this->commandTree = UniquePointer<CommandTree>::make();

    const auto helpNode = CommandNode::make("help");
    const auto exitNode = CommandNode::make("exit");
    const auto echoNode = CommandNode::make("echo");

    helpNode->setNodeDescription("Prints this help menu.");
    exitNode->setNodeDescription("Exits the engine.");
    echoNode->setNodeDescription("Outputs text to the console.");

    exitNode->addExecutable([engine](const auto* context) {
        engine->stop();
    });
        
    helpNode->addExecutable([this](const auto* context) {
        consoleHelp(context);
    });

    echoNode->addArgument("text", ArgumentType::String)
            ->addExecutable([this](const auto* context) {

                String str = context->getString("text");
                logger->debug("{}", str);        
                
            });
    
    registerCommand(helpNode);
    registerCommand(exitNode);
    registerCommand(echoNode);
    
}

Console::~Console(){
    
    logger.release();
    Console::instance = nullptr;
    
}

void Console::registerCommand(SharedPointer<CommandNode> const& node) const
{
    commandTree->registerNode(node);
}

void Console::init()
{
    this->consoleThread = std::thread(&Console::consoleLoop, this);
    this->consoleThread.detach();
}

void Console::consoleLoop(){
    
    while (engine->isRunning()) {
        
        String line = logger->prompt();

        if(!line.isEmpty())
        {
            execute(line);
        }        

    }
    
}

void Console::execute(String const& command) const {

    Array<String> args = command.split(' ');
    const String name = args.getAt(0);
    args.removeAt(0);
        
    try
    {
        commandTree->execute(name, args);
    }
    catch(CommandError const& error)
    {
        logger->error(error.what());
    }
    
}

void Console::consoleHelp(const CommandContext* context)
{

    logger->info("Printing help: ");

    const auto nodes = commandTree->getNodes();
    
    for (auto const& node : nodes)
    {
        logger->info("{}: {}", node->getNodeName(), node->getNodeDescription());
    }
    
}
