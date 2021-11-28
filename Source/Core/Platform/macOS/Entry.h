//
//  Main.h
//  MidnightEngine
//
//  Created by Pierre on 27/11/2021.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface Entry : NSObject<NSApplicationDelegate>

- (int) entry:(int)argc argv:(const char* _Nonnull * _Nonnull)argv;

- (int) initEngine:(NSWindow*)window;

- (void) loop;

+ (NSEvent*) getNextEvent;

@end

NS_ASSUME_NONNULL_END
