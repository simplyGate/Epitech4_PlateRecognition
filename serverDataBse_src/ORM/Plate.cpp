

#include "Plate.hpp"

#include "Image.hpp"

#include "Utils/TraceLogger.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <sstream>
#include <iostream>


namespace ORM
{

  const std::string	Plate::Field_id("id");
  const std::string	Plate::Field_plateNumber("plate_number");
  const std::string	Plate::Field_userName("user_name");
  const std::string	Plate::Field_isWanted("is_wanted");
  const std::string	Plate::Table("Licence_Plates");

  //////////////////////////////////////////////////////////////////////

  QString	Plate::GetPlateNumber() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_plateNumber
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

  QString	Plate::GetUserName() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_userName
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

  bool		Plate::IsWanted() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_isWanted
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
      return ( (query.value(0).toInt() != 0) ? (true) : (false) );

    return (false);
  }

  //////////////////////////////////////////////////////////////////////

  bool		Plate::SetIsWanted(bool iswanted) const
  {
    std::stringstream	sstr;

    sstr << "UPDATE " << Table
	 << " SET " << Field_isWanted << "=" << ( (iswanted) ? ("1") : ("0") )
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
	return (false);
      }

    return (true);
  }

  bool		Plate::SetUserName(const QString& username) const
  {
    std::stringstream	sstr;

    sstr << "UPDATE " << Table
	 << " SET " << Field_userName << "=\"" << username.toStdString() << "\""
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
	return (false);
      }

    return (true);
  }

  //////////////////////////////////////////////////////////////////////

  bool		Plate::Create( Plate** ppPlate_out,
			       const QString& platenb,
			       const QString& username,
			       bool iswanted /*= false*/ )
  {
    std::stringstream	sstr;

    sstr << "INSERT INTO " << Table << "("
	 << Field_plateNumber << ", "
	 << Field_userName << ", "
	 << Field_isWanted
	 << ") VALUES ("
	 << "\"" << platenb.toStdString() << "\", "
	 << "\"" << username.toStdString() << "\", "
	 << ( (iswanted) ? ("1") : ("0") )
	 << ");";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString()
	     << ", query : " << sstr.str() );
      }
    else
      {

	if (ppPlate_out != NULL)
	  {
	    *ppPlate_out = NULL;

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
	      *ppPlate_out = new Plate( query.value(0).toInt() );
	  }

	return (true);
      }

    return (false);
  }

  void		Plate::Dealloc( Plate* pPlate )
  {
    delete pPlate;
  }

  void		Plate::EraseFromDatabase( Plate* pPlate )
  {
    std::stringstream	sstr;

    sstr << "DELETE FROM " << Table
	 << " WHERE " << Field_id << "=" << pPlate->m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
      }

    Dealloc( pPlate );
  }

  //////////////////////////////////////////////////////////////////////

  std::vector< Plate* >	Plate::GetAll()
  {
    std::vector< Plate* >	All_Plates;

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
	  All_Plates.push_back( new Plate( query.value(0).toInt() ) );
      }

    return (All_Plates);
  }

  //////////////////////////////////////////////////////////////////////

  Plate*	Plate::FindPlateByNumber(const QString& platenb)
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_id
	 << " FROM " << Table
	 << " WHERE " << Field_plateNumber << "=\"" << platenb.toStdString() << "\""
	 << " ORDER BY " << Field_id << " ASC"
	 << " LIMIT 1"
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	LOG( "QSqlQuery::exec, error : "
	     << query.lastError().text().toStdString() );
    return (NULL);
      }
    else if ( query.first() )
      return ( new Plate( query.value(0).toInt() ) );

    return (NULL);
  }

};
