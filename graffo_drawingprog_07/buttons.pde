

///////////////////////////////////////////////////////////////////////////////////////////////////// menu button

boolean button(int buttonX, int buttonY, int buttonW, int buttonH, String buttonLabel) {
  boolean pressed=false;

  boolean over=(mouseX>=buttonX && mouseY>=buttonY && mouseX<= buttonX+buttonW && mouseY<=buttonY+buttonH);
  if (over) {
    fill(128);
    if (nMousePressed && pMousePressed==false) {
      pressed=true;
      fill(160);
    }
  } else {
    fill(96);
  }

  noStroke();
  rect(buttonX, buttonY, buttonW, buttonH);
  fill(255);
  textSize(18);
  textAlign(CENTER);
  text(buttonLabel, buttonX+buttonW/2, buttonY+buttonH*.7);
  return pressed;
}

///////////////////////////////////////////////////////////////////////////////////////////////////// color picker


int colorButton(int buttonX, int buttonY, int buttonW, int buttonH, int buttonColor) {
  int returnColor=lineColor;

  noStroke();
  boolean over=(mouseX>=buttonX && mouseY>=buttonY && mouseX<= buttonX+buttonW && mouseY<=buttonY+buttonH);
  if (over) {
    stroke(255);
    strokeWeight(1);
    if (nMousePressed && pMousePressed==false) {
      returnColor=buttonColor;
      println("change color");
    }
  }
  fill(buttonColor);
  rect(buttonX, buttonY, buttonW, buttonH);
  return returnColor;
}
