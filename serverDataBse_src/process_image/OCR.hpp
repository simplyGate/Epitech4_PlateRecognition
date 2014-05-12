

#ifndef OCR_HPP
#define OCR_HPP


#include <iostream>
#include <QImage>
#include <vector>
#include <QColor>


class OCR
{
private :

  struct	Character
  {
  public :

    std::string	s_name;
    QImage	s_img;

  public :

    Character( const std::string& name,
	       const QImage& img )
      : s_name(name),
	s_img(img)
    {}

  };

private :

  std::vector<Character>	dictionnary;
  int	over;

public :

  struct	t_analyze_output
  {
    std::string	s_char;
    int		s_match;
  };

public :

  OCR();

  void init_dico_picture();

public :

  static void	epure_picture(QImage &img);

public :

  void	analyse_picture(const QImage& copie, t_analyze_output& out);

// public :

  // void	set_over(int i)
  // {
  //   over = i;
  // }

  // int get_over()
  // {
  //   return over;
  // }

};


#endif	// OCR_HPP

