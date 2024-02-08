// Copyright 2021, Aline Normoyle, alinen

#include "image.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include <vector>
#include <cmath>

namespace agl {

void Image::clean(){
   w = 0;
   h = 0;
   ch = 3;
   if (_data != nullptr){
      if (stb_load){
         stbi_image_free(_data);
         _data = nullptr;
         stb_load = false;
      }
      if (_data != nullptr){
         delete[] _data;
         _data = nullptr;
      }
   }


}

Image::Image() {
   // default constructor
   w = 0;
   h = 0;
   ch = 3;
   _data = new struct Pixel[w*h];
}

Image::Image(int width, int height)  {
   w = width;
   h = height;
   ch = 3;
   _data = new struct Pixel[w*h];
   // general constructor

}

Image::Image(const Image& orig){
   // copy constructor
   w = orig.width();
   h = orig.height();
   ch = orig.ch;
   _data = new struct Pixel[w*h];
   memcpy(_data, orig._data, w*h*sizeof(struct Pixel));

  
}

Image& Image::operator=(const Image& orig) {
   // assignment operator
  if (&orig == this) {
   return *this;
  }
  if (_data != nullptr){
   clean();
  }

   w = orig.width();
   h = orig.height();
   ch = orig.ch;
   _data = new struct Pixel[w*h];
   memcpy(_data, orig._data, w*h*sizeof(struct Pixel));
   
   return *this;
}

Image::~Image() {
   // destructor
   clean();
}

int Image::width() const { 
   return w;
}

int Image::height() const {
   return h;
}

char* Image::data() const {
   return (char *) _data;
}

void Image::set(int width, int height, unsigned char* data) { 
   if (_data != nullptr){
      clean();
   }
   w = width;
   h = height;
   // _data = (char*) data;
   _data = new struct Pixel[w*h];
   memcpy(_data, data, w*h*sizeof(struct Pixel));



}

bool Image::load(const std::string& filename, bool flip) {
   stbi_set_flip_vertically_on_load(flip);
   int width,height,channels = 0;
   unsigned char *pic = stbi_load(filename.c_str() , &width, &height, &channels, 3);

   
   if (pic != nullptr){
      if (_data != nullptr){
         clean();
      }
      w = width;
      h = height;
      ch = channels;

      _data = (struct Pixel *) pic;
      stb_load = true;
      return true;
   }
   return false;
}


bool Image::save(const std::string& filename, bool flip) const {
   stbi_set_flip_vertically_on_load(flip);
   if(_data != nullptr){ 
      return stbi_write_png((filename.c_str()) , width(), height(), ch, data(), width() * sizeof(struct Pixel)); //valgrind
   }
   return false;
}

Pixel Image::get(int row, int col) const {
   if((row >= 0) && (col >= 0) && (row < height()) && (col < width()) && (_data != nullptr)){
      return _data[row*width() + col];
   }
   return Pixel{ 255, 0, 0 };
}

void Image::set(int row, int col, const Pixel& color) {
   //check for valid inputs
   if((row >= 0) && (col >= 0) && (row < height()) && (col < width()) && (_data != nullptr)){

      _data[row*width() + col] = color;
   }
}

Pixel Image::get(int i) const
{
   if( i >= 0 && i < w*h  && (_data != nullptr)){
      return _data[i];
   }
   return Pixel{ 0, 0, 0 };
}

void Image::set(int i, const Pixel& c)
{
   if( i >= 0 && i < w*h && (_data != nullptr)){
      _data[i] = c;
   }
}

/// //////////////////
Image Image::resize(int width, int height) const {
   Image result(width, height);
   int _i,_j;
   for(int i = 0; i < height; i++){
      for(int j = 0; j<width; j++){

         _i = floor((i/(height-1.0f))*(h-1.0f));
         _j = floor((j/(width-1.0f))*(w-1.0f));
         if((_i < h) && (_j < w) && (0 <= _i) && (0 <= _j)){
            result.set(i,j,get(_i,_j));
         } else{
            result.set(i,j,Pixel{0,255,0});
         }
      }

   }
   return result;
}

Image Image::flipHorizontal() const {
   Image result(w, h);
   for(int i = 0; i < round(h/2.0f + 0.5f) ; i++){
      for(int j = 0; j < w; j++){
         struct Pixel swap = get((w-1)-i,j);
         result.set((w-1)-i,j, get(i,j));
         result.set(i,j,swap);
      }
   }

   return result;

}

Image Image::flipVertical() const {
   Image result(0, 0);
   return result;
}

Image Image::rotate90() const {
   Image result(h, w);
   Pixel rgb; 

   for(int i = 0; i < h; i++){
      for(int j = 0; j<w; j++){
         rgb = get(i,j);

         result.set(j,i,rgb);
      }
   }
   return result;
}

Image Image::subimage(int startx, int starty, int width, int height) const {
   Image sub(width, height);
   for(int _i = startx, i = 0; _i < startx+sub.height(); _i++, i++){
      for(int _j = starty, j = 0; _j < starty+sub.width(); _j++, j++){
         if ((_i < h) && (_j < w) && (_i >= 0) && (_j >= 0)){
            sub.set(i,j,get(_i,_j));
         } else {
            sub.set(i,j,Pixel{0,0,0});
         }
      }
   }
   return sub;
}

void Image::replace(const Image& image, int startx, int starty) {
   int width = image.width();
   int height = image.height();
   if(_data == nullptr || image.data() == nullptr){return;}

   for(int _i = startx, i = 0; _i < h && i < height; _i++, i++){
      for(int _j = starty, j = 0; _j < w && j < width; _j++, j++){
         set(_i,_j,image.get(i,j));
      }
   }
  
}

Image Image::swirl() const {
   Image result(w, h);
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         unsigned char r = from.b;
         unsigned char g = from.r;
         unsigned char b = from.g;
         result.set(i,j,Pixel{r,g,b});
      }
   }
   return result;
}


