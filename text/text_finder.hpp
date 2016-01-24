#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <algorithm>
#include <iostream>
#include <memory>

using namespace std;
using namespace cv;

struct tess_result
{
    tess_result(Point _SW, Point _NE, int _val, string _unit) 
    : SW(_SW), NE(_NE), val(_val), unit(_unit), height(_NE.y-_SW.y) {};
    Point SW;
    Point NE;
    int val;
    string unit;
    int height;
};

class text_finder
{
    public:
    text_finder(string file_path) : file(file_path), words() {};
    
    void save(string output_file);
    void process();
    const vector<tess_result>& get_words() { return words; }

    private:
    string file;
    vector<tess_result> words;
};

