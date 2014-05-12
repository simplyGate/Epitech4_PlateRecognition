


#ifndef	SEGMENTATION_HPP
#define	SEGMENTATION_HPP


#include <cv.h>
// #include <highgui.h>
// #include <cvaux.h>


struct		t_seg_char
{
public :

  cv::Mat	s_mat;
  cv::Point2f	s_center;

public :

  t_seg_char()
  {}

  t_seg_char( cv::Mat& mat,
	      cv::Point2f& center )
    : s_mat(mat),
      s_center(center)
  {
  }

};


// void	Segment( cv::Mat& input, std::vector<t_seg_char>& output );
void	Segment( cv::Mat& input, std::vector<t_seg_char>& output, float ratio_limit, int out_id );


#endif

