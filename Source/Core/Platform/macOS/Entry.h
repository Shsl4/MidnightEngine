#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>
#import <SDL2/SDL.h>

NS_ASSUME_NONNULL_BEGIN

@interface Entry : NSObject <NSApplicationDelegate>

- (int)entry:(int)argc argv:(const char *_Nonnull *_Nonnull)argv;

- (int)initEngine:(SDL_Window *)window;

- (void)update;

+ (NSEvent *)getNextEvent;

@end

NS_ASSUME_NONNULL_END
