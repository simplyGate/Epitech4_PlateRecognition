

#ifndef	ORM_LOG_HPP
#define	ORM_LOG_HPP


namespace ORM
{

  class	Log
  {
  private : // attrib(s)

    int	m_id;

    static const std::string	Field_id;
    static const std::string	Field_createDate;
    static const std::string	Field_attachedImage;
    static const std::string	Field_warningLevel;
    static const std::string	Table;

  private : // ctor(s) / dtor

    Log();
    ~Log();

  public : // public enum

    enum	e_LogLevelWarning {
      eImageProcessed = 0,
      eProcessFailure,
      eWantedUser
    };

  public : // method(s)

    QDateTime		GetCreateDate() const;
    Image*		GetAttachedImage() const;
    e_LogLevelWarning	GetLogWarning() const;

    void		SetLogWarning(e_LogLevelWarning logw) const;

  public : // static method(s)

    static bool		CreateLog(const QDateTime& dt, e_LogLevelWarning logw, Image* pImg);

  };

};


#endif

