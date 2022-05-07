#include <Console/CommandNode.h>
#include <Core/Engine.h>

#include <Console/ArgumentCommandNode.h>
#include <Console/ExecutableCommandNode.h>
#include <Console/CommandError.h>

CommandNode* CommandNode::findChildNode(String const& name) const
{
    
    for(auto const& node : nodes)
    {
        if(node->nodeName == name)
        {
            return node;
        }
    }

    return nullptr;
    
}

CommandNode* CommandNode::findFirstOf(const NodeType type) const
{

    for(auto const& node : nodes)
    {
        if(node->nodeType == type)
        {
            return node;
        }
    }

    return nullptr;
    
}

bool CommandNode::containsNode(CommandNode* node) const
{
    for(auto const& childNode : nodes) {
        
        if(node == childNode)
        {
            return true;
        }
        
        if(childNode->containsNode(node))
        {
            return true;
        }
        
    }

    return false;
    
}

CommandNode::CommandNode(String name): nodeName(std::move(name)), nodeType(NodeType::Literal)
{
        
}

CommandNode* CommandNode::make(const String& name)
{
    return Allocator<CommandNode>().construct(name);
}

CommandNode* CommandNode::addLiteral(const String& name)
{
    
    CommandError::throwIf(isLocked(), "Tried to add an argument to a locked node.");
    CommandError::throwIf(name.isEmpty(), "Tried to register a node without a name.");
    CommandError::throwIf(findChildNode(name), "A node named {} already exists for this command.", name);

    const auto node = make(name);
    nodes += node;
    return node;
    
}

CommandNode* CommandNode::addArgument(const String& name, const ArgumentType type)
{

    CommandError::throwIf(isLocked(), "Tried to add an argument to a locked node.");
    CommandError::throwIf(this->nodeType == NodeType::Executable, "Tried to add an argument to an executable node.");
    CommandError::throwIf(name.isEmpty(), "Tried to register an argument without a name.");
    CommandError::throwIf(findChildNode(name), "An argument named {} already exists for this command.", name);

    const auto node = ArgumentCommandNode::make(name, type);
    nodes += node;
    return node;
    
}

CommandNode* CommandNode::setExecutable(CommandFunction const& function)
{

    CommandError::throwIf(isLocked(), "Tried to set executable on a locked node.");
    CommandError::throwIf(findFirstOf(NodeType::Executable), "This node already has an executable bound.");
    
    const auto node = ExecutableCommandNode::make("exec", function);
    
    nodes += node;
    
    return node;
        
}

bool CommandNode::isExecutable() const
{
    return this->nodeType == NodeType::Executable;
}

CommandNode::CommandFunction CommandNode::getExecutable() const {
    
    const auto* node = findFirstOf(NodeType::Executable)->cast<ExecutableCommandNode>();

    CommandError::throwIf(!node, "Tried to get executable on a non-executable node.");

    return node->getCommandFunction();
    
}

void CommandNode::lock()
{
    
    for(auto const& node : nodes)
    {
        node->lock();
    }

    this->locked = true;
    
}