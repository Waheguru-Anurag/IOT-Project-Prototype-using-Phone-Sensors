import 'dart:async';

import 'package:flutter/material.dart';
import 'package:get_ip/get_ip.dart';
import 'package:http/http.dart' as http;
import 'package:sensors/sensors.dart';

void main() {
  runApp(MaterialApp(home: MyHomePage()));
}

class MyHomePage extends StatefulWidget {
  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  String ipAddress = "http://192.168.100.24";
  List<bool> isSelected = [false];
  double posx, posy, posz, azimuth, roll, pitch;
  List<double> _accelerometerValues;
  List<double> _gyroscopeValues;
  List<StreamSubscription<dynamic>> _streamSubscriptions =
      <StreamSubscription<dynamic>>[];

  @override
  void initState() {
    super.initState();
    _streamSubscriptions
        .add(accelerometerEvents.listen((AccelerometerEvent event) {
      setState(() {
        _accelerometerValues = <double>[event.x, event.y, event.z];
      });
    }));
    _streamSubscriptions.add(gyroscopeEvents.listen((GyroscopeEvent event) {
      setState(() {
        _gyroscopeValues = <double>[event.x, event.y, event.z];
      });
    }));
  }

  Future<http.Response> ledOff() {
    String url = ipAddress + "/off";
    return http.get(url);
  }

  Future<http.Response> ledOn() {
    String url = ipAddress + "/on";
    return http.get(url);
  }

  Future<String> getipAddress() async {
    return await GetIp.ipAddress;
  }

  Future<http.Response> postData(double posx, double posy, double posz,
      double azimuth, double pitch, double roll) async {
    String query =
        "?posx=$posx&posy=$posy&posz=$posz,azimuth=$azimuth&pitch=$pitch&roll=$roll";
    String url = ipAddress + "/post" + query;
    return await http.get(url);
  }

  @override
  Widget build(BuildContext context) {
    if (isSelected[0]) {
      postData(
          _accelerometerValues[0],
          _accelerometerValues[1],
          _accelerometerValues[2],
          _gyroscopeValues[0],
          _gyroscopeValues[1],
          _gyroscopeValues[2]);
    }
    return Scaffold(
      appBar: AppBar(
        title: Text("IOT App"),
      ),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: <Widget>[
            Container(
              decoration: BoxDecoration(
                color: Colors.white,
              ),
              child: ToggleButtons(
                  children: [
                    Icon(Icons.flash_on, color: Colors.deepOrangeAccent),
                  ],
                  selectedColor: Colors.blue,
                  isSelected: isSelected,
                  onPressed: (int index) {
                    setState(() {
                      isSelected[index] = !isSelected[index];
                    });
                    if (isSelected[index]) {
                      ledOn();
                    } else {
                      ledOff();
                    }
                  }),
            ),
            Padding(
              padding: EdgeInsets.all(4.0),
              child: Text(
                'Enable',
                style: TextStyle(
                  letterSpacing: 2,
                  fontWeight: FontWeight.bold,
                ),
              ),
            )
          ],
        ),
      ),
    );
  }
}
