#include <string.h>
#include <stdio.h>
#include <jni.h>
#include <inc/faac.h>
//#include <inc/cmnMemory.h>
#include <android/log.h>

#define DEBUG 0

#if DEBUG
#define LOG(msg, args...) __android_log_print(ANDROID_LOG_ERROR, "aac-enc", msg, ## args)
#else
// #define LOG(msg, args...)
#endif

#define LOG(msg, args...) __android_log_print(ANDROID_LOG_ERROR, "libfaac", msg, ## args)


/* utility functions */

void throwException(JNIEnv* env, const char *name, const char *msg)
{
  jclass cls = (*env)->FindClass(env, name);
  /* if cls is NULL, an exception has already been thrown */
  if (cls != NULL) {
    (*env)->ThrowNew(env, cls, msg);
  }
  /* free the local ref */
  (*env)->DeleteLocalRef(env, cls);
}

/* java native functions */


static faacEncHandle aacEncHdl = NULL;
static unsigned long inputSamples;
static unsigned long maxOutputBytes;



void
Java_com_todoroo_aacenc_AACEncoder_init( JNIEnv* env,
                                         jobject thiz,
                                         int bitrate,
                                         int channels,
                                         int sampleRate,
                                         int bitsPerSample,
                                         jstring outputFile)
{

  if (bitsPerSample != 16) {
    throwException(env, "java/lang/IllegalArgumentException", 
                   "Unsupported sample depth. Only 16 bits per sample is supported");
    return;
  }

 // unsigned long sampleRate = 16000;  // per channel
 // unsigned int channels = 2;


 LOG( " Goooo........aac init encoding......sr: %d  ch: %d" , sampleRate, channels );

    if(aacEncHdl != NULL){
       LOG( " Aac Encoder has been initilezed........" );
        return;
    }


  aacEncHdl = faacEncOpen((unsigned long)sampleRate,(unsigned int)channels,&inputSamples,&maxOutputBytes);

  LOG( "inputSample: %d , maxOutputBytes: %d" ,inputSamples , maxOutputBytes );

  faacEncConfigurationPtr faacConf = faacEncGetCurrentConfiguration(aacEncHdl);

  faacConf->mpegVersion = MPEG4;
  faacConf->inputFormat = FAAC_INPUT_16BIT ;// FAAC_INPUT_FLOAT;
  faacConf->aacObjectType = LOW;
  faacConf->useLfe = 0;
  faacConf->useTns = 1;
  faacConf->jointmode = JOINT_IS;
  faacConf->allowMidside = 0;
  faacConf->bitRate = 24000;  // per channel
  faacConf->outputFormat = 1;
  faacConf->bandWidth = 8000;
  faacConf->pnslevel = 4;

  int ret = faacEncSetConfiguration(aacEncHdl,faacConf);

  if(ret == 1){
      LOG(   "faacEncSetConfiguration ret = %d ......configure successfully......" , ret);

  }else{
       aacEncHdl = NULL;
      LOG(   "faacEncSetConfiguration ret = %d ......configure Failed......" , ret);

  }


}

void
Java_com_todoroo_aacenc_AACEncoder_encode( JNIEnv* env,
                                           jobject thiz,
                                           jbyteArray inputArray)
{

  LOG("writing to handle: %x", 0);

}

void
Java_com_todoroo_aacenc_AACEncoder_uninit( JNIEnv* env,
                                           jobject thiz)
{
    if(aacEncHdl != NULL){
        faacEncClose(aacEncHdl);
        aacEncHdl = NULL;
    }

      LOG( "aacEncHdl Uninited......"  );

}


void JNICALL Java_com_todoroo_aacenc_AACEncoder_getJNIByteArrayArg(JNIEnv    *jenv, jobject jobj)
{


    jclass clazz = (*jenv)->FindClass(jenv, "com/todoroo/aacenc/AACEncoder"); // class path
    jmethodID mid = (*jenv)->GetMethodID(jenv, clazz, "dataArrived", "([B)V");// function name

    jbyteArray retArray;
#if 0
    char data[] = {'a','b',3,4,5};
    int data_size = 5;
#else
    char data[512]  ;
    int data_size = 512;
    memset(data, 'T', data_size);
#endif

    if(!retArray)
        retArray = (*jenv)->NewByteArray(jenv, data_size);

    if((*jenv)->GetArrayLength(jenv, retArray) != data_size)
    {
        (*jenv)->DeleteLocalRef(jenv, retArray);
        retArray = (*jenv)->NewByteArray(jenv, data_size);
    }

    void *temp = (*jenv)->GetPrimitiveArrayCritical(jenv, (jarray)retArray, 0);
    memcpy(temp, data, data_size);
    (*jenv)->ReleasePrimitiveArrayCritical(jenv, retArray, temp, 0);

    (*jenv)->CallVoidMethod(jenv, jobj, mid, retArray);
}

JNIEXPORT jbyteArray
JNICALL Java_com_todoroo_aacenc_AACEncoder_pcmToAacEncoding( JNIEnv* env,
                                           jobject thiz,
                                           jbyteArray pcmBa)
{


    if(aacEncHdl == NULL){
        LOG(   "faac not initialed...."  );
        jbyteArray rtnBa = (*env)->NewByteArray(env,0);
        (*env)->SetByteArrayRegion(env,  rtnBa,0, 0, 0);
         return rtnBa;
    }


    jbyte* buffer = (*env)->GetByteArrayElements(env, pcmBa,   0);
    int pcmLen = (*env)->GetArrayLength(env, pcmBa);

    unsigned char *encodedBuf = (unsigned char *) malloc(   maxOutputBytes  );

    // we confiugured FAAC_INPUT_16BIT as input type, so 2 bytes is a sample

    // LOG( "bytes len in: %d" , pcmLen );


      unsigned long okInputSmapleCnt = pcmLen / 2;


      if(okInputSmapleCnt > inputSamples){
          LOG( "input smaple: %d ,  allow max len: %d" , okInputSmapleCnt ,  inputSamples);
         okInputSmapleCnt = inputSamples;
      }


      int nbByteUsed =   faacEncEncode(aacEncHdl, (int32_t *)buffer  ,okInputSmapleCnt , encodedBuf, maxOutputBytes);


     // LOG( "Encoded bytes: %d" , nbByteUsed );


    jbyteArray rtnBa = (*env)->NewByteArray(env,nbByteUsed);
     (*env)->SetByteArrayRegion(env,  rtnBa,0, nbByteUsed, encodedBuf);

    free(encodedBuf) ;


    return rtnBa;

}





JNIEXPORT jint JNICALL 
JNI_OnLoad (JavaVM * vm, void * reserved) 
{ 
  return JNI_VERSION_1_6; 
} 
