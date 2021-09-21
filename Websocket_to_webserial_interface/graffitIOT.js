//----------------------------------------- GRAFFIT_IOT USER INTERFACE
//----------------------------------------- start Loop (50Hz)
var intervalId = window.setInterval(function(){
  countLoop();
}, 50);

//----------------------------------------- Loop
var loopCount=0;
var focus=false;
var refSize=1; // reference size
var w,h;
function countLoop(){
  // get window size
  w =  document.getElementById('w100p').getBoundingClientRect().width ;
  h =  Math.max(document.getElementById('h100p').getBoundingClientRect().height,window.innerHeight) ;
  refSize=Math.min(w,h);
  loopCount++;                 // increment loop count
  focus=document.hasFocus();   // check if window has focus
  updateDebug();
  calculateDirections();
  updateGFX();
}

//----------------------------------------- Keybd listener

var key='';
var paint=false;
var up=false;
var down=false;
var left=false;
var right=false;

document.addEventListener('keydown', function(event) { // check if WASD, arrow keys or SHIFT is pushed
    if(event.keyCode == 37|| event.keyCode == 65) {
        left=true;
    }
    else if(event.keyCode == 39 || event.keyCode == 68) {
        right=true;
    }
    else if(event.keyCode == 38 || event.keyCode == 87) {
        up=true;
    }
    else if(event.keyCode == 40 || event.keyCode == 83) {
        down=true;
    }
    else if(event.keyCode == 32) {
        paint=true;
    }else{
      key=event.keyCode;
    }
});

document.addEventListener('keyup', (event) => { // check if WASD, arrow keys or SHIFT is released
  if(event.keyCode == 37|| event.keyCode == 65) {
      left=false;
  }
  else if(event.keyCode == 39 || event.keyCode == 68) {
      right=false;
  }
  else if(event.keyCode == 38 || event.keyCode == 87) {
      up=false;
  }
  else if(event.keyCode == 40 || event.keyCode == 83) {
      down=false;
  }else if(event.keyCode==32){paint=false;}
});

//----------------------------------------- update Debug
function updateDebug(){
  //document.getElementById('debug').innerHTML= '//////// debug ////////<br>';
  //document.getElementById('debug').innerHTML+='focus:&nbsp;'+focus+'<br>';
  //document.getElementById('debug').innerHTML+='size:&nbsp;&nbsp;'+Math.round(w)+'px * '+Math.round(h)+'px<br>';
  //document.getElementById('debug').innerHTML+='count:&nbsp;'+loopCount+'<br>';
  //document.getElementById('debug').innerHTML+='--- keyboard input: ---<br>';
  //document.getElementById('debug').innerHTML+='keycode:&nbsp;'+key+'<br>';
  //document.getElementById('debug').innerHTML+='up:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+up+'<br>';
  //document.getElementById('debug').innerHTML+='down:&nbsp;&nbsp;&nbsp;&nbsp;'+down+'<br>';
  //document.getElementById('debug').innerHTML+='left:&nbsp;&nbsp;&nbsp;&nbsp;'+left+'<br>';
  //document.getElementById('debug').innerHTML+='right:&nbsp;&nbsp;&nbsp;'+right+'<br>';
  //document.getElementById('debug').innerHTML+='space:&nbsp;&nbsp;&nbsp;'+paint+'<br>';
  document.getElementById('debug').innerHTML='&gt; WEBSOCKET SERVER STATUS:<br>';
  if (wsConnection){
    document.getElementById('debug').innerHTML+='<b style="color:#00aa00">&gt; WEBSOCKET CONNECTED =]</b>';
  }else{
    document.getElementById('debug').innerHTML+='<b style="color:#aa0000">&gt; WEBSOCKET NOT CONNECTED!</b><br>&gt; CLOSE WINDOW AND RELOAD';
  }
}

//----------------------------------------- calculate directions based on keybd input
var showArrow=false, lr=1, ud=1;
function calculateDirections(){

  // calculate arrow directions based on up,down,left,right:
  showArrow=false;
  lr=0;
  ud=0;

  if (left) {showArrow=true;lr-=1;}
  if (right){showArrow=true;lr+=1;}

  if (up)   {showArrow=true;ud-=1;}
  if (down) {showArrow=true;ud+=1;}

  if (wsConnection && focus){
    var p=0;
    if (paint){p=1;}
    doSend((loopCount%10)+','+lr+','+ud+','+p); // send c,x,y,paint via websocket
  }
}

