#pragma once

#include <Console/CommandNode.h>

class ENGINE_API ArgumentCommandNode final : public CommandNode
{

public:

    ArgumentCommandNode() = delete;

    explicit ArgumentCommandNode(String name, ArgumentType type);

    static SharedPointer<ArgumentCommandNode> make(String const& name, ArgumentType type);

    FORCEINLINE ArgumentType getArgumentType() const {
        return this->argumentType;
    }
        
private:
    
    ArgumentType argumentType;
    
};