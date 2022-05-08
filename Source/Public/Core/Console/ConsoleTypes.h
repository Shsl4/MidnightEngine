#pragma once

#include <Core/EngineMacros.h>

enum class ENGINE_API NodeType
{
    Literal,
    Argument,
    Executable
};

enum class ENGINE_API ArgumentType
{
    String,
    Int64,
    Double,
    Bool
};