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


#ifndef IMG_PLATE_HPP
#define IMG_PLATE_HPP


#include "Segmentation.hpp"


#include <string.h>
#include <vector>

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

/* using namespace std; */
/* using namespace cv; */

struct	img_Plate
{
 public :

  img_Plate();
  img_Plate(cv::Mat img, cv::Rect pos);

 public :

  // std::string str();

 public :

  cv::Rect	position;
  cv::Mat	plateImg;

  std::vector<t_seg_char>	segment_chars;

  // std::vector<char>	chars;
  // std::vector<cv::Rect>	charsPos;

};


#endif	// PLATE_HPP