Image Image::subtract(const Image& other) const {
   
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   Image o(w,h);
   o = other.resize(w,h);
   int red;
   int green;
   int blue;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         red = (get(i,j).r) - (o.get(i,j).r);
         green = (get(i,j).g) - (o.get(i,j).g);
         blue = (get(i,j).b) - (o.get(i,j).b);
         // if (red > 255){red = 255;}
         // if (green > 255){green = 255;}
         // if (blue > 255){blue = 255;}
         if (red < 0){red = 0;}
         if (green < 0){green = 0;}
         if (blue < 0){blue = 0;}
         toSet.r = red;
         toSet.g = green; 
         toSet.b = blue;
         result.set(i,j,toSet);
      }
   }
   return result;
}

Image Image::multiply(const Image& other) const {
   Image o(w,h);
   o = other.resize(w,h);
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   int red;
   int green;
   int blue;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         red = (get(i,j).r) * (o.get(i,j).r);
         green = (get(i,j).g) * (o.get(i,j).g);
         blue = (get(i,j).b) * (o.get(i,j).b);
         if (red > 255){red = 255;}
         if (green > 255){green = 255;}
         if (blue > 255){blue = 255;}
         // if (red < 0){red = 0;}
         // if (green < 0){green = 0;}
         // if (blue < 0){blue = 0;}
         toSet.r = red;
         toSet.g = green; 
         toSet.b = blue;
         result.set(i,j,toSet);
      }
   }
   return result;
}

Image Image::difference(const Image& other) const {
   Image o(w,h);
   o = other.resize(w,h);
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   int red;
   int green;
   int blue;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         red = (get(i,j).r) + (o.get(i,j).r);
         green = (get(i,j).g) + (o.get(i,j).g);
         blue = (get(i,j).b) + (o.get(i,j).b);
         if (red > 255){red = 255;}
         if (green > 255){green = 255;}
         if (blue > 255){blue = 255;}
         // if (red < 0){red = 0;}
         // if (green < 0){green = 0;}
         // if (blue < 0){blue = 0;}
         toSet.r = red;
         toSet.g = green; 
         toSet.b = blue;
         result.set(i,j,toSet);
      }
   }
   return result;
}


Image Image::crosshatch(const Image& other) const {
   Image a(w,h);
   Image b(w,h);
   Image c(w,h);
   a = toTile(other);
   b = toTile(other.rotate90());
   b = a.subtract(b);
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel light = a.get(i,j);
         struct Pixel med = b.get(i,j);
         Pixel rgb = get(i,j);
         if (rgb.r<50){result.set(i,j,med);}
         else if (rgb.r<130){result.set(i,j,light);}
         else {result.set(i,j,Pixel{255,255,255});}
      }
   }
   return result;
}

Image Image::add(const Image& other) const {
   
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   Image o(w,h);
   o = other.resize(w,h);
   int red;
   int green;
   int blue;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         red = (get(i,j).r) + (o.get(i,j).r);
         green = (get(i,j).g) + (o.get(i,j).g);
         blue = (get(i,j).b) + (o.get(i,j).b);
         if (red > 255){red = 255;}
         if (green > 255){green = 255;}
         if (blue > 255){blue = 255;}
         // if (red < 0){red = 0;}
         // if (green < 0){green = 0;}
         // if (blue < 0){blue = 0;}
         toSet.r = red;
         toSet.g = green; 
         toSet.b = blue;
         result.set(i,j,toSet);
      }
   }
   return result;
}

