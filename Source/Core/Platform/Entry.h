#pragma once

#define SDL_MAIN_HANDLED

#include <Platform/PlatformData.h>
#include <Core/Engine.h>

#ifdef __APPLE__

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface Entry : NSObject <NSApplicationDelegate>

- (int)entry:(int)argc argv:(const char *_Nonnull *_Nonnull)argv;

- (int)initEngine:(PlatformData)data;

- (void)update;

@end

NS_ASSUME_NONNULL_END

int macOS_main(int argc, const char** argv);

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