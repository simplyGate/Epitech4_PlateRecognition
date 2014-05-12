

#include "QtImageWindow.hpp"

#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>

#include <QStringList>
#include <QFileDialog>

#include <iostream>
#include <sstream>



#include "DetectRegions.hpp"

#include "Segmentation.hpp"

#include "../ORM/ORM.hpp"





#define	D_IMG_SAVE( l_img, l_filename )	(void)0

// #define	D_IMG_SAVE( l_img, l_filename )		\
//   {						\
//     std::stringstream	sstr;			\
//     sstr << "./my_images/" << l_filename;	\
// 						\
//     imwrite( sstr.str(), l_img );		\
//   }



QtImageWindow*	QtImageWindow::m_pInstance = NULL;

namespace
{

  std::string	l_getFilename(const std::string& s)
  {
    char	sep = '/';
    char	sepExt='.';

#ifdef _WIN32
    sep = '\\';
#endif

    std::cerr << "l_getFilename : " << s << std::endl;

    size_t i = s.rfind(sep, s.length( ));

    // if (i != std::string::npos)
    {
      std::string fn(s);

      // if (i != std::string::npos)
      // 	fn = (s.substr(i+1, s.length( ) - i));

      size_t j = fn.rfind(sepExt, fn.length( ));

      if (j != std::string::npos)
	return fn.substr(0,j);
      else
	return fn;
    }
    // else
    //   return "";
  }


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
    // cvtColor(tmp, result, CV_BGR2RGB);
    cvtColor(tmp, result, CV_RGB2BGR);
    return result;
  }


};


QtImageWindow::QtImageWindow(QWidget *parent)
  : QWidget(parent),
    m_pGridLayout_output(NULL)
{

  m_pInstance = this;

  move(30, 30);
  setMinimumSize(300, 300);


  // m_OCR.init_dico_picture();


  {
    m_pGridLayout = new QGridLayout;

    {
      QPushButton*	pPButton_open = new QPushButton("Open");
      connect( pPButton_open, SIGNAL(clicked()), this, SLOT(OpenFile()) );

      m_pGridLayout->addWidget( pPButton_open, 0, 0 );
    }

    {
      m_pPButton_launch = new QPushButton("Launch");
      connect( m_pPButton_launch, SIGNAL(clicked()), this, SLOT(ProcessImage()) );
      m_pPButton_launch->setEnabled(false);

      m_pGridLayout->addWidget( m_pPButton_launch, 0, 1 );
    }

    {
      QPushButton*	pPButton_debug = new QPushButton("Debug");
      connect( pPButton_debug, SIGNAL(clicked()), this, SLOT(Debug()) );

      m_pGridLayout->addWidget( pPButton_debug, 0, 2 );

      pPButton_debug->hide();
    }

    //////////////////////////////////////////////////////////////////////

    {
      m_pLabel_filname = new QLabel;
      m_pLabel_filname->setText( "empty filename" );

      m_pGridLayout->addWidget( m_pLabel_filname, 1, 0, 1, 2 );
    }

    //////////////////////////////////////////////////////////////////////

    {
      m_pQSlider_seg_value = new QSlider(Qt::Horizontal);
      m_pQSlider_seg_value->setRange(0, 255);
      m_pQSlider_seg_value->setValue(128);

      connect( m_pQSlider_seg_value, SIGNAL(valueChanged(int)),
	       this, SLOT(UpdateSegValue(int)) );

      m_pGridLayout->addWidget( m_pQSlider_seg_value, 2, 0 );
    }

    {
      m_pLabel_seg_value = new QLabel;
      m_pLabel_seg_value->setText( "128 / 255" );

      m_pGridLayout->addWidget( m_pLabel_seg_value, 2, 1 );
    }

    //////////////////////////////////////////////////////////////////////

    {

      QScrollArea*	pQScrollArea = new QScrollArea;

      pQScrollArea->setBackgroundRole( QPalette::Dark );
      pQScrollArea->setWidgetResizable ( true );

      m_pQWidget_scrollArea_child = new QWidget;
      pQScrollArea->setWidget( m_pQWidget_scrollArea_child );

      m_pGridLayout->addWidget( pQScrollArea,
      				3, 0,
      				4, 2 );

      m_pQScrollArea = pQScrollArea;

      // m_pQWidget_scrollArea_child = new QWidget;

      // m_pGridLayout->addWidget( m_pQWidget_scrollArea_child,
      // 				3, 0,
      // 				4, 2 );

    }

    {
      // m_pOnglets = new QTabWidget;
      // m_pOnglets->setMinimumSize(300, 300);
    }

    setLayout( m_pGridLayout );
  }




  { // tmp

    // QImage d;

    // // if ( !d.load("/media/CORSAIR/OCR/out_1/out_crop_00007_ratio_2.jpg") )
    // if ( !d.load("./my_images/region_04_segment_char_00.png") )

    //   std::cerr << "creve" << std::endl;

    // m_OCR.epure_picture(d);

    // d.save("./epured2.png");

    // std::cerr << std::endl;
    // std::cerr << "coucou : " << m_OCR.analyse_picture( d ) << std::endl;
    // std::cerr << std::endl;

  } // /tmp

}

