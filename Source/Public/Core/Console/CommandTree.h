#pragma once

#include <Core/Object.h>
#include <Core/EngineMacros.h>

#include <Logging/Logger.h>
#include <Memory/AutoReleasePointer.h>

#include <Console/CommandNode.h>

class ENGINE_API CommandTree: public Object
{

public:

    CommandTree();

    void execute(String const& command, Array<String> const& args);

    void registerNode(CommandNode* node);
    
private:
    
    static void gatherLeaves(CommandNode* node, Array<CommandNode*>& storage);
    
    AutoReleasePointer<CommandNode> rootNode;
    AutoReleasePointer<Logger> logger;

};