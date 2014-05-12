

#ifndef	PLATE_HPP
#define	PLATE_HPP


#include <string>
#include <vector>

#include <QString>


namespace ORM
{

  class	Image;

  class	Plate
  {

    friend class	Image;

  private : // attrib(s)

    int	m_id;
  public:
    static const std::string	Field_id;
    static const std::string	Field_plateNumber;
    static const std::string	Field_userName;
    static const std::string	Field_isWanted;
    static const std::string	Table;

  private : // ctor(s) / dtor

    Plate(int id) : m_id(id) {}
    ~Plate() {}

  public : // getter(s)

    QString		GetPlateNumber() const;
    QString		GetUserName() const;
    bool		IsWanted() const;

  public : // setter(s)

    bool		SetIsWanted(bool value) const;
    bool		SetUserName(const QString& username) const;

  public : // static method(s)

    static bool		Create( Plate** ppPlate_out,
				const QString& platenb,
				const QString& username,
				bool iswanted = false );

    static void		Dealloc( Plate* pPlate );
    static void		EraseFromDatabase( Plate* pPlate );

    ///

    static std::vector< Plate* >	GetAll();

    ///

    static Plate*	FindPlateByNumber(const QString& platenb);

  };

};


#endif	// ORM_PLATE_HPP

