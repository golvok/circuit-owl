#include <text/text_finder.hpp>

using namespace cv;
using namespace std;

bool pair_compare(const TessResult& firstElem, const TessResult& secondElem) {
  return firstElem.height < secondElem.height;
}

void TextFinder::process(const cv::Mat& image)
{
    tesseract::TessBaseAPI tess;

	if (tess.Init(nullptr, "eng")) {
		throw runtime_error("could not init tesseract");
	}

    //----OpenCV to tesseract
    //cv::Mat gray;
    //cv::cvtColor(image, gray, CV_BGR2GRAY);
    // ...other image pre-processing here...    
    tess.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
    tess.SetImage((uchar*)image.data, image.cols, image.rows, 1, image.cols);

	//auto pix_up = std::unique_ptr<Pix>(pixRead(file.c_str()));
	tess.SetVariable("save_blob_choices", "T");
	// tess.SetVariable("tessedit_char_whitelist", "1234567890RVA"); //makes it worse somehow...
	//tess.SetImage(pix_up.get());
	tess.Recognize(NULL);

	tesseract::ResultIterator* ri = tess.GetIterator();
	// tesseract::PageIteratorLevel level = tesseract::RIL_WORD;
	tesseract::PageIteratorLevel level = tesseract::RIL_WORD;

	if (ri) {
	  do {
	    const char* word = ri->GetUTF8Text(level);
	    float conf = ri->Confidence(level);
	    int x1, y1, x2, y2;
	    ri->BoundingBox(level, &x1, &y1, &x2, &y2);
        
        std::string text(word);
        size_t char_index = 0;

        while(char_index < text.length())
        {
            if(text[char_index] < '0' || text[char_index] > '9')
            {
                break;
            }
            char_index++;
        }        
        
        int val = atoi(text.substr(0, char_index).c_str());
        text = text.substr(char_index, text.length() - char_index + 1);

        TessResult new_tess(cv::Point(x1,y1), cv::Point(x2,y2), val, text);
        words.push_back(new_tess);
	    printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
	           word, conf, x1, y1, x2, y2);
	    delete[] word;
	  } while (ri->Next(level));
	}
    
    sort(words.begin(), words.end(), pair_compare);
}

void TextFinder::save(std::string output_file)
{
    cv::Mat image = imread(output_file.c_str(), 1 );
    int heightThresh = words.front().height;

    printf("height thresh is %d\n", heightThresh);

    for(const TessResult& word : words)
    {
        if(word.height <= heightThresh * 2)
        {
            printf("printing word with height %d\n", word.height);
            /*
            rectangle(
                image,
                word.SW,
                word.NE,
                cv::Scalar(255, 255, 255),
                -1
            );*/
            
            cv::rectangle(
                image,
                word.SW,
                word.NE,
                cv::Scalar(255, 0, 0)
            );
    
            std::string output = std::to_string(word.val) + word.unit;

            putText(image, output, word.SW, 
    FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(255,0,0), 1, CV_AA);
        }
    }

    imwrite(output_file, image );
}
