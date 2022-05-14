#include <Console/ArgumentCommandNode.h>

ArgumentCommandNode::ArgumentCommandNode(String name, const ArgumentType type):
    CommandNode(std::move(name)), argumentType(type)
{
    this->nodeType = NodeType::Argument;
}

SharedPointer<ArgumentCommandNode> ArgumentCommandNode::make(String const& name, ArgumentType type)
{
    return SharedPointer<ArgumentCommandNode>::make(name, type);
}