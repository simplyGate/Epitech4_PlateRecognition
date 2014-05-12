#include "OCR.hpp"
#include <sstream>

OCR::OCR()
{
  init_dico_picture();
}

void	OCR::init_dico_picture()
{
	char	charac_name;
	
	std::string s;
	QImage font;
	if ( font.load("./process_image/res/mp1_license-plate_1.jpg") )
		std::cerr << "success" << std::endl;
	else
		std::cerr << "failure" << std::endl;
	
	int tmp_x = 86 + 33;
	int tmp_y = 0;
	
	charac_name = '.';
	s = charac_name;
	dictionnary.reserve(128);
	
	while (charac_name <= 'Z')
	{
		QImage  tmp_img = font.copy(tmp_x, tmp_y, 20, 40);
		
		this->epure_picture(tmp_img);
		
		dictionnary.push_back( Character(s, tmp_img) );
		if (charac_name == '.')
			charac_name = '0';
		else if (charac_name == '9')
			charac_name = 'A';
		else
			charac_name++;
		
		s = charac_name;
		
		tmp_x += 66 + 20;
		
		if (tmp_x > 850)
		{
			tmp_x = 33;
			tmp_y += 100 + 40;
		}
	}
	over = 20;
}

void	OCR::epure_picture(QImage& img)
{
	for (int x = 0; x < img.width(); x++)
		for (int y = 0; y < img.height(); y++)
		{
			QColor	in_color = img.pixel( x, y );
			
			int	avg = (in_color.red() + in_color.blue() + in_color.green()) / 3;
			
			int	value;

			if (avg < 128)
				value = 0;
			else
				value = 255;
			
			img.setPixel( x, y, qRgb(value, value, value) );
		}
}

void	OCR::analyse_picture(const QImage& copie, t_analyze_output& out)
{
	int		percent_dico = -1;
	int		percent_copy = -1;
	
	std::string	charac = "+";
	std::string	charac_dico = "+";
	std::string	charac_copy = "+";

	for (unsigned int i = 0; i < dictionnary.size(); i++)
	{
		int	nb_black_px_alpha = 0;
		int	nb_black_px_copie = 0;
		int	nb_px_match = 0;
		{ // match loop
			for (int y = 0; y < dictionnary[i].s_img.height(); ++y)
				if ( y < copie.height() )
					for (int x = 0; x < dictionnary[i].s_img.width(); ++x)
						if ( x < copie.width() )
						{
							int	dico_red = static_cast<QColor>(dictionnary[i].s_img.pixel(x,y)).red();
							int	input_red = static_cast<QColor>(copie.pixel(x,y)).red();

							if (dico_red == 0 && dico_red == input_red)
								nb_px_match++;
							if (dico_red == 0)
								++nb_black_px_alpha;
							if (input_red == 0)
								++nb_black_px_copie;
						}
		} // /match loop
		int	black_percent_alpha = nb_black_px_alpha * 100 / (dictionnary[i].s_img.height() * dictionnary[i].s_img.width());
		int	black_percent_copie = nb_black_px_copie * 100 / (copie.height() * copie.width());
	
		if ( black_percent_copie <= black_percent_alpha + over && black_percent_copie >= black_percent_alpha - over && nb_px_match != 0 )
		{
			int	tmp_ratio_dico = nb_px_match * 100 / nb_black_px_alpha;
			int	tmp_ratio_input = nb_px_match * 100 / nb_black_px_copie;

			if ( tmp_ratio_dico >= 50 || tmp_ratio_input >= 50 )
			{
				{
					if ( percent_dico < tmp_ratio_dico && percent_copy < tmp_ratio_input )
					{
						percent_dico = tmp_ratio_dico;		    
						percent_copy = tmp_ratio_input;
						charac = dictionnary[i].s_name;
					}
				}
			}
		}
	}
	out.s_char = charac;
	out.s_match = (percent_dico + percent_copy) / 2;
}