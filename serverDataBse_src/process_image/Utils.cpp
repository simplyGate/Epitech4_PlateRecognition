

#include "Utils.hpp"


int	intersectsegment( const cv::Point& A,
			  const cv::Point& B,
			  const cv::Point& I,
			  const cv::Point& P )
{
  cv::Point	D, E;
  D.x = B.x - A.x;
  D.y = B.y - A.y;
  E.x = P.x - I.x;
  E.y = P.y - I.y;

  double	denom = D.x * E.y - D.y * E.x;

  if (denom == 0)
    return -1;   // erreur, cas limite

  double	t = - (A.x * E.y - I.x * E.y - E.x * A.y + E.x * I.y) / denom;

  if (t < 0 || t >= 1)
    return 0;

  double	u = - (-D.x * A.y + D.x * I.y + D.y * A.x - D.y * I.x) / denom;

  if (u < 0 || u >= 1)
    return 0;

  return 1;
}


bool	Collision(std::vector< cv::Point >& tab, const cv::Point& P)
// bool	Collision(const Point* tab, int nbp, const Point& P)
{
  unsigned int	i;
  cv::Point	I;

  I.x = 10000 + rand() % 100;   // 10000 + un nombre aléatoire entre 0 et 99
  I.y = 10000 + rand() % 100;
  int	nbintersections = 0;

  for (i = 0; i < tab.size(); i++)
    {
      cv::Point A = tab[i];
      cv::Point B;

      if ((int)i == (int)tab.size() - 1)  // si c'est le dernier point, on relie au premier
	B = tab[0];
      else           // sinon on relie au suivant.
	B = tab[i + 1];

      int iseg = intersectsegment(A, B, I, P);

      if (iseg == -1)
	return Collision(tab, P);  // cas limite, on relance la fonction.

      nbintersections += iseg;
    }

  if ( (nbintersections % 2) == 1)  // nbintersections est-il impair ?
    return true;
  else
    return false;
}


