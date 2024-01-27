#include <iostream>
#include "image.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{

   Image cat;
   if (!cat.load("../images_/fire.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   cat = cat.cquant();
   cat = cat.crand();



   cat.save("cat4.png");

   
   // // load image
   // Image woods2;
   // if (!woods2.load("../images/woods.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   
   // // sobel-blur image
   // Image sobel = woods2.blur();
   // sobel = sobel.edgeFinder();

   // Image blend = woods2.alphaBlend(sobel, 0.4f);
   // blend = blend.contrast(25);
   // blend = blend.saturate(15);
   // blend = blend.gammaCorrect(2.1);

   // blend.save("sobel2.png"); 

   // Image earth;
   // if (!earth.load("../images/earth.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }

   // //line an image
   // Image lined = earth.blur();
   // lined = lined.edgeFinder();
   // lined = lined.invert();
   // lined.save("lines.png"); 

   // lined = earth.alphaBlend(lined, 0.5f);
   // lined.save("lined.png"); 

   // // monochrome sobel
   // Image earth;
   // if (!earth.load("../images/hair.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // Image sludge = earth.blur();
   // sludge =  sludge.edgeFinder();
   // sludge =  sludge.contrast(100);
   // Image sludge1 = sludge.subimage(0,0,sludge.width()-5,sludge.height()-5);
   // Image sludge2 = sludge.subimage(0,5,sludge.width()-5,sludge.height());
   // Image sludge3 = sludge.subimage(6,0,sludge.width(),sludge.height()-6);
   // sludge1 = sludge1.resize(sludge.width(),sludge.height());
   // sludge2 = sludge2.resize(sludge.width(),sludge.height());
   // sludge3 = sludge3.resize(sludge.width(),sludge.height());
   
   // sludge1 = sludge1.monochrome(0);
   // sludge1 = sludge1.saturate(30);
   // sludge2 = sludge2.monochrome(1);
   // sludge2 = sludge2.saturate(30);
   // sludge3 = sludge3.monochrome(2);
   // sludge3 = sludge3.saturate(30);

   // sludge1 = sludge1.alphaBlend(sludge2, 0.8f);
   // sludge = sludge1.alphaBlend(sludge3, 0.8f);
   // sludge = sludge.gammaCorrect(2.2);
   // sludge = sludge.contrast(80);

   // sludge.save("sludge.png"); 

   // // binary + blur
   // Image cat;
   // if (!cat.load("../images/cat.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // Image catCol = cat.cquant();

   // cat.save("cat1.png");
   // Image catLine = cat.contrast(100);
   // catLine = cat.edgeFinder();
   // catLine = catLine.invert();
   // catLine = catLine.contrast(100);
   // int w = cat.width();
   // int h = cat.height();
   // cat = cat.resize(cat.width()/5,cat.height()/5);
   // cat = cat.blur();
   // cat = cat.resize(w,h);
   // cat = cat.saturate(20);
   // cat = cat.contrast(20);
   // cat = cat.blur();
   // cat = cat.alphaBlend(catLine, 0.3f);

   // cat.save("cat.png"); 

   

   // // bird sobel, invert, monochrome, filter
   // Image bird;
   // if (!bird.load("../images/fire.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // Image fire = bird.edgeFinder();
   // fire = fire.invert();
   // fire.save("fire.png"); 

   // bird = bird.blur();
   // bird = bird.edgeFinder();
   // Image birdW = bird.invert();
   // Image birdR = bird.monochrome(0);
   // Image birdY = bird.monochrome(3);
   // Image birdO = birdY.alphaBlend(birdR, 0.4f);


   
   // birdO.save("bird.png"); 
   // Image woods;
   // if (!woods.load("../images/woods1.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // // binary
   // Image binary = woods.contrast(25);
   // binary = binary.saturate(255);
   // binary.save("binary.png");
   return 0;
}

