

#include "Image.hpp"

#include "Plate.hpp"

#include "../Utils/TraceLogger.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <sstream>
#include <iostream>


namespace ORM
{

  const std::string	Image::Field_id("id");
  const std::string	Image::Field_pathName("pathname");
  const std::string	Image::Field_attachedPlate("attached_plate");
  const std::string	Image::Field_status("status");
  const std::string	Image::Table("Licence_Images");

  //////////////////////////////////////////////////////////////////////

  QString		Image::GetPathName() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_pathName
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
      return query.value(0).toString();

    return QString("");
  }

  Plate*	Image::GetAttachedPlate() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_attachedPlate
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
	int	attached_plate_id = query.value(0).toInt();

	if (attached_plate_id != -1)
	  {
	    sstr.str("");
	    sstr.clear();

	    sstr << "SELECT " << Plate::Field_id
		 << " FROM " << Plate::Table
		 << " WHERE " << Plate::Field_id << "=" << attached_plate_id
		 << ";";

	    qstr_query_in = sstr.str().c_str();

	    query.clear();
	    if ( !query.exec( qstr_query_in ) )
	      {
		LOG( "QSqlQuery::exec, error : "
		     << query.lastError().text().toStdString() );
	      }
	    else if ( query.first() )
	      return ( new Plate( query.value(0).toInt() ) );

	  }
      }

    return (NULL);
  }

  Image::e_ImageStatus		Image::GetStatus() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_status
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

	if (value >= 0 && value < eImageStatus_count)
	  return ( static_cast<e_ImageStatus>(value) );
      }

    return (eUnknowStatus);
  }

  //////////////////////////////////////////////////////////////////////

  bool		Image::AttachPlate(const Plate* pPlate) const
  {
    if (pPlate != NULL)
      {

	std::stringstream	sstr;

	sstr << "UPDATE " << Table
	     << " SET " << Field_attachedPlate << "=" << pPlate->m_id
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

  bool		Image::SetStatus(e_ImageStatus status) const
  {
    if (status >= 0 && status < eImageStatus_count)
      {

	std::stringstream	sstr;

	sstr << "UPDATE " << Table
	     << " SET " << Field_status << "=" << static_cast<int>(status)
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

  bool		Image::Create( Image** ppImage_out,
			       const QString& pathName,
			       const Plate* pPlate /*= NULL*/,
			       e_ImageStatus status /*= eUnprocessed*/ )
  {
    std::stringstream	sstr;

    if (status < 0 || status >= eImageStatus_count)
      status = eUnknowStatus;

    int	attached_plate_id = ( (pPlate != NULL)
			      ? (pPlate->m_id)
			      : (-1) );

    sstr << "INSERT INTO " << Table << "("
	 << Field_pathName << ", "
	 << Field_attachedPlate << ", "
	 << Field_status
	 << ") VALUES ("
	 << "\"" << pathName.toStdString() << "\", "
	 << attached_plate_id << ", "
	 << static_cast<int>(status)
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

	if (ppImage_out != NULL)
	  {
	    *ppImage_out = NULL;

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
	      *ppImage_out = new Image( query.value(0).toInt() );
	  }

	return (true);
      }

    return (false);
  }

  void		Image::Dealloc( Image* pImage )
  {
    delete pImage;
  }

  void		Image::EraseFromDatabase( Image* pImage )
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

  std::vector< Image* >	Image::GetAll()
  {
    std::vector< Image* >	All_Images;

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
	  All_Images.push_back( new Image( query.value(0).toInt() ) );
      }

    return (All_Images);
  }

  //////////////////////////////////////////////////////////////////////

  std::vector< Image* >	Image::GetImageByPlate(const Plate* pPlate)
  {
    std::vector< Image* >	Images;

    if (pPlate != NULL)
      {

	std::stringstream	sstr;

	sstr << "SELECT " << Field_id
	     << " FROM " << Table
	     << " WHERE " << Field_attachedPlate << "=" << pPlate->m_id
	     << " LIMIT 1"
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
	      Images.push_back( new Image( query.value(0).toInt() ) );
	  }

      } // if (pPlate != NULL)

    return (Images);
  }

  Image*	Image::GetImageByPathName(const QString& pathName)
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_id
	 << " FROM " << Table
	 << " WHERE " << Field_pathName << "=\"" << pathName.toStdString() << "\""
	 // << " LIMIT 1"
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }
    else if ( query.first() )
      return ( new Image( query.value(0).toInt() ) );

    return (NULL);
  }

  std::vector< Image* >	Image::GetImageByStatus(e_ImageStatus status)
  {
    std::vector< Image* >	Images;

    if (status < 0 || status >= eImageStatus_count)
      status = eUnknowStatus;

    std::stringstream	sstr;

    sstr << "SELECT " << Field_id
	 << " FROM " << Table
	 << " WHERE " << Field_status << "=" << static_cast<int>(status)
	 // << " LIMIT 1"
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
	  Images.push_back( new Image( query.value(0).toInt() ) );
      }

    return (Images);
  }

};
