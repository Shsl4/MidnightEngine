#pragma once

#include <Platform/PlatformData.h>
#include <Core/Engine.h>

class ENGINE_API Entry {

public:

    int entry(int argc, const char **argv, std::function<Engine*()> const& provider);

    int initEngine(PlatformData data);

    void mainThread() const;

private:

    bool hasTerminated = false;
    Engine* engine = nullptr;

};
