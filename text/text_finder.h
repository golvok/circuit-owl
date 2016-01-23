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
    tess_result(Point _SW, Point _NE, string _word) : SW(_SW), NE(_NE), word(_word), height(_NE.y-_SW.y) {};
    Point SW;
    Point NE;
    string word;
    int height;
};

class text_finder
{
    public:
    text_finder(string file_path) : file(file_path), words() {};
    
    void save();
    void process();
    const vector<tess_result>& get_words() { return words; }

    private:
    string file;
    vector<tess_result> words;
};