//flip
//rotate 90
//rotate 45?

Image Image::lightest(const Image& other) const {
   Image o(w,h);
   o = other.resize(w,h);
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   int red;
   int green;
   int blue;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         if ((get(i,j).r) > (o.get(i,j).r)){red = get(i,j).r;} else {red = o.get(i,j).r;}
         if ((get(i,j).g) > (o.get(i,j).g)){green = get(i,j).g;} else {green = o.get(i,j).g;}
         if ((get(i,j).b) > (o.get(i,j).b)){blue = get(i,j).b;} else {blue = o.get(i,j).b;}
         toSet.r = red;
         toSet.g = green; 
         toSet.b = blue;
         result.set(i,j,toSet);
      }
   }
   return result;
}

Image Image::darkest(const Image& other) const {
   Image o(w,h);
   o = other.resize(w,h);
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   int red;
   int green;
   int blue;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         if ((get(i,j).r) < (o.get(i,j).r)){red = get(i,j).r;} else {red = o.get(i,j).r;}
         if ((get(i,j).g) < (o.get(i,j).g)){green = get(i,j).g;} else {green = o.get(i,j).g;}
         if ((get(i,j).b) < (o.get(i,j).b)){blue = get(i,j).b;} else {blue = o.get(i,j).b;}
         toSet.r = red;
         toSet.g = green; 
         toSet.b = blue;
         result.set(i,j,toSet);
      }
   }
   return result;
}

Image Image::monochrome(int opt) const {
   int red,green,blue = 0;
   switch(opt){
    case 0: // red
         red = 1;
         green = 0;
         blue = 0;
         break;
    case 1:
         green = 1;
         red = 0;
         blue = 0;
         break;
    case 2:
         blue = 1;
         green = 0;
         red = 0;
         break;
   case 3: // yellow
         red = 1;
         green = 1;
         blue = 0;
         break;
    case 4: // magenta
         red = 1;
         green = 0;
         blue = 1;
         break;
    case 5: // teal
         blue = 1;
         green = 1;
         red = 0;
         break;
    default:
         return grayscale();
}

   Image result(w, h);
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         unsigned char r = (from.r)*(red);
         unsigned char g = (from.g)*(green);
         unsigned char b = (from.b)*(blue);
         result.set(i,j,Pixel{r,g,b});
      }
   }
   return result;
}

Image Image::saturate(unsigned char ammount) const{
   Image result(w, h);
   unsigned char a = ammount;
   unsigned char r,g,b = 0;
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         if(from.r >= from.g && from.r>= from.g){
            (from.r < 255-2*a)? (r = from.r + 2*a) : (r = 255);
            (from.g-a > 0)? (g = from.g - a) : (g = 0);
            (from.b-a > 0)? (b = from.b - a) : (b = 0);
         } else if(from.g >= from.r && from.g>= from.b){
            (from.g < 255-2*a)? (g = from.g + 2*a) : (g = 255);
            (from.r-a > 0)? (r = from.r - a) : (r = 0);
            (from.b-a > 0)? (b = from.b - a) : (b = 0);

         } else if(from.b >= from.g && from.b>= from.r){
            (from.b < 255-2*a)? (b = from.b + 2*a) : (b = 255);
            (from.r-a > 0)? (r = from.r - a) : (r = 0);
            (from.g-a > 0)? (g = from.g - a) : (g = 0);
         }
         result.set(i,j,Pixel{r,g,b});
      }
   }
   return result;

}

Image Image::contrast(unsigned char ammount, unsigned char cutoff) const{
   Image result(w, h);
   unsigned char a = ammount;
   unsigned char r,g,b = 0;
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         int sum = from.r + from.g + from.b;
         if(sum>(cutoff*3)){
            (from.r + a < 255)? (r = from.r + a) : (r = 255);
            (from.g + a < 255)? (g = from.g + a) : (g = 255);
            (from.b + a < 255)? (b = from.b + a) : (b = 255);
            
         } else {
            (from.r - a > 0)? (r = from.r - a) : (r = 0);
            (from.g - a > 0)? (g = from.g - a) : (g = 0);
            (from.b - a > 0)? (b = from.b - a) : (b = 0);
         }
         result.set(i,j,Pixel{r,g,b});
      }
   }
   return result;

}


