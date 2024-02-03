#include <iostream>
#include "image.h"
using namespace std;
using namespace agl;

int main(int argc, char** argv)
{

   Image cat;
   if (!cat.load("../source_images/cow.png")) {
      // if (!hatch.load("../source_images/still3.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   Image pastel;
   if (!pastel.load("../texture/pastel_back.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   // Image lines = cat.sobel();
   // lines = cat.darkest(lines);
   // Image p = cat.quantization(8).crand();
   // cat = lines.lightest(p);

   cat = cat.vingette();
   cat.save("sobel_1.png");
   return 0;

   Image hi = cat.cquant().crand().lightest(cat.toTile(pastel).invert());
   cat = cat.alphaBlend(hi,.5);

   cat.save("test.png");


   
   return 0;


   //load in image
   Image hatch;
   if (!hatch.load("../texture/pencil1.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   Image hatch1;
   if (!hatch1.load("../texture/pencil2.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   // cat.tensor().save("tensor.png");

   

   // cat = cat.cquant();
   // cat = cat.crand().alphaBlend(cat,.4);
   // cat.save("cowpaint.png");

   //one
   hatch1 = hatch1.resize(200,200);
   hatch = hatch.resize(200,200);
   Image white = cat;
   white.fill(Pixel{30,30,30});
   // cat = cat.grayscale().cquant();
   // Image cat1 = cat.subtract(white);
   // cat = cat.add(white);
   // cat1 = cat1.crosshatch(hatch1);
   // cat = cat.crosshatch(hatch);
   // cat.darkest(cat1).save("crosshatch.png");

   //two
   Image cat1 = cat.subtract(white).crosshatch(hatch1);
   cat = cat.crosshatch(hatch);
   cat.darkest(cat1).save("crosshatchpng1.png");


   return 0;
   

   // grayscale
   cat = cat.grayscale();
   // gaussians .5
   float k = 1.4;
   Image cat_5 = cat.gaussian(k);
   // gaussians 2.0
   Image cat2 = cat.gaussian(1.6*k);
   // dog
   // cat = cat2.subtract(cat_5);
   // cat.save("dog1.png");

   
   cat = cat2.dog(cat_5,5.0f);
   cat.save("dog5.png");
   
   // threshold
   cat = cat.threshold(200,0.02f);
   cat.save("threshold200_1.png");

   // // Image tile;
   // // if (!tile.load("../texture/pencil1.png")) {
   // //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   // //    exit(0);
   // // }

   // // cow = cow.add(cow).add(cow);
   // // cat.fill(Pixel{255,255,255});
   // // cat.subtract(cow).save("cow7.png");



   // Image cat1 = cat.grayscale();
   // cat1.save("grayscale.png");
   // // gaussians .5
   // Image cat2 = cat1.gaussian(0.5f);
   // // gaussians 2.0
   // Image cat3 = cat1.gaussian(2.0f);
   // // dog
   // cat3.subtract(cat2).save("dog_1.png");
   // cat1 = cat3.dog(cat2, 0.4);
   // cat1.save("dog.png");
   // // threshold
   // cat1 = cat1.threshold(50.0f, 0.4f);
   // cat1.save("threshold.png");
   // cat = cat.cquant();
   // Image cat1 = cow.add(cow).add(cow);
   // cat.crand().blur().subtract(cat1).save("cowlines.png");
   // cat.crand().gaussian(1.0f).cquant().subtract(cat1).save("cowlines1.png");






   // Image background(cat.width(),cat.height());
   // background.fill(Pixel{0,0,0});

   // Image pannel;
   // pannel = cat.crosshatch(tile);

   // pannel.save("crosshatch3.png");
   // Image cat;
   // if (!cat.load("crosshatch3.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // Image pannel;
   // if (!pannel.load("../texture/stiple1.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // Image paper;
   // if (!paper.load("../source_images/paper_texture.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // paper = paper.resize(cat.width(), cat.height());
   // // pannel = cat.toTile(pannel);

   // cat = cat.subtract(pannel.invert());
   // cat = paper.subtract(cat.invert());
   // cat.save("crosshatch3.png");
   
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

