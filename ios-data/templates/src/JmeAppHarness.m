#include <jni.h>
#import <UIKit/UIKit.h>

#ifndef JNIEXPORT
#define JNIEXPORT __attribute__ ((visibility("default"))) \
  __attribute__ ((used))
#endif

#ifndef _Included_JmeAppHarness
#define _Included_JmeAppHarness
#endif

JNIEXPORT void JNICALL
Java_JmeAppHarness_showDialog(JNIEnv* e, jobject c, jstring text) {
    const char* chars = (*e)->GetStringUTFChars(e, text, 0);
    NSString* string = [[NSString alloc] initWithUTF8String : chars];
    (*e)->ReleaseStringUTFChars(e, text, chars);
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle : @"Message"
            message : string
            delegate : nil
            cancelButtonTitle : @"OK"
            otherButtonTitles : nil];
    [alert show];
    [alert release];
}