Image Image::redTeal(unsigned char ammount) const{
   Image result(w, h);
   unsigned char a = ammount;
   unsigned char r,g,b = 0;
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         if(from.r > from.b){
            (from.r + 2*a < 255)? (r = from.r + a) : (r = 255);
            (from.g - a > 0)? (g = from.g - a) : (g = 0);
            b = from.b;
            
         } else {
            (from.r - a > 0)? (r = from.r - a) : (r = 0);
            (from.b + 2*a < 255)? (b = from.b + 2*a) : (b = 255);
            (from.g + a < 255)? (g = from.g + a) : (g = 255);
            // g = from.g; 
         }
         result.set(i,j,Pixel{r,g,b});
      }
   }
   return result;
   
}

  // returns the average value [0,255] of a given pixel
  // optional greyscale flag
unsigned char average(struct Pixel rgb, bool greyscale = false){
   if(greyscale){
      return (rgb.r)*.11 + (rgb.g)*.59 + (rgb.b)*.11;
   } else {
      return (rgb.r)/3 + (rgb.g)/3 + (rgb.b)/3;
   }

}
Image Image::sobel() const {
   Image result(w,h);
   int Gx[9] = {-1,0,1,-2,0,2,-1,0,1};
   int Gy[9] = {-1,-2,-1,0,0,0,1,2,1};

   unsigned char pix;
   int sumx, sumy;
   double sum;
   for(int i = 0; i < h; i++){
      for(int j = 0; j<w; j++){ 
         pix = 0;
         sumx = 0;
         sumy = 0;
         for(int x = i-1, idx = 0; x<=i+1; x++, idx++){
            for(int y = j-1, idy = 0; y<=j+1; y++, idy++){
               if(x < h && y < w && 0<=x && 0<=y){
                  sumx += (Gx[idx*3 + idy])*((int) average(get(x,y)));
                  sumy += (Gy[idx*3 + idy])*((int) average(get(x,y)));
               } else {
                  sumx += (Gx[idx*3 + idy])*((int) average(get(i,j)));
                  sumy += (Gy[idx*3 + idy])*((int) average(get(i,j)));
               }
            }
         }
         sum = sqrt(pow(sumx,2)+pow(sumy,2));
         if(sum>255){
            pix = 255;
         } else if(sum<0){
            pix = 0;
         } else{
            pix = sum;
         }
         // red = sum % 255;
         result.set(i,j,Pixel{pix,pix,pix});
      }
   }
   return result;
}


Image Image::blur() const {
   Image result(w,h);
   // int G[9] = {1,1,1,1,1,1,1,1,1};

   struct Pixel pix;
   float sumr, sumg, sumb;
   for(int i = 0; i < h; i++){
      for(int j = 0; j<w; j++){ 
         pix = {0,0,0};
         sumr = 0;
         sumg = 0;
         sumb = 0;
         for(int x = i-1, idx = 0; x<=i+1; x++, idx++){
            for(int y = j-1, idy = 0; y<=j+1; y++, idy++){
               if(x < h && y<w && 0<=x && 0<=y){
                  sumr += get(x,y).r;
                  sumg += get(x,y).g;
                  sumb += get(x,y).b;
               } else {
                  sumr += get(i,j).r;
                  sumg += get(i,j).g;
                  sumb += get(i,j).b;
               }
            }
         }
         sumr = sumr/9.0f;
         sumg = sumg/9.0f;
         sumb = sumb/9.0f;
         if(sumr>255){
            pix.r = 255;
         } else if(sumr<0){
            pix.r = 0;
         } else{
            pix.r = sumr;
         }

         if(sumg>255){
            pix.g = 255;
         } else if(sumg<0){
            pix.g = 0;
         } else{
            pix.g = sumg;
         }

         if(sumb>255){
            pix.b = 255;
         } else if(sumb<0){
            pix.b = 0;
         } else{
            pix.b = sumb;
         }
         // red = sum % 255;
         result.set(i,j,pix);
      }
   }
   return result;
}


//gaussian vingette
Image Image::vingette() const {
   Image result(w,h);
      std::vector<float> k;
      int kernel_size = std::min(w,h);
      float sigma = kernel_size/6.0f;
      float frac = (2*M_PI*pow(sigma,2));
      int half = floor(kernel_size/2);

      for (int i = -1*half; i <= half; i++) {
         for (int j = -1*half; j <= half; j++) {
            float val = (pow(M_E,-1*(pow(i,2)+pow(j,2))/(2*pow(sigma,2))));
            k.push_back(val);
         }
      }
      // loop through image
      for(int i = 0; i < h; i++){
         for(int j = 0; j < w; j++){ 
            // if we are not in kernel, don't evaluate just set to black
            // if ((w>h)?(j<kernel_size/2 || j>kernel_size*1.5): (i<kernel_size/2 || i>kernel_size*1.5)){
            //    result.set(i,j,Pixel{0,0,0});
            // }
            Pixel from = get(i,j);
            //move kernel index half of the longer side + half the kernel size
               
            int index = (((float)(i)/(float)(h))*kernel_size)*kernel_size + ((float)j/(float)w)*kernel_size;
            float f = k[index];
            Pixel to = Pixel{ceil(from.r * f),ceil(from.g * f),ceil(from.b * f)};
            result.set(i,j,to);
         }
      }
      
return result;
}

