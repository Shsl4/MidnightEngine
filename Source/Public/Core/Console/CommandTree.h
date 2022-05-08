#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>

#include <Memory/AutoReleasePointer.h>

#include <Console/CommandNode.h>
#include <Console/CommandContext.h>

#include <Exception/ParseError.h>
#include <Exception/CommandError.h>

class ENGINE_API CommandTree: public Object
{

public:

    CommandTree();

    void execute(String const& command, Array<String> const& args);
    
    void registerNode(CommandNode* node);

    FORCEINLINE CommandNode* getRootNode() const {
        return rootNode.raw();
    }

    FORCEINLINE Array<CommandNode*> getNodes() const{
        return { rootNode->nodes };
    }
    
private:
    
    static void gatherLeaves(CommandNode* node, Array<CommandNode*>& storage);
    
    AutoReleasePointer<CommandNode> rootNode;

};