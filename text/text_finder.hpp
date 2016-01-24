#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <algorithm>
#include <iostream>
#include <memory>

struct TessResult
{
    TessResult(cv::Point _SW, cv::Point _NE, int _val, std::string _unit) 
    : SW(_SW), NE(_NE), val(_val), unit(_unit), height(_NE.y-_SW.y) {};

    cv::Point SW;
    cv::Point NE;
    int val;
    std::string unit;
    int height;
};

class TextFinder
{
    public:
        TextFinder() : words() {};
        
        void process(const cv::Mat& image);
        void save(std::string output_file);        
        const std::vector<TessResult>& get_words() { return words; }

    private:
        std::vector<TessResult> words;
};