float distance(Pixel p, Pixel po) {
  // d = √ [(x2 – x1)2 + (y2 – y1)2 + (z2 – z1)2]
  int r = p.r-po.r;
  int g = p.g-po.g;
  int b = p.b-po.b;
  float d = pow(r,2) + pow(g,2) + pow(b,2);
  return sqrt(d);
}

Image Image::quantization(unsigned char count) const{
   
   Image result(w,h);
   if(_data == nullptr ){return result;}
   float scale = sqrt(((float)count)/(float)(h*w));
   unsigned char height = round(scale*h);
   unsigned char width = round(scale*w);
 
   Image pallet = Image(width, height);
   std::set<Pixel> vec_pal;
   vec_pal.insert(Pixel{0,0,0});
   vec_pal.insert(Pixel{255,255,255});
   
   for(int i = 0; i < pallet.height(); i++){
      for(int j = 0; j < pallet.width(); j++){ 
         vec_pal.insert(pallet.get(i,j));
      }
   }

   // std::vector<Pixel> set_pal(vec_pal.begin(), vec_pal.end());
   // 3 sets, rotated values
   // r = rgb
   // g = grb
   for (Pixel c : vec_pal){
      Pixel set = Pixel{c.g,c.r,c.b};
      // set.push_back(set)
   }
   // b = brg
   for (Pixel c : vec_pal){
      Pixel set = Pixel{c.b,c.g,c.r};
      // set.push_back(set)
   }

            // float d = distance(Pixel{c.g,c.r,c.b}, col);
            // float d = distance(Pixel{c.b,c.g,c.g}, col);

   // make set
   // copy into 3 vectors sorted respectivly by rgb
   // for each pixel in image
   // closest r 
   // closest g
   // closest b

   // the size of the dot product
   // sorted by distance to origin
   // in dist find vec of pt to origin 
   //reassign colors to original image
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){ 
         int min = 500;
         Pixel col = get(i,j);
         Pixel to_col = Pixel{255,0,0};
         // beter dist fn for speed
         // set sorted by distance 3 dif cat?
         // turn set into vector sorted by r,g,b (6 comparisons per loop)
         for (Pixel c : vec_pal){
            float d = distance(c, col);
            if (d<min){
               min = d;
               to_col = c;
            }
         }
         result.set(i,j,to_col);
      }
   }
   return result;
   
}


Image Image::tensor() const {
   Image result(w,h);

   // sobel kernel
   int Gx[9] = {-1,0,1,-2,0,2,-1,0,1};
   int Gy[9] = {-1,-2,-1,0,0,0,1,2,1};

   unsigned char pixx,pixy;
   int sumx, sumy;
   // for each pixel apply sobel kernel
   for(int i = 0; i < h; i++){
      for(int j = 0; j<w; j++){ 
         sumx = 0;
         sumy = 0;
         // apply sobel kernel
         for(int x = i-1, idx = 0; x<=i+1; x++, idx++){
            for(int y = j-1, idy = 0; y<=j+1; y++, idy++){
               if(x < h && y < w && 0<=x && 0<=y){
                  sumx += (Gx[idx*3 + idy]/4.0)*((int) average(get(x,y)));
                  sumy += (Gy[idx*3 + idy]/4.0)*((int) average(get(x,y)));
               } else {
                  sumx += (Gx[idx*3 + idy]/4.0)*((int) average(get(i,j)));
                  sumy += (Gy[idx*3 + idy]/4.0)*((int) average(get(i,j)));
               }
            }
         }

         // construct data
         // structure tensor
         int ST[4] = {sumx*sumx, sumx*sumy, sumx*sumy, sumy*sumy};
         // eigen values
         float e1 = (ST[0] + ST[4] + sqrt(pow(ST[0]-ST[4],2)+4*pow(ST[1],2)))/2;
         float e2 = (ST[0] + ST[4] - sqrt(pow(ST[0]-ST[4],2)+4*pow(ST[1],2)))/2;
         // eigen vectors (should be dirrection of least change)
         // first eigen vector
         float t1[2] = {e1-ST[0],-1*ST[1]};
         // // second eigen vector (only need 1)
         // float t2[2] = {-1*ST[1],e2-ST[3]}; 

         // map unbound vector to 255
         float mag = sqrt(pow(t1[0],2) + pow(t1[1],2));
         t1[0] = (((t1[0]/mag) + 1)/2.0)*255;
         t1[1] = (((t1[1]/mag) + 1)/2.0)*255;
         result.set(i,j,Pixel{t1[0], t1[1], mag});

         // result.set(i,j,(t1[1]<0)?Pixel{t1[1],0,0}:Pixel{0,t1[1],0});
         // // green and red tensor flow
         // sumy>255 ? pixy=255 : (sumy<0 ? pixy=0 : (pixy=sumy));
         // sumx>255 ? pixx=255 : (sumx<0 ? pixx=0 : (pixx=sumx));
         // result.set(i,j,Pixel{sumx,sumy,0});
      }
   }
   
   return result;
}


