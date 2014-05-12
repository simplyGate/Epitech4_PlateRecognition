

// Cristóbal Carnero Liñán <grendel.ccl@gmail.com>


#include "Segmentation.hpp"

////
#include "OCR.hpp"
////

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

#include <cvblob.h>

#include <iostream>
#include <iomanip>



#define	D_IPL_IMG_SAVE( l_ipl_img, l_filename )	(void)0

// #define	D_IPL_IMG_SAVE( l_ipl_img, l_filename )		\
//   {							\
//     std::stringstream	tmp_sstr;			\
//     tmp_sstr << "./out_seg/" << l_filename;		\
// 							\
//     cvSaveImage( tmp_sstr.str().c_str(), (l_ipl_img));	\
//   }





//// TMP
namespace
{

  QImage	Mat2QImage(cv::Mat const& src)
  {
    // make the same cv::Mat
    cv::Mat temp;

    // cvtColor Makes a copt, that what i need
    cvtColor(src, temp, CV_BGR2RGB);

    QImage	dest( (uchar*)temp.data,
		      temp.cols, temp.rows, temp.step,
		      QImage::Format_RGB888 );

    QImage	dest2(dest);

    // enforce deep copy
    dest2.detach();

    return (dest2);
  }

  cv::Mat	QImage2Mat(QImage const& src)
  {
    cv::Mat tmp(src.height(),src.width(),CV_8UC3,(uchar*)src.bits(),src.bytesPerLine());
    cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
    cvtColor(tmp, result,CV_BGR2RGB);
    return result;
  }

};
//// /TMP



