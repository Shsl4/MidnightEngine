#include <Console/CommandContext.h>

#include <Exception/ParseError.h>
#include <Exception/CommandError.h>

bool CommandContext::getBool(String const& argName) const {
        
    Entry* entry = getEntry(argName);
        
    CommandError::throwIf(!entry, "There are no arguments named {}", argName);
    CommandError::throwIf(entry->type != ArgumentType::Bool, "{} is not a boolean!", argName);

    const auto* boolEntry = dynamic_cast<BoolEntry*>(entry);
                
    return boolEntry->value;
        
}

Int64 CommandContext::getInt64(String const& argName) const {
        
    Entry* entry = getEntry(argName);
        
    CommandError::throwIf(!entry, "There are no arguments named {}", argName);
    CommandError::throwIf(entry->type != ArgumentType::Int64, "{} is not an integer!", argName);

    const auto* intEntry = dynamic_cast<Int64Entry*>(entry);
        
    return intEntry->value;
        
}

double CommandContext::getDouble(String const& argName) const {
        
    Entry* entry = getEntry(argName);
        
    CommandError::throwIf(!entry, "There are no arguments named {}", argName);
    CommandError::throwIf(entry->type != ArgumentType::Double, "{} is not a floating point number!", argName);

    const auto* doubleEntry = dynamic_cast<DoubleEntry*>(entry);
        
    return doubleEntry->value;
        
}

String CommandContext::getString(String const& argName) const {
        
    Entry* entry = getEntry(argName);
        
    CommandError::throwIf(!entry, "There are no arguments named {}", argName);
    CommandError::throwIf(entry->type != ArgumentType::String, "{} is not a string!", argName);

    const auto* stringEntry = dynamic_cast<StringEntry*>(entry);
        
    return stringEntry->value;
        
}


void CommandContext::tryParse(String const& arg, String const& name, const ArgumentType type) {
    switch (type) {
        
    case ArgumentType::String:
        
        addString(arg, name);
        break;
        
    case ArgumentType::Int64:

        try
        {
            addInt64(arg.toInteger(), name);
        }
        catch(ParseError const& e)
        {
            throw CommandError(e.what());
        }
        break;
        
    case ArgumentType::Double: 

        try
        {
            addDouble(arg.toDouble(), name);
        }
        catch(ParseError const& e)
        {
            throw CommandError(e.what());
        }
        break;
        
    case ArgumentType::Bool: 

        try
        {
            addBool(arg.toBool(), name);
        }
        catch(ParseError const& e)
        {
            throw CommandError(e.what());
        }
        break;

    }
    
}

CommandContext::Entry* CommandContext::getEntry(String const& name) const
{
    for(auto* e : entries)
    {
        if (e->argumentName == name)
        {
            return e;
        }
    }

    return nullptr;
        
}

CommandContext::Entry::Entry(ArgumentType type, String argumentName): type(type), argumentName(std::move(argumentName)) {
    
}

CommandContext::BoolEntry::BoolEntry(const ArgumentType t, String name, bool v): Entry(t, std::move(name)), value(v) {

}

CommandContext::DoubleEntry::DoubleEntry(const ArgumentType t, String name, double v): Entry(t, std::move(name)), value(v) {

}

CommandContext::Int64Entry::Int64Entry(const ArgumentType t, String name, Int64 v): Entry(t, std::move(name)), value(v) {

}

CommandContext::StringEntry::StringEntry(const ArgumentType t, String name, String v): Entry(t, std::move(name)),
    value(std::move(v)) {

}

void CommandContext::addBool(bool value, String const& name)
{
    entries += Allocator<BoolEntry>().construct(ArgumentType::Bool, name, value);
}

void CommandContext::addDouble(double value, String const& name)
{
    entries += Allocator<DoubleEntry>().construct(ArgumentType::Double, name, value);
}

void CommandContext::addInt64(Int64 value, String const& name)
{
    entries += Allocator<Int64Entry>().construct(ArgumentType::Int64, name, value);
}

void CommandContext::addString(String const& value, String const& name)
{
    entries += Allocator<StringEntry>().construct(ArgumentType::String, name, value);
}
