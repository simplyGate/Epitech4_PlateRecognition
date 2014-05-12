#ifndef		SERVER_HPP
#define		SERVER_HPP

#include	<QtWidgets>
#include	<QtNetwork>
#include	<map>

#include	"Protocole.hpp"
#include	"ORM/ORM.hpp"

class		Server : public QWidget
{
  Q_OBJECT

  private :

  struct	t_client
  {
  public :

    bool	s_sync_needed;

    QTcpSocket*	s_pQTcpSocket;

    Client_to_Server::e_Client_to_Server	s_pendingPacket_query;
    Client_to_Server::e_Client_to_Server	s_previous_query;

    unsigned short	s_image_w, s_image_h;
    unsigned short	s_pathname_size;
    std::string		s_pathname;

    unsigned short	s_chunk_size;
    char*		s_chunk_data;
    int			s_chunk_cursor;

  public :

    t_client(QTcpSocket* pQTcpSocket);

  public :

    void	Reset();
    void	AskSync(const std::string& message);

  };

private :

  typedef bool	(Server::*pFunc_server)(t_client* pClient, QDataStream& in);

  pFunc_server	m_tabProtoCallback[ Client_to_Server::eCount ];


  bool	Command_AskStatus(t_client* pClient, QDataStream& in);
  bool	Command_AskUpload(t_client* pClient, QDataStream& in);
  bool	Command_Upload(t_client* pClient, QDataStream& in);
  bool	Command_UploadFinished(t_client* pClient, QDataStream& in);
  bool	Command_Sync(t_client* pClient, QDataStream& in);

public : // ctor(s) / dtor

  Server();
  ~Server();

private slots :

  void	TestDirtyPopup();

  void	Listen_Stop();

  void	NewClient();
  void	Receive();
  void	RemoveClient();

public : // method(s) : Network

  void	AppendLog( const std::string& log );

private : // GUI

  QSpinBox*	m_pServeurPort;
  QPushButton*	m_pPButtonListen;

  QLabel*	m_pLabelStatus;
  QPushButton*	m_pPButtonQuit;

  QListView*		m_pListViewLogs;
  QStringListModel*	m_pStringListModelLogs;

private : // Network

  QTcpServer*	m_pQTcpServer;

  typedef std::map< QTcpSocket*, t_client* >	t_clients;
  typedef t_clients::iterator			t_clients_itr;

  t_clients	m_mapClients;

  bool		m_isShutingDown;

private : // DataBase

  ORM::DbManager*	m_DataBase;
};


#endif	// SERVER_HPP

