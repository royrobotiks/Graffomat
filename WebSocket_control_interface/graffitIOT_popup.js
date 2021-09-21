var w=0,h=0;
function init(){
  // get screen size
  w =  window.screen.width;
  h =  window.screen.height;

}
function openControl(){
  var left = 5;
  var top = 10;
  var controlPopup = window.open ('graffitIOT.html', '', 'toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=no, resizable=no, copyhistory=no, width='+(w/5)+', height='+(h/2)+', top='+top+', left='+left);
  controlPopup.focus();
  document.getElementById('links').innerHTML=  '';
}

function openSerial(){
  var left = 5;
  var top = 10;
  var serialPopup  = window.open ('WStoSerial_client.html', '', 'toolbar=no, location=no, directories=no, status=no, menubar=no, scrollbars=no, resizable=no, copyhistory=no, width='+(w/5)+', height='+(h/4)+', top='+(h*.6)+', left='+left);
  serialPopup.focus();
}
//----------------------------------------- start init() when document is loaded
window.addEventListener("load", init, false);
