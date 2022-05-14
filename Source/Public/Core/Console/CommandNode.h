#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>

#include <Logging/Logger.h>
#include <Console/ConsoleTypes.h>

#include <Memory/SharedPointer.h>

class ENGINE_API CommandNode : public Object {
    
public:

    using CommandFunction = std::function<void(const class CommandContext*)>;

    CommandNode() = delete;

    explicit CommandNode(String name);

    static SharedPointer<CommandNode> make(const String& name);

    CommandNode* addLiteral(const String& name);
    
    CommandNode* addArgument(const String& name, ArgumentType type);
    
    CommandNode* addExecutable(CommandFunction const& function);

    void lock();

    void setNodeDescription(String const& description);

    NODISCARD CommandFunction getExecutable() const;
    
    bool containsNode(const CommandNode* node) const;

    void putPath(const CommandNode* node, Array<String>& storage, String string) const;
    
    NODISCARD Array<String> getPaths() const;

    FORCEINLINE bool isLeaf() const {
        return this->nodes.getSize() == 0;
    }
    
    FORCEINLINE bool isLocked() const {
        return this->locked;
    }
    
    FORCEINLINE String getNodeName() const {
        return this->nodeName;
    }
    
    FORCEINLINE String getNodeDescription() const {
        return this->nodeDescription.isEmpty() ? "No description provided." : this->nodeDescription;
    }
    
    FORCEINLINE NodeType getNodeType() const {
        return this->nodeType;
    }
    
protected:

    friend class CommandTree;
    
    NODISCARD CommandNode* findChildNode(String const& name) const;
    NODISCARD CommandNode* findLiteralNode(String const& name) const;
    NODISCARD CommandNode* findFirstOf(NodeType type) const;
    
    String nodeName;
    String nodeDescription;
    Array<SharedPointer<CommandNode>> nodes;
    NodeType nodeType = NodeType::Executable;

private:

    bool locked = false;
    
};
