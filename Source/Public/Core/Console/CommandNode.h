#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>

#include <Logging/Logger.h>
#include <Console/ConsoleTypes.h>

class ENGINE_API CommandNode : public Object
{
public:

    using CommandFunction = std::function<CommandResult(const CommandNode*)>;

    CommandNode() = delete;

    explicit CommandNode(String name);

    static CommandNode* make(const String& name);

    CommandNode* addLiteral(const String& name);
    
    CommandNode* addArgument(const String& name, ArgumentType type);
    
    CommandNode* setExecutable(CommandFunction const& function);

    void lock();

    NODISCARD bool isExecutable() const;

    NODISCARD CommandFunction getExecutable() const;
    
    bool containsNode(CommandNode* node) const;

    FORCEINLINE bool isLeaf() const {
        return this->nodes.getSize() == 0;
    }
    
    FORCEINLINE bool isLocked() const {
        return this->locked;
    }

    FORCEINLINE String getNodeName() const {
        return this->nodeName;
    }
    
    FORCEINLINE NodeType getNodeType() const {
        return this->nodeType;
    }
    
protected:

    friend class CommandTree;
    
    NODISCARD CommandNode* findChildNode(String const& name) const;
    NODISCARD CommandNode* findFirstOf(NodeType type) const;
    
    String nodeName;
    AutoReleaseArray<CommandNode*> nodes;
    NodeType nodeType = NodeType::Executable;

private:

    bool locked = false;
    
};