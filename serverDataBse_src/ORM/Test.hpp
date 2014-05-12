

#ifndef	ORM_TEST_HPP
#define	ORM_TEST_HPP


// #include <QSqlQuery>
#include <QString>
#include <QDateTime>

#include <vector>


namespace ORM
{

  class	myTest
  {
  private :

    static const std::string	Field_id;
    static const std::string	Field_name;
    static const std::string	Field_age;
    static const std::string	Field_create_date;
    static const std::string	Field_comments;
    static const std::string	Table;

  private :

    int	m_id;

  private :

    myTest(int id) : m_id(id) {}
    ~myTest() {}

  public :

    static myTest*	Create( const QString& name, int age, const QString& comments );

    static std::vector<myTest*>	GetAll();

    static void	Dealloc( myTest* pTest_to_delete );
    static void	EraseFromDatabase( myTest* pTest_to_delete );

    static void	Dealloc( std::vector<myTest*>& pTests_to_delete );
    static void	EraseFromDatabase( std::vector<myTest*>& pTests_to_delete );

  public :

    QString	GetName() const;
    int		GetAge() const;
    QDateTime	GetCreateDate() const;
    QString	GetComments() const;

  };

};


#endif

