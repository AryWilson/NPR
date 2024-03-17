// Copyright 2021, Aline Normoyle, alinen

#ifndef AGL_IMAGE_H_
#define AGL_IMAGE_H_

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cmath>
#include <array>


namespace agl {

/**
 * @brief Holder for a RGB color
 * 
 */
struct Pixel {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};
inline bool operator<(const Pixel& lhs, const Pixel& rhs)
{
  return lhs.r < rhs.r;
}


float distance(Pixel p, Pixel po);

// /**
//  * @brief Holder for square kernel of unknow size
//  * 
//  */
// struct Kernel {
//     int size;
//     vector<int> k;
// };


/**
 * @brief Implements loading, modifying, and saving RGB images
 */
class Image {
 public:
  Image();
  Image(int width, int height);
  Image(const Image& orig);
  Image& operator=(const Image& orig);


  virtual ~Image();
  void clean();

  /** 
   * @brief Load the given filename 
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally when loaded
   * 
   * @verbinclude sprites.cpp
   */
  bool load(const std::string& filename, bool flip = false);

  /** 
   * @brief Save the image to the given filename (.png)
   * @param filename The file to load, relative to the running directory
   * @param flip Whether the file should flipped vertally before being saved
   */
  bool save(const std::string& filename, bool flip = true) const;

  /** @brief Return the image width in pixels
   */
  int width() const;

  /** @brief Return the image height in pixels
   */
  int height() const;

  /** 
   * @brief Return the RGB data
   *
   * Data will have size width * height * 4 (RGB)
   */
  char* data() const;

  /**
   * @brief Replace image RGB data
   * @param width The new image width
   * @param height The new image height
   *
   * This call will replace the old data with the new data. Data should 
   * match the size width * height * 3
   */
  void set(int width, int height, unsigned char* data);

  /**
   * @brief Get the pixel at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   */ 
  Pixel get(int row, int col) const;

  /**
   * @brief Set the pixel RGBA color at index (row, col)
   * @param row The row (value between 0 and height)
   * @param col The col (value between 0 and width)
   *
   * Pixel colors are unsigned char, e.g. in range 0 to 255
   */ 
  void set(int row, int col, const Pixel& color);

  /**
 * @brief Set the pixel RGB color at index i
 * @param i The index (value between 0 and width * height)
 *
 * Pixel colors are unsigned char, e.g. in range 0 to 255
 */
  Pixel get(int i) const;

  /**
 * @brief Set the pixel RGB color at index i
 * @param i The index (value between 0 and width * height)
 *
 * Pixel colors are unsigned char, e.g. in range 0 to 255
 */
  void set(int i, const Pixel& c);


  // resize the image
  Image resize(int width, int height) const;

  // flip around the horizontal midline
  Image flipHorizontal() const;

  // flip around the vertical midline
  Image flipVertical() const;

  // rotate the Image 90 degrees
  Image rotate90() const;

  // Return a sub-Image having the given top,left coordinate and (width, height)
  Image subimage(int x, int y, int w, int h) const;

  

  // Replace the portion starting at (row, col) with the given image
  // Clamps the image if it doesn't fit on this image
  void replace(const Image& image, int x, int y);

  // expects brush image (virt) is virtically aligned
  // applies the given brush white background, black ink to the image
  // virt is brush image
  // startx and starty are the starting indexes (where in the image to start drawing) 
  // xym is the tensor pixel, dirrection of least change (guides rotation)
  // col is the color of the brush (usually the color of that pixel)
  void brush(Image virt, int startx, int starty, Pixel xym, Pixel col);
  Image rotate(Pixel xym);

