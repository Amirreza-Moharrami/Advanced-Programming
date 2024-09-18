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


//ImageProcessor-----------------------------------------------------------------------------------------------------------

class ImageProcessor {
public:
    ImageProcessor(const string& inputFilename, const string& outputFilename, ImageFilter* filter)
        : inputFilename(inputFilename), outputFilename(outputFilename), filter(filter) {}

    ~ImageProcessor() {
        delete filter;
    }

    void processImage() {
        Bmp bmp;
        Bmp result;

        readBmp(bmp);
        result=filter->applyFilter(bmp);
        writeBmp(result);        
    }

private:
    string inputFilename;
    string outputFilename;
    ImageFilter* filter;

    ReadResult readBmp(Bmp& bmp) {
        return read(bmp, inputFilename);
    }

    bool writeBmp(Bmp& bmp) {
        return write(bmp, outputFilename);
    }
};
//Filter Request------------------------------------------------------------------------------------------------------------------------------

class FilterRequest
{

public:
    FilterRequest(vector<string> _inputs) {inputsArguments=_inputs;}
    vector<string> get_inputs();
    vector<vector<string>> get_file_address();
    vector<int> extract_view_nums(string nums);
    ImageFilter* get_filter(string filterCode);
    vector<ImageFilter*> get_filters(vector<string> inputs);
    void generate_filtered_picture();


private:
    int argc;
    char* argv;
    vector<string> inputsArguments;
    string inputFilename;
    string outputFilename;
    ImageFilter* filter;
    ImageProcessor* processor;

};


vector<string> FilterRequest::get_inputs()
{
    return inputsArguments;
}

vector<int> FilterRequest::extract_view_nums(string nums)
{
    vector<int> numbers;
    
    stringstream ss(nums);
    string xx, yy, ww, hh;
    getline(ss, xx, ':');
    getline(ss, yy, ':');
    getline(ss, ww, ':');
    getline(ss, hh, ' ');

    numbers.push_back(stoi(xx));
    numbers.push_back(stoi(yy));
    numbers.push_back(stoi(ww));
    numbers.push_back(stoi(hh));

    return numbers;
}

ImageFilter* FilterRequest::get_filter(string filterCode)
{
    ImageFilter* filter;
    if(filterCode=="-B") {filter=new GaussianBlurFilter();}
    else if(filterCode=="-S") {filter=new SharpenFilter();}
    else if(filterCode=="-E") {filter=new EmbossFilter();}
    else if(filterCode=="-I") {filter=new InvertFilter();}
    else if(filterCode=="-G") {filter=new GrayScaleFilter();}


    return filter;
}

vector<ImageFilter*> FilterRequest::get_filters(vector<string> inputs)
{
    
    vector<ImageFilter*> filters;
    Bmp bmp;
    read(bmp,inputFilename);
    int j=0;

    for(int i=0; i< inputs.size(); i++)
    {
        if((inputs[i]=="-B")||(inputs[i]=="-S")||(inputs[i]=="-E")||(inputs[i]=="-I")||(inputs[i]=="-G"))
        {
            ImageFilter* filter=get_filter(inputs[i]); 
            filter->assign_view(0,0,bmp.infoHdr.width,bmp.infoHdr.height); 
            filters.push_back(filter);  
            j++;  
        }
        else
        {
            vector<int> nums=extract_view_nums(inputs[i]);
            filters[j-1]->assign_view(nums[0],nums[1],nums[2],nums[3]);
            filters[j-1]->isCreating=false;
        }

    }    
    return filters;
}

void FilterRequest::generate_filtered_picture()
{
    vector<string> inputs=get_inputs();
    string fileInput,fileOutput;
    char key;    

    do
    {
        cin >> fileInput >> fileOutput;
        inputFilename=fileInput;
        outputFilename=fileOutput;
    

        vector<ImageFilter*> filters=get_filters(inputs);

        for(int i=0; i< filters.size(); i++)
        {
            if(i>0)
            {
                inputFilename=fileOutput;
            }
            ImageFilter* filter=filters[i];
            ImageProcessor* processor = new ImageProcessor(inputFilename, outputFilename, filter);
            processor->processImage();
            delete processor;        
        }

        system("stty raw");
        key=getchar();
        system("stty cooked");        
    } while (key != 27);

    
}

//------------------------------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {
    


    string input;
    vector<string> inputs;
    char key;

    for(int i=1; i< argc; i++)
    {
        string input=argv[i];
        inputs.push_back(input);
    }
        

    FilterRequest filterRequest(inputs);

    filterRequest.generate_filtered_picture();

    return 0;
}