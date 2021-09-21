
int mX=0;
int mY=0;
void drawCursor() {

  mX=(mouseX-xOff)/scale;
  mY=(mouseY-yOff)/scale;

  float mXGrid=(int(float(mX)/(grid)+.5))*(grid);
  float mYGrid=(int(float(mY)/(grid)+.5))*(grid);


  mX=constrain(int(mXGrid), 1, xMax);
  mY=constrain(int(mYGrid), 1, yMax);

  strokeWeight(1);
  if (invert) {
    stroke(255, 128);
  } else {
    stroke(0, 128);
  }
  line((mX+5)*scale+xOff, (mY+5)*scale+yOff, (mX-5)*scale+xOff, (mY-5)*scale+yOff);
  line((mX-5)*scale+xOff, (mY+5)*scale+yOff, (mX+5)*scale+xOff, (mY-5)*scale+yOff);
}



void lineTo(int command, float newX, float newY, float servo) {
  command=constrain(command, 1, 255);
  newX=constrain(newX, 1, 255);
  newY=constrain(newY, 1, 255);
  servo=constrain(servo, 1, 255);

  if (invert) {
    stroke(lineColor, int(servo)*.75);
  } else {
    stroke(lineColor, int(servo)*.75);
  }
  strokeWeight(capSize);

  if (servo==1 && showPath) {
    strokeWeight(1);
    stroke(255, 0, 0);
  }


  line(oldX*scale+xOff, oldY*scale+yOff, newX*scale+xOff, newY*scale+yOff);
  oldX=newX;
  oldY=newY;
}

///////////////////////////////////////////////////////////////////////////////////////
void drawCanvas() {
  int alpha=255;
  if (showBgnd) {
    alpha=0;
  }
  if (invert) {
    fill(0, alpha);
  } else {
    fill(255, alpha);
  }

  noStroke();
  
  rect(xOff, yOff, 220*scale, 200*scale);
  fill(64);
  rect(xOff+260*scale, yOff, 800, 200*scale);

  if (invert) {
    stroke(48);
  } else {
    stroke(200);
  }
  if (showGrid) {
    strokeWeight(1);
    for (int i=0; i<=xMax; i+=grid) {
      line(i*scale+xOff, yOff, i*scale+xOff, yOff+yMax*scale);
    }
    for (int i=0; i<=yMax; i+=grid) {
      line(xOff, i*scale+yOff, xOff+xMax*scale, i*scale+yOff);
    }
  }
}




/////////////////////////////////////
// draw all lines

void drawLines() {
  oldX=1;
  oldY=1;
  for (int i=0; i<=lineIndex; i++) { 
    lineTo(cP[i], xP[i], yP[i], sP[i]);
  }
  if (lineIndex>0 && mouseOnCanvas)lineTo(1, mX, mY, 64);
}

////////////////////////////////////
// undo last line
void undo() {
  if (lineIndex>0)lineIndex--;
}

////////////////////////////////////
// optimize path
void optimizePath() {
  int[] nxP = new int[30000]; //x-position
  int[] nyP = new int[30000]; //y-position
  int[] nsP = new int[30000]; //servo
  int[] ncP = new int[30000]; //command
  int newLineIndex=0;

  for (int i=0; i<lineIndex; i++) { 
    if (sP[i+1]==255 || sP[i]==255) {
      ncP[newLineIndex]=cP[i];
      nxP[newLineIndex]=xP[i];
      nyP[newLineIndex]=yP[i];
      nsP[newLineIndex]=sP[i];
      newLineIndex++;
    }
  }

  lineIndex=newLineIndex;

  for (int i=0; i<lineIndex; i++) { 

    cP[i]=ncP[i];
    xP[i]=nxP[i];
    yP[i]=nyP[i];
    sP[i]=nsP[i];
  }
}
