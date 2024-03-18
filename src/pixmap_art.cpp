#include <iostream>
#include "image.h"
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
using namespace agl;



int main(int argc, char** argv)
{
   srand(time(0));

   Image still;
   Image brush;
   if (!brush.load("../texture/orb1.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   
   // brush = brush.rotate90();
   Image brush_thin;
   if (!brush_thin.load("../texture/pencil_line.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

/* PAINTERLY*/
   string names[] = {"still3","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};
   // PEOPLE: small brush, multiple layers, low cutoff, high cutoff with smaller/thinner brush
   // string peps[] = {"fam1","man","kid","lottie","orangutan","sturgeon"}; 
   // STILLS, LANDSCAPE : larger brush, mid cutoff
   // string land[] = {"party","cow","house","walkway","bat","bench","cityscape","cloud","hut","sea1","shell","still1","still3","texture","orangutan","lottie"};

   int m = 900;
   for(string n : names){
      if (!still.load("../source_images/"+n+".png")) {
         break;
      }
      if (max(still.width(),still.height())>m){
         still = still.resize((still.width()/(float)still.height())*(m),m);
      }
      still = still.cnormalize();
      Image background = still.paint(still,brush_thin,0,2);
      background = still.paint(background,brush_thin,80,1.5);
      still.paint(background,brush_thin,120,.8).save("../src/"+n+"paint1.png");
   }
   
   return 0;
  


/*LINEART
   string names[] = {"still3","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};
   int m = 1000;
   for(string n : names){
      if (!still.load("../source_images/"+n+".png")) {
         break;
      }
      if (max(still.width(),still.height())>m){
         still = still.resize((still.width()/(float)still.height())*(m),m);
      }
      Image t1= still.tensor(true).blur();
      Image t2= still.tensor(false).blur();
      Image grey  = still.blur().grayscale().normalize();

      float s_ = min(still.width(),still.height())/400.0f;
      float sigma = s_;
      float k = .75;
      float cutoff = 6;
   
      Image line = ((grey.gaussian(sigma)).subtract(grey.gaussian(sigma*k))).threshold(cutoff,.2).dirrected_gaussian(1.2,t1);

      line.invert().save("../src/"+n+"lineart.png");
   }
   
   return 0;*/

/* CHARCOAL & WATERCOLOR
      string names[] = {"still3","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};
   int m = 900;
   for(string n : names){
      if (!still.load("../source_images/"+n+".png")) {
         break;
      }
      if (max(still.width(),still.height())>m){
         still = still.resize((still.width()/(float)still.height())*(m),m);
      }
      Image t1= still.tensor(true).blur();
      Image t2= still.tensor(false).blur();
      Image grey  = still.grayscale().normalize();

      float s_ = min(still.width(),still.height())/150.0f;
      float sigma = s_;
      float k = 1.3;
      float tau  = 5;
      float cutoff = 180;
   
      Image line = ((grey.dirrected_gaussian_1(sigma,t1)).dog(grey.dirrected_gaussian_1(sigma*k,t1),tau)).dirrected_gaussian(2,t1).threshold(cutoff,.01);

      line.save("../src/"+n+"charcoal.png");
   }
   
   return 0;
*/
/* CROSSHATCH

   Image pannel;
   if (!pannel.load("../texture/ink1.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   Image pannel1;
   if (!pannel1.load("../texture/ink2.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

   // still = still.resize(still.width()/2,still.height()/2);
   still = still.grayscale();
   float s_ = min(still.width(),still.height())/150.0f;
   Image t = still.tensor(false).gaussian(1.5);
   Image t1 = still.tensor(true).gaussian(1);
   Image grey = still.grayscale().invert().gaussian(1);
   Image lines = ((grey.dirrected_gaussian_1(s_*.1,t)).subtract(grey.dirrected_gaussian_1(s_*1.5,t))).threshold(30,.1).dirrected_gaussian(1,t1);//
   // lines.save("v1.png");
   // return 0;

   int size = min(still.height(),still.width())/6;
   pannel = pannel.resize(size + rand()%20 -10,size+ rand()%20 -10);
   pannel1 = pannel1.resize(size+ rand()%20 -10,size+ rand()%20 -10);
   Image pannel2 = pannel.resize(size+ rand()%20 -10,size+ rand()%20 -10).rotate90();
   Image pannel3 = pannel1.resize(size+ rand()%20 -10,size+ rand()%20 -10).rotate90();
   

   Image black = still.threshold(30,.1);
   Image dark = still.threshold(50,.1);
   Image med = still.threshold(100,.1);
   Image light = still.threshold(120,.1);
   Image white = still.threshold(160,.1);
   black.save("black.png");
   dark.save("black1.png");
   med.save("black2.png");
   light.save("black3.png");
   white.save("black4.png");

   pannel = still.toTile(pannel);
   pannel1 = still.toTile(pannel1);
   pannel2 = still.toTile(pannel2);
   pannel3 = still.toTile(pannel3);
   pannel.save("panel.png");
   pannel1.save("panel1.png");
   pannel2.save("panel2.png");
   pannel3.save("panel3.png");
   pannel = pannel.lightest(dark);
   pannel1 = pannel1.lightest(med);
   pannel2 = pannel2.lightest(light);
   pannel3 = pannel3.lightest(white);
   pannel.save("panel4.png");
   pannel1.save("panel5.png");
   pannel2.save("panel6.png");
   pannel3.save("panel7.png");
   pannel = pannel.darkest(pannel1);
   pannel1 = pannel2.darkest(pannel3);
   pannel = pannel.darkest(pannel1);
  
   
   pannel.darkest(black).darkest(lines).save("fin.png");
   return 0;
*/








//====================================================
/* LIGHT EDGE... yikes
// binary threshold of a photo
still = still.resize(still.width()*.8,still.height()*.8);
float s_ = min(still.width(),still.height())/150.0f;
Image light = still.threshold(140,.08); 
// light.sobel().save("v2.png");
// edge detection and blur
Image t = light.tensor(false).blur();
Image t_ = light.tensor(true).blur();
// t.save("v2.png");
// return 0;
Image edge = ((light.dirrected_gaussian_1(s_*.1,t)).subtract(light.dirrected_gaussian_1(s_*4,t))).threshold(40,.1).blur();//.dirrected_gaussian(2,t_)
edge.save("v2.png");
Image white = still;
white.fill(Pixel{0,0,0});
// use edge detection as mask
// white = white.alphaBlend(still.saturate(10),edge);

// add
still = still.alphaBlend(still.saturate(20),edge);
// still = still.lightest(white);
still.save("v1.png");
return 0;*/


/* PAINTERLY WITH TENSOR
   // Image tensor =still.tensor(false).blur();
   Image tensor1 = still.tensor(false).blur();
   Image white = still;
   for(int i = 0; i < still.height(); i++){
      for(int j = 0; j < still.width(); j++){ 
         if (tensor1.get(i,j).b<90){
            // if it's chaotic turn it white
            white.set(i,j,Pixel{255,255,255});
         } else {
            white.set(i,j,Pixel{0,0,0});
         }
      }
   }
   float s_ = min(still.width(),still.height())/180.0f;
   
   Image blur = still.gaussian(3);
   Image still_ = still.alphaBlend(blur,white.blur());
   // det.save("v3.png");
   Image tens = still_.tensor(false);
   Image tens1 = still_.tensor(true).cquant().blur();
   
   // still.dirrected_gaussian_1(1,tensor).subtract(still.dirrected_gaussian_1(2,tensor)).save("v2.png");
   white.fill(Pixel{255,255,255});
   still = still.cquant();
   still_ = still.dirrected_gaussian(s_,tens).dirrected_gaussian_1(s_,tens1);
   // Image det = still.dirrected_gaussian(s_/3,tens);
   // still_ = still_.alphaBlend(det,white.vingette().blur());
   still_.save("../cloud.png");
   return 0;
   */


/* ITERATE THROUGH sigmas
   float sigma = .5;
   float k = .5;
   float tau  = 1;
   string name;
   still = still.resize(still.width()*.8,still.height()*.9);
   Image s = still.tensor(false).blur();
   Image s_ = still.tensor(true).blur();
   still = still.grayscale();
   Image one,two;
   for(int i =3;i<10;i++){
      sigma = i*.2;
      for(int j =10;j<20;j++){
         k = j*.1;
         for(float m =1;m<9;m+=.5){
            tau = m;
            name = to_string(sigma)+"_"+to_string(k)+"_"+to_string(tau)+".png";
            one = still.dirrected_gaussian_1(sigma,s);
            two = still.dirrected_gaussian_1(sigma*k,s);
            one.dog(two,tau).dirrected_gaussian(1,s_).save(name);
            

         }
      }
   }

   
   return 0;
*/
   // // still.dirrected_gaussian(5.5,still.crand().tensor(true).blur()).save("v.png");
   // Image s1 = still.tensor(true);
   // Image s = still.tensor(false).saturate(30).cquant().dirrected_gaussian_1(2,s1);

   // still = still.grayscale();
   // Image one = still.dirrected_gaussian_1(1.5,s);
   // Image two = still.dirrected_gaussian_1(2.1,s);
   // one.dog(two,4).threshold(150,.01).save("v3.png");
   
   // return 0;

   // Image still1 = still.grayscale();
   // // Image s = still.tensor(false).blur();
   // // Image s1 = still.tensor(true);
   // // s = s.dirrected_gaussian(.5,s1);
   // s.save("v1.png");
   // Image c1 = still1.dirrected_gaussian_1(2.5,s);
   // Image c2 = still1.dirrected_gaussian_1(1.9,s);
   // c2 = c2.dog(c1,10.0f);
   // // c2=c2.add(still.crand());
   // c2.save("v.png");
   // // Image w = c2;
   // // w.fill(Pixel{255,255,255});
   // // w = w.vingette().invert();
   // // still.subtract(c2).save("v1.png");

   // return 0;

   // // ((ce.threshold(250,.004).multiply(still.crand())).saturate(20)).save("v1.png");

   // // Image t = still.tensor().blur();
   // // // still = still.dirrected_gaussian(5.10f,t);
   // // // still.save("v.png");

   // // Image cat_5 = still.dirrected_gaussian_1(.8,t);
   // // // gaussians 2.0
   // // Image cat2 = still.dirrected_gaussian_1(1.8,t);
   // // cat2 = cat2.dog(cat_5,1.2f).threshold(240,0.008).dirrected_gaussian(1.1,t);
   // // cat2.save("v1.png");

   // // return 0;


   // // Image pastel;
   // // if (!pastel.load("../texture/pencil_mark.png")) {
   // //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   // //    exit(0);
   // // }

   // Image cat = still;
   // cat.fill(Pixel{255,0,0});
   // cat.resize(400,300).vingette().save("v1.png");
   // cat.resize(300,400).vingette().save("v.png");
   // return 0;
   // // still = still.cquant();
   
   // Image center;
   // // if (!center.load("../build/center.png")) {
   // //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   // //    exit(0);
   // // }
   // center = cat.tensor(false);
   // center = center.gaussian(.5);
   // // Image line = cat.sobel();

   // // grayscale
   // cat = cat.grayscale();
   // // gaussians .5
   // // float k = 1.9;
   // // float m = .4;
   // float m = 1.9;
   // float k = 0.4;
   // Image cat_5 = cat.dirrected_gaussian(k,center);
   // // gaussians 2.0
   // Image cat2 = cat.dirrected_gaussian(m*k,center);
   // dog
   // cat = cat2.subtract(cat_5);
   // cat.save("dog1.png");
   
   //apply a vinette
   //apply white lineart to black vinnete
   // subtract


   
   // cat2 = cat2.dog(cat_5,1.2f).threshold(210,0.008).dirrected_gaussian(1.9,center);
   // cat2.save("center2.png");
   // return 0;

   // // cat2 = cat2.multiply(still.crand());
   // Image coal = cat2;
   
   // // return 0;
   
   // // k = 2.1;
   // // m = .5;
   // k = 1.4;
   // m = .5;
   // cat_5 = cat.gaussian(k);
   // // gaussians 2.0
   // cat2 = cat.gaussian(m*k);
   // cat2 = cat2.dog(cat_5,10.0f);
   // Image line = cat2.threshold(50,.04).dirrected_gaussian(1.9,center);
   // coal = coal.darkest(line);
   // coal.save("center5.png");
   // // coal.multiply(still).save("center2.png");
   // return 0;

   // Image n = cat.multiply(still);
   // n.save("center4.png");
   // // n = cat.subtract(line);
   // // n.save("center5.png");
   // // threshold
   // cat = cat.threshold(100,0.1f);
   // cat.save("center3.png");
   // cat = cat.threshold(170,0.1f);
   // cat.save("center6.png");
   
   
   // // filter that takes one grey and one color and produces a color but perserves the grey's value
   // // 100,100,100
   // // 100,0,0
   // // 

   // return 0;


   
   // Image w = center;
   // w.fill(Pixel{0,0,0});

   // for(int i =0;i<w.height();i++){
   //    float y = i;
   //    for(int j =0;j<w.width();j++){
   //       float x = j;
   //       Pixel rgb = center.get(y,x);
   //       if(rgb.b>90){
   //          for (int n =0;n<10;n++){
   //             w.set(y,x,Pixel{255,0,0});
   //             x+=(rgb.r)/127.5 - 1;
   //             y+=(rgb.g)/127.5 - 1;
   //          }
   //       }

         
         
   //    }
   // }
   // w.save("center1.png");
   

   // center.save("center.png");
   // center = center.gaussian(1.4);
   // center = center.vfc();
   // center.save("center1.png");
   // return 0;
   // center = center.dirrected_gaussian(1.6,center);
   // center.save("center2.png");

   // cat = cat.dirrected_gaussian(5.6,center);
   // cat.save("center2.png");
   // return 0;
   // cat = cat.gaussian(1.4);
   // Image white = cat;
   // white.fill(Pixel{255,255,255});
   
   // int x,y,i,j;
   // Pixel to_set = Pixel{255,0,0};
   // for(int a = 0;a<cat.height();a+=5){      
   //    for(int b = 0;b<cat.width();b+=5){
   //       i = a;
   //       j = b;
   //       to_set = cat.get(i,j);
   //       for (int n = 0;n<30;n++){
   //          Pixel p = center.get(i,j);
   //          x = p.r>0?1:0;
   //          y = p.b<100?(p.g>0?1:0):(p.g>0?-1:0);
   //          i = i+x;
   //          j = j+y;
   //          white.set(i,j,to_set);
   //       }
   //    }
   // }
   
   // white.save("center2.png");

   // cat = cat.gaussian(1.4);
   // cat.save("center1.png");
   // cat = cat.vfc();
   // cat.save("center2.png");

   return 0 ;

   // Image lines = cat.sobel().invert();
   // Image detail = cat.vingette();
   // cat = cat.lightest();
   // lines = cat.darkest(lines);
   // Image p = cat.quantization(8).crand();
   // cat = lines.lightest(p);

   // // cat = cat.vingette();
   // Image lines = cat.sobel().invert();
   // // lines = cat.darkest(lines).crand();
   // Image detail = cat.cquant();
   // Image blur = detail.crand();

   // Image white = cat;
   // white.fill(Pixel{255,255,255});
   // Image center = white.vingette();
   // Image big_center = center.subimage(.1*center.width(),.1*center.height(),.8*center.width(),.8*center.height()).resize(center.width(),center.height());
   // big_center = big_center.add(big_center).gaussian(3.5);
   // big_center.save("center.png");
   // Image edge = center.invert();
   // detail = detail.darkest(lines).add(edge);//detail with small white edges
   // // blur.save("sobel_1.png");
   // // detail.save("sobel_2.png");
   // blur.add(detail).save("sobel.png");


   // return 0;

   // Image hi = cat.cquant().crand().lightest(cat.toTile(pastel).invert());
   // cat = cat.alphaBlend(hi,.5);

   // cat.save("test.png");


   
   // return 0;


   // //load in image
   // Image hatch;
   // if (!hatch.load("../texture/pencil1.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // Image hatch1;
   // if (!hatch1.load("../texture/pencil2.png")) {
   //    std::cout << "ERROR: Cannot load image! Exiting...\n";
   //    exit(0);
   // }
   // // cat.tensor().save("tensor.png");

   

   // // cat = cat.cquant();
   // // cat = cat.crand().alphaBlend(cat,.4);
   // // cat.save("cowpaint.png");

   // //one
   // hatch1 = hatch1.resize(200,200);
   // hatch = hatch.resize(200,200);
   // Image white_ = cat;
   // white_.fill(Pixel{30,30,30});
   // // cat = cat.grayscale().cquant();
   // // Image cat1 = cat.subtract(white);
   // // cat = cat.add(white);
   // // cat1 = cat1.crosshatch(hatch1);
   // // cat = cat.crosshatch(hatch);
   // // cat.darkest(cat1).save("crosshatch.png");

   // //two
   // Image cat1 = cat.subtract(white).crosshatch(hatch1);
   // cat = cat.crosshatch(hatch);
   // cat.darkest(cat1).save("crosshatchpng1.png");


   // return 0;
   

   // // grayscale
   // cat = cat.grayscale();
   // // gaussians .5
   // float k = 1.4;
   // Image cat_5 = cat.gaussian(k);
   // // gaussians 2.0
   // Image cat2 = cat.gaussian(1.6*k);
   // // dog
   // // cat = cat2.subtract(cat_5);
   // // cat.save("dog1.png");

   
   // cat = cat2.dog(cat_5,5.0f);
   // cat.save("dog5.png");
   
   // // threshold
   // cat = cat.threshold(200,0.02f);
   // cat.save("threshold200_1.png");

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

