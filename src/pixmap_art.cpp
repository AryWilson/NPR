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

   /* visualize tensor
   if (!still.load("../source_images/line.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   // still = still.resize(still.width()/2,still.height()/2);
   Image t = still.tensor(true);
   float x,y,ii,jj;
   Pixel c;
   
   for(int i = 0; i<still.height(); i=i+1){
      for(int j = 0; j<still.width(); j=j+1){
         if(t.get(i,j).b>240){
            still.set(i,j,Pixel{255,0,0});
            ii = i;
            jj = j;
            for(int n = 0; n<10; n++){
               c = t.get(round(ii),round(jj));
               if(c.b<50){break;}
               x = ((float)c.r/127.5f)-1.0f;
               y = ((float)c.g/127.5f)-1.0f;
               ii = ii + y;
               jj = jj + x;
               still.set(round(ii),round(jj),Pixel{0,255,0});
            }
         }
      }
   }

   still.save("../src/image.png");
   return 0;*/

   Image brush;
   if (!brush.load("../texture/brush.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   Image orb;
   if (!orb.load("../texture/orb1.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   Image brush_thin;
   if (!brush_thin.load("../texture/pencil_line.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }

/*cartoon?? - tensor as mask
   string names[] = {"still3","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};

   int m = 900;
   for(string n : names){
      if (!still.load("../source_images/"+n+".png")) {
         continue;
      }
      if (max(still.width(),still.height())>m){
         still = still.resize((still.width()/(float)still.height())*(m),m);
      }
      Image quant = still.cquant(1);
      Image mask = still.tensor(true);
      for (int i =0; i<mask.height(); i++){
         for (int j =0; j<mask.width(); j++){
            if(mask.get(i,j).b<50){
               mask.set(i,j,Pixel{0,0,0});
            } else {
               mask.set(i,j,Pixel{255,255,255});
            }
         }

      }
      mask.save("../src/"+n+"toonl.png");
      still.alphaBlend(quant,mask).save("../src/"+n+"toon.png");

   }
   
   return 0;*/
   // string names[] = {"biologist","crowd","dorian","dress","girls","nails","ring","skunk1","skunk2","skunk3","students","braids"};
   // string names[] = {"kid", "man", "party", "house", "shell", "sturgeon", "starfish", "bat", "coast", "cityscape", "bench", "hut", "sea1"};
   // string names[] = {"still3","still4","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};
   string names[] = {"catsith1"};
   int m = 1000;


/* PAINTERLY*/
   // string names[] = {"still3","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};
   // PEOPLE: small brush, multiple layers, low cutoff, high cutoff with smaller/thinner brush
   // string peps[] = {"fam1","man","kid","lottie","orangutan","sturgeon"}; 
   // STILLS, LANDSCAPE : larger brush, mid cutoff
   // string land[] = {"party","cow","house","walkway","bat","bench","cityscape","cloud","hut","sea1","shell","still1","still3","texture","orangutan","lottie"};
   // int m = 1000;
   for(string n : names){
      if (!still.load("../source_images/"+n+".png")) {
         cout<<"what";
         continue;
      }
      // if (max(still.width(),still.height())>m){
      //    still = still.resize((still.width()/(float)still.height())*(m),m);
      // }
      // Image grey = still.grayscale().normalize();
      // grey.save("../src/00"+n+"paint.png");
      // still.normalize().save("../src/0123"+n+".png");
      // still.wnormalize().save("../src/0weight"+n+".png");
      // continue;
      still = still.cnormalize().alphaBlend(still,.8);
      Image background = still.paint(still,brush,0,.7);
      background = still.paint(background,orb,70,.5);
      still.paint(background,orb,150,.25).save("../src/000"+n+"paint.png");
   }
   return 0;


/*LINEART
   // string names[] = {"still3","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};
   // int m = 1000;
   for(string n : names){
      if (!still.load("../source_images/"+n+".png")) {
         continue;
      }
      if (max(still.width(),still.height())>m){
         still = still.resize((still.width()/(float)still.height())*(m),m);
      }
      Image t1= still.blur().tensor(true).blur();
      // Image t2= still.tensor(false).blur();
      // Image grey  = still.blur().grayscale().normalize();

      // float s_ = min(still.width(),still.height())/400.0f;
      // float sigma = s_;
      // float k = .75;
      // float cutoff = 6;
   
      // Image line = ((grey.gaussian(sigma)).subtract(grey.gaussian(sigma*k))).threshold(cutoff,.2).dirrected_gaussian(1.2,t1);

      // line.invert().save("../out/"+n+"lineart.png");
      Image white = still;
      white.fill(Pixel{255,255,255});
      still.paint(white,brush_thin,90,.9).save("../out/"+n+"lineart1.png");

   }*/
   

/* CHARCOAL & WATERCOLOR*/
      // string names[] = {"still3","kid", "lottie", "man", "fam1", "walkway", "cow", "orangutan", "party", "still1", "cloud", "house", "shell", "sturgeon", "starfish", "bat", "texture", "coast", "cityscape", "woods", "bench", "hut", "sea1"};
   // int m = 900;
   for(string n : names){
      if (!still.load("../source_images/"+n+".png")) {
         continue;
      }
      if (max(still.width(),still.height())>m){
         still = still.resize((still.width()/(float)still.height())*(m),m);
      }
      Image t1= still.blur().tensor(true).blur();
      Image t2= still.tensor(false).blur();
      Image grey  = still.grayscale();
      grey = grey.alphaBlend(grey.normalize(),.6);
      // grey = grey.normalize();

      float s_ = min(still.width(),still.height())/150.0f;
      float sigma = s_;
      float k = 1.2;
      float tau  = 21;
      float cutoff = 190;
      Image mask = t1.mask().blur().threshold(80,.009);
      Image line = ((grey.dirrected_gaussian_1(sigma,t1)).dog(grey.dirrected_gaussian_1(sigma*k,t1),tau)).dirrected_gaussian(2.5,t1).threshold(cutoff,.008);
      Image grain = line.crand().rnoise(3).alphaBlend(line,mask);
      
      line.save("../pm/"+n+"charcoal0.png");
      grain.save("../pm/"+n+"charcoal1.png");

      // for(int i=0; i<still.height(); i++){
      //    for(int j=0; j<still.width(); j++){
      //       if(grey.get(i,j).r>210){still.set(i,j,Pixel{255,255,255});}
      //    }
      // }
      // still = still.cquant(4).crand().blur();
      // line.multiply(still).save("../out/"+n+"water.png");
   }
   return 0;
   
/* CROSSHATCH & LINEART*/

   Image pannel_;
   if (!pannel_.load("../texture/ink1.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   Image pannel_1;
   if (!pannel_1.load("../texture/ink2.png")) {
      std::cout << "ERROR: Cannot load image! Exiting...\n";
      exit(0);
   }
   for(string n : names){
      Image pannel = pannel_;
      Image pannel1 = pannel_1;
      if (!still.load("../source_images/"+n+".png")) {
         continue;
      }
      if (max(still.width(),still.height())>m){
         still = still.resize((still.width()/(float)still.height())*(m),m);
      }
      Image col = still;
      still = still.grayscale();
      Image t1 = still.blur().tensor(true).blur();
      // Image lines = ((grey.dirrected_gaussian_1(sigma,t2)).subtract(grey.dirrected_gaussian_1(sigma*k,t2))).threshold(10,.5).dirrected_gaussian(1,t1);//
      Image t2= still.blur().tensor(false).blur();

      float sigma = min(still.width(),still.height())/400.0f;
      float k = .79;
      float cutoff = 7;
      
      Image line = ((still.gaussian(sigma)).subtract(still.gaussian(sigma*k))).threshold(cutoff,.2).dirrected_gaussian(1.2,t1);

      // Image lines = t1.monochrome(2).grayscale().threshold(25,.1).dirrected_gaussian(1,t1);
      // lines.invert().save("../out/"+n+"lineart.png");
      int size = min(still.height(),still.width())/6.9f;
      pannel = pannel.resize(size + rand()%20 -10,size+ rand()%20 -10);
      pannel1 = pannel1.resize(size+ rand()%20 -10,size+ rand()%20 -10);
      Image pannel2 = pannel.resize(size+ rand()%20 -10,size+ rand()%20 -10).rotate90();
      Image pannel3 = pannel1.resize(size+ rand()%20 -10,size+ rand()%20 -10).rotate90();

      still = still.normalize();
      Image black = still.threshold(20,.04);
      Image dark = still.threshold(70,.09);
      Image med = still.threshold(120,.09);
      Image light = still.threshold(150,.09);
      Image white = still.threshold(200,.09);
      // keep the whites white (line-free)
      Image w = white;
      w.fill(Pixel{0,0,0});
      line = line.alphaBlend(w,light.blur());


      pannel = still.toTile(pannel);
      pannel1 = still.toTile(pannel1);
      pannel2 = still.toTile(pannel2);
      pannel3 = still.toTile(pannel3);

      pannel3 = pannel3.lightest(white).add(pannel1.invert());
      pannel = pannel.lightest(dark);
      pannel1 = pannel1.lightest(med);
      pannel2 = pannel2.lightest(light);

      pannel = pannel.subtract(pannel1.invert());
      pannel1 = pannel2.subtract(pannel3.invert());
      pannel = pannel.subtract(pannel1.invert());
      

      pannel.darkest(black).subtract(line).save("../out/"+n+"crosshatch2.png");
      // col.alphaBlend(w.invert(),pannel.darkest(black)).save("../out/"+n+"coloredPencil.png");
   }

   return 0;








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


   return 0;
}

