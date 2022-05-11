#pragma once

#include <Core/Object.h>
#include <Core/EngineTypes.h>

#include <Memory/String.h>
#include <Console/CommandNode.h>

class ENGINE_API CommandContext : public Object
{
    
public:

    NODISCARD bool getBool(String const& argName) const;

    NODISCARD Int64 getInt64(String const& argName) const;

    NODISCARD double getDouble(String const& argName) const;

    NODISCARD String getString(String const& argName) const;

    void tryParse(String const& arg, String const& name, ArgumentType type);
    
private:

    struct Entry {
        
        Entry(ArgumentType type, String argumentName);

        virtual ~Entry() = default;
        
        ArgumentType type;
        String argumentName;
        
    };

    struct BoolEntry : public Entry
    {

        BoolEntry(const ArgumentType t, String name, bool v);

        bool value;
        
    };

    struct DoubleEntry : public Entry
    {

        DoubleEntry(const ArgumentType t, String name, double v);

        double value;
        
    };

    struct Int64Entry : public Entry
    {

        Int64Entry(const ArgumentType t, String name, Int64 v);

        Int64 value;
        
    };

    struct StringEntry : public Entry
    {

        StringEntry(const ArgumentType t, String name, String v);

        String value;
        
    };

    void addBool(bool value, String const& name);

    void addDouble(double value, String const& name);

    void addInt64(Int64 value, String const& name);

    void addString(String const& value, String const& name);

    NODISCARD Entry* getEntry(String const& name) const;

    Array<SharedPointer<Entry>> entries;
    
};