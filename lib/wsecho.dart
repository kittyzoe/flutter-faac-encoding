import 'dart:async';
import 'dart:convert';
import 'dart:ffi';
import 'dart:typed_data';
import 'package:convert/convert.dart';

//import 'package:intl/intl.dart';

import 'package:flutter/foundation.dart';
import 'package:web_socket_channel/io.dart';
import 'package:flutter/material.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

import 'package:ffi/ffi.dart';
//import 'package:fluttertoast/fluttertoast.dart';

import 'package:native_src/native_src.dart';
import 'test_rec_snd.dart';

//  void main() => runApp(MyApp());

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    final title = 'WebSocket Demo';
    return MaterialApp(
      title: title,
      home: MyHomePage(
        title: title,
        // channel: IOWebSocketChannel.connect('ws://echo.websocket.org'),
      //   channel: IOWebSocketChannel.connect('ws://192.168.1.105:1234'),

        channel: IOWebSocketChannel.connect('ws://192.168.1.108:1234'),

       // channel: IOWebSocketChannel.connect('wss://wss.langrensha.game:443/voicews/?token=6da1tjreb4c1butorl0c0iv8e7&utma=5a5dba1f8f8fc8ab2d59101387a35085&version=1.7.3&device=iPhone 12 Pro Max&os=iOS&os_version=14.2&device_type=2'),

      ),
    );
  }
}

class MyHomePage extends StatefulWidget {
  final String title;
   final WebSocketChannel channel;

  MyHomePage({Key key, @required this.title , @required this.channel  })
      : super(key: key);

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  TextEditingController _controller = TextEditingController();

  NativeSrc myNavSrc = NativeSrc();


  @override
  void initState() {
    super.initState();

    new Future.delayed(Duration(milliseconds:   100)).then((_) {

    //  IOWebSocketChannel. connect("ws://echo.websocket.org").stream.listen((event) {
        initData();
    });

  }




  void initData()
  {
    myNavSrc.native_initFaac(24000,2 , 32000, 16);
  }



  void testPassData(){

    Uint8List aa = Uint8List(37);
    aa.fillRange(0, aa.length, 0xfa);

    var outBuff = allocate<Uint8>(count: aa.length);
    outBuff.asTypedList(aa.length).setAll(0, aa);



    Pointer<Int32> rtnBa =   myNavSrc.pcmToAacByFaac(aa.length , outBuff );

    print('rtnba: $rtnBa');


    print('rtnba0: ${rtnBa.elementAt(0)}');
    print('rtnba1: ${rtnBa.elementAt(1)}');

    int len = rtnBa.asTypedList(1)[0] ;


    print('rtnBa  : len = $len');

    free(outBuff);

    ///////////////////////////////////////////////////////////////

    print('222222222.......................=======================================');


    Uint8List datLs = Uint8List(38);
    datLs.fillRange(0, datLs.length, 0x38);

    final pcmBuffBa = allocate<Uint8>(count: datLs.length);
    pcmBuffBa.asTypedList(datLs.length).setAll(0, datLs);

    final pcmBaLen = allocate<Int32>(count: 1);
    pcmBaLen.asTypedList(1).setAll(0, [datLs.length]);

    int testSize =  48;

    final aacBuffBa = allocate<Uint8>(count: testSize);
    final aacBaLen = allocate<Int32>(count: 1);
    aacBaLen.asTypedList(1).setAll(0, [testSize]);


    int rtnVal = myNavSrc.pcmToAacFunc(pcmBaLen,pcmBuffBa, aacBaLen,  aacBuffBa);

    print('......  .........................rtnVal: $rtnVal');

    int aacLen =  aacBaLen[0];

    Uint8List aacDatLs = aacBuffBa.asTypedList(aacLen);

    print('aacLen: $aacLen , dat: ${hex.encode(aacDatLs)}');


    free(pcmBuffBa);
    free(pcmBaLen);
    free(aacBuffBa);
    free(aacBaLen);

  }



  void testF(){
    var testBB = BytesBuilder();

    final dstBuff = allocate<Uint8>(count: 55);

     testBB.add(dstBuff.asTypedList(55));


    // testBB.takeBytes();

    // testBB.to


  }



  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text( '${widget.title} '),
      ),
      body: Padding(
        padding: const EdgeInsets.all(20.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: <Widget>[
            Form(
              child: TextFormField(
                controller: _controller,
                decoration: InputDecoration(labelText: 'Send a message..'),
              ),
            ),
            StreamBuilder(
              stream:   widget.channel.stream,
              builder: (context, snapshot) {
                return Padding(
                  padding: const EdgeInsets.symmetric(vertical: 24.0),
                  child: Text(snapshot.hasData ? '${snapshot.data}' : ''),
                );
              },
            )
          ],
        ),
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: _sendMessage,
        tooltip: 'Send message',
        child: Icon(Icons.send),
      ), // This trailing comma makes auto-formatting nicer for build methods.
    );
  }

  void _sendMessage() {

    Navigator.push(
      context,
      MaterialPageRoute(builder: (context) => SimpleRecorder()),
    );


    return;
    //
    // Fluttertoast.showToast(
    //   msg: " send msg... ",
    //   toastLength: Toast.LENGTH_SHORT,
    //   gravity: ToastGravity.CENTER,
    //   backgroundColor: Colors.greenAccent,
    //   textColor: Colors.deepOrange,
    //   fontSize: 24.0,
    // );

    if (_controller.text.isNotEmpty) {
      widget.channel.sink.add(_controller.text);
    }
  }

  @override
  void dispose() {
    widget.channel.sink.close();
    super.dispose();
  }
}