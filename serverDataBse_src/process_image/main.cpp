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

// Main entry code OpenCV

// #include <cv.h>
// #include <highgui.h>
// #include <cvaux.h>
// #include <ml.h>

// #include <iostream>
// #include <vector>


// #include "DetectRegions.hpp"

// #include "Segmentation.hpp"



#include <QApplication>

#include "QtImageWindow.hpp"


int	main(int argc, char** argv)
{
  QApplication	app(argc, argv);

  QtImageWindow	fenetre;
  fenetre.show();

  return app.exec();
}

