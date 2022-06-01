#include <Console/CommandNode.h>
#include <Core/Engine.h>
#include <Exception/Exception.h>

#include <Console/ArgumentCommandNode.h>
#include <Console/ExecutableCommandNode.h>

#include "Utilities/ArrayUtils.h"

CommandNode* CommandNode::findChildNode(String const& name) const
{
    
    for(auto const& node : nodes)
    {
        if(node->nodeName == name)
        {
            return node.raw();
        }
    }

    return nullptr;
    
}

CommandNode* CommandNode::findLiteralNode(String const& name) const
{
    for(auto const& node : nodes)
    {
        if(node->nodeName == name && nodeType == NodeType::Literal)
        {
            return node.raw();
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
            return node.raw();
        }
    }

    return nullptr;
    
}

bool CommandNode::containsNode(const CommandNode* node) const
{
    for(auto const& childNode : nodes) {
        
        if(node == childNode.raw())
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

void CommandNode::putPath(const CommandNode* node, Array<String>& storage, String string) const
{
    
    if (!node->isLeaf()) {

        String format;

        if (node->getNodeType() == NodeType::Argument)
        {
            format = fmt::format("<{}> ", node->getNodeName());   
        }
        else
        {
            format = fmt::format("{} ", node->getNodeName());   
        }

        string += format;

        for(const auto& child : node->nodes) {
            putPath(child.raw(), storage, string);
        }
        
    }
    else
    {
        storage += string;
    }
    
}

Array<String> CommandNode::getPaths() const {

    Array<String> paths;

    putPath(this, paths, "");
    
    return paths;
    
}

CommandNode::CommandNode(String name): nodeName(std::move(name)), nodeType(NodeType::Literal)
{
        
}

SharedPointer<CommandNode> CommandNode::make(const String& name)
{
    return SharedPointer<CommandNode>::make(name);
}

CommandNode* CommandNode::addLiteral(String const& name)
{
    
    CommandError::throwIf(isLocked(), "Tried to add an argument to a locked node.");
    CommandError::throwIf(name.isEmpty(), "Tried to register a node without a name.");
    CommandError::throwIf(ArrayUtils::contains(name, ' '), "Node names may not include whitespaces");
    CommandError::throwIf(name == "exec", "\"exec\" is not allowed as a node name.");
    CommandError::throwIf(findChildNode(name), "A node named {} already exists for this command.", name);

    const auto node = make(name);
    nodes += node;
    return node.raw();
    
}

CommandNode* CommandNode::addArgument(const String& name, const ArgumentType type)
{

    CommandError::throwIf(isLocked(), "Tried to add an argument to a locked node.");
    CommandError::throwIf(this->nodeType == NodeType::Executable, "Tried to add an argument to an executable node.");
    CommandError::throwIf(name.isEmpty(), "Tried to register an argument without a name.");
    CommandError::throwIf(ArrayUtils::contains(name, ' '), "Node names may not include whitespaces");
    CommandError::throwIf(name == "exec", "\"exec\" is not allowed as a node name.");
    CommandError::throwIf(findChildNode(name), "An argument named {} already exists for this command.", name);
    CommandError::throwIf(findFirstOf(NodeType::Argument), "An argument node already exists for this command.");

    const auto node = ArgumentCommandNode::make(name, type);
    nodes += node;
    return node.raw();
    
}

CommandNode* CommandNode::addExecutable(CommandFunction const& function)
{

    CommandError::throwIf(isLocked(), "Tried to set executable on a locked node.");
    CommandError::throwIf(findFirstOf(NodeType::Executable), "This node already has an executable bound.");
    
    const auto node = ExecutableCommandNode::make("exec", function);
    
    nodes += node;
    
    return node.raw();
        
}

CommandNode::CommandFunction CommandNode::getExecutable() const {

    auto* node = findFirstOf(NodeType::Executable);

    CommandError::throwIf(!node, "This node does not have an executable node attached.");
    
    const auto* execNode = node->cast<ExecutableCommandNode>();

    CommandError::throwIf(!execNode, "Tried to get executable on a non-executable node.");

    return execNode->getCommandFunction();
    
}

void CommandNode::lock()
{
    
    for(auto const& node : nodes)
    {
        node->lock();
    }

    this->locked = true;
    
}

void CommandNode::setNodeDescription(String const& description)
{
    this->nodeDescription = description;
}
