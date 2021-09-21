//........................................... read encoder
boolean ya = false;
boolean yb = false;
boolean pya, pyb;
boolean xa = false;
boolean xb = false;
boolean pxa, pxb;

void readPositions() {
  pya = ya;
  pyb = yb;
  ya = analogRead(YENCODER1) > 500;
  yb = analogRead(YENCODER2) > 500;
  if (ya != pya) {
    if (!ya) {
      if (yb) {
        yPos++;
      } else {
        yPos--;
      }
    } else {
      if (yb) {
        yPos--;
      } else {
        yPos++;
      }
    }
  }

    if (yb != pyb) {
    if (!yb) {
      if (!ya) {
        yPos++;
      } else {
        yPos--;
      }
    } else {
      if (!ya) {
        yPos--;
      } else {
        yPos++;
      }
    }
  }

  /////X/////

  pxa = xa;
  pxb = xb;
  xa = analogRead(XENCODER1) > 300;
  xb = analogRead(XENCODER2) > 300;
  if (xa != pxa) {
    if (xa) {
      if (xb) {
        xPos++;
      } else {
        xPos--;
      }
    } else {
      if (xb) {
        xPos--;
      } else {
        xPos++;
      }
    }
  }

    if (xb != pxb) {
    if (xb) {
      if (!xa) {
        xPos++;
      } else {
        xPos--;
      }
    } else {
      if (!xa) {
        xPos--;
      } else {
        xPos++;
      }
    }
  }
}
