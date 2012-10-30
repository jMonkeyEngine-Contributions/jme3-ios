#import <UIKit/UIKit.h>
#import <OpenGLES/ES2/gl.h>
#import <GLKit/GLKit.h>
#include <jni.h>

@interface jmeAppDelegate : NSObject <UIApplicationDelegate>

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet GLKView *glview;
@property (nonatomic, retain) EAGLContext *ctx;
@property JavaVM* vm;
@property jobject app;
@property jobject harness;
@property jmethodID pauseMethod;
@property jmethodID reactivateMethod;
@property jmethodID closeMethod;

@end
