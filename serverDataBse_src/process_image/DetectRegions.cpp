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


#include "DetectRegions.hpp"

#include "Utils.hpp"


#include <iostream>
#include <sstream>

#include <cmath>


#define	D_IMG_SAVE( l_img, l_filename )	(void)0

// #define	D_IMG_SAVE( l_img, l_filename )		\
//   {						\
//   std::stringstream	sstr;			\
//   sstr << "./out_loc/" << l_filename;		\
// 						\
//   imwrite( sstr.str(), l_img );			\
// }


// bool	Collision(std::vector< cv::Point >& tab, const cv::Point& P);


void DetectRegions::setFilename(const std::string& s)
{
  filename = s;
}

DetectRegions::DetectRegions()
{
  showSteps = false;
  saveRegions = false;
}

bool DetectRegions::verifySizes(const cv::RotatedRect& mr)
{

  float error = 0.9;


  //Spain car plate size: 52x11 aspect 4,7272
  float aspect = 4.7272;

  //Set a min and max area. All other patchs are discarded
  int min = 15 * aspect * 15; // minimum area
  int max = 125 * aspect * 125; // maximum area


  //Get only patchs that match to a respect ratio.
  float rmin = aspect - aspect * error;
  float rmax = aspect + aspect * error;

  int area = mr.size.height * mr.size.width;
  float r = (float)mr.size.width / (float)mr.size.height;
  if (r < 1)
    r = (float)mr.size.height / (float)mr.size.width;

  if ( ( area < min || area > max ) ||
       ( r < rmin || r > rmax ) )
    return false;
  else
    return true;

}

cv::Mat	DetectRegions::histeq(const cv::Mat& in)
{
  cv::Mat	out(in.size(), in.type());

  if (in.channels() == 3)
    {
      cv::Mat		hsv;
      std::vector<cv::Mat>	hsvSplit;
      cvtColor(in, hsv, CV_BGR2HSV);
      split(hsv, hsvSplit);
      equalizeHist(hsvSplit[2], hsvSplit[2]);
      merge(hsvSplit, hsv);
      cvtColor(hsv, out, CV_HSV2BGR);
    }

  else if (in.channels() == 1)
    equalizeHist(in, out);

  return out;
}

void	DetectRegions::UseSobel(const cv::Mat& input, cv::Mat& img_threshold)
{
  D_IMG_SAVE( input, "00_img_input.png" );


  //convert image to gray
  cv::Mat img_gray;
  cvtColor(input, img_gray, CV_BGR2GRAY);
  cv::blur( img_gray, img_gray, cv::Size(5,5) );

  D_IMG_SAVE( img_gray, "01_img_Gray.png" );



  //Find vertical lines. Car plates have high density of vertical lines
  cv::Mat img_sobel;
  Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);

  D_IMG_SAVE( img_sobel, "02_img_Sobel.png" );


  //threshold image
  threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU + CV_THRESH_BINARY);

  D_IMG_SAVE( img_threshold, "03_img_Threshold.png" );


  //Morphplogic operation close
  cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3) );
  morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);

  D_IMG_SAVE( img_threshold, "04_img_Close.png" );

}

