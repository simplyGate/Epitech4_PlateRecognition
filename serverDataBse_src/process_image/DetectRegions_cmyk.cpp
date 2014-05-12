/**
 * ocr.cpp:
 * Read the digits from a scratchcard. See the tutorial at
 * http://opencv-code.com/tutorials/how-to-read-the-digits-from-a-scratchcard
 *
 * Compile with:
 * g++ -I/usr/local/include -L/usr/local/lib ocr.cpp -o ocr \
 *       -lopencv_core -lopencv_imgproc -lopencv_highgui
 */


#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <tesseract/baseapi.h>

#include "DetectRegions.hpp"

#include <iostream>
#include <sstream>

// Covert RGB to CMYK using the formula from
// http://rapidtables.com/convert/color/rgb-to-cmyk.htm
void rgb2cmyk(const cv::Mat& src, std::vector<cv::Mat>& cmyk)
{
  CV_Assert(src.type() == CV_8UC3);

  cmyk.clear();
  for (int i = 0; i < 4; ++i)
    cmyk.push_back(cv::Mat(src.size(), CV_32F));

  for (int i = 0; i < src.rows; ++i)
    for (int j = 0; j < src.cols; ++j)
      {
	cv::Vec3b p = src.at<cv::Vec3b>(i,j);

	float r = p[2] / 255.;
	float g = p[1] / 255.;
	float b = p[0] / 255.;
	float k = (1 - std::max(std::max(r,g),b));

	cmyk[0].at<float>(i,j) = (1 - r - k) / (1 - k);
	cmyk[1].at<float>(i,j) = (1 - g - k) / (1 - k);
	cmyk[2].at<float>(i,j) = (1 - b - k) / (1 - k);
	cmyk[3].at<float>(i,j) = k;
      }

}

// int main(int ac, char **av)
// {
//   cv::Mat im0 = cv::imread(av[1]);
//   if (!im0.data)
//     return -1;

//   cv::imwrite("./out/input.png", im0);

//   std::vector<cv::Mat> cmyk;
//   rgb2cmyk(im0, cmyk);

//   cv::Mat im1;
//   // im1 = cmyk[1] > 0.25;
//   im1 = cmyk[1].mul(1 - cmyk[2]) > 0.25;

//   // Use CvBlobs after

//   std::string	letters[4] = {
//     "C", "M", "Y", "K"
//   };
//   // cv::imwrite("./out/channel_C.png", cmyk[0]);
//   // cv::imwrite("./out/channel_M.png", cmyk[1]);
//   // cv::imwrite("./out/channel_Y.png", cmyk[2]);
//   // cv::imwrite("./out/channel_K.png", cmyk[3]);


//   for (unsigned int i = 0; i < cmyk.size(); ++i)
//     {
//       cv::Mat im1;
//       // im1 = cmyk[1] > 0.25;
//       im1 = cmyk[i].mul(1 - cmyk[ (i + 1) % cmyk.size() ]) > 0.25;

//       cv::Mat im2;
//       im1.convertTo(im2, CV_8U);
//       //cv::imshow("image2.jpg", im2);

//       // cv::imwrite("./out/image2.png", im2);

//       std::stringstream	sstr;
//       sstr << "./out/image" << i << "_" << letters[i] << ".png";
//       cv::imwrite( sstr.str().c_str(), im2 );



//       // std::vector<std::vector<cv::Point> > contours;
//       // cv::findContours(im2, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

//       // double max_area = 0;
//       // int max_idx = 0;
//       // for (int j = 0; j < contours.size(); j++)
//       // 	{
//       // 	  double area = cv::contourArea(contours[j]);
//       // 	  max_idx  = area > max_area ? j : max_idx;
//       // 	  max_area = area > max_area ? area : max_area;
//       // 	}

//       // im2.setTo(cv::Scalar(0));
//       // cv::drawContours(im2, contours, max_idx, cv::Scalar(255), -1);

//       // // cv::imshow("image2Countour.jpg", im2);


//       // {
//       // 	std::stringstream	sstr;
//       // 	sstr << "./out/image" << i << "_" << letters[i] << "_image2Countour.png";
//       // 	cv::imwrite( sstr.str().c_str(), im2 );
//       // }


//       // cv::Mat im3;
//       // cv::cvtColor(im0, im3, CV_BGR2GRAY);
//       // im3 = ((255 - im3) & im2) > 200;
//       // cv::imshow("image3.jpg", im3);


//       // {
//       // 	std::stringstream	sstr;
//       // 	sstr << "./out/image" << i << "_" << letters[i] << "_image3.png";
//       // 	cv::imwrite( sstr.str().c_str(), im3 );
//       // }


//       // cv::Mat dst = im3.clone();
//       // cv::findContours(dst.clone(), contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
//       // for (int j = 0; j < contours.size(); j++)
//       // 	{
//       // 	  if (cv::contourArea(contours[j]) < 100)
//       // 	    cv::drawContours(dst, contours, j, cv::Scalar(0), -1);
//       // 	}


//       // {
//       // 	std::stringstream	sstr;
//       // 	sstr << "./out/image" << i << "_" << letters[i] << "_src.png";
//       // 	cv::imwrite( sstr.str().c_str(), im0 );
//       // }

//       // {
//       // 	std::stringstream	sstr;
//       // 	sstr << "./out/image" << i << "_" << letters[i] << "_dst.png";
//       // 	cv::imwrite( sstr.str().c_str(), dst );
//       // }


//       // cv::imshow("source.jpg", im0);
//       // cv::imshow("final.jpg", dst);

//   // cv::waitKey(0);


//     }

//   return 0;
// }


void	DetectRegions::UseCMYK( const cv::Mat& input,
				std::vector<cv::Mat>& imgs_threshold )
{

  const cv::Mat&	im0 = input;

  if (!im0.data)
    return;

  // cv::imwrite("./out/input.png", im0);

  std::vector<cv::Mat> cmyk;
  rgb2cmyk(im0, cmyk);

  cv::Mat im1;
  // im1 = cmyk[1] > 0.25;
  im1 = cmyk[1].mul(1 - cmyk[2]) > 0.25;

  // // Use CvBlobs after
  // std::string	letters[4] = {
  //   "C", "M", "Y", "K"
  // };

  imgs_threshold.reserve(4);

  for (unsigned int i = 0; i < cmyk.size(); ++i)
    {
      cv::Mat im1;
      // im1 = cmyk[1] > 0.25;
      im1 = cmyk[i].mul(1 - cmyk[ (i + 1) % cmyk.size() ]) > 0.25;

      cv::Mat im2;
      im1.convertTo(im2, CV_8U);

      imgs_threshold.push_back(im2);

      // cv::imwrite("./out/image2.png", im2);

      // std::stringstream	sstr;
      // sstr << "./out/image" << i << "_" << letters[i] << ".png";
      // cv::imwrite( sstr.str().c_str(), im2 );

    }

}
