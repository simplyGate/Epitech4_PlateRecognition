

#include "Log.hpp"

#include "Image.hpp"

#include "Utils/TraceLogger.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <sstream>
#include <iostream>


namespace ORM
{

  const std::string	Log::Field_id("id");
  const std::string	Log::Field_warningLevel("warning_level");
  const std::string	Log::Field_attachedImage("attached_image");
  const std::string	Log::Field_createDate("create_date");
  const std::string	Log::Table("Licence_Logs");

  //////////////////////////////////////////////////////////////////////

  QDateTime		Log::GetCreateDate() const
  {
    QString	date = "";

    std::stringstream	sstr;

    sstr << "SELECT " << Field_createDate
	 << " FROM " << Table
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else if ( query.first() )
      date = query.value(0).toString();

    return QDateTime::fromString( date, "yyyy-MM-dd hh:mm:ss" );
  }

  Image*		Log::GetAttachedImage() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_attachedImage
	 << " FROM " << Table
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else if ( query.first() )
      {
	int	attached_image_id = query.value(0).toInt();

	if (attached_image_id != -1)
	  {
	    sstr.str("");
	    sstr.clear();

	    sstr << "SELECT " << Image::Field_id
		 << " FROM " << Image::Table
		 << " WHERE " << Image::Field_id << "=" << attached_image_id
		 << ";";

	    qstr_query_in = sstr.str().c_str();

	    query.clear();
	    if ( !query.exec( qstr_query_in ) )
	      {
		LOG( "QSqlQuery::exec, error : "
		     << query.lastError().text().toStdString() );
	      }
	    else if ( query.first() )
	      return ( new Image( attached_image_id ) );

	  }
      }

