#include <Console/ExecutableCommandNode.h>

ExecutableCommandNode::ExecutableCommandNode(String name, CommandFunction function):
    CommandNode(std::move(name)), function(std::move(function))
{
    this->nodeType = NodeType::Executable;
}

ExecutableCommandNode* ExecutableCommandNode::make(String const& name, CommandFunction const& function)
{
    return Allocator<ExecutableCommandNode>().construct(name, function);
}
