

#ifndef IMG_QTIMAGEWINDOW_HPP
#define IMG_QTIMAGEWINDOW_HPP


#include <QMainWindow>

#include <QPixmap>
#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include <QSlider>
#include <QScrollArea>

#include "DetectRegions.hpp"

#include "OCR.hpp"

// #include "Segmentation.hpp"


// class QtImageWindow : public QMainWindow
class QtImageWindow : public QWidget
{
  Q_OBJECT

public :

  explicit QtImageWindow(QWidget *parent = NULL);
  ~QtImageWindow();

private slots :

  void	UpdateSegValue(int value);

  void	OpenFile();
  void	ProcessImage();

  void	Debug();


public :

  static void	Run_ProcessImage(QImage& qimg, const std::string& filename);

private :

  void	priv_Run_ProcessImage(QImage& qimg, const std::string& filename, bool update_gui = false);

private :


  static QtImageWindow*	m_pInstance;

  QGridLayout*	m_pGridLayout;

  QLabel*	m_pLabel_filname;

  QPushButton*	m_pPButton_launch;

  std::string	m_filename;

  QSlider*	m_pQSlider_seg_value;
  QLabel*	m_pLabel_seg_value;

  // QTabWidget*	m_pOnglets;

  QScrollArea*	m_pQScrollArea;
  QWidget*	m_pQWidget_scrollArea_child;

  QGridLayout*	m_pGridLayout_output;

private :

  std::vector< img_Plate >	m_posible_regions;

private :

  OCR	m_OCR;

};


#endif	// QTIMAGEWINDOW_HPP

