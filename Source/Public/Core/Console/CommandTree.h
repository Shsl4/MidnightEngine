#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>

#include <Memory/UniquePointer.h>

#include <Console/CommandNode.h>
#include <Console/CommandContext.h>

#include "Memory/SharedPointer.h"

class ENGINE_API CommandTree: public Object
{

public:

    CommandTree();

    void execute(String const& command, Array<String> const& args);
    
    void registerNode(SharedPointer<CommandNode> const& node) const;

    FORCEINLINE WeakPointer<CommandNode> getRootNode() const {
        return rootNode.weak();
    }

    FORCEINLINE Array<SharedPointer<CommandNode>> getNodes() const{
        return rootNode->nodes;
    }
    
private:
    
    static void gatherLeaves(CommandNode* node, Array<CommandNode*>& storage);
    
    SharedPointer<CommandNode> rootNode;

};