void	QtImageWindow::Debug()
{
  m_filename = "./test/9588DWV.jpg";

  {
    std::stringstream	sstr;

    sstr << "filename : " << m_filename.c_str();

    m_pLabel_filname->setText( sstr.str().c_str() );
    m_pPButton_launch->setEnabled(true);
  }

  ProcessImage();
}

void	QtImageWindow::UpdateSegValue(int value)
{
  std::stringstream	sstr;
  sstr << value << " / 255";
  m_pLabel_seg_value->setText( sstr.str().c_str() );
}

void		QtImageWindow::OpenFile()
{
  QStringList	fileNames = QFileDialog::getOpenFileNames( this,
                                                           tr("Open File"),
                                                           "/path/to/file/",
                                                           tr("Images (*.png *.jpg);;All files (*)") );

  if (fileNames.size() == 0)
    {
      m_pLabel_filname->setText( "no filename selected" );
      m_pPButton_launch->setEnabled(false);
      return;
    }


  m_filename = fileNames.first().toStdString();

  // std::cerr << std::endl
  // 	    << "Filename : " << m_filename << std::endl
  // 	    << std::endl;

  {
    std::stringstream	sstr;

    sstr << "filename : " << m_filename.c_str();

    m_pLabel_filname->setText( sstr.str().c_str() );
    m_pPButton_launch->setEnabled(true);
  }

}


void		QtImageWindow::ProcessImage()
{
  /**/
  QImage	qimg;

  if ( qimg.load( QString(m_filename.c_str()) ) )
    {
      std::cerr << std::endl
		<< "Process : " << m_filename << std::endl
		<< std::endl;

      priv_Run_ProcessImage( qimg, m_filename, true );
    }

  else
    {
      std::cerr << "Fail to load : \"" << m_filename << "\"" << std::endl;
    }
}

void		QtImageWindow::Run_ProcessImage(QImage& qimg, const std::string& filename)
{
  m_pInstance->priv_Run_ProcessImage(qimg, filename, false);
}

