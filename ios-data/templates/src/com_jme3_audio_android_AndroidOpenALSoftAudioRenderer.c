#include "com_jme3_audio_android_AndroidOpenALSoftAudioRenderer.h"
#include "OpenAL/alc.h"
#include "OpenAL/al.h"
//#include "OpenAL/alext.h"
#include "OpenAL/oalMacOSX_OALExtensions.h"
// for fprintf(stderr, "YourApp", "formatted message");
// #include <android/log.h>
#include <jni.h>
#include <stddef.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <time.h>


static jboolean created = JNI_FALSE;

#define BUFFER_COUNT 1
ALuint* buffers[BUFFER_COUNT] = { 0 };
ALuint* source = 0;

int getError() {
    int errorcode = alGetError();
//    fprintf(stderr, "getError: %d", errorcode);
    return errorcode;
}

/* InitAL opens the default device and sets up a context using default
 * attributes, making the program ready to call OpenAL functions. */
int InitAL()
{
    ALCdevice *device;
    ALCcontext *ctx;

    /* Open and initialize a device with default settings */
    device = alcOpenDevice(NULL);
    if(!device)
    {
        fprintf(stderr, "Could not open a device!\n");
        return 1;
    }

    ctx = alcCreateContext(device, NULL);
    fprintf(stderr, "NULL: %d", NULL);
    fprintf(stderr, "Created context: %d", ctx);
    fprintf(stderr, "Created context addr: %d", &ctx);
    if(ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        if(ctx != NULL)
            alcDestroyContext(ctx);
        alcCloseDevice(device);
        fprintf(stderr, "Could not set a context!\n");
        return 1;
    }

    printf("Opened \"%s\"\n", alcGetString(device, ALC_DEVICE_SPECIFIER));
    fprintf(stderr, "Opened %s", alcGetString(device, ALC_DEVICE_SPECIFIER));
    return 0;
}

/* CloseAL closes the device belonging to the current context, and destroys the
 * context. */
void CloseAL()
{
    ALCdevice *device;
    ALCcontext *ctx;
    ALCboolean result;

//    fprintf(stderr, "Getting current context");
    ctx = alcGetCurrentContext();
//    getError();
    if(ctx == NULL){
        fprintf(stderr, "No context found");
        return;
    }

//    fprintf(stderr, "Getting current context device");
    device = alcGetContextsDevice(ctx);
    if(device == NULL) {
        fprintf(stderr, "No device found");
        return;
    } else {
        fprintf(stderr, "alcGetContextsDevice device: %d", device);
        fprintf(stderr, "alcGetContextsDevice device addr: %d", &device);
    }
//    getError();

//    fprintf(stderr, "Setting context to NULL");
    result = alcMakeContextCurrent(NULL);
//    fprintf(stderr, "alcMakeContextCurrent returned");
//    fprintf(stderr, "alcMakeContextCurrent returned with result: %d", result);
    if(!result) {
        fprintf(stderr, "alcMakeContextCurrent failed");
        return;
    }

//    fprintf(stderr, "Destroying context: %d", ctx);
//    fprintf(stderr, "Destroying context addr: %d", &ctx);
    alcDestroyContext(ctx);

//    fprintf(stderr, "Closing device");
    result = alcCloseDevice(device);
//    fprintf(stderr, "alcCloseDevice result: %d", result);
}


JNIEXPORT jboolean JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alIsCreated
  (JNIEnv* env, jclass cl)
{
    return created;
}


JNIEXPORT jboolean JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alCreate
  (JNIEnv* env, jclass cl)
{
//    fprintf(stderr, "Starting Audio Engine");

    InitAL();
    created = JNI_TRUE;
    return created;

}

JNIEXPORT jboolean JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alDestroy
  (JNIEnv* env, jclass cl)
{

//    fprintf(stderr, "alDestroy");
    CloseAL();
    created = JNI_FALSE;
    return created;

}

JNIEXPORT jstring JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alcGetString
  (JNIEnv* env, jclass cl, jint param)
{
//    fprintf(stderr, "alcGetString for param: %d", param);

    ALCdevice *device;
    ALCcontext *ctx;

    ctx = alcGetCurrentContext();
    if(ctx != NULL) {
        device = alcGetContextsDevice(ctx);
//        fprintf(stderr, "alcGetString param value: %s", alcGetString(device, param));
        return (*env)->NewStringUTF(env, alcGetString(device, param));
    }
}

JNIEXPORT jstring JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGetString
  (JNIEnv* env, jclass cl, jint param)
{
//    fprintf(stderr, "alGetString for param: %d", param);
//    fprintf(stderr, "alGetString param value: %s", alGetString(param));
    return (*env)->NewStringUTF(env, alGetString(param));
}

JNIEXPORT jint JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGenSources
  (JNIEnv *env, jclass cl)
{
    ALuint source;
    alGenSources(1, &source);
//    fprintf(stderr, "alGenSources: %d", source);
    return source;
}

JNIEXPORT jint JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGetError
  (JNIEnv *env, jclass cl)
{
    return getError();
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alDeleteSources
  (JNIEnv* env, jclass cl, jint numSources, jobject intbufSources)
{
//    fprintf(stderr, "alDeleteSources numSources: %d", numSources);

    ALuint* pIntBufSources = (ALuint*) (*env)->GetDirectBufferAddress(env, intbufSources);
    alDeleteSources((ALsizei)numSources, pIntBufSources);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGenBuffers
  (JNIEnv* env, jclass cl, jint numBuffers, jobject intbufBuffers)
{
    ALuint* pIntBufBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, intbufBuffers);
    alGenBuffers((ALsizei)numBuffers, pIntBufBuffers);
//    for (int i=0; i<numBuffers; i++) {
//        fprintf(stderr, "alGenBuffers[%d]: %d", i, *(pIntBufBuffers+i));
//    }

}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alDeleteBuffers
  (JNIEnv* env, jclass cl, jint numBuffers, jobject intbufBuffers)
{
//    fprintf(stderr, "alDeleteBuffers numBuffers: %d", numBuffers);

    ALuint* pIntBufBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, intbufBuffers);
//    fprintf(stderr, "alDeleteBuffers Buffers: %d", *pIntBufBuffers);
//    for (int i=0; i<numBuffers; i++) {
//        if(alIsBuffer(*(pIntBufBuffers+i)) == AL_TRUE) {
//            fprintf(stderr, "alDeleteBuffers[%d]: %d", i, *(pIntBufBuffers+i));
//            fprintf(stderr, "alDeleteBuffers buffer is a known buffer");
//        }
//    }
    alDeleteBuffers((ALsizei)numBuffers, pIntBufBuffers);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSourceStop
  (JNIEnv *env, jclass cl, jint source)
{
//    fprintf(stderr, "alSourceStop for source: %d", source);
    alSourceStop((ALuint)source);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSourcei
  (JNIEnv *env, jclass cl, jint source, jint param, jint value)
{
//    fprintf(stderr, "alSourcei for source: %d, param: %d, value: %d", source, param, value);
    alSourcei((ALuint)source, (ALenum)param, (ALint)value);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alBufferData
  (JNIEnv* env, jclass cö, jint buffer, jint format, jobject bufferData, jint bufferSize, jint frequency)
{
//    fprintf(stderr, "alBufferData for source: %d, format: %d, size: %d, frequency: %d", buffer, format, bufferSize, frequency);
    ALuint* pBufferData = (ALuint*) (*env)->GetDirectBufferAddress(env, bufferData);
    alBufferData((ALuint)buffer, (ALenum)format, pBufferData, (ALsizei)bufferSize, (ALsizei)frequency);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSourcePlay
  (JNIEnv *env, jclass cl, jint source)
{
//    fprintf(stderr, "alSourcePlay for source: %d", source);
    alSourcePlay((ALuint)source);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSourcePause
  (JNIEnv *env, jclass cl, jint source)
{
//    fprintf(stderr, "alSourcePause for source: %d", source);
    alSourcePause((ALuint)source);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSourcef
  (JNIEnv *env, jclass cl, jint source, jint param, jfloat value)
{
//    fprintf(stderr, "alSourcef for source: %d, param: %d, value: %f", source, param, value);
    alSourcef((ALuint)source, (ALenum)param, (ALfloat)value);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSource3f
  (JNIEnv *env, jclass cl, jint source, jint param, jfloat value1, jfloat value2, jfloat value3)
{
//    fprintf(stderr, "alSource3f for source: %d, param: %d, value1: %f, value2: %f, value3: %f", source, param, value1, value2, value3);
    alSource3f((ALuint)source, (ALenum)param, (ALfloat)value1, (ALfloat)value2, (ALfloat)value3);
}

JNIEXPORT jint JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGetSourcei
  (JNIEnv *env, jclass cl, jint source, jint param)
{
//    fprintf(stderr, "alGetSourcei for source: %d, param: %d", source, param);
    ALint result;
    alGetSourcei((ALuint)source, (ALenum)param, &result);
    return (jint)result;
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSourceUnqueueBuffers
  (JNIEnv* env, jclass cl, jint source, jint numBuffers, jobject buffers)
{
//    fprintf(stderr, "alSourceUnqueueBuffers for source: %d, numBuffers: %d", source, numBuffers);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffers);

//    for (ALuint i=0; i<numBuffers; i++) {
//        fprintf(stderr, "alSourceUnqueueBuffers, checking buffer[%d]: %d", i, *(pBuffers+i));
//        ALboolean isBuffer = alIsBuffer(*(pBuffers+i));
//        fprintf(stderr, "buffer check result: %d", isBuffer);
//    }
    alSourceUnqueueBuffers((ALuint)source, (ALsizei)numBuffers, pBuffers);
//    for (ALuint i=0; i<numBuffers; i++) {
//        fprintf(stderr, "alSourceUnqueueBuffers[%d]: %d", i, *(pBuffers+i));
//    }
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSourceQueueBuffers
  (JNIEnv* env, jclass cl, jint source, jint numBuffers, jobject buffers)
{
//    fprintf(stderr, "alSourceQueueBuffers for source: %d, numBuffers: %d", source, numBuffers);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffers);
    alSourceQueueBuffers((ALuint)source, (ALsizei)numBuffers, pBuffers);
//    for (ALuint i=0; i<numBuffers; i++) {
//        fprintf(stderr, "alSourceQueueBuffers[%d]: %d", i, *(pBuffers+i));
//    }
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alListener
  (JNIEnv* env, jclass cl, jint param, jobject bufferData)
{
//    fprintf(stderr, "alListener for param: %d", param);
    ALfloat* pBufferData = (ALfloat*) (*env)->GetDirectBufferAddress(env, bufferData);
    alListenerfv((ALenum)param, pBufferData);
//    getError();
//    for (int i=0; i<4; i++) {
//        fprintf(stderr, "alListener[%d]: %f", i, *(pBufferData+(i*sizeof(ALfloat))));
//    }

}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alListenerf
  (JNIEnv *env, jclass cl, jint param, jfloat value)
{
//    fprintf(stderr, "alListenerf for param: %d, value: %f", param, value);
    alListenerf((ALenum)param, (ALfloat)value);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alListener3f
  (JNIEnv *env, jclass cl, jint param, jfloat value1, jfloat value2, jfloat value3)
{
//    fprintf(stderr, "alListener3f for param: %d, value1: %f, value2: %f, value3: %f", param, value1, value2, value3);
    alListener3f((ALenum)param, (ALfloat)value1, (ALfloat)value2, (ALfloat)value3);
}

JNIEXPORT jboolean JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alcIsExtensionPresent
  (JNIEnv* env, jclass cl, jstring extension)
{

    ALCdevice *device;
    ALCcontext *ctx;
    ALboolean result;
//    char buf[128];
    const char* strExtension;
    jsize strLength = (*env)->GetStringUTFLength(env, extension);

    ctx = alcGetCurrentContext();
    if(ctx != NULL) {
        device = alcGetContextsDevice(ctx);

        if (device != NULL) {
            strExtension = (*env)->GetStringUTFChars(env, extension, NULL);
            if (strExtension == NULL) {
                return JNI_FALSE; /* OutOfMemoryError already thrown */
            }
//            fprintf(stderr, "alcIsExtensionPresent for param: %s with size: %d", strExtension, strLength);

            result = alcIsExtensionPresent(device, strExtension);
//            fprintf(stderr, "alcIsExtensionPresent found: %d", result);

            (*env)->ReleaseStringUTFChars(env, extension, strExtension);

            return (jboolean)result;
        } else {
            fprintf(stderr, "device is null in alcIsExtensionPresent");
        }

    } else {
        fprintf(stderr, "current context is null in alcIsExtensionPresent");
    }

    return JNI_FALSE;
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alcGetInteger
  (JNIEnv* env, jclass cl, jint param, jobject buffer, jint bufferSize)
{
    ALCdevice *device;
    ALCcontext *ctx;

//    fprintf(stderr, "alcGetInteger for param: %d", param);
    ALCint* pBuffers = (ALCint*) (*env)->GetDirectBufferAddress(env, buffer);

    ctx = alcGetCurrentContext();
    if(ctx != NULL) {
        device = alcGetContextsDevice(ctx);

        if (device != NULL) {
            alcGetIntegerv(device, (ALCenum)param, (ALCsizei)bufferSize, pBuffers);
        } else {
            fprintf(stderr, "device is null in alcGetInteger");
        }

    } else {
        fprintf(stderr, "current context is null in alcGetInteger");
    }
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGenAuxiliaryEffectSlots
  (JNIEnv* env, jclass cl, jint numSlots, jobject buffer)
{
//    fprintf(stderr, "alGenAuxiliaryEffectSlots for numSlots: %d", numSlots);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffer);
//    alGenAuxiliaryEffectSlots((ALsizei)numSlots, pBuffers);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGenEffects
  (JNIEnv* env, jclass cl, jint numEffects, jobject buffer)
{
//    fprintf(stderr, "alGenEffects for numEffects: %d", numEffects);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffer);
//    alGenEffects((ALsizei)numEffects, pBuffers);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alEffecti
  (JNIEnv *env, jclass cl, jint effect, jint param, jint value)
{
//    fprintf(stderr, "alEffecti for effect: %d, param: %d, value: %d", effect, param, value);
//    alEffecti((ALuint)effect, (ALenum)param, (ALint)value);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alAuxiliaryEffectSloti
  (JNIEnv *env, jclass cl, jint effectSlot, jint param, jint value)
{
//    fprintf(stderr, "alAuxiliaryEffectSloti for effect: %d, param: %d, value: %d", effectSlot, param, value);
//    alAuxiliaryEffectSloti((ALuint)effectSlot, (ALenum)param, (ALint)value);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alDeleteEffects
  (JNIEnv* env, jclass cl, jint numEffects, jobject buffer)
{
//    fprintf(stderr, "alDeleteEffects for numEffects: %d", numEffects);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffer);
//    alDeleteEffects((ALsizei)numEffects, pBuffers);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alDeleteAuxiliaryEffectSlots
  (JNIEnv* env, jclass cl, jint numEffectSlots, jobject buffer)
{
//    fprintf(stderr, "alDeleteAuxiliaryEffectSlots for numEffectSlots: %d", numEffectSlots);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffer);
//    alDeleteAuxiliaryEffectSlots((ALsizei)numEffectSlots, pBuffers);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alGenFilters
  (JNIEnv* env, jclass cl, jint numFilters, jobject buffer)
{
//    fprintf(stderr, "alGenFilters for numFilters: %d", numFilters);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffer);
//    alGenFilters((ALsizei)numFilters, pBuffers);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alFilteri
  (JNIEnv *env, jclass cl, jint filter, jint param, jint value)
{
//    fprintf(stderr, "alFilteri for filter: %d, param: %d, value: %d", filter, param, value);
//    alFilteri((ALuint)filter, (ALenum)param, (ALint)value);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alFilterf
  (JNIEnv *env, jclass cl, jint filter, jint param, jfloat value)
{
//    fprintf(stderr, "alFilterf for filter: %d, param: %d, value: %f", filter, param, value);
//    alFilterf((ALuint)filter, (ALenum)param, (ALfloat)value);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alSource3i
  (JNIEnv *env, jclass cl, jint source, jint param, jint value1, jint value2, jint value3)
{
//    fprintf(stderr, "alSource3i for source: %d, param: %d, value1: %d, value2: %d, value3: %d", source, param, value1, value2, value3);
    alSource3i((ALuint)source, (ALenum)param, (ALint)value1, (ALint)value2, (ALint)value3);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alDeleteFilters
  (JNIEnv* env, jclass cl, jint numFilters, jobject buffer)
{
//    fprintf(stderr, "alDeleteFilters for numFilters: %d", numFilters);
    ALuint* pBuffers = (ALuint*) (*env)->GetDirectBufferAddress(env, buffer);
//    alDeleteFilters((ALsizei)numFilters, pBuffers);
}

JNIEXPORT void JNICALL Java_com_jme3_audio_android_AndroidOpenALSoftAudioRenderer_alEffectf
  (JNIEnv *env, jclass cl, jint effect, jint param, jfloat value)
{
//    fprintf(stderr, "alEffectf for effect: %d, param: %d, value: %d", effect, param, value);
//    alEffectf((ALuint)effect, (ALenum)param, (ALfloat)value);
}

