/*****************************************************************************
*   Number Plate Recognition using SVM and Neural Networks
******************************************************************************
*   by David Millán Escrivá, 5th Dec 2012
*   http://blog.damiles.com
******************************************************************************
*   Ch5 of the book "Mastering OpenCV with Practical Computer Vision Projects"
*   Copyright Packt Publishing 2012.
*   http://www.packtpub.com/cool-projects-with-opencv/book
*****************************************************************************/


#ifndef DetectRegions_h
#define DetectRegions_h


#include <string.h>
#include <vector>

#include "img_Plate.hpp"

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

/* using namespace std; */
/* using namespace cv; */

class	DetectRegions
{
 public :

  bool		saveRegions;
  bool		showSteps;
  std::string	filename;

 public :

  DetectRegions();
  void		setFilename(const std::string& f);
  void		run(const cv::Mat& input, std::vector<img_Plate>& plates);
  void		run2(const cv::Mat& input, std::vector<img_Plate>& plates);

 private :

  void		UseSobel( const cv::Mat& input,
			  cv::Mat& img_threshold );

  void		UseCMYK( const cv::Mat& input,
			std::vector<cv::Mat>& imgs_threshold );

  void		part2( const cv::Mat& input,
		       std::vector<img_Plate>& output,
		       cv::Mat& img_threshold,
		       const std::string& out_id );

 private :

  bool		verifySizes(const cv::RotatedRect& mr);
  cv::Mat	histeq(const cv::Mat& in);
};


#endif

