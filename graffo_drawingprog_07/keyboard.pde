void checkKeys() {
  if (keyPressed && !pKeyPressed) {                            // check keyboard:
    if (key == 'd') {                                          // d: delete last line
      undo();
    }
    if (key == ' ') {                                          // space: move to without painting
      lineIndex++;
      xP[lineIndex]=mX;
      yP[lineIndex]=mY;
      cP[lineIndex]=2;
      sP[lineIndex]=1;
    }
    if (key == 'e') {                           // e: export to file
      exportFile();
    }
    if (key == 'l') {                           // l: load file
      loadFile();
    }
    if (key == CODED) {
      if (keyCode == UP) {
        for (int i=0; i<=lineIndex; i++) {      // shift image
          yP[i]=constrain(yP[i]-1, 1, 200);
        }
      } else if (keyCode == DOWN) {
        for (int i=0; i<=lineIndex; i++) {      // shift image
          yP[i]=constrain(yP[i]+1, 1, 200);
        }
      } else if (keyCode == LEFT) {
        for (int i=0; i<=lineIndex; i++) {      // shift image
          xP[i]=constrain(xP[i]-1, 1, 220);
        }
      } else if (keyCode == RIGHT) {        
        for (int i=0; i<=lineIndex; i++) {      // shift image
          xP[i]=constrain(xP[i]+1, 1, 220);
        }
      }
    }
  }
}