void	DetectRegions::part2( const cv::Mat& input,
			      std::vector<img_Plate>& output,
			      cv::Mat& img_threshold,
			      const std::string& out_id )
{

  cv::Mat	my_input;
  input.copyTo(my_input);

  //Find contours of possibles plates
  std::vector< std::vector< cv::Point> > contours;
  findContours( img_threshold,
  		contours, // a vector of contours
  		CV_RETR_EXTERNAL, // retrieve the external contours
  		// CV_CHAIN_APPROX_NONE ); // all pixels of each contours
		CV_CHAIN_APPROX_SIMPLE );


  //Start to iterate to each contour founded
  std::vector< std::vector<cv::Point> >::iterator itc = contours.begin();
  std::vector<cv::RotatedRect> rects;



  cv::Mat	my_input_rect;
  input.copyTo(my_input_rect);

  //Remove patch that are no inside limits of aspect ratio and area.
  while (itc != contours.end())
    {

      //Create bounding rect of object
      cv::RotatedRect	mr = minAreaRect(cv::Mat(*itc));

      if (!verifySizes(mr))
	{
	  itc = contours.erase(itc);

	  // rotated rectangle drawing
	  cv::Point2f	rect_points[4];
	  mr.points( rect_points );
	  for (int j = 0; j < 4; ++j)
	    line( my_input_rect, rect_points[j], rect_points[ (j + 1) % 4 ],
		  cv::Scalar(255,0,0), 1, 8 );

	}
      else
  	{
  	  ++itc;
  	  rects.push_back(mr);

	  // rotated rectangle drawing
	  cv::Point2f	rect_points[4];
	  mr.points( rect_points );
	  for (int j = 0; j < 4; ++j)
	    line( my_input_rect, rect_points[j], rect_points[ (j + 1) % 4 ],
		  cv::Scalar(0,255,0), 2, 8 );

  	}

    }


  D_IMG_SAVE( my_input_rect, "img_" << out_id << "Rect.png" );


  // Draw blue contours on a white image
  cv::Mat result;
  input.copyTo(result);
  cv::drawContours( result, contours,
		    -1, // draw all contours
		    cv::Scalar(255,0,0), // in blue
		    1 ); // with a thickness of 1
		    // 3 ); // with a thickness of 1

  D_IMG_SAVE( result, "04_img_" << out_id << "Contours.png" );


  // std::cerr << "rects.size : " << rects.size() << std::endl;

  std::vector<cv::Mat>	Mats;

  for (unsigned int i = 0; i < rects.size(); ++i)
    {


      //For better rect cropping for each posible box
      //Make floodfill algorithm because the plate has white background
      //And then we can retrieve more clearly the contour box
      circle(result, rects[i].center, 3, cv::Scalar(0,255,0), -1);

      //get the min size between width and height
      // float minSize = ( (rects[i].size.width < rects[i].size.height)
      float	minSize = ( (rects[i].size.width > rects[i].size.height)
			    ? (rects[i].size.width)
			    : (rects[i].size.height) );
      minSize = minSize - minSize * 0.5;

      //initialize rand and get 5 points around center for floodfill algorithm
      srand ( time(NULL) );

      //Initialize floodfill parameters and variables
      cv::Mat mask;
      mask.create(input.rows + 2, input.cols + 2, CV_8UC1);
      mask = cv::Scalar::all(0);
      int loDiff = 30;
      int upDiff = 30;
      int connectivity = 4;
      int newMaskVal = 255;
      // int NumSeeds = 100;
      cv::Rect ccomp;
      int flags = connectivity + (newMaskVal << 8) + CV_FLOODFILL_FIXED_RANGE + CV_FLOODFILL_MASK_ONLY;


      int	max_size = rects[i].size.width * rects[i].size.height;


      cv::Rect	b_rect = rects[i].boundingRect();

      int	min_x = b_rect.x;
      int	min_y = b_rect.y;

      int	max_x = min_x + b_rect.width;
      int	max_y = min_y + b_rect.height;

      for (int local_y = min_y; local_y < max_y; local_y += 5)
	for (int local_x = min_x; local_x < max_x; local_x += 5)
	  {
	    cv::Point	seed;

	    seed.x = local_x;
	    seed.y = local_y;

	    if (Collision( contours[i], seed ))
	      {
		cv::Mat	tmp_mask;
		tmp_mask.create( input.rows + 2, input.cols + 2, CV_8UC1 );
		tmp_mask = cv::Scalar::all(0);

		int	area = floodFill( input, tmp_mask, seed,
	  				  cv::Scalar(255,0,0), &ccomp,
	  				  cv::Scalar(loDiff, loDiff, loDiff),
	  				  cv::Scalar(upDiff, upDiff, upDiff), flags );



		{
		  cv::Point	c( ccomp.x + ccomp.width / 2,
			   ccomp.y + ccomp.height / 2 );

		  cv::Size	s( ccomp.width, ccomp.height );

		  cv::RotatedRect	tmp_rect( c, s, 0 );

		  // rotated rectangle drawing
		  cv::Point2f	rect_points[4];
		  tmp_rect.points( rect_points );
		  for (int j = 0; j < 4; ++j)
		    line( my_input, rect_points[j], rect_points[ (j + 1) % 4 ],
			  cv::Scalar(0,255,255), 1, 8 );
		}


		bool	rect_invalid = ( ccomp.x < min_x || ccomp.x > max_x ||
					 ccomp.y < min_y || ccomp.y > max_y );


		cv::Point	left_top( min_x, min_y );
		cv::Point	right_top( max_x, min_y );

		cv::Point	left_bottom( min_x, max_y );
		cv::Point	right_bottom( max_x, max_y );

		if (area > max_size)
		  {
		    circle( result, seed, 1, cv::Scalar(255,0,0), -1 );
		    circle( my_input, seed, 1, cv::Scalar(255,0,0), -1 );
		  }

		else if (rect_invalid)
		  {
		    circle( result, seed, 1, cv::Scalar(255,0,0), -1 );
		    circle( my_input, seed, 1, cv::Scalar(255,0,0), -1 );
		  }

		else
		  {
		    circle( result, seed, 1, cv::Scalar(0,255,0), -1 );
		    circle( my_input, seed, 1, cv::Scalar(0,255,0), -1 );

		    floodFill( input, mask, seed,
			       cv::Scalar(255,0,0), &ccomp,
			       cv::Scalar(loDiff, loDiff, loDiff),
			       cv::Scalar(upDiff, upDiff, upDiff), flags );
		  }

	      }
	    else
	      {
		circle( result, seed, 1, cv::Scalar(255,0,0), -1 );
		circle( my_input, seed, 1, cv::Scalar(255,0,0), -1 );
	      }

	  } // for (int j = 0; j < NumSeeds; ++j)


      {


	// rotated rectangle drawing
	cv::Point2f	rect_points[4];
	rects[i].points( rect_points );
	for (int j = 0; j < 4; ++j)
	  line( my_input, rect_points[j], rect_points[ (j + 1) % 4 ],
		cv::Scalar(255,255,255), 2, 8 );


	D_IMG_SAVE( mask, "img_" << out_id << "" << i << "_01_Mask.png" );

      }

      //cvWaitKey(0);

      //Check new floodfill mask match for a correct patch.
      //Get all points detected for get Minimal rotated Rect
      std::vector<cv::Point>	pointsInterest;
      cv::Mat_<uchar>::iterator	itMask = mask.begin<uchar>();
      cv::Mat_<uchar>::iterator	end = mask.end<uchar>();
      for (; itMask != end; ++itMask)
	if (*itMask == 255)
	  pointsInterest.push_back(itMask.pos());

      if (pointsInterest.size() < 2)
	continue;

      cv::RotatedRect	minRect = minAreaRect(pointsInterest);

      if (verifySizes(minRect))
	{

	  // rotated rectangle drawing
	  cv::Point2f rect_points[4]; minRect.points( rect_points );
	  for( int j = 0; j < 4; j++ )
	    line( result, rect_points[j], rect_points[(j+1)%4], cv::Scalar(0,0,255), 1, 8 );

	  //Get rotation matrix
	  float r = (float)minRect.size.width / (float)minRect.size.height;
	  float angle=minRect.angle;

	  if (r < 1)
	    angle = 90 + angle;

	  cv::Mat rotmat = getRotationMatrix2D(minRect.center, angle,1);

	  //Create and rotate image
	  cv::Mat img_rotated;
	  warpAffine(input, img_rotated, rotmat, input.size(), CV_INTER_CUBIC);

	  //Crop image
	  cv::Size rect_size = minRect.size;

	  if (r < 1)
	    std::swap( rect_size.width, rect_size.height );

	  cv::Mat img_crop;
	  getRectSubPix(img_rotated, rect_size, minRect.center, img_crop);


	  D_IMG_SAVE( img_crop, "img_" << out_id << "" << i << "_02_crop.png" );

	  cv::Mat	resultResized;
	  resultResized.create(33,144, CV_8UC3);
	  resize(img_crop, resultResized, resultResized.size(), 0, 0, cv::INTER_CUBIC);


	  D_IMG_SAVE( resultResized, "img_" << out_id << "" << i << "_03_resultResized.png" );

	  output.push_back( img_Plate( resultResized, minRect.boundingRect() ) );

	  // //Equalize croped image
	  // cv::Mat grayResult;
	  // cvtColor(resultResized, grayResult, CV_BGR2GRAY);
	  // // blur(grayResult, grayResult, Size(3,3));
	  // grayResult = histeq(grayResult);


	  // D_IMG_SAVE( grayResult, "img_" << out_id << "" << i << "_04_grayResult.png" );

	  // output.push_back( Plate( grayResult, minRect.boundingRect() ) );

	} // if (verifySizes(minRect))

    } // for (int i = 0; i < rects.size(); ++i)

  D_IMG_SAVE( result, "10_img_" << out_id << "Contours.png" );

  D_IMG_SAVE( my_input, "11_img_" << out_id << "my_input.png" );
}

void	DetectRegions::run(const cv::Mat& input, std::vector<img_Plate>& plates)
{

  cv::Mat	img_threshold;

  UseSobel( input, img_threshold );
  part2( input, plates, img_threshold, "_" );
}

void	DetectRegions::run2(const cv::Mat& input, std::vector<img_Plate>& plates)
{
  std::vector<cv::Mat>	imgs_threshold;

  UseCMYK( input, imgs_threshold );

  for (unsigned int i = 0; i < imgs_threshold.size(); ++i)
    {

      D_IMG_SAVE( imgs_threshold[i], "00_img_" << i << "_input.png" );


      //Morphplogic operation close
      cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3) );
      morphologyEx(imgs_threshold[i], imgs_threshold[i], CV_MOP_CLOSE, element);


      D_IMG_SAVE( imgs_threshold[i], "00_img_" << i << "_Close.png" );


      std::stringstream	sstr;
      sstr << i << "_";

      part2( input, plates, imgs_threshold[i], sstr.str() );

    }

}