void		QtImageWindow::priv_Run_ProcessImage(QImage& qimg, const std::string& filename, bool update_gui)
{

  // {
  //   std::stringstream	sstr;
  //   sstr << "./" << "debug_qt.png";

  //   qimg2.save( sstr.str().c_str() );
  // }


  // cv::Mat	input_image = QImage2Mat( qimg );

  qimg = qimg.convertToFormat(QImage::Format_RGB888);
  qimg = qimg.rgbSwapped();

  // QImage	swapped = qimg.rgbSwapped();
  // QImage&	qimg = swapped;

  // return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();

  cv::Mat	input_image(qimg.height(), qimg.width(),
			    // format,
			    // CV_8UC2,
			    CV_8UC3,
			    // CV_8UC4,
			    qimg.bits(),
			    qimg.bytesPerLine());

  // cvtColor(input_image, input_image, CV_BGR2RGB);

  // {
  //   std::stringstream	sstr;
  //   sstr << "./" << "debug.png";

  //   imwrite( sstr.str(), input_image );
  // }


  if (input_image.empty())
    {
      std::cerr << "Fail to load : \"" << filename << "\"" << std::endl;
      return;
    }


  std::cerr << "run" << std::endl;

  if (update_gui)
    std::cerr << "GUI ON" << std::endl;
  else
    std::cerr << "GUI OFF" << std::endl;

  //*/

  // }



  // void		QtImageWindow::ProcessImage()
  // {

  std::string	final_str;


  /**

  bool	update_gui = true;

  std::cerr << std::endl
  	    << "Process : " << m_filename << std::endl
  	    << std::endl;


  cv::Mat	input_image = cv::imread( m_filename, 1 );

  if (input_image.empty())
    {
      std::cerr << "Fail to load : \"" << m_filename << "\"" << std::endl;
      return;
    }

  //*/


  if (update_gui && m_pGridLayout_output != NULL)
    {
      // m_pGridLayout->removeItem( m_pGridLayout_output );

      QLayoutItem*	pChild;

      while ( (pChild = m_pGridLayout_output->takeAt(0)) != NULL )
	{
	  pChild->widget()->hide();
	  delete pChild;
	}

      // m_pQWidget_scrollArea_child->setLayout( NULL );

      delete m_pGridLayout_output, m_pGridLayout_output = NULL;
    }


  //Detect posibles plate regions
  DetectRegions	detectRegions;
  detectRegions.setFilename( filename.c_str() );


  m_posible_regions.clear();

  detectRegions.run( input_image, m_posible_regions );
  // detectRegions.run2( input_image, posible_regions );


  // std::cout << "Num possible region: " << m_posible_regions.size() << "\n";


  if (!m_posible_regions.empty())
    {


      // std::vector<std::string>	tmp_string;

      if (update_gui)
	{

	  m_pGridLayout_output = new QGridLayout;
	  // m_pGridLayout->addLayout( m_pGridLayout_output, 3, 0, 1, 2 );


	  m_pQWidget_scrollArea_child->setLayout( m_pGridLayout_output );
	  m_pQScrollArea->setWidget( m_pQWidget_scrollArea_child );

	}


      for ( unsigned int i = 0; i < m_posible_regions.size(); ++i )
	// if (i == 4)
	{




	  // {

	  // std::stringstream	sstr;

	  // sstr << "./my_images/"
	  // 	 << "region_"
	  // 	 << std::setw(2) << std::setfill('0') << i << ".png";

	  // imwrite( sstr.str(), m_posible_regions[i].plateImg );

	  D_IMG_SAVE( m_posible_regions[i].plateImg,
		      "region_"
		      << std::setw(2) << std::setfill('0') << i
		      << ".png" );

	  // }


	  if (update_gui)
	    {

	      cv::Mat&	curr_Mat = m_posible_regions[i].plateImg;

	      QImage*	pQImg = new QImage( Mat2QImage(curr_Mat) );

	      QPixmap*	pPixmap = new QPixmap;
	      pPixmap->convertFromImage( *pQImg );

	      QLabel*	pLabel = new QLabel();
	      pLabel->setPixmap( *pPixmap );

	      m_pGridLayout_output->addWidget( pLabel,
					       2 + i * 2, 0,
					       1, 2 );

	    }


	  std::vector<t_seg_char>&	output = m_posible_regions[i].segment_chars;


	  float		ratio_limit = static_cast<float>(m_pQSlider_seg_value->value()) / 255.0f;

	  Segment( m_posible_regions[i].plateImg, output, ratio_limit, i );


	  OCR::t_analyze_output*	pTab_out = new OCR::t_analyze_output[ output.size() ];

	  for ( unsigned int j = 0; j < output.size(); ++j )
	    // if (j == 0)
	    {

	      // {

	      // 	std::stringstream	sstr;

	      // 	sstr << "./my_images/"
	      // 	     << "region_"
	      // 	     << std::setw(2) << std::setfill('0') << i << "_segment_char_"
	      // 	     << std::setw(2) << std::setfill('0') << j << ".png";

	      // 	imwrite( sstr.str(), output[j].s_mat );

	      D_IMG_SAVE( output[j].s_mat,
			  "region_"
			  << std::setw(2) << std::setfill('0') << i << "_segment_char_"
			  << std::setw(2) << std::setfill('0') << j << ".png" );

	      // }


	      {

		cv::Mat&	curr_Mat = output[j].s_mat;

		QImage*		pQImg = new QImage( Mat2QImage(curr_Mat) );

		if (update_gui)
		  {
		    QPixmap*	pPixmap = new QPixmap;
		    pPixmap->convertFromImage( *pQImg );

		    QLabel*	pLabel = new QLabel();
		    pLabel->setPixmap( *pPixmap );

		    // m_pGridLayout_output->addWidget( pLabel, 2 + i, 2 + j);
		    m_pGridLayout_output->addWidget( pLabel,
						     2 + i * 2,
						     2 + j );
		  }

		{

		  // std::cerr << "analyze " << i << " - " << j << std::endl;

		  std::string	ret_val;

		  m_OCR.epure_picture( *pQImg );


		  // OCR::t_analyze_output	out;

		  // ret_val = m_OCR.analyse_picture( *pQImg );
		  m_OCR.analyse_picture( *pQImg, pTab_out[j] );


		  // std::cerr<< "analyse : " << pTab_out[j].s_match << " | " << pTab_out[j].s_char << std::endl;


		  if (update_gui)
		    {

		      std::stringstream	sstr;

		      sstr << pTab_out[j].s_char
			   << "(" << pTab_out[j].s_match << ")";


		      QLabel*	tmp_pLabel = new QLabel;
		      tmp_pLabel->setText( sstr.str().c_str() );

		      m_pGridLayout_output->addWidget( tmp_pLabel,
						       2 + i * 2 + 1,
						       2 + j );

		    }





		  // {
		  //   QGridLayout*	gridLayout = new QGridLayout;

		  //   int		char_by_line = 5;

		  //   for (unsigned int k = 0; k < m_OCR.dictionnary.size(); k++)
		  //     {
		  // 	Character charac = m_OCR.dictionnary[k];

		  // 	/*{  save image

		  // 	  QImage  tmp_image = charac.get_img();

		  // 	  std::stringstream   sstr;
		  // 	  sstr << "./out/char_" << i << ".png";

		  // 	  tmp_image.save( QString( sstr.str().c_str() ) );

		  // 	  }  //save image*/

		  // 	QPixmap pixmap_img = QPixmap::fromImage(charac.get_img());

		  // 	QLabel  *label  = new QLabel;

		  // 	label->setPixmap(pixmap_img);

		  // 	gridLayout->addWidget( label,
		  // 			       k / char_by_line,
		  // 			       k % char_by_line );

		  //     }

		  //   m_pGridLayout_output->addLayout( gridLayout,  5,5,  1,1 );
		  // }






		  {
		    // std::stringstream	sstr;

		    // sstr << "./my_images/"
		    // 	 << "epur_" << i << "_" << j << ".png";

		    // pQImg->save( sstr.str().c_str() );
		  }

		  // std::stringstream	sstr;

		  // sstr << " i = " << i
		  //      << " j = " << j
		  //      << " => \"" << ret_val << "\".";


		  // tmp_string.push_back( sstr.str() );

		  // std::cerr << "/analyze " << i << " - " << j << std::endl;

		}

	      }


	    } // for j


	  {
	    std::stringstream	sstr;

	    int	nbr_valid_char = 0;

	    for ( unsigned int j = 0; j < output.size(); ++j )
	      if (pTab_out[j].s_match > 0)
		++nbr_valid_char;

	    // if (output.size() < 7)
	    if (nbr_valid_char < 7)
	      sstr << "/!\\ not enought characters";
	    else
	      {
		std::vector<int>	index;
		for ( unsigned int j = 0; j < output.size(); ++j )
		  index.push_back(j);

		while (index.size() > 7)
		  {

		    int	worst = 0;

		    for ( unsigned int j = 1; j < index.size(); ++j )
		      if ( pTab_out[ index[j] ].s_match < pTab_out[ index[worst] ].s_match )
			worst = j;

		    index.erase( index.begin() + worst );

		  }

		for ( unsigned int j = 0; j < index.size(); ++j )
		  sstr << pTab_out[ index[j] ].s_char;

		final_str = sstr.str();
	      }

	    if (update_gui)
	      {

		QLabel*	tmp_pLabel = new QLabel;
		tmp_pLabel->setText( sstr.str().c_str() );

		m_pGridLayout_output->addWidget( tmp_pLabel,
						 2 + i * 2 + 1, 0,
						 1, 2 );

	      }

	  }


	} // for i

      // for ( unsigned int i = 0; i < tmp_string.size(); ++i )
      // 	std::cerr << tmp_string[i] << std::endl;

    } // !empty


  { /// MERGE

    bool	success = !final_str.empty();

    std::string		image_name = l_getFilename( filename );

    ORM::Image*		pOrmImage = NULL;

    {

      std::stringstream	sstr;

      sstr << image_name << ".png";
      std::string	new_filename = sstr.str();


      std::cerr<< "new_filename : " << new_filename << std::endl;


      pOrmImage = ORM::Image::GetImageByPathName( QString(new_filename.c_str()) );

      if (pOrmImage == NULL)
	{
	  success = ORM::Image::Create( &pOrmImage,
					QString(new_filename.c_str()),
					NULL, ( success
						? ORM::Image::eProcess_succeed
						: ORM::Image::eProcess_failure ) );
	}

    }

    ORM::Plate*	pOrmPlate = NULL;

    { // plate

      if (success)
	{

	  pOrmPlate = ORM::Plate::FindPlateByNumber( QString(final_str.c_str()) );

	  if (pOrmPlate != NULL)
	    {
	      pOrmImage->AttachPlate( pOrmPlate );
	      ORM::Plate::Dealloc( pOrmPlate );
	    }

	  else
	    {

	      ORM::Plate::Create( &pOrmPlate,
				  QString(final_str.c_str()),
				  QString("unknow") );

	    }

	  if (pOrmImage != NULL && pOrmPlate != NULL)
	    pOrmImage->AttachPlate( pOrmPlate  );

	}

    } // /plate

    { // log

      ORM::Log::e_WarningLevel	w_lvl = ( !success
					  ? ORM::Log::eProcessFailure
					  : ( (pOrmPlate && pOrmPlate->IsWanted())
					      ? ORM::Log::eWantedUser
					      : ORM::Log::eImageProcessed ) );

      ORM::Log::Create( NULL, pOrmImage, w_lvl );

    } // /log

  } /// /MERGE



  std::cerr<< "final : " << final_str << std::endl;



}

QtImageWindow::~QtImageWindow()
{
  // delete m_pImage, m_pImage = NULL;
}