  // swirl the colors 
  Image swirl() const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel + other.pixel
  // Assumes that the two images are the same size
  Image add(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel - other.pixel
  // Assumes that the two images are the same size
  Image subtract(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = this.pixel * other.pixel
  // Assumes that the two images are the same size
  Image multiply(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = abs(this.pixel - other.pixel)
  // Assumes that the two images are the same size
  Image difference(const Image& other) const;

  // Apply a givent texture to mid tone 
  // testing 
  Image crosshatch(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = max(this.pixel, other.pixel)
  // Assumes that the two images are the same size
  Image lightest(const Image& other) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    result.pixel = min(this.pixel, other.pixel)
  // Assumes that the two images are the same size
  Image darkest(const Image& other) const;

  // only display the specified rgb color of the image
  // default returns grayscale
  Image monochrome(int opt) const;

  // makes the image more vibrant by the specified ammount
  Image saturate(unsigned char ammount) const;

  //give edge-aligned and blurred tensor of an image
  // cahnge tempurature based on eigen vector
  Image tempurature(unsigned char ammount,unsigned char cutoff, Image tensor) const;

  // makes the darkest values darker and the lightest values lighter
  Image contrast(unsigned char ammount, unsigned char cutoff) const;

  // applies red-teal filter
  Image redTeal(unsigned char ammount) const;

  // returns the average va;ue [0,255] of a given pixel
  // optional greyscale flag
  // unsigned char average(struct Pixel rgb, bool greyscale = false);

  // Sobel Edge Detector
  Image sobel()const; 

  // Sobel Edge Detector and vector flow build from tensor
  // edge align boolean 
  Image tensor(bool edge_aligned)const; 

  // vector flow build from tensor (not working)
  Image vfc()const; 


  // Image vectorgraph()const;  

  // box blur
  Image blur() const;

  // color quant based on resize(), count is pallet size
  Image quantization(unsigned char count) const;

  // Gaussian blur
  Image gaussian(float sigma) const;
  //here we want edge aligned = true on tensor
  Image dirrected_gaussian(float sigma,Image tensor) const;
  //here we want edge aligned = false on tensor
  Image dirrected_gaussian_1(float sigma,Image tensor) const;


  // subtract given image from image, scaled by tau
  Image dog(const Image& other, float tau) const;


  // threshold fn
  Image threshold(float k, float phi) const;

  //gaussian vingette
  Image vingette() const;


  // Apply gamma correction
  Image gammaCorrect(float gamma) const;

  // Apply the following calculation to the pixels in 
  // our image and the given image:
  //    this.pixels = this.pixels * (1-alpha) + other.pixel * alpha
  // Assumes that the two images are the same size
  Image alphaBlend(const Image& other, float amount) const;

  // if mask pixel is white, other will show up, if black, input image
  Image alphaBlend(const Image& other, const Image& other_mask) const;


  // Convert the image to grayscale
  Image invert() const;

  // Convert the image to grayscale
  Image grayscale() const;

  Image toTile(const Image& tile) const;
  Image toTile_test(const Image& tile,const Image& tensor) const;

  // return a bitmap version of this image
  Image colorJitter(int size) const;

  // return a bitmap version of this image
  Image bitmap(int size) const;

  // Fill this image with a color
  void fill(const Pixel& c);

  // very rough color quantization options 0-5 for color range
  Image cquant(unsigned char i) const;

  // kernel color randomization
  Image crand() const;
  // color randomization +=
  Image rnoise(unsigned char max) const;

  // return summary statistics
  // std::vector<float> sumarize();
  std::array<int, 255> sumarize();
  Image normalize();

  // for stroke-based sort tensor by magnitude
  // expects brush image is virtically aligned
  // depends on brush fn
  // cutoff is how many pixels are brushed (0-every pixel, 255-none)
  // base - for color selection/tensor generation, 
  // *this is background image
  Image paint(const Image& fbrush, unsigned char cutoff, float weight);
  Image mask();


 private:
  int w,h,ch = 0;
  struct Pixel * _data = nullptr;
  bool stb_load = false;

};
}  // namespace agl
#endif  // AGL_IMAGE_H_
