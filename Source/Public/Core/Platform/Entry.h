#pragma once

#include <Platform/PlatformData.h>
#include <Core/Engine.h>

#ifdef __APPLE__

ENGINE_API int macOS_main(int argc, const char** argv);

#else

class ENGINE_API Entry {

public:

    int entry(int argc, const char **argv, std::function<Engine*()> const& provider);

    int initEngine(PlatformData data);

    void mainThread() const;

private:

    bool hasTerminated = false;
    Engine* engine;

};

#endif