//----------------------------------------- update SVGs
function updateGFX(){
  var boxSize=w-22;
  var boxCenter=boxSize/2;
  var boxLeft=10;
  var xArrow = boxCenter + (lr)*boxSize*.2;
  var yArrow = boxCenter + (ud)*boxSize*.2;


  var showInArrow = false;
  var arrowColor='#000';
  var inArrowColor='#000';
  var bgndColor='#aaa';
  var rectColor='#ccc';
  var innerRectColor='#ddd';
  var textColor='#555';

  var inXArrow = boxCenter;
  var inYArrow = boxCenter;
  if (clients>0){ // calculate democratic vote arrow direction if a client is connected
    inXArrow += (inX/clients)*boxSize*.25;
    inYArrow += (inY/clients)*boxSize*.25;
    if (inX!=0 || inY!=0){showInArrow=true;} // show arrow if it points to a direction
  }

  var inArrow='';
    if (showInArrow){inArrow='<line x1="'+(boxCenter)+'" y1="'+(boxCenter)+'" \
    x2="'+inXArrow+'" y2="'+inYArrow+'" stroke="#000" stroke-opacity="0.3" \
    stroke-width="'+(boxSize/18)+'" marker-end="url(#inArrowhead)"/>';
  }



  //...................................... generate arrow box
  var infoText='<tspan x="'+(boxSize*.07)+'" y="'+(boxSize*.11)+'">CLICK WINDOW TO FOCUS / CONTROL!</tspan>';

  if (focus){
    bgndColor='#ccc';
    rectColor='#0c0';
    innerRectColor='#0e0';
    textColor='#050';
    infoText='<tspan x="'+(boxSize*.07)+'" y="'+(boxSize*.11)+'">IOT REALTIME SPRAY CAN CONTROL:</tspan>\
    <tspan x="'+(boxSize*.07)+'" y="'+(boxSize*.16)+         '">===============================</tspan>\
    <tspan x="'+(boxSize*.07)+'" y="'+(boxSize*.21)+          '">MOVE:  WASD [/ARROW] KEYS</tspan>';
  }

  if (paint){
    rectColor='#c00';
    innerRectColor='#e00';
    textColor='#500';
    arrowColor='#fff';
  }
  var arrow='';
    if (ud!=0 || lr!=0){arrow='<line x1="'+(boxSize*.5)+'" y1="'+(boxSize*.5)+'" \
    x2="'+xArrow+'" y2="'+yArrow+'" stroke="'+arrowColor+'" stroke-width=\
    "'+(boxSize/20)+'" marker-end="url(#arrowhead)"/>';
  }
  document.body.style.backgroundColor = bgndColor;

  document.getElementById('arrowBox').innerHTML ='\
  <svg  width="'+boxSize+'" height="'+boxSize+'">\
  <defs>\
    <marker id="arrowhead" markerWidth="5" markerHeight="5" fill="'+arrowColor+'"\
    refX=".5" refY="2.5" orient="auto">\
      <polygon points="0 0, 4 2.5, 0 5" />\
    </marker>\
    <marker id="inArrowhead" markerWidth="5" markerHeight="5" fill="#000" fill-opacity="0.3"\
    refX="0" refY="2.5" orient="auto">\
      <polygon points="0 0, 4 2.5, 0 5" />\
    </marker>\
  </defs>\
  <rect x="0" y="0" width="'+boxSize+'" height="'+boxSize+'" fill="'+rectColor+'"/>\
  <rect x="'+(boxSize*.05)+'" y="'+(boxSize*.05)+'" width="'+(boxSize*.9)+'" height="'+(boxSize*.9)+'" fill="'+innerRectColor+'" />\
  <circle cx="'+boxCenter+'" cy="'+boxCenter+'" r="'+(boxSize*.2)+'" stroke="'+rectColor+'" stroke-width="'+(boxSize/100)+'" fill="'+innerRectColor+'" />\
  <circle cx="'+boxCenter+'" cy="'+boxCenter+'" r="'+(boxSize*.1)+'" stroke="'+rectColor+'" stroke-width="'+(boxSize/100)+'" fill="'+innerRectColor+'" />\
  <text  fill="'+textColor+'" font-size="'+(boxSize/22)+'px">'+infoText+'</text>\
  '+inArrow+arrow+'\
  </svg>\
  ';

  var visibility='hidden'; // only show arrow if key is pressed
  if (showArrow){visibility='visible';}


  //...................................... generate paint bar
  var barColor='#eee';
  var barBgnd ='#bbb';
  var barTextColor='#333';

  if (focus){
    barColor='#0a0';
    barBgnd ='#aaa';
    barTextColor='#333';
  }
  var barWidth=0;
  if (clients>0){barWidth=inPaint/clients};
  if (barWidth>.5){barColor='#e00';}
  //var barText='<tspan x="'+(boxSize*.025)+'" y="'+(boxSize*.06)+'">PAINT VOTE: '+inPaint+'/'+clients+'</tspan>';

  var barText='<tspan x="'+(boxSize*.03)+'" y="'+(boxSize*.067)+'">PAINT: SPACE BAR</tspan>\
               <tspan x="'+(boxSize*.03)+'" y="'+(boxSize*.117)+'">================</tspan>\
               <tspan x="'+(boxSize*.03)+'" y="'+(boxSize*.167)+'">VOTE: '+inPaint+'/'+clients+' = '+Math.round(barWidth*100)+'%</tspan>';

  document.getElementById('paintBar').innerHTML ='\
  <svg  width="'+boxSize+'" height="'+(boxSize/5)+'">\
  <rect x="0" y="0" width="'+boxSize+'" height="'+(boxSize/5)+'" fill="'+barBgnd+'"/>\
  <rect x="0" y="0" width="'+(boxSize*barWidth)+'" height="'+(boxSize/5)+'" fill="'+barColor+'" />\
  <text  fill="'+barTextColor+'" font-size="'+(boxSize/22)+'px">'+barText+'</text>\
  </svg>\
  ';
}
