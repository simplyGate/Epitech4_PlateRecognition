

#ifndef	LOG_HPP
#define	LOG_HPP


#include <string>
#include <vector>

#include <QString>
#include <QDateTime>


namespace ORM
{

  class	Image;

  class	Log
  {

    friend class	Image;

  private : // attrib(s)

    int	m_id;

  public:
    static const std::string	Field_id;
    static const std::string	Field_warningLevel;
    static const std::string	Field_attachedImage;
    static const std::string	Field_createDate;
    static const std::string	Table;

  private : // ctor(s) / dtor

    Log(int id) : m_id(id) {}
    ~Log() {}

  public : // public enum(s)

    enum	e_WarningLevel
      {
	eUnknowWarning = 0,

	eImageProcessed,
	eProcessFailure,
	eWantedUser,

	eWarningLevel_count
      };

  public : // getter(s)

    QDateTime		GetCreateDate() const;
    Image*		GetAttachedImage() const;
    e_WarningLevel	GetWarninglevel() const;

  public : // setter(s)

    bool		SetWarninglevel(e_WarningLevel logw) const;

  public : // static method(s)

    static bool		Create( Log** ppLog_out,
				const Image* pPlate,
				e_WarningLevel logw );

    static void		Dealloc( Log* pLog );
    static void		EraseFromDatabase( Log* pLog );

    ///

    static std::vector< Log* >	GetAll();

    ///

    static std::vector< Log* >	GetLogByWarningLevel(e_WarningLevel logw);
    static std::vector< Log* >	GetLogByImage(const Image* pImage);
    static std::vector< Log* >	GetLogByDataTimeRange( const QDateTime& after,
						       const QDateTime& before );

    // static std::vector< Log* >	GetImageByPlate(const Plate* pPlate);
    // static Image*	GetImageByPathName(const QString& pathName);
    // static std::vector< Image* >	GetImageByStatus(e_WarningLevel status);

  };

};


#endif	// ORM_PLATE_HPP