Image Image::vfc() const{
   Image result(w,h);
   for(int i = 0; i < h; i++){
      for(int j = 0; j<w; j++){ 

         // structure tensor
         Pixel pix = get(i,j);
         int sumx = pix.r;
         int sumy = pix.g;
         int ST[4] = {sumx*sumx, sumx*sumy, sumx*sumy, sumy*sumy};
         // eigen values
         float e1 = (ST[0] + ST[4] + sqrt(pow(ST[0]-ST[4],2)+4*pow(ST[1],2)))/2;
         float e2 = (ST[0] + ST[4] - sqrt(pow(ST[0]-ST[4],2)+4*pow(ST[1],2)))/2;
         // eigen vectors (should be dirrection of least change)
         // first eigen vector
         float t1[2] = {e1-ST[0],-1*ST[1]};
         // // second eigen vector (only need 1)
         // float t2[2] = {-1*ST[1],e2-ST[3]}; 

         // map unbound vector to 255?
         // t1[1] can be negative, t1[0] won't be, use blue to indicate (white=negative, black = positive)
         result.set(i,j,Pixel{t1[0], std::abs(t1[1]), (t1[1]<0)?255:0});

         // result.set(i,j,(t1[1]<0)?Pixel{t1[1],0,0}:Pixel{0,t1[1],0});
      }
   }
   return result;

}


  Image Image::gaussian(float sigma) const{

   //build gaussian kernel
      std::vector<float> k;
      float frac = (2*M_PI*pow(sigma,2));
      int size = ceil(sigma*6);
      if (size <3){size=3;} else if (size>20){size =20;}
      int half = floor(size/2);

      for (int i = -1*half; i <= half; i++) {
         for (int j = -1*half; j <= half; j++) {
            float val = (pow(M_E,-1*(pow(i,2)+pow(j,2))/(2*pow(sigma,2))));
            k.push_back(val);
         }
      }


      Image result(w, h);


      // kernel ops
      struct Pixel pix;
      float sumr, sumg, sumb;
      for(int i = 0; i < h; i++){
         for(int j = 0; j<w; j++){ 
            pix = {0,0,0};
            sumr = 0;
            sumg = 0;
            sumb = 0;
            for(int x = i-half, idx = 0; x<=i+half; x++, idx++){
               for(int y = j-half, idy = 0; y<=j+half; y++, idy++){
                  float temp = k[idx*size + idy];
                  if(x < h && y<w && 0<=x && 0<=y){ ////here hey ary here HEY 9:50
                     sumr += (get(x,y).r)*temp;
                     sumg += (get(x,y).g)*temp;
                     sumb += (get(x,y).b)*temp;
                  } else {
                     sumr += (get(i,j).r)*temp;
                     sumg += (get(i,j).g)*temp;
                     sumb += (get(i,j).b)*temp;
                  }
               }
            }
            sumr = sumr/frac;
            sumg = sumg/frac;
            sumb = sumb/frac;
            if(sumr>255){
               pix.r = 255;
            } else if(sumr<0){
               pix.r = 0;
            } else{
               pix.r = sumr;
            }

            if(sumg>255){
               pix.g = 255;
            } else if(sumg<0){
               pix.g = 0;
            } else{
               pix.g = sumg;
            }

            if(sumb>255){
               pix.b = 255;
            } else if(sumb<0){
               pix.b = 0;
            } else{
               pix.b = sumb;
            }
            result.set(i,j,pix);
         }
      }

      return result;

  }

  Image Image::dirrected_gaussian(float sigma,Image tensor) const{
   //assumes tensor was build from image

   //build gaussian kernel
      std::vector<float> k;
      float frac = (sqrt(2*M_PI)*sigma);
      int size = ceil(sigma*6);
      if (size <3){size=3;} else if (size>20){size =20;}
      int half = floor(size/2);

      // construct 1d gaussian vector
      for (int i = -1*half; i <= half; i++) {
         float val = (pow(M_E,-1*(pow(i,2))/(2*pow(sigma,2))));
         k.push_back(val);

      }

      Image result(w, h);

      // apply the gaussian vector along the axis of least change (from flow vector chart)
      // use 'tensor' to calculate the pixels within kernel range to the left and right of the center pixel to scale by respective values in the gaussian vector
      struct Pixel pix;
      float sumr, sumg, sumb;
      // for each pixel in the image
      for(int i = 0; i < h; i++){
         for(int j = 0; j<w; j++){ 
            // start with the center color
            float f = (k[half]);
            Pixel c = get(i,j);
            sumr = f*c.r;
            sumg = f*c.g;
            sumb = f*c.b;

            // positive half
            Pixel vec = tensor.get(i,j);
            // get the y component of the first eigen vector
            float vec_x = vec.r/127.5 -1 ; // TBD: depending on construction i think this may have a detrimentally smaller range [0,255] vs [-255,255] of y... 
            // go to location within kernel specified by vector so loop through gaussian vector not kernel
            float vec_y = vec.g/127.5 -1;
            int col_x = i;
            int col_y = j;
            for(int x = half; x<size; x++){
               // get the coordinates of the pixel pointed to by the eigen vec
               col_x += vec_x; // move right 1 or 0
               col_y +=  vec_y; // move up/down from eigen vec

               // check bound of pixel asking for
               col_x = (col_x>=0 & col_x<width())?col_x:i;
               col_y = (col_y>=0 & col_y<height())?col_y:j;

               // get the color pointed to by the eigen vector
               c = get(col_x,col_y);
               f = (k[x]);
               // update gaussian sum
               sumr += f*c.r;
               sumg += f*c.g;
               sumb += f*c.b;

               // update vector
               vec = tensor.get(col_x,col_y);
               vec_x = vec.r/127.5 -1;
               vec_y = vec.g/127.5 -1;
            }

            // negative half
            col_x = i;
            col_y = j;
            for(int x = half; x>=0; x--){
               // get the coordinates of the pixel pointed to by the negative eigen vec
               col_x -= vec_x; 
               col_y -=  vec_y; // move up/down from eigen vec

               // check bound of pixel asking for
               col_x = (col_x>=0 & col_x<width())?col_x:i;
               col_y = (col_y>=0 & col_y<height())?col_y:j;

               // get the color pointed to by the eigen vector
               c = get(col_x,col_y);
               f = (k[x]);
               // update gaussian sum
               sumr += f*c.r;
               sumg += f*c.g;
               sumb += f*c.b;

               // update vector, get the vector at the new coord for next calculation
               vec = tensor.get(col_x,col_y);
               vec_x = vec.r/127.5 -1;
               vec_y = vec.g/127.5 -1;
            }
 
            sumr = sumr/frac;
            sumg = sumg/frac;
            sumb = sumb/frac;
            sumr = sumr>255?255:sumr;
            sumg = sumg>255?255:sumg;
            sumb = sumb>255?255:sumb;
            result.set(i,j,Pixel{sumr,sumg,sumb});
         }
      }

      return result;

  }
   
