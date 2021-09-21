

int[] xP = new int[30000]; //x-position
int[] yP = new int[30000]; //y-position
int[] sP = new int[30000]; //servo
int[] cP = new int[30000]; //command

int     lineColor  = #000000;
int     xMax=220; // canvas size in cm
int     yMax=200;
int     xOff=20;  // origin of canvas on the screen
int     yOff=20;
float   oldX=0; // coordinates of printhead after drawing line
float   oldY=0;
int     scale=4;
int     lineIndex=0;
boolean pMousePressed=false;
boolean nMousePressed=false;
boolean pKeyPressed=false;
boolean invert=false;
boolean mouseOnCanvas=false;
String  fileName="---";
int     grid=4;
int     capSize=15;
PrintWriter output;
boolean showGrid=false;
boolean showPath=false;
PImage  bgndImage;
boolean showBgnd=false; 

void setup() {
  size(1500, 840);
  smooth(0);
  xP[0]=1;
  yP[0]=1;
  cP[0]=1;
  sP[0]=1;
}

void draw() {
  nMousePressed=mousePressed;
  background(64);
  if (showBgnd) {
    image(bgndImage, 0, 0);
  }
  drawCanvas();

  mouseOnCanvas=(mouseX>=xOff && mouseX<=xOff+xMax*scale && mouseY>=yOff && mouseY<=yOff+yMax*scale);
  if  (mouseOnCanvas) { //if mouse is on canvas:
    drawCursor();
    if (mousePressed&&!pMousePressed && (mouseButton == LEFT)) { // check mouse and add new line if pressed
      lineIndex++;
      xP[lineIndex]=mX;
      yP[lineIndex]=mY;
      cP[lineIndex]=1;
      sP[lineIndex]=1;
      if (lineIndex>1) {
        sP[lineIndex]=255;
      }
    }

    if (mousePressed&&!pMousePressed && (mouseButton == RIGHT)) { // check mouse and add new empty line if pressed
      lineIndex++;
      xP[lineIndex]=mX;
      yP[lineIndex]=mY;
      cP[lineIndex]=2;
      sP[lineIndex]=1;
    }
  }

  drawLines();

  // menu

  int menuY=20;

  if (button(920, menuY, 220, 40, "LOAD .PLT")) {
    loadFile();
  }
  if (button(920, menuY+=60, 220, 40, "SAVE .PLT")) {
    exportFile();
  }
  if (button(920, menuY+=60, 220, 40, "UNDO LAST LINE")) {
    undo();
  }
  if (button(920, menuY+=60, 220, 40, "CLEAR ALL LINES")) {
    lineIndex=0;
  }
  if (button(920, menuY+=60, 220, 40, "SAVE IMAGE")) {
    saveImageFile();
  }
  if (button(920, menuY+=60, 220, 40, "LOAD BGND IMAGE")) {
    loadImageFile();
  }
  if (button(920, menuY+=60, 220, 40, !showPath?"SHOW PATH":"HIDE PATH" )) {
    showPath=!showPath;
  }  
  if (button(920, menuY+=60, 220, 40, "OPTIMIZE PATH" )) {
    optimizePath();
  }
  if (button(920, menuY+=60, 220, 40, !showGrid?"SHOW GRID":"HIDE GRID")) {
    showGrid=!showGrid;
  }
  if (button(920, menuY+=60, 100, 40, "GRID +")) {
    grid++;
  }  
  if (button(1040, menuY, 100, 40, "CAP +")) {
    capSize++;
  } 
  if (button(920, menuY+=60, 100, 40, "GRID -")) {
    grid=constrain(grid-1, 1, 100);
  }
  if (button(1040, menuY, 100, 40, "CAP -")) {
    capSize=constrain(capSize-1, 1, 100);
  }
  
  menuY=20;
  
  lineColor=colorButton(1160, menuY, 40, 40, #ff0000);
  lineColor=colorButton(1220, menuY, 40, 40, #ffff00);
  lineColor=colorButton(1280, menuY, 40, 40, #00ff00);
  lineColor=colorButton(1340, menuY, 40, 40, #00ffff);

  lineColor=colorButton(1160, menuY+=60, 40, 40, #0000ff);
  lineColor=colorButton(1220, menuY, 40, 40, #ff00ff);
  lineColor=colorButton(1280, menuY, 40, 40, #ffffff);
  lineColor=colorButton(1340, menuY, 40, 40, #000000);
  
  if (button(1160, menuY+=60, 220, 40, "INVERT BGND")) {
    invert=!invert;
  }

  // legend
  textSize(18);
  textAlign(LEFT);
  fill(255);
  text("NAME: "+fileName, 1160, menuY+=75);
  text("LINES: "+lineIndex, 1160, menuY+=30);
  text("GRID:  "+grid+" cm", 1160, menuY+=30);
  text("CAP SIZE: "+capSize, 1160, menuY+=30);
  text("X: "+mX+" cm", 1160, menuY+=30);
  text("Y: "+mY+" cm", 1160, menuY+=30);

  checkKeys();

  if (imageFilePath!="") { // save image when everything is drawn and if there's a new filepath
    save(imageFilePath);
    imageFilePath="";
  }

  pKeyPressed   = keyPressed;
  pMousePressed = nMousePressed;
}
