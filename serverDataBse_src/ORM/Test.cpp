

#include "Test.hpp"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>

#include <iostream>
#include <sstream>


namespace ORM
{

  const std::string	myTest::Field_id("id");
  const std::string	myTest::Field_name("name");
  const std::string	myTest::Field_age("age");
  const std::string	myTest::Field_create_date("create_date");
  const std::string	myTest::Field_comments("comments");
  const std::string	myTest::Table("TABLE_TEST");

  myTest*	myTest::Create( const QString& name, int age, const QString& comments )
  {
    std::stringstream	sstr;

    QString	curr_date = QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss" );

    sstr << "INSERT INTO " << Table << "("
	 << Field_name << ", "
	 << Field_age << ", "
	 << Field_create_date << ", "
	 << Field_comments
	 << ") VALUES ("
	 << "\"" << name.toStdString() << "\", "
	 << age << ", "
	 << "\"" << curr_date.toStdString() << "\", "
	 << "\"" << comments.toStdString() << "\""
	 << ");";

    QString	qstr_query_in( sstr.str().c_str() );

    std::cerr << "QUERY [ " << sstr.str() << " ]" << std::endl;

    std::cerr << "<1" << std::endl;

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }
    else
      {
	std::cerr << "2" << std::endl;

	std::stringstream	sstr2;

	sstr2 << "SELECT " << Field_id
	      << " FROM " << Table
	      << " ORDER BY " << Field_id << " DESC"
	      << " LIMIT 1"
	      << ";";

	QString	qstr_query_in2( sstr2.str().c_str() );

	QSqlQuery	query2;
	if ( !query2.exec( qstr_query_in2 ) )
	  {
	    std::cerr << "query.exec" << std::endl
		      << query2.lastError().text().toStdString();
	  }
	else if ( query2.first() )
	  {
	    std::cerr << "OK : " << query2.value(0).toInt() << std::endl;

	    return (new myTest( query2.value(0).toInt() ) );
	  }
      }

    return (NULL);
  }

  std::vector<myTest*>	myTest::GetAll()
  {
    std::vector<myTest*>	All_Tests;


    std::stringstream	sstr;

    sstr << "SELECT " << Field_id
	 << " FROM " << Table
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }
    else
      {
	while (query.next())
	  All_Tests.push_back( new myTest( query.value(0).toInt() ) );
      }

    return (All_Tests);
  }

  void	myTest::Dealloc( myTest* pTest_to_delete )
  {
    delete pTest_to_delete;
  }

  void	myTest::EraseFromDatabase( myTest* pTest_to_delete )
  {
    std::stringstream	sstr;

    sstr << "DELETE FROM " << Table
	 << " WHERE " << Field_id << "=" << pTest_to_delete->m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }

    Dealloc( pTest_to_delete );
  }

  void	myTest::Dealloc( std::vector<myTest*>& pTests_to_delete )
  {
    for ( std::vector<myTest*>::iterator itr = pTests_to_delete.begin();
	  itr != pTests_to_delete.end();
	  ++itr )
      delete *itr;
  }

  void	myTest::EraseFromDatabase( std::vector<myTest*>& pTests_to_delete )
  {
    if ( pTests_to_delete.empty() )
      return;

    std::stringstream	sstr;

    sstr << "DELETE FROM " << Table
	 << " WHERE ";

    for ( unsigned int i = 0; i < pTests_to_delete.size(); ++i )
      {
	if (i > 0)
	  sstr << " OR ";

	sstr << Field_id << "=" << pTests_to_delete[i]->m_id;

	delete pTests_to_delete[i];
      }

    sstr << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }
  }

  ////////////////////////////////////////////

  QString	myTest::GetName() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_name
	 << " FROM " << Table
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }
    else if ( query.first() )
      return query.value(0).toString();

    return QString("");
  }

  int	myTest::GetAge() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_age
	 << " FROM " << Table
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }
    else if ( query.first() )
      return query.value(0).toInt();

    return (-1);
  }

  QDateTime	myTest::GetCreateDate() const
  {
    QString	date = "";


    std::stringstream	sstr;

    sstr << "SELECT " << Field_create_date
	 << " FROM " << Table
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }
    else if ( query.first() )
      date = query.value(0).toString();

    std::cerr << date.toStdString() << std::endl;

    return QDateTime::fromString( date, "yyyy-MM-dd hh:mm:ss" );
  }

  QString	myTest::GetComments() const
  {
    std::stringstream	sstr;

    sstr << "SELECT " << Field_comments
	 << " FROM " << Table
	 << " WHERE " << Field_id << "=" << m_id
	 << ";";

    QString	qstr_query_in( sstr.str().c_str() );

    QSqlQuery	query;
    if ( !query.exec( qstr_query_in ) )
      {
	std::cerr << "query.exec" << std::endl
		  << query.lastError().text().toStdString();
      }
    else if ( query.first() )
      return query.value(0).toString();

    return "";
  }

};

