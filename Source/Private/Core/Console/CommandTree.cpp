#include <Console/CommandTree.h>
#include <Core/Engine.h>
#include <Console/CommandError.h>

CommandTree::CommandTree()
{
    this->rootNode = AutoReleasePointer<CommandNode>::make("root");
    this->logger = AutoReleasePointer<Logger>::make("Console");
}

void CommandTree::execute(String const& command, Array<String> const& args)
{

    CommandNode* node = rootNode->findChildNode(command);

    if (!node)
    {
        logger->error("{}: Unknown command.", command);
        return;
    }

    auto result = node->getExecutable()(node);
    
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
            CommandError::throwIf(!leaf->isExecutable(), "One of the leaves is not executable. Make sure to bind a function for every command.");
        }
    
        node->lock();
        rootNode->nodes += node;
        
    }
    catch(CommandError const& error)
    {
        logger->error(fmt::format("Error while registering command {}: {}", node->getNodeName(), error.what()).c_str());
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