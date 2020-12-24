
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <faac.h>

#ifdef __ANDROID__
#include <android/log.h>
#endif


void TLOG(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
#ifdef __ANDROID__
   // __android_log_vprint(ANDROID_LOG_VERBOSE, "tootzoe", fmt, args);
    __android_log_vprint(ANDROID_LOG_ERROR, "tootzoe", fmt, args);
#else
    vprintf(fmt, args);
#endif
    va_end(args);
}

static faacEncHandle aacEncHdl = NULL;
static unsigned long inputSamples;
static unsigned long maxOutputBytes;

extern "C" {
__attribute__((visibility("default"))) __attribute__((used))
int32_t testFunc(int32_t x, int32_t y){


    unsigned long sampleRate = 16000;  // per channel
    unsigned int channels = 2;
    int bitsPerSample  = 16;

    if (bitsPerSample != 16) {
      TLOG( " Unsupported sample depth. Only 16 bits per sample is supported....." );
      return 0;
    }

    TLOG( " Goooo.. .aac init encoding......sr: %d  ch: %d" , sampleRate, channels );

       if(aacEncHdl != NULL){
          TLOG( " Aac Encoder has been initilezed........" );
           return 0;
       }


     aacEncHdl = faacEncOpen((unsigned long)sampleRate,(unsigned int)channels,&inputSamples,&maxOutputBytes);

     TLOG( "inputSample: %d , maxOutputBytes: %d" ,inputSamples , maxOutputBytes );

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
         TLOG(   "faacEncSetConfiguration ret = %d ......configure successfully......" , ret);

     }else{
          aacEncHdl = NULL;
         TLOG(   "faacEncSetConfiguration ret = %d ......configure Failed......" , ret);

     }




    TLOG("%s called.... x: %d y: %d.. 33.....", __func__, x, y);


    if(aacEncHdl != NULL){
        faacEncClose(aacEncHdl);
        aacEncHdl = NULL;
    }

      TLOG( "aacEncHdl Uninited......"  );


    return x*y + y ;
}

__attribute__((visibility("default"))) __attribute__((used))
int32_t initFaac(int32_t bitrate, int32_t channels, int32_t sampleRate, int32_t bitsPerSample){

    if (bitsPerSample != 16) {
      TLOG( " Unsupported sample depth. Only 16 bits per sample is supported....." );
      return 0;
    }

    TLOG( " Goooo.. .aac init encoding......sr: %d  ch: %d" , sampleRate, channels );

       if(aacEncHdl != NULL){
          TLOG( " Aac Encoder has been initilezed........" );
           return 0;
       }


     aacEncHdl = faacEncOpen((unsigned long)sampleRate,(unsigned int)channels,&inputSamples,&maxOutputBytes);

     TLOG( "inputSample: %d , maxOutputBytes: %d" ,inputSamples , maxOutputBytes );

     faacEncConfigurationPtr faacConf = faacEncGetCurrentConfiguration(aacEncHdl);

     faacConf->mpegVersion = MPEG4;
     faacConf->inputFormat = FAAC_INPUT_16BIT ;// FAAC_INPUT_FLOAT;
     faacConf->aacObjectType = LOW;
     faacConf->useLfe = 0;
     faacConf->useTns = 1;
     faacConf->jointmode = JOINT_IS;
     faacConf->allowMidside = 0;
     faacConf->bitRate = bitrate;  // per channel
     faacConf->outputFormat = 1;
     faacConf->bandWidth = 8000;
     faacConf->pnslevel = 4;

     int ret = faacEncSetConfiguration(aacEncHdl,faacConf);

     if(ret == 1){
         TLOG(   "faacEncSetConfiguration ret = %d ......configure successfully......" , ret);

     }else{
          aacEncHdl = NULL;
         TLOG(   "faacEncSetConfiguration ret = %d ......configure Failed......" , ret);

     }

     return ret;

}


__attribute__((visibility("default"))) __attribute__((used))
void uninitFaac( ){

    if(aacEncHdl != NULL){
        faacEncClose(aacEncHdl);
        aacEncHdl = NULL;
    }

    TLOG( "aacEncHdl Uninited......" );
}




__attribute__((visibility("default"))) __attribute__((used))
uint32_t * pcmToAacByFaac(int32_t len, uint8_t *buff){


    TLOG("%s called.... len: %d .................................", __func__, len);

    for(int i = 0 ; i < len ; i ++)
        TLOG("%d" , buff[i]);


    uint32_t *rtnBa = new uint32_t[2];
    rtnBa[0] = 66;
    rtnBa[1] = 0;
   // memset(rtnBa, 0xfb , 66);

    return rtnBa ;
}

///////////////////////

__attribute__((visibility("default"))) __attribute__((used))
int32_t pcmToAacFunc(int32_t *inLen,uint8_t *inBuff , int32_t *outLen, uint8_t *outBuff ){


    TLOG("%s called.... inlen: %d  inBuff0: %d , outLen: %d outBuff0:%d.....", __func__,
                            *inLen ,*inBuff, *outLen , *outBuff
    );


    for(int i = 0 ; i < *inLen ; i ++)
          TLOG("%d" , inBuff[i]);


   // memset(outBuff, 0xaa, *outLen);


   // *outLen = 2048;

     //outBuff   =(uint8_t*) realloc(outBuff , *outLen);

    for(int i = 0 ; i < *outLen ; i ++)
      outBuff[i] =  i;



  //  uint32_t *rtnBa = new uint32_t[2];
  //  rtnBa[0] = 66;
  //  rtnBa[1] = 0;
   // memset(rtnBa, 0xfb , 66);

    return 88 ;
}


}








