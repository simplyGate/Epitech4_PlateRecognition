

#ifndef	IMAGE_HPP
#define	IMAGE_HPP


#include <string>
#include <vector>

#include <QString>


namespace ORM
{

  class	Plate;
  class	Log;

  class	Image
  {

    friend class	Plate;
    friend class	Log;

  private : // attrib(s)

    int	m_id;

  public:
    static const std::string	Field_id;
    static const std::string	Field_pathName;
    static const std::string	Field_attachedPlate;
    static const std::string	Field_status;
    static const std::string	Table;

  private : // ctor(s) / dtor

    Image(int id) : m_id(id) {}
    ~Image() {}

  public : // public enum(s)

    enum	e_ImageStatus
      {
	eUnknowStatus = 0,

	eUnprocessed,
	eProcess_succeed,
	eProcess_failure,

	eImageStatus_count
      };

  public : // getter(s)

    QString		GetPathName() const;
    Plate*		GetAttachedPlate() const;
    e_ImageStatus	GetStatus() const;

  public : // setter(s)

    bool		AttachPlate(const Plate* pPlate) const;
    bool		SetStatus(e_ImageStatus status) const;

  public : // static method(s)

    static bool		Create( Image** ppImage_out,
				const QString& pathName,
				const Plate* pPlate = NULL,
				e_ImageStatus status = eUnprocessed );

    static void		Dealloc( Image* pImage );
    static void		EraseFromDatabase( Image* pImage );

    ///

    static std::vector< Image* >	GetAll();

    ///

    static std::vector< Image* >	GetImageByPlate(const Plate* pPlate);
    static Image*			GetImageByPathName(const QString& pathName);
    static std::vector< Image* >	GetImageByStatus(e_ImageStatus status);

  };

};


#endif	// ORM_PLATE_HPP

