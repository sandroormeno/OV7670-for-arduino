/*
  This sketch reads a raw Stream of RGB565 pixels
  from the Serial port and displays the frame on
  the window.

  Use with the Examples -> CameraCaptureRawBytes Arduino sketch.

  This example code is in the public domain.
*/




import processing.serial.*;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

Serial myPort;

import controlP5.*;

ControlP5 cp5;

// must match resolution used in the sketch
int cameraWidth = 160;
int cameraHeight = 120;

final int cameraBytesPerPixel = 2;

final int bytesPerFrame = cameraWidth * cameraHeight * cameraBytesPerPixel;

PImage myImage;
byte[] frameBuffer = new byte[bytesPerFrame];
//PImage img;
int sice = 3;
void setup()
{
  size(820, 520);
  noStroke();
  //img = createImage(cameraWidth, cameraHeight, RGB); //ALPHA 
  // if you have only ONE serial port active
  //myPort = new Serial(this, Serial.list()[0], 9600); // if you have only ONE serial port active
  // if you know the serial port name
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 1000000); //                    // Windows


  // wait for full frame of bytes
  
  myPort.buffer(bytesPerFrame);  
  //print(myPort);
  myPort.clear();

  myImage = createImage(cameraWidth, cameraHeight, RGB);
       
}

void draw()
{
  image(myImage, 0, 0);
  noStroke();
  if(cameraHeight == 30){
    sice =16;
  }else if (cameraHeight == 60){
    sice =8;
  }else{
   sice =4;
  }
  
  for (int h = 0; h < cameraWidth; h++) {
    for (int w = 0; w < cameraHeight; w++) {
    //img.pixels[i] = color(0, 90, 102, i % img.width * 2); 
      color c = get(h, w);
      //println(c);
      fill(c);
      //fill(153);
      square((h*sice)+cameraWidth, w*sice, sice);
    }
  }
}



void serialEvent(Serial myPort) {


  // read the saw bytes in
  myPort.readBytes(frameBuffer);
  //print(frameBuffer);

  // create image to set byte values
  PImage img = createImage(cameraWidth, cameraHeight, RGB); //ALPHA 
 

  // access raw bytes via byte buffer
  ByteBuffer bb = ByteBuffer.wrap(frameBuffer);
  bb.order(ByteOrder.BIG_ENDIAN);

  int i = 0;

  img.loadPixels();
  while (bb.hasRemaining()) {
    // read 16-bit pixel
    short p = bb.getShort();

    // convert RGB565 to RGB 24-bit
    int r = ((p >> 11) & 0x1f) << 3;
    int g = ((p >> 5) & 0x3f) << 2;
    int b = ((p >> 0) & 0x1f) << 3;
    int a = ((p >> 8) & 0xFF);  // solo para YUV

    // set pixel color
    img.pixels[i++] = color(r, g, b);
    //img.pixels[i++] = color(a);
    //img.pixels[i++] =  color(255, 255, 255, a);
    //pixels[i++] = color(a);
    //print(a);
    //print("\n");
  }
  img.updatePixels();

  // assign image for next draw
  myImage = img;

}

//Save image when mouse pressed the dialog
void mousePressed() 
{
  save("RGB565_QVGA_gray_30x30.jpg");
}