    return (NULL);
  }

  Log::e_WarningLevel	Log::GetWarninglevel() const
  {
    QString	date = "";

    std::stringstream	sstr;

    sstr << "SELECT " << Field_warningLevel
	 << " FROM " << Table
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else if ( query.first() )
      {
	int	value = query.value(0).toInt();

	if (value >= 0 && value < eWarningLevel_count)
	  return ( static_cast<e_WarningLevel>(value) );
      }

    return (eUnknowWarning);
  }

  //////////////////////////////////////////////////////////////////////

  bool		Log::SetWarninglevel(e_WarningLevel logw) const
  {
    if (logw >= 0 && logw < eWarningLevel_count)
      {

	std::stringstream	sstr;

	sstr << "UPDATE " << Table
	     << " SET " << Field_warningLevel << "=" << static_cast<int>(logw)
	     << " WHERE " << Field_id << "=" << m_id
	     << ";";

	QString	qstr_query_in( sstr.str().c_str() );

	QSqlQuery	query;
	if ( !query.exec( qstr_query_in ) )
	  {
	    LOG( "QSqlQuery::exec, error : "
		 << query.lastError().text().toStdString() );
	  }
	else
	  return (true);

      }

    return (false);
  }

  //////////////////////////////////////////////////////////////////////

  bool		Log::Create( Log** ppLog_out,
			     const Image* pImage,
			     e_WarningLevel logw )
  {
    std::stringstream	sstr;

    QString	curr_date = QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" );

    if (logw < 0 || logw >= eWarningLevel_count)
      logw = eUnknowWarning;

    int	attached_image_id = ( (pImage != NULL)
			      ? (pImage->m_id)
			      : (-1) );

    sstr << "INSERT INTO " << Table << "("
	 << Field_warningLevel << ", "
	 << Field_attachedImage << ", "
	 << Field_createDate
	 << ") VALUES ("
	 << static_cast<int>(logw) << ", "
	 << attached_image_id << ", "
	 << "\"" << curr_date.toStdString() << "\""
	 << ");";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else
      {

	if (ppLog_out != NULL)
	  {
	    *ppLog_out = NULL;

	    sstr.str("");
	    sstr.clear();

	    sstr << "SELECT " << Field_id
		 << " FROM " << Table
		 << " ORDER BY " << Field_id << " DESC"
		 << " LIMIT 1"
		 << ";";

	    qstr_query_in = sstr.str().c_str();

	    query.clear();
	    if ( !query.exec( qstr_query_in ) )
	      {
		LOG( "QSqlQuery::exec, error : "
		     << query.lastError().text().toStdString() );
	      }
	    else if ( query.first() )
	      *ppLog_out = new Log( query.value(0).toInt() );
	  }

	return (true);
      }

    return (false);
  }

  void		Log::Dealloc( Log* pImage )
  {
    delete pImage;
  }

  void		Log::EraseFromDatabase( Log* pImage )
  {
    std::stringstream	sstr;

    sstr << "DELETE FROM " << Table
	 << " WHERE " << Field_id << "=" << pImage->m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }

    Dealloc( pImage );
  }

  //////////////////////////////////////////////////////////////////////

  std::vector< Log* >	Log::GetAll()
  {
    std::vector< Log* >	All_Logs;

    std::stringstream	sstr;

    sstr << "SELECT " << Field_id
	 << " FROM " << Table
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else
      {
	while (query.next())
	  All_Logs.push_back( new Log( query.value(0).toInt() ) );
      }

    return (All_Logs);
  }

  //////////////////////////////////////////////////////////////////////

  std::vector< Log* >	Log::GetLogByWarningLevel(e_WarningLevel logw)
  {
    if (logw < 0 || logw >= eWarningLevel_count)
      logw = eUnknowWarning;

    std::vector< Log* >	Logs;

    std::stringstream	sstr;

    sstr << "SELECT " << Field_id
	 << " FROM " << Table
	 << " WHERE " << Field_warningLevel << "=" << logw
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else
      {
	while (query.next())
	  Logs.push_back( new Log( query.value(0).toInt() ) );
      }

    return (Logs);
  }

  std::vector< Log* >	Log::GetLogByImage(const Image* pImage)
  {
    std::vector< Log* >	Logs;

    if (pImage != NULL)
      {

	std::stringstream	sstr;

	sstr << "SELECT " << Field_id
	     << " FROM " << Table
	     << " WHERE " << Field_attachedImage << "=" << pImage->m_id
	     << ";";

	QString	qstr_query_in( sstr.str().c_str() );

	QSqlQuery	query;
	if ( !query.exec( qstr_query_in ) )
	  {
	    LOG( "QSqlQuery::exec, error : "
		 << query.lastError().text().toStdString() );
	  }
	else
	  {
	    while (query.next())
	      Logs.push_back( new Log( query.value(0).toInt() ) );
	  }

      }

    return (Logs);
  }

  std::vector< Log* >	Log::GetLogByDataTimeRange( const QDateTime& after,
						    const QDateTime& before )
  {
    QString	qstr_after = after.toString( "yyyy-MM-dd hh:mm:ss" );
    QString	qstr_before = before.toString( "yyyy-MM-dd hh:mm:ss" );

    std::vector< Log* >	Logs;

    std::stringstream	sstr;

    sstr << "SELECT " << Field_id
	 << " FROM " << Table
	 << " WHERE " << Field_createDate << " > " << qstr_before.toStdString()
	 << " AND " << Field_createDate << " < " << qstr_after.toStdString()
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else
      {
	while (query.next())
	  Logs.push_back( new Log( query.value(0).toInt() ) );
      }

    return (Logs);

  }


  // static std::vector< Log* >	GetLogForXDays(int days);



  // std::vector< Image* >	Log::GetImageByPlate(const Plate* pPlate)
  // {
  //   std::vector< Image* >	Images;

  //   if (pPlate != NULL)
  //     {

  // 	std::stringstream	sstr;

  // 	sstr << "SELECT " << Field_id
  // 	     << " FROM " << Table
  // 	     << " WHERE " << Field_attachedPlate << "=" << pPlate->m_id
  // 	     << " LIMIT 1"
  // 	     << ";";

  // 	QString	qstr_query_in( sstr.str().c_str() );

  // 	QSqlQuery	query;
  // 	if ( !query.exec( qstr_query_in ) )
  // 	  {
  // 	    LOG( "QSqlQuery::exec, error : "
  // 		 << query.lastError().text().toStdString() );
  // 	  }
  // 	else
  // 	  {
  // 	    while (query.next())
  // 	      Images.push_back( new Image( query.value(0).toInt() ) );
  // 	  }

  //     } // if (pPlate != NULL)

  //   return (Images);
  // }

  // Image*	Log::GetImageByPathName(const QString& pathName)
  // {
  //   std::stringstream	sstr;

  //   sstr << "SELECT " << Field_id
  // 	 << " FROM " << Table
  // 	 << " WHERE " << Field_pathName << "=\"" << pathName.toStdString() << "\""
  // 	 << " LIMIT 1"
  // 	 << ";";

  //   QString	qstr_query_in( sstr.str().c_str() );

  //   QSqlQuery	query;
  //   if ( !query.exec( qstr_query_in ) )
  //     {
  // 	LOG( "QSqlQuery::exec, error : "
  // 	     << query.lastError().text().toStdString() );
  //     }
  //   else if ( query.first() )
  //     return ( new Image( query.value(0).toInt() ) );

  //   return (NULL);
  // }

  // std::vector< Image* >	Log::GetImageByStatus(e_ImageStatus status)
  // {
  //   std::vector< Image* >	Images;

  //   if (status < 0 || status >= eImageStatus_count)
  //     status = eUnknowStatus;

  //   std::stringstream	sstr;

  //   sstr << "SELECT " << Field_id
  // 	 << " FROM " << Table
  // 	 << " WHERE " << Field_status << "=" << static_cast<int>(status)
  // 	 << " LIMIT 1"
  // 	 << ";";

  //   QString	qstr_query_in( sstr.str().c_str() );

  //   QSqlQuery	query;
  //   if ( !query.exec( qstr_query_in ) )
  //     {
  // 	LOG( "QSqlQuery::exec, error : "
  // 	     << query.lastError().text().toStdString() );
  //     }
  //   else
  //     {
  // 	while (query.next())
  // 	  Images.push_back( new Image( query.value(0).toInt() ) );
  //     }

  //   return (Images);
  // }

};
