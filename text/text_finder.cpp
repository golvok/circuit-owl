#include "text_finder.h"

using namespace cv;

bool pairCompare(const tess_result& firstElem, const tess_result& secondElem) {
  return firstElem.height < secondElem.height;
}

void text_finder::process()
{
    tesseract::TessBaseAPI tess;

	if (tess.Init(nullptr, "eng")) {
		throw runtime_error("could not init tesseract");
	}
    
    printf("opening file %s\n", file.c_str());

	auto pix_up = std::unique_ptr<Pix>(pixRead(file.c_str()));
	tess.SetVariable("save_blob_choices", "T");
	// tess.SetVariable("tessedit_char_whitelist", "1234567890RVA"); //makes it worse somehow...
	tess.SetImage(pix_up.get());
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
        tess_result new_tess(Point(x1,y1), Point(x2,y2), string(word));
        words.push_back(new_tess);
	    printf("word: '%s';  \tconf: %.2f; BoundingBox: %d,%d,%d,%d;\n",
	           word, conf, x1, y1, x2, y2);
	    delete[] word;
	  } while (ri->Next(level));
	}
    
    sort(words.begin(), words.end(), pairCompare);
}

void text_finder::save(string output_file)
{
    Mat image = imread(file.c_str(), 1 );
    int heightThresh = words.front().height * 2;

    printf("height thresh %d\n", heightThresh);

    for(const tess_result& word : words)
    {
        if(word.height <= heightThresh)
        {
            printf("printing word with height %d\n", word.height);
            rectangle(
                image,
                word.SW,
                word.NE,
                cv::Scalar(255, 255, 255),
                -1
            );
        }
    }

    imwrite(output_file, image );
}

