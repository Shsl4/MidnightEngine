#pragma once

#include <Platform/PlatformData.h>
#include <Memory/UniquePtr.h>
#include <Core/Engine.h>

#ifdef __APPLE__

ENGINE_API int macOS_main(int argc, const char** argv);

#else

class Entry {

public:

    int entry(int argc, const char **argv);

    int initEngine(PlatformData data);

    void update() const;

private:

    bool hasTerminated = false;
    UniquePtr<Engine> engine;

};

#endif
