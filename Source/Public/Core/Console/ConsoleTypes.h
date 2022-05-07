#pragma once

#include <Core/EngineMacros.h>

enum class ENGINE_API NodeType
{
    Literal,
    Argument,
    Executable
};

enum class ENGINE_API CommandResult
{
    Success,
    Error
};

enum class ENGINE_API ArgumentType
{
    String,
    Int64,
    UInt64,
    Float,
    Double,
    Bool
};