#ifndef MATBIN_H
#define MATBIN_H

namespace cv
{
	class Mat;
}
// Standard C Lib
#include <string>

class CNNBSSSerializeMat
{
public:
	CNNBSSSerializeMat() {};
	~CNNBSSSerializeMat() {};

	void SerializeMatBin(cv::Mat& mat, std::string filename);
	cv::Mat DeserializeMatBin(std::string filename);

};

#endif // ! MATBIN_H