

#ifndef	DBMANAGER_HPP
#define	DBMANAGER_HPP


#include <QSqlDatabase>


namespace ORM
{

  class	DbManager
  {
  private :

    static DbManager*	m_pInstance;

    QSqlDatabase	m_DataBase;

  private :

    DbManager() {};
    ~DbManager() {};

  public :

    static bool	OpenDb(const char* pDbName);
    static bool	IsOpen();
    static void	Close();
    static QSqlDatabase getDb();

  };

};


#endif	 // ORM_DBMANAGER_HPP

