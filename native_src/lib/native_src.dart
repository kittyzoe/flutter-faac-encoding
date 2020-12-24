
import 'dart:async';

import 'dart:typed_data';

import 'package:flutter/services.dart';
import 'dart:ffi'  ;
import 'dart:io';


final DynamicLibrary myNativeFunc = DynamicLibrary.open("libthorffis.so");

typedef _pcm_aac_faac = Pointer<Int32> Function(Int32 len  , Pointer<Uint8>  buff  );
typedef _PcmAacFaac = Pointer<Int32> Function(  int len,     Pointer<Uint8>  buff  ) ;

class NativeSrc {
  static const MethodChannel _channel =
      const MethodChannel('native_src');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }

  final int Function(int bitrate,  int channels, int sampleRate, int bitPerSample ) native_initFaac = myNativeFunc.lookup<NativeFunction<Int32 Function(Int32,Int32,Int32,Int32)>>("initFaac").asFunction();
  final void Function() native_uninitFaac = myNativeFunc.lookup<NativeFunction<Void Function( )>>("uninitFaac").asFunction();


  final int Function(int x, int y) nativeMultiple = myNativeFunc.lookup<NativeFunction<Int32 Function(Int32,Int32)>>("testFunc").asFunction();
  final   _PcmAacFaac pcmToAacByFaac = myNativeFunc.lookup<NativeFunction<_pcm_aac_faac>>("pcmToAacByFaac").asFunction();

  final int Function(Pointer<Int32> ,  Pointer<Uint8>, Pointer<Int32> ,  Pointer<Uint8> ) pcmToAacFunc  =
  myNativeFunc.lookup<NativeFunction<Int32 Function(Pointer<Int32> ,Pointer<Uint8>, Pointer<Int32>, Pointer<Uint8> )>>('pcmToAacFunc').asFunction();



}
