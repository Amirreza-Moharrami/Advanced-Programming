#include "bmp.hpp"
#include "filter.hpp"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;



//ImageFilter-----------------------------------------------------------------------------------------------------------

void ImageFilter::assign_view(int x_, int y_, int w_, int h_) 
{
    view.x=x_;
    view.y=y_;
    view.w=w_;
    view.h=h_;
}

int ImageFilter::clamp(int value, int min, int max) 
{
    return std::max(min, std::min(value, max));
}

Bmp ImageFilter::applyFilter(Bmp& bmp) 
{
    width = bmp.infoHdr.width;
    height = bmp.infoHdr.height;

    Bmp result;
    if(isCreating)
    {
        create(result, width, height);
    }
    else
    {
        result=bmp;
    }
    
    for (int row = view.x; row < view.h; ++row) {
        for (int col = view.y; col < view.w; ++col) {
            applyPixelFilter(bmp, result, row, col);
        }
    }

    bmp = result;
    return result;
}

//Sharpen-----------------------------------------------------------------------------------------------------------


void SharpenFilter::applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) 
{
                
    int redSum = 0, grnSum = 0, bluSum = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            if (isValidPixel(input, newRow, newCol)) {
                Pixel pixel = getFilePixel(input, newRow, newCol);

                redSum += kernel[i + 1][j + 1] * pixel.red;
                grnSum += kernel[i + 1][j + 1] * pixel.grn;
                bluSum += kernel[i + 1][j + 1] * pixel.blu;
            }
        }
    }
    Pixel& outputPixel = getFilePixel(output, row, col);
    outputPixel.red = clamp(redSum, 0, 255);
    outputPixel.grn = clamp(grnSum, 0, 255);
    outputPixel.blu = clamp(bluSum, 0, 255);

    output.data[row][col].red=outputPixel.red;
    output.data[row][col].grn=outputPixel.grn;
    output.data[row][col].blu=outputPixel.blu;

}

bool SharpenFilter::isValidPixel(const Bmp& bmp, int row, int col) {
    return row >= 0 && row < bmp.infoHdr.height && col >= 0 && col < bmp.infoHdr.width;
}


//Emboss-----------------------------------------------------------------------------------------------------------


void EmbossFilter::applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) 
{

    int redSum = 0, grnSum = 0, bluSum = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            if (isValidPixel(input, newRow, newCol)) {
                Pixel pixel = getFilePixel(input, newRow, newCol);

                redSum += kernel[i + 1][j + 1] * pixel.red;
                grnSum += kernel[i + 1][j + 1] * pixel.grn;
                bluSum += kernel[i + 1][j + 1] * pixel.blu;
            }
        }
    }

    Pixel& outputPixel = getFilePixel(output, row, col);
    outputPixel.red = clamp(redSum, 0, 255);
    outputPixel.grn = clamp(grnSum, 0, 255);
    outputPixel.blu = clamp(bluSum, 0, 255);

    output.data[row][col].red=outputPixel.red;
    output.data[row][col].grn=outputPixel.grn;
    output.data[row][col].blu=outputPixel.blu;
}

bool EmbossFilter::isValidPixel(const Bmp& bmp, int row, int col) 
{
    return row >= 0 && row < bmp.infoHdr.height && col >= 0 && col < bmp.infoHdr.width;
}



//Guassian Blur-----------------------------------------------------------------------------------------------------------

void GaussianBlurFilter::applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) 
{

    int redSum = 0, grnSum = 0, bluSum = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newRow = row + i;
            int newCol = col + j;

            if (isValidPixel(input, newRow, newCol)) {
                Pixel pixel = getFilePixel(input, newRow, newCol);

                redSum += kernel[i + 1][j + 1] * pixel.red;
                grnSum += kernel[i + 1][j + 1] * pixel.grn;
                bluSum += kernel[i + 1][j + 1] * pixel.blu;
            }
        }
    }

    Pixel& outputPixel = getFilePixel(output, row, col);
    outputPixel.red = clamp(redSum / divisor, 0, 255);
    outputPixel.grn = clamp(grnSum / divisor, 0, 255);
    outputPixel.blu = clamp(bluSum / divisor, 0, 255);


    output.data[row][col].red=outputPixel.red;
    output.data[row][col].grn=outputPixel.grn;
    output.data[row][col].blu=outputPixel.blu;

}

bool GaussianBlurFilter::isValidPixel(const Bmp& bmp, int row, int col) 
{
    return row >= 0 && row < bmp.infoHdr.height && col >= 0 && col < bmp.infoHdr.width;
}

//Grayscale-----------------------------------------------------------------------------------------------------------


void GrayScaleFilter::applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) 
{

    int redSum = 0, grnSum = 0, bluSum = 0, pixelAverage=0;

    Pixel pixel = getFilePixel(input, row, col);

    pixelAverage=(pixel.red+pixel.grn+pixel.blu)/3;
    redSum = pixelAverage;
    grnSum = pixelAverage;
    bluSum = pixelAverage;


    Pixel& outputPixel = getFilePixel(output, row, col);
    outputPixel.red = clamp(redSum, 0, 255);
    outputPixel.grn = clamp(grnSum, 0, 255);
    outputPixel.blu = clamp(bluSum, 0, 255);

    output.data[row][col].red=outputPixel.red;
    output.data[row][col].grn=outputPixel.grn;
    output.data[row][col].blu=outputPixel.blu;

}

//Invert-----------------------------------------------------------------------------------------------------------


void InvertFilter::applyPixelFilter(const Bmp& input, Bmp& output, int row, int col) 
{

    int redSum = 0, grnSum = 0, bluSum = 0, pixelAverage=0;

    Pixel pixel = getFilePixel(input, row, col);

    redSum = 255-pixel.red;
    grnSum = 255-pixel.grn;
    bluSum = 255-pixel.blu;


    Pixel& outputPixel = getFilePixel(output, row, col);
    outputPixel.red = clamp(redSum, 0, 255);
    outputPixel.grn = clamp(grnSum, 0, 255);
    outputPixel.blu = clamp(bluSum, 0, 255);

    output.data[row][col].red=outputPixel.red;
    output.data[row][col].grn=outputPixel.grn;
    output.data[row][col].blu=outputPixel.blu;

}
