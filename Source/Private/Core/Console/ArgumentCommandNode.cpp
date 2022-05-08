#include <Console/ArgumentCommandNode.h>

ArgumentCommandNode::ArgumentCommandNode(String name, const ArgumentType type):
    CommandNode(std::move(name)), argumentType(type)
{
    this->nodeType = NodeType::Argument;
}

ArgumentCommandNode* ArgumentCommandNode::make(String const& name, ArgumentType type)
{
    return Allocator<ArgumentCommandNode>().construct(name, type);
}