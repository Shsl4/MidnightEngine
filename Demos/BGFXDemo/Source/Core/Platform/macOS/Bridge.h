//
//  Platform_macOS.h
//  BGFXDemo
//
//  Created by Pierre on 14/11/2021.
//

#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface Bridge : NSObject

+ (void) initEngine:(NSWindow*)window;

+ (void) renderFrame;

@end

NS_ASSUME_NONNULL_END


