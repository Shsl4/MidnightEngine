#import <Platform/PlatformData.h>
#import <Foundation/Foundation.h>
#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

@interface Entry : NSObject <NSApplicationDelegate>

- (int)entry:(int)argc argv:(const char *_Nonnull *_Nonnull)argv;

- (int)initEngine:(PlatformData)data;

- (void)update;

@end

NS_ASSUME_NONNULL_END
