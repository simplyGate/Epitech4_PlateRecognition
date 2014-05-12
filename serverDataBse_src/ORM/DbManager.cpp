

#include "DbManager.hpp"

#include <QSqlError>

#include <iostream>


namespace ORM
{

  DbManager*	DbManager::m_pInstance = NULL;

  bool	DbManager::OpenDb(const char* pDbName)
  {
    if (m_pInstance == NULL)
      {
	m_pInstance = new DbManager;

	m_pInstance->m_DataBase = QSqlDatabase::addDatabase("QSQLITE");
	m_pInstance->m_DataBase.setDatabaseName( QString(pDbName) );

	if (!m_pInstance->m_DataBase.open())
	  {
        std::cerr << "Fail to open the database \"" << pDbName << "\"." << std::endl
              << "Error :" << std::endl
              << m_pInstance->m_DataBase.lastError().text().toStdString() << std::endl;

	    delete m_pInstance, m_pInstance = NULL;
	  }
      }

    return (m_pInstance != NULL);
  }

  bool	DbManager::IsOpen()
  {
    return (m_pInstance != NULL);
  }

  void	DbManager::Close()
  {
    if (m_pInstance != NULL)
      {
	m_pInstance->m_DataBase.close();
	delete m_pInstance, m_pInstance = NULL;
      }
  }

  QSqlDatabase DbManager::getDb()
  {
    return (m_pInstance->m_DataBase);
  }

};
