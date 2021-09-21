console.log('graffitIOT server running on port 3000');

//// Start Websocket Server
var WebSocketServer = require('ws').Server,
    wss = new WebSocketServer({port: 3000}),
    CLIENTS=[];
wss.on('connection', newConnection);

//// Program loop
var x=0;        // 0...2
var y=0;        // 0...2
var paint=0;    // 0,1
var messages=0; // amount of messages received in this loop
var graffomatConnectCounter=100;

var loop = setInterval(function(){
  var graffomatConnected=0;
  if(graffomatConnectCounter>0){graffomatConnected=1;} // check if plotter is connected
  console.log('c:'+messages.toString().padStart(2, '0')+'  x:'+x+' y:'+y+' p:'+paint+' g:'+graffomatConnected); // send out message to all clients
  sendAll(messages+','+x.toString().padStart(2, ' ')+','+y.toString().padStart(2, ' ')+','+paint.toString().padStart(2, ' ')+', '+graffomatConnected);
  // reset all values (will be set by received messages)
  x=0;
  y=0;
  paint=0;
  messages=0;
  if (graffomatConnectCounter>0){graffomatConnectCounter-=1;}
}, 50);

//// Send message to all clients
function sendAll (message) {
    for (var i=0; i<CLIENTS.length; i++) {
        CLIENTS[i].send(message);
    }
}


//// Receive message from client
function newConnection(wss) {
    console.log('=========================');
    console.log('new connection: '+CLIENTS.length);
    CLIENTS.push(wss);
    wss.on('message', function(message) {
      var received = message.split(",");
      if (received[0]<10){ // if message is from web interface
        messages+=1;       // count this message
        x     += parseInt(received[1]);
        y     += parseInt(received[2]);
        paint += parseInt(received[3]);
      }else if(received[0]==10){ // if message is from graffomat
        graffomatConnectCounter=100; // update connect counter
      }
    });
}
