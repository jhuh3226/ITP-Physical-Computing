let serial; // variable to hold an instance of the serialport library
//let portName = '/dev/tty.usbmodem14601'; // fill in your serial port name here
let portName = '/dev/tty.usbmodem14601'; // fill in your serial port name here
let inData;
let inData2;
let inData3;
let inData4;
let inDataValue;
let inData2Value;
let inData3Value;
let inData4Value;

let passingData = {};
let xPos = 0;

/*------webRTC------*/
// Peer variables
let startPeer;
/*------------------*/

let optionsSerial = {
  baudrate: 9600
}; // change the data rate to whatever you wish

function setup() {
  createCanvas(400, 400);
  //background(0x08, 0x16, 0x40);

  serial = new p5.SerialPort(); // make a new instance of the serialport library
  serial.on('list', printList); // set a callback function for the serialport list event
  serial.on('connected', serverConnected); // callback for connecting to the server
  serial.on('open', portOpen); // callback for the port opening
  serial.on('data', serialEvent); // callback for when new data arrives
  serial.on('error', serialError); // callback for errors
  serial.on('close', portClose); // callback for the port closing

  serial.list(); // list the serial ports
  serial.open(portName); // open a serial port
  serial.open(portName, optionsSerial);

  /*------webRTC------*/
  const options = {
    architecture: 'MobileNetV1',
    imageScaleFactor: 0.3,
    outputStride: 16,
    flipHorizontal: true,
    minConfidence: 0.5,
    scoreThreshold: 0.5,
    nmsRadius: 20,
    detectionType: 'single',
    inputResolution: 513,
    multiplier: 0.75,
    quantBytes: 2,
  };

  // Start socket client automatically on load
  // By default it connects to http://localhost:80
  // WebRTCPeerClient.initSocketClient();

  // To connect to server remotely pass the ngrok address
  // See https://github.com/lisajamhoury/WebRTC-Simple-Peer-Examples#to-run-signal-server-online-with-ngrok
  WebRTCPeerClient.initSocketClient('https://d9fe4078bf9e.ngrok.io');

  // Start the peer client
  WebRTCPeerClient.initPeerClient();
  /*------------------*/
}

function draw() {
  background(255);
  textSize(50);
  text(inDataValue, width / 2, height / 2);
  text(inData2Value, width / 2, height / 2 + 50);
  text(inData3Value, width / 2, height / 2 + 100);
  text(inData4Value, width / 2, height / 2 + 150);
  
  inDataValue = int(inData);
  inData2Value = int(inData2);
  inData3Value = int(inData3);
  inData4Value = int(inData4);

  /*------webRTC------*/
  if (
    !WebRTCPeerClient.isPeerStarted()
  ) {
    console.log('returning!');
    return;
  }
  
  // // Get the incoming data from the peer connection
  // const newData = WebRTCPeerClient.getData();

  // // Check if there's anything in the data
  // if (newData === null) {
  //   return;
  // }
  // else{
  //      // If there is data
  //     passingData = newData.data;
  // }
  /*------------------*/

  //send data
  if (WebRTCPeerClient.isPeerStarted()) {
    passingData={inDataValue, inData2Value, inData3Value, inData4Value};
    WebRTCPeerClient.sendData(passingData);
  }
}

// get the list of ports:
function printList(portList) {
  // portList is an array of serial port names
  for (var i = 0; i < portList.length; i++) {
    // Display the list the console:
    console.log(i + portList[i]);
  }
}

function serverConnected() {
  console.log('connected to server.');
}

function portOpen() {
  console.log('the serial port opened.')
}

/*important part*/
function serialEvent() {
  // read a string from the serial port
  // until you get carriage return and newline:
  var inString = serial.readStringUntil('\r\n');

  //check to see that there's actually a string there:
  if (inString.length > 0) {
    
    var sensors = split(inString, ','); // split the string on the commas
     if (sensors.length > 3) {    
      inData = sensors[0];
      inData2 = sensors[1];
      inData3 = sensors[2];
      inData4 = sensors[3];     
     }
  }
}

function serialError(err) {
  console.log('Something went wrong with the serial port. ' + err);
}

function portClose() {
  console.log('The serial port closed.');
}