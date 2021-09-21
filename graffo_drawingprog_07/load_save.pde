

void loadFile() {
  selectInput("Select a file to process:", "loadSelected", dataFile( "*.PLT"));
}

void loadSelected(File selection) {
  if (selection == null) {
    println("window closed");
  } else {
    println("User selected " + selection.getAbsolutePath());
    BufferedReader reader = createReader(selection.getAbsolutePath());
    fileName=selection.getName();
    String line = null;
    try {
      while ((line = reader.readLine()) != null) {
        String[] pieces = split(line, ",");
        cP[lineIndex] = int(pieces[0]);
        xP[lineIndex] = int(pieces[1]);
        yP[lineIndex] = int(pieces[2]);
        sP[lineIndex] = int(pieces[3]);
        if (cP[lineIndex]!=3) { // skip calibration
          lineIndex++;
        }
      }
      reader.close();
    } 
    catch (IOException e) {
      e.printStackTrace();
    }
  }
  lineIndex--;
}

//////////////

void exportFile() {
  selectOutput("Save file as:", "saveSelected", dataFile( ".PLT"));
}

void saveSelected(File selection) {
  if (selection == null) {
    println("window closed");
  } else {
    fileName=selection.getName();
    println("exporting: "+selection.getAbsolutePath());
    output = createWriter(selection.getAbsolutePath()); 
    output.println("003,001,001,001"); // calibrate
    for (int i=0; i<=lineIndex; i++) { // draw all lines
      exportLineTo(cP[i], xP[i], yP[i], sP[i]);
    }
    output.flush();  
    output.close();
  }
}


void exportLineTo(int command, float newX, float newY, float servo) {  
  newX=constrain(newX, 1, 255);
  newY=constrain(newY, 1, 255);
  servo=constrain(servo, 1, 255);
  command=constrain(command,1,255);
  output.println(command+","+(nf(newX, 3, 0))+","+(nf(newY, 3, 0))+","+(nf(servo, 3, 0))); // Write the coordinate to the file
}

////////////// save image
String imageFilePath="";

void saveImageFile() {
  imageFilePath="";
  selectOutput("Save image as:", "saveImage", dataFile( ".PNG"));
}
void saveImage(File selection) {
  imageFilePath=selection.getAbsolutePath();
}

void loadImageFile() {
  imageFilePath="";
  selectInput("Load background image:", "loadImg", dataFile( ".PNG"));
}

void loadImg(File selection) {
  bgndImage = loadImage(selection.getAbsolutePath());
  showBgnd=true;
}