Image Image::threshold(float k, float phi) const{
   Image result(w, h);
   Pixel rgb;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j<w ; j++){
         rgb = get(i,j);
         unsigned char inGrey = (rgb.r +rgb.g + rgb.b)/3;
         if (inGrey> k){
            result.set(i,j,Pixel{255,255,255});
         } else {
            unsigned char outGrey = 255*(1 + tanh(phi*(inGrey-k)));
            // unsigned char outGrey = 127.5*(1 + tanh(phi*(inGrey-k*0.9f)));
            result.set(i,j,Pixel{outGrey, outGrey, outGrey});
         }
      }
   }
   return result;
}


Image Image::dog(const Image& other, float tau) const {
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   float red;
   float green;
   float blue;
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         red = (1.0f+tau)*((float)get(i,j).r) - tau*((float)other.get(i,j).r);
         green = (1.0f+tau)*((float)get(i,j).g) - tau*((float)other.get(i,j).g);
         blue = (1.0f+tau)*((float)get(i,j).b) - tau*((float)other.get(i,j).b);
         if (red > 255){red = 255;}
         if (green > 255){green = 255;}
         if (blue > 255){blue = 255;}
         if (red < 0){red = 0;}
         if (green < 0){green = 0;}
         if (blue < 0){blue = 0;}
         toSet.r = red;
         toSet.g = green; 
         toSet.b = blue;
         result.set(i,j,toSet);
      }
   }
   return result;
}