// void	Segment( cv::Mat& input, std::vector<t_seg_char>& output, int out_id )
void    Segment( cv::Mat& input, std::vector<t_seg_char>& output, float ratio_limit, int out_id )
{

  IplImage*		img = new IplImage( input );


  CvSize	imgSize = cvGetSize(img);

  IplImage*	frame = cvCreateImage( imgSize, img->depth, img->nChannels );

  IplConvKernel*	morphKernel = cvCreateStructuringElementEx( 5, 5, 1, 1,
								    CV_SHAPE_RECT, NULL );

  cvConvertScale(img, frame, 1, 0);

  IplImage*	segmentated = cvCreateImage(imgSize, 8, 1);


  // for (unsigned int j = 0; j < imgSize.height; j++)
  //   for (unsigned int i = 0; i < imgSize.width; i++)
  for (int j = 0; j < imgSize.height; j++)
    for (int i = 0; i < imgSize.width; i++)
      {
	CvScalar c = cvGet2D(frame, j, i);

	double b = ((double)c.val[0]) / 255.0;
	double g = ((double)c.val[1]) / 255.0;
	double r = ((double)c.val[2]) / 255.0;

	// unsigned char f = 255 * ( (r > g + 0.2) &&
	// 			      (r > b + 0.2) );

	// unsigned char f = 255 * ( (r > g) &&
	// 			      (r > b) );

	// unsigned char f = 255 * ( (b > g) &&
	// 			  (b > r) );


	float	error = 0.3;

	// unsigned char	f = 255 * ( r < 0.2 &&
	// unsigned char	f = 255 * ( r < 0.6 &&
	// unsigned char	f = 255 * ( r < 0.7 &&
	// unsigned char	f = 255 * ( r < 0.8 &&
				    // unsigned char	f = 255 * ( 
	unsigned char	f = 255 * ( r < ratio_limit &&

				    r > b - error && r < b + error &&
							 r > g - error && r < g + error &&

									      g > r - error && g < r + error &&
												   g > b - error && g < b + error &&

															b > r - error && b < r + error &&
																	     b > g - error && b < g + error );

	// unsigned char f = 255 * ( (r > 0.75f) &&
	// 			      (g > 0.75f) &&
	// 			      (b > 0.75f) );

	cvSet2D(segmentated, j, i, CV_RGB(f, f, f));
      }

  // cvMorphologyEx(segmentated, segmentated, NULL, morphKernel, CV_MOP_OPEN, 1);

  // cvShowImage("segmentated", segmentated);

  IplImage*	labelImg = cvCreateImage(cvGetSize(frame), IPL_DEPTH_LABEL, 1);

  cvb::CvBlobs blobs;
  // unsigned int result = cvLabel(segmentated, labelImg, blobs);
  cvLabel(segmentated, labelImg, blobs);

  // std::cout << "result = " << result << std::endl;

  // cvFilterByArea(blobs, 500, 1000000);
  cvFilterByArea(blobs, 50, 1000000);


  { // mask

    /**/

    // std::cout << "blobs.size = " << blobs.size() << std::endl;

    for ( cvb::CvBlobs::iterator itr = blobs.begin();
	  itr != blobs.end();
	  ++itr )
      {

	IplImage*	my_segmentated = cvCreateImage( imgSize, img->depth, img->nChannels );
	// IplImage*	my_segmentated = cvCreateImage( imgSize, 8, 1 );

	for (int j = 0; j < imgSize.height; j++)
	  for (int i = 0; i < imgSize.width; i++)
	    cvSet2D(my_segmentated, j, i, CV_RGB(0, 0, 0));

	// cvRenderBlobs( labelImg, blobs, frame, frame,
	// 	       CV_BLOB_RENDER_COLOR |
	// 	       CV_BLOB_RENDER_CENTROID |
	// 	       CV_BLOB_RENDER_BOUNDING_BOX |
	// 	       CV_BLOB_RENDER_ANGLE );

	cvRenderBlob( labelImg, itr->second,
		      my_segmentated,
		      my_segmentated,
		      CV_BLOB_RENDER_COLOR);

	{

	  // std::stringstream	sstr;
	  // sstr << "./out/"
	  //      << out_id << "_"
	  //      << "out_my_segmentated_"
	  //      << std::setw(5) << std::setfill('0')
	  //      << itr->first << ".jpg";

	  // cvSaveImage(sstr.str().c_str(), my_segmentated);

	  // D_IPL_IMG_SAVE( my_segmentated,
	  // 		  "./out/"
	  // 		  << out_id << "_"
	  // 		  << "out_my_segmentated_"
	  // 		  << std::setw(5) << std::setfill('0')
	  // 		  << itr->first << ".jpg" );


	  D_IPL_IMG_SAVE( my_segmentated,
			  "seg_"
			  << std::setw(2) << std::setfill('0') << out_id
			  << "_my_segmentated_"
			  << std::setw(2) << std::setfill('0') << itr->first
			  << ".png" );


	}

	cvReleaseImage(&my_segmentated);

      }

    //*/

  } // /mask



  { // save

    // float	margin_w = 1.0f;
    // float	margin_h = 1.0f;

    float	margin_w = 2.0f;
    float	margin_h = 1.0f;


    // std::cerr << "begin" << std::endl;

    for ( cvb::CvBlobs::iterator itr = blobs.begin();
	  itr != blobs.end();
	  ++itr )
      {

	// std::cerr << "blob : " << itr->first << std::endl;

	/// choper image
	/// epuration
	/// check marge horizontale / verticale
	/// adjust marge if needed then repeat

	{ // prototype

	  cv::Size2f	tmp_size( itr->second->maxx - itr->second->minx + margin_w,
				  itr->second->maxy - itr->second->miny + margin_h );

	  cv::Point2f	tmp_center( itr->second->minx + tmp_size.width / 2 - margin_w / 2,
				    itr->second->miny + tmp_size.height / 2 - margin_h / 2 );

	  cv::Mat	img_crop(frame, true);
	  getRectSubPix( img_crop,
			 tmp_size,
			 tmp_center,
			 img_crop );

	  QImage	QImg( Mat2QImage(img_crop) );

	  OCR::epure_picture( QImg );



	  float	min_x = 0;
	  float	min_y = 0;
	  float	max_x = QImg.width() - 1;
	  float	max_y = QImg.height() - 1;

	  for (int y = 0; y < QImg.height(); ++y)
	    {
	      int x = 0;
	      for (; x < QImg.width(); ++x)
		if (((QColor)(QImg.pixel(x, y))).red() == 0)
		  break;

	      if (x == QImg.width())
		++min_y;
	      else
		break;
	    }

	  for (int y = QImg.height() - 1; y >= 0; --y)
	    {
	      int x = 0;
	      for (; x < QImg.width(); ++x)
		if (((QColor)(QImg.pixel(x, y))).red() == 0)
		  break;

	      if (x == QImg.width())
		--max_y;
	      else
		break;
	    }

	  for (int x = 0; x < QImg.width(); ++x)
	    {
	      int y = 0;
	      for (; y < QImg.height(); ++y)
		if (((QColor)(QImg.pixel(x, y))).red() == 0)
		  break;

	      if (y == QImg.height())
		++min_x;
	      else
		break;
	    }

	  for (int x = QImg.width() - 1; x >= 0; --x)
	    {
	      int y = 0;
	      for (; y < QImg.height(); ++y)
		if (((QColor)(QImg.pixel(x, y))).red() == 0)
		  break;

	      if (y == QImg.height())
		--max_x;
	      else
		break;
	    }

	  cv::Size2f	new_size( max_x - min_x + 1,
				  max_y - min_y + 1 );

	  if ( new_size.width > 3 &&
	       new_size.height > 3 )
	    {

	      QImg = QImg.copy( min_x, min_y,
				(int)new_size.width,
				(int)new_size.height );

	      cv::Mat	final_crop = QImage2Mat(QImg);

	      resize( final_crop, final_crop, cv::Size(20, 40), 0, 0, cv::INTER_NEAREST);


	      output.push_back( t_seg_char(final_crop, tmp_center) );

	    }

	} // /prototype


	/// OLD

	/**

	do
	  {

	    cv::Size2f	tmp_size( itr->second->maxx - itr->second->minx + margin_w,
				  itr->second->maxy - itr->second->miny + margin_h );

	    if ( tmp_size.width < 2 ||
		 tmp_size.height < 2 )
	      break;

	    cv::Point2f	tmp_center( itr->second->minx + tmp_size.width / 2 - margin_w / 2,
				    itr->second->miny + tmp_size.height / 2 - margin_h / 2 );


	    std::cerr << "tmp_center : " << tmp_center.x << " / " << tmp_center.y << std::endl;
	    std::cerr << "tmp_size   : " << tmp_size.width << " / " << tmp_size.height << std::endl;

	    cv::Mat	img_crop(frame, true);
	    getRectSubPix( img_crop,
			   tmp_size,
			   tmp_center,
			   img_crop );


	    resize( img_crop, img_crop, cv::Size(20, 40), 0, 0, cv::INTER_NEAREST);


	    QImage	QImg( Mat2QImage(img_crop) );

	    OCR::epure_picture( QImg );

	    bool	top_has_black = false;
	    bool	bottom_has_black = false;

	    {
	      for (int x = 0; x < QImg.width(); ++x)
		{
		  QColor	in_color_top = QImg.pixel( x, 0 );
		  QColor	in_color_bottom = QImg.pixel( x, QImg.height() - 1 );

		  if (in_color_top.red() == 0)
		    top_has_black = true;

		  if (in_color_bottom.red() == 0)
		    bottom_has_black = true;

		}

	      if (!top_has_black || !bottom_has_black)
		margin_h -= 0.5f;
	    }



	    bool	left_has_black = false;
	    bool	right_has_black = false;

	    {
	      for (int y = 0; y < QImg.height(); y++)
		{
		  QColor	in_color_left = QImg.pixel( 0, y );
		  QColor	in_color_right = QImg.pixel( QImg.width() - 1, y );

		  if (in_color_left.red() == 0)
		    left_has_black = true;

		  if (in_color_right.red() == 0)
		    right_has_black = true;

		}

	      if (!left_has_black || !right_has_black)
		margin_w -= 0.5f;
	    }



	    if ( top_has_black && bottom_has_black &&
		 left_has_black && right_has_black )
	      {
		output.push_back( t_seg_char(img_crop, tmp_center) );
		break;
	      }

	  }
	while (true);

	//*/

	/// /OLD


      }

    // std::cerr << "end" << std::endl;


    cvRenderBlobs(labelImg, blobs, frame, frame, CV_BLOB_RENDER_BOUNDING_BOX);

    // cvSaveImage("./out/out_frame.jpg", frame);
    // cvSaveImage("./out/out_segmentated.jpg", segmentated);


    D_IPL_IMG_SAVE( frame,
		    "seg_"
		    << std::setw(2) << std::setfill('0') << out_id
		    << "_frame.png" );

    D_IPL_IMG_SAVE( segmentated,
		    "seg_"
		    << std::setw(2) << std::setfill('0') << out_id
		    << "_segmentated.png" );


  } // /save

  { // sort

    std::vector<t_seg_char>	tmp_output;

    while (!output.empty())
      {
	std::vector<t_seg_char>::iterator	it_best = output.begin();

	for ( std::vector<t_seg_char>::iterator it = output.begin();
	      it != output.end();
	      ++it )
	  if ( it_best->s_center.x > it->s_center.x )
	    it_best = it;

	tmp_output.push_back( *it_best );
	output.erase( it_best );
      }

    output.swap( tmp_output );

  } // /sort


  cvReleaseImage(&labelImg);
  cvReleaseImage(&segmentated);

  cvReleaseStructuringElement(&morphKernel);
  cvReleaseImage(&frame);

}

