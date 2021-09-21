//----------------------------------------- GRAFFIT_IOT WEBSOCKET CLIENT
var wsUri = "wss://control.uber.space/ws"; // traffic on /ws is piped to port:3000 (configured via uber space backend tool)
var wsConnection=false;                    // flag keeps track of websocket connection

//----------------------------------------- initialize websockets
function init()
{
  websocket = new WebSocket(wsUri);
  websocket.onopen    = function(evt) {wsConnection=true;};
  websocket.onmessage = function(evt) {newMessage(evt.data);};
  websocket.onerror   = function(evt) {wsConnection=false;};
  websocket.onclose   = function(evt) {wsConnection=false;};
}

//----------------------------------------- send message via websocket
function doSend(message){websocket.send(message);}

//----------------------------------------- process incoming messages (via websocket)
var inX=0,
    inY=0,
    inPaint=0,
    clients=0;
    graffomatConnected=0;

function newMessage(message){
  var received = message.split(",");
  clients            = parseInt(received[0]);
  inX                = parseInt(received[1]);
  inY                = parseInt(received[2]);
  inPaint            = parseInt(received[3]);
  graffomatConnected = parseInt(received[4]);
}

//----------------------------------------- start init() when document is loaded
window.addEventListener("load", init, false);