Image Image::gammaCorrect(float gamma) const {

   Image result(w, h);
   for(int i = 0; i < h ; i++){
      for(int j = 0; j<w ; j++){
         Pixel temp = get(i,j);
         unsigned char r = round(pow(temp.r/255.0f,(1.0f/gamma))*255.0f);
         unsigned char g = round(pow(temp.g/255.0f,(1.0f/gamma))*255.0f);
         unsigned char b = round(pow(temp.b/255.0f,(1.0f/gamma))*255.0f);
         result.set(i,j,Pixel{r,g,b});
      }
   }
 
   return result;
}

Image Image::alphaBlend(const Image& other, float alpha) const {

   Image o(w,h);
   o = other.resize(w,h);
   Image result(w,h);
   if(_data == nullptr || other.data() == nullptr){return result;}
   for(int i = 0; i < h ; i++){
      for(int j = 0; j < w ; j++){
         struct Pixel toSet;
         toSet.r = (get(i,j).r)*(1-alpha) + (o.get(i,j).r)*(alpha);
         toSet.g = (get(i,j).g)*(1-alpha) + (o.get(i,j).g)*(alpha);
         toSet.b = (get(i,j).b)*(1-alpha) + (o.get(i,j).b)*(alpha);
         result.set(i,j,toSet);
      }
   }
   return result;
}

Image Image::invert() const {
   Image result(w, h);
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         unsigned char max = 255;
         unsigned char r = max - from.r;
         unsigned char g = max - from.g;
         unsigned char b = max - from.b;
         result.set(i,j,Pixel{r,g,b});
      }
   }
   return result;
}




Image Image::cquant() const {
   Image result(w, h);
   float d = 51;
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         unsigned char r = (((unsigned char)(((float)from.r)/d)))*d;
         unsigned char g = (((unsigned char)(((float)from.g)/d)))*d;
         unsigned char b = (((unsigned char)(((float)from.b)/d)))*d;

         result.set(i,j,Pixel{r,g,b});
      }
   }
   return result;
}


Image Image::crand() const {
   Image result(w, h);
   unsigned char margin = (unsigned char)((w+h)/200);
   if(margin < 2){margin=2;}
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         int i_ = i + rand() % margin - margin/2;
         if (i_ < 0){i_ = 0;}
         else if (i_ > h-1){i_ = h-1;}

         int j_ = j + rand() % margin - margin/2;
         if (j_ < 0){j_ = 0;}
         else if (j_ > w-1){j_ = w-1;}
         struct Pixel from = get(i_,j_);
         
         result.set(i,j,from);
      }
   }
   return result;
}

Image Image::grayscale() const {
   Image result(w, h);
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel rgb = get(i,j);
         unsigned char val = rgb.r*.11 + rgb.g*.59 + rgb.b*.11;
         result.set(i,j,Pixel{val, val, val});

      }
   }

   return result;
}

//tiles given image over the size of the image
Image Image::toTile(const Image& tile) const {
   Image result(w, h);
   result.fill(Pixel{255,255,255});
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = tile.get(i%tile.height(),j%tile.width());
         result.set(i,j,from);
      }
   }
   return result;
}

Image Image::colorJitter(int size) const {
   Image result(w, h);
   result.fill(Pixel{0,0,0});
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         struct Pixel from = get(i,j);
         struct Pixel to;
         int add;

         // green
         to = result.get(i,j);
         add = from.g + to.g;
         if (add>255){add=255;}
         struct Pixel center = Pixel{to.r,(unsigned char) add,to.b};
         result.set(i,j,center);
         
         // red
         to = result.get(i,j-size);
         add = from.r + to.r;
         if (add>255){add=255;}
         struct Pixel left = Pixel{(unsigned char) add,to.g,to.b};
         if ((int)j - size > 0) {result.set(i,j-size,left);}

         // blue
         to = result.get(i,j+size);
         add = from.b + to.b;
         if (add>255){add=255;}
         struct Pixel right = Pixel{to.r,to.g,(unsigned char) add};
         if ((int)j + size < result.w) {result.set(i,j+size,right);}
         
      }
   }

   return result;
}

Image Image::bitmap(int size) const {
   Image image(0, 0);
   
   return image;
}

void Image::fill(const Pixel& c) {
   for(int i = 0; i < h; i++){
      for(int j = 0; j < w; j++){
         set(i,j,c);
      }
   }
}



}  // namespace agl

