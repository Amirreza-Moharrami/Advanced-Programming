#include "bmp.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

struct View
{
    int x=0;
    int y=0;
    int w=350;
    int h=350;
};

//ImageFilter-----------------------------------------------------------------------------------------------------------

class ImageFilter {
protected:
    View view;
    virtual void applyPixelFilter(const Bmp& input, Bmp& output, int row, int col)=0;
    int clamp(int value, int min, int max);

public:
    bool isCreating=true;
    int width;
    int height;
    Bmp applyFilter(Bmp& bmp);
    void assign_view(int x_, int y_, int w_, int h_);

};

//Sharpen-----------------------------------------------------------------------------------------------------------

class SharpenFilter : public ImageFilter {
public:

private:

    int kernel[3][3] = 
    {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    void applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) override;

    bool isValidPixel(const Bmp& bmp, int row, int col);

};

//Emboss-----------------------------------------------------------------------------------------------------------

class EmbossFilter : public ImageFilter {
public:

private:

    int kernel[3][3] = {
        {-2, -1, 0},
        {-1, 1, 1},
        {0, 1, 2}
    };

    void applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) override;

    bool isValidPixel(const Bmp& bmp, int row, int col);

};

//Guassian Blur-----------------------------------------------------------------------------------------------------------

class GaussianBlurFilter : public ImageFilter {
public:

private:

    int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };

    int divisor = 16;

    void applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) override;
    
    bool isValidPixel(const Bmp& bmp, int row, int col);
};

//Grayscale-----------------------------------------------------------------------------------------------------------

class GrayScaleFilter : public ImageFilter {
public:

private:

    void applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) override;
};

//Invert-----------------------------------------------------------------------------------------------------------

class InvertFilter : public ImageFilter {
public:

private:
   
    void applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) override;
};