"use strict";

class serialConnection {
    constructor() {
        this.encoder = new TextEncoder();
        this.decoder = new TextDecoder();
    }
    async init() {
        if ('serial' in navigator) {
            try {
                const port = await navigator.serial.requestPort();
                await port.open({ baudRate: 38400 });
                this.reader = port.readable.getReader();
                this.writer = port.writable.getWriter();
                let signals = await port.getSignals();
                startLoop();
            }
            catch (err) {
                document.getElementById('debug').innerHTML='There was an error opening the serial port:'+ err;
            }
        }
        else {
            document.getElementById('debug').innerHTML='Web serial doesn\'t seem to be enabled in your browser. Try enabling it by visiting:';
            document.getElementById('debug').innerHTML+='chrome://flags/#enable-experimental-web-platform-features';
        }
    }
    async write(data) {
        const dataArrayBuffer = this.encoder.encode(data);
        return await this.writer.write(dataArrayBuffer);
    }

}




////////////////////////////////////////////////////////////////////////////////

const serialPort   = new serialConnection();
const messageForm  = document.getElementById('message-form');
const messageInput = document.getElementById('message-input');
const submitButton = document.getElementById('submit-button');

var serialAvailable=false;

function selectSerial(){ // when button 'SELECT SERIAL' is pushed
  serialPort.init();
  serialAvailable=true;
}

  //----------------------------------------- start Loop
function startLoop(){
  document.getElementById('links').innerHTML=  '<a href="#" onClick="openControl()">&nbsp;OPEN GRAFFOMAT IOT CONTROL PANEL&nbsp;</a>';
  //openControl(); // open websocket control popup
  var intervalId = window.setInterval(function(){
    serialSendLoop();
  }, 50);
}

  //----------------------------------------- Loop (50Hz)
var counter=0;
function serialSendLoop(){
  counter++;
  var rotor='-\\|/ ';

  var divisor = clients;
  var xSpeed,ySpeed,servo;

  if(divisor>0){
    xSpeed  = (inX/divisor)*50+60; //   maxleft=10, nospeed=60, maxright=110
    ySpeed  = (inY/divisor)*50+60; //   maxup=10,   nospeed=60, maxdown=110
    servo   = (Math.round(inPaint/divisor)*99)+1; // nopaint=1,  paint=100
  }else{
    xSpeed  = 60;
    ySpeed  = 60;
    servo   = 1;
  }

  var chksm = ((0 + 4 + xSpeed + ySpeed + servo) % 99) + 1;
  var sendMessage='0/4/'+xSpeed+'/'+ySpeed+'/'+servo+'/'+chksm;

  // weird workaround for command '4' here (direct speed control of the plotter):
  // since values >=128 are resolved as 2 bytes (WTF!?), all values which are sent
  // stay below 128.
  // thus checksum goes from 1...100!

  var asciiMessage=String.fromCharCode(0)+String.fromCharCode(4)+String.fromCharCode(xSpeed)+String.fromCharCode(ySpeed)+String.fromCharCode(servo)+String.fromCharCode(chksm);

  serialPort.write(asciiMessage);


  document.getElementById('message').innerHTML= 'WEBSOCKET IN:&nbsp;&nbsp;&nbsp;&nbsp; c:'+clients+' x:'+inX+' y:'+inY+' p:'+inPaint+'<br>';
  document.getElementById('message').innerHTML+='SERIAL BYTES OUT: '+sendMessage+'<br><br>';

  var websocketStatus='WEBSOCKET SERVER CONNECTED '+rotor.substring(Math.round(counter/2)%4,Math.round(counter/2)%4+1);
  if (wsConnection==false){websocketStatus='<b style="color:#aa0000">WEBSOCKET CONNECTION FAIL!</b>';}
  document.getElementById('message').innerHTML+=websocketStatus;
}
