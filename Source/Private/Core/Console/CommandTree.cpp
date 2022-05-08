#include <Console/CommandTree.h>
#include <Core/Engine.h>

#include <Console/ArgumentCommandNode.h>
#include <Console/ExecutableCommandNode.h>

CommandTree::CommandTree()
{
    this->rootNode = AutoReleasePointer<CommandNode>::make("root");
}

void CommandTree::execute(String const& command, Array<String> const& args)
{
    
    CommandNode* node = rootNode->findLiteralNode(command);
    const size_t argc = args.getSize();
    const auto context = AutoReleasePointer<CommandContext>::make();

    if (!node)
    {
        Console::getLogger()->error("{}: Unknown command.", command);
        return;
    }    
   
    for (size_t i = 0; i < argc; ++i)
    {
        String& arg = args[i];
        CommandNode* nextNode = node->findLiteralNode(arg);

        if(nextNode)
        {
            node = nextNode;
            continue;
        }

        nextNode = node->findFirstOf(NodeType::Argument);

        if(nextNode)
        {
            
            const auto* argNode = nextNode->cast<ArgumentCommandNode>();
                
            context->tryParse(arg, argNode->getNodeName(), argNode->getArgumentType());

            node = nextNode;

            continue;
               
        }
        
        CommandError::throwError("Too many arguments provided. Expected {}, got {}.", i, argc);

    }
    
    node = node->findFirstOf(NodeType::Executable);

    const Array<String> paths = rootNode->findLiteralNode(command)->getPaths();

    String finalString = "Available paths: \n";

    for (size_t i = 0; i < paths.getSize(); ++i)
    {
        finalString += String(fmt::format("- {}", paths[i]));
        if(i != paths.getSize() - 1)
        {
            finalString += String("\n");
        }
    }
    
    CommandError::throwIf(!node, "This command requires more arguments.\n{}", finalString);
    
    node->cast<ExecutableCommandNode>()->getCommandFunction()(context.raw());
    
}

void CommandTree::registerNode(CommandNode* node)
{
    try
    {
        CommandError::throwIf(node->getNodeName().isEmpty(), "Tried to register an unnamed node.");
        CommandError::throwIf(rootNode->containsNode(node), "This node is already present in the command tree.");
        CommandError::throwIf(node->nodeType != NodeType::Literal, "You may only register command literals.");

        Array<CommandNode*> leaves;
        gatherLeaves(node, leaves);

        for (auto const& leaf : leaves)
        {
            CommandError::throwIf(leaf->nodeType != NodeType::Executable, "One of the leaves is not executable."
                                 " Make sure to bind a function for every command.");
        }
    
        node->lock();
        rootNode->nodes += node;
        
    }
    catch(CommandError const& error)
    {
        Console::getLogger()->error(fmt::format("Error while registering command {}: {}",
            node->getNodeName(), error.what()).c_str());
    }
    
}

void CommandTree::gatherLeaves(CommandNode* node, Array<CommandNode*>& storage)
{

    for (auto const& child : node->nodes)
    {
        gatherLeaves(child, storage);
    }

    if(node->isLeaf())
    {
        storage += node;
    }
    
}