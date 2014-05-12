

#include "Server.hpp"

#include "process_image/QtImageWindow.hpp"


#include <QImage>

#include <sstream>
#include <iostream>
#include <cstring>


#define	D_SERVER_LOG(log)			\
  {						\
    std::stringstream	sstr;			\
    sstr << log;				\
    AppendLog( sstr.str().c_str() );		\
  }


////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////


Server::t_client::t_client(QTcpSocket* pQTcpSocket)
  : s_sync_needed(false),

    s_pQTcpSocket(pQTcpSocket),
    s_pendingPacket_query( Client_to_Server::eUnknowQuery ),

    s_image_w(0), s_image_h(0),
    s_pathname_size(0),

    s_chunk_size(0),
    s_chunk_data(NULL)
{
}

void	Server::t_client::Reset()
{
  s_image_w = 0;
  s_image_h = 0;

  s_pathname_size = 0;
  s_pathname = "";

  s_pendingPacket_query = Client_to_Server::eUnknowQuery;
  s_previous_query = Client_to_Server::eUnknowQuery;

  s_chunk_size = 0;
  delete[] s_chunk_data, s_chunk_data = NULL;
  s_chunk_cursor = 0;
}

void	Server::t_client::AskSync(const std::string& message)
{
  s_sync_needed = true;
  Reset();

  QByteArray paquet;
  QDataStream out(&paquet, QIODevice::WriteOnly);

  unsigned short	str_size = message.size();

  unsigned char	query_byte = (unsigned char)Server_to_Client::eError;
  unsigned char	answer_byte = 0;

  out.writeRawData( (char*)&(query_byte), 1 );
  out.writeRawData( (char*)&(answer_byte), 1 );
  out.writeRawData( (char*)&str_size, 2 );
  out.writeRawData( message.c_str(), str_size );

  s_pQTcpSocket->write( paquet );
}


////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////
////////////////////////////////////


Server::Server()
  : m_pQTcpServer(NULL),
    m_isShutingDown(false)
{

  { // GUI

    m_pLabelStatus = new QLabel;
    m_pLabelStatus->setText( "Server OFF" );


    m_pPButtonQuit = new QPushButton(tr("Quit"));
    connect(m_pPButtonQuit, SIGNAL(clicked()), qApp, SLOT(quit()));

    m_pServeurPort = new QSpinBox;
    m_pServeurPort->setMinimumSize(QSize(60, 0));
    m_pServeurPort->setMinimum(1024);
    m_pServeurPort->setMaximum(65535);
    m_pServeurPort->setValue(4242);



    QPushButton*	pPButtonTest = new QPushButton(tr("Test de Popup de merde"));
    connect(pPButtonTest, SIGNAL(clicked()), this, SLOT(TestDirtyPopup()));
    pPButtonTest->hide();


    m_pPButtonListen = new QPushButton;
    m_pPButtonListen->setText("Listen");
    connect( m_pPButtonListen, SIGNAL(clicked()), this, SLOT(Listen_Stop()) );



    m_pStringListModelLogs = new QStringListModel(this);
    m_pListViewLogs = new QListView;
    m_pListViewLogs->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_pListViewLogs->setModel( m_pStringListModelLogs );


    QVBoxLayout*	pVLayout = new QVBoxLayout(this);

    pVLayout->addWidget( m_pListViewLogs );

    pVLayout->addWidget( m_pLabelStatus );

    QHBoxLayout*	pHLayout = new QHBoxLayout;
    pHLayout->addWidget( m_pServeurPort );
    pHLayout->addWidget( m_pPButtonListen );

    pVLayout->addLayout( pHLayout );

    pVLayout->addWidget( m_pPButtonQuit );

    pVLayout->addWidget( pPButtonTest );

    setLayout( pVLayout );

    setWindowTitle( tr("Licence Plate - Serveur") );
  } // GUI

  { // protocol

    m_tabProtoCallback[ Client_to_Server::eAskStatus ] = &Server::Command_AskStatus;
    m_tabProtoCallback[ Client_to_Server::eAskUpload ] = &Server::Command_AskUpload;
    m_tabProtoCallback[ Client_to_Server::eUpload ] = &Server::Command_Upload;
    m_tabProtoCallback[ Client_to_Server::eUploadFinished ] = &Server::Command_UploadFinished;
    m_tabProtoCallback[ Client_to_Server::eSync ] = &Server::Command_Sync;

  } // /protocol

  { // DataBase
    

  }

}



void	Server::TestDirtyPopup()
{
  QMessageBox::information(this, "Crève connard !", "Bonjour je suis une popup de merde !");
}



bool	Server::Command_AskStatus(t_client* pClient, QDataStream& in)
{
  {
    D_SERVER_LOG( "Command_AskStatus" );
  }

  {
    if ( pClient->s_previous_query == Client_to_Server::eAskUpload ||
	 pClient->s_previous_query == Client_to_Server::eUpload )
      {
	D_SERVER_LOG( "upload was already launched" );
	pClient->AskSync( "sync needed : upload was already launched" );
	return (false);
      }
  }

  QTcpSocket*	pCSocket = pClient->s_pQTcpSocket;

  { // size

    int	min_size_to_receive = (int)sizeof(unsigned short);

    if (pClient->s_pathname_size == 0)
      {

	if (pCSocket->bytesAvailable() < min_size_to_receive)
	  {
	    D_SERVER_LOG( "uncomplete size (" << pCSocket->bytesAvailable() << ")"
			  << " expected : " << min_size_to_receive );
	    return (false);
	  }

	in.readRawData( (char*)&(pClient->s_pathname_size), 2 );

	if (pClient->s_pathname_size == 0)
	  {
	    D_SERVER_LOG( "sync needed : null size for the pathname" );
	    pClient->AskSync( "sync needed : null size for the pathname" );
	    return (false);
	  }
      }

    if (pCSocket->bytesAvailable() < pClient->s_pathname_size)
      {
	D_SERVER_LOG( "uncomplete packet (" << pCSocket->bytesAvailable() << ")"
		      << " expected : " << pClient->s_pathname_size );
	return (false);
      }

  } // /size

  { // get

    int	size_to_read = pClient->s_pathname_size;

    char*	pData = new char[ size_to_read + 1 ];
    memset( pData, 0, size_to_read + 1 );

    in.readRawData( pData, size_to_read );

    pData[ size_to_read ] = '\0';

    D_SERVER_LOG( "pathname received : " << pData );

    { // send

      QString		i_pathname(pData);
      std::string	message( "pathname received" );

      QByteArray paquet;
      QDataStream out(&paquet, QIODevice::WriteOnly);

      { // write

	unsigned short	str_size = message.size();

	unsigned char	query_byte = (unsigned char)Server_to_Client::eAnswerStatus;
	unsigned char	answer_byte = 1;

	{ // Data Base check

	  if (!m_DataBase->IsOpen())
	    {
	      message = "Data Base is not ready";
	      D_SERVER_LOG(message);
	      str_size = message.size();
	      answer_byte = 0;
	    }

	  else
	    {
	      ORM::Image*	pImage = ORM::Image::GetImageByPathName(i_pathname);

	      if (pImage != NULL)
		{
		  answer_byte = pImage->GetStatus();

		  if (answer_byte == 2)
		    {
		      ORM::Plate*	pPlate = pImage->GetAttachedPlate();

		      QString	plate_nb = pPlate->GetPlateNumber();
		      QString	plate_usr = pPlate->GetUserName();
		      bool	is_wanted = pPlate->IsWanted();

		      std::stringstream	sstr;
		      sstr << "[" << "plate number : " << plate_nb.toStdString() << "]"
			   << "[" << "username : " << plate_usr.toStdString() << "]"
			   << "[" << "wanted : " << (is_wanted ? "true" : "false") << "]";

		      message = sstr.str();
		      str_size = message.size();
		    }

		}

	      else
		{
		  message = "Image Does not exists";
		  D_SERVER_LOG(message);
		  str_size = message.size();
		  answer_byte = 0;
		}
	    }

	} // /Data Base check

	out.writeRawData( (char*)&(query_byte), 1 );
	out.writeRawData( (char*)&(answer_byte), 1 );
	out.writeRawData( (char*)&str_size, 2 );
	out.writeRawData( message.c_str(), str_size );

      } // /write

      {
	pCSocket->write( paquet );
      }

      {
	pClient->s_pathname_size = 0;
	pClient->s_pathname = "";

	pClient->s_previous_query = pClient->s_pendingPacket_query;
	pClient->s_pendingPacket_query = Client_to_Server::eUnknowQuery;
      }

    } // /send

    delete pData;

  } // /get

  return (true);
}

bool	Server::Command_AskUpload(t_client* pClient, QDataStream& in)
{
  {
    D_SERVER_LOG( "Command_AskUpload" );
  }

  {
    if ( pClient->s_previous_query == Client_to_Server::eAskUpload ||
	 pClient->s_previous_query == Client_to_Server::eUpload )
      {
	D_SERVER_LOG( "upload was already launched" );
	pClient->AskSync( "sync needed : upload was already launched" );
	return (false);
      }
  }


  /// RECEIVE : pathname (size + char)
  /// SEND : query + answer + message (size + char)

  QTcpSocket*	pCSocket = pClient->s_pQTcpSocket;

  { // size

    int	min_size_to_receive = ((int)sizeof(unsigned short)) * 3;

    // D_SERVER_LOG("pCSocket->bytesAvailable() : " << pCSocket->bytesAvailable());

    // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    if (pClient->s_pathname_size == 0)
      {
	// On n'a pas reçu la taille du message en entier
	if (pCSocket->bytesAvailable() < min_size_to_receive)
	  {
	    D_SERVER_LOG( "uncomplete size (" << pCSocket->bytesAvailable() << ")"
			  << " expected : " << min_size_to_receive );
	    return (false);
	  }

	// Si on a reçu la taille du message en entier, on la récupère
	// in.readRawData( (char*)&(pClient->s_pendingPacket_size), 2 );
	in.readRawData( (char*)&(pClient->s_image_w), 2 );
	in.readRawData( (char*)&(pClient->s_image_h), 2 );
	in.readRawData( (char*)&(pClient->s_pathname_size), 2 );

	// D_SERVER_LOG("image width received : " << pClient->s_image_w);
	// D_SERVER_LOG("image height received : " << pClient->s_image_h);
	// D_SERVER_LOG("pathname size received : " << pClient->s_pathname_size);

	if ( pClient->s_image_w == 0 || pClient->s_image_w > 2000 ||
	     pClient->s_image_h == 0 || pClient->s_image_h > 2000 ||
	     pClient->s_pathname_size == 0 )
	  {
	    D_SERVER_LOG( "bad values" );
	    // pClient->Reset();

	    if ( pClient->s_image_w == 0 || pClient->s_image_w > 2000 )
	      pClient->AskSync( "sync needed : bad image width" );
	    else if ( pClient->s_image_h == 0 || pClient->s_image_h > 2000 )
	      pClient->AskSync( "sync needed : bad image height" );
	    else if ( pClient->s_pathname_size == 0 )
	      pClient->AskSync( "sync needed : null size for the pathname" );

	    return (false);
	  }

      }

    // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
    if (pCSocket->bytesAvailable() < pClient->s_pathname_size)
      {

	D_SERVER_LOG( "uncomplete packet (" << pCSocket->bytesAvailable() << ")"
		      << " expected : " << pClient->s_pathname_size );

	// Si on n'a pas encore tout reçu, on arrête la méthode
	return (false);
      }

    D_SERVER_LOG( "received " << pClient->s_pathname_size );

  } // /size

  { // get

    int	size_to_read = pClient->s_pathname_size;

    char*	pData = new char[ size_to_read + 1 ];
    memset( pData, 0, size_to_read + 1 );

    in.readRawData( pData, size_to_read );

    pData[ size_to_read ] = '\0';

    D_SERVER_LOG( "pathname received : " << pData );

    pClient->s_pathname = std::string( pData );

    { // send

      QString		i_pathname(pData);
      std::string	message( "upload authorized" );

      QByteArray paquet;
      QDataStream out(&paquet, QIODevice::WriteOnly);

      unsigned char	answer_byte = 1;
      { // write

	unsigned short	str_size = message.size();

	unsigned char	query_byte = (unsigned char)Server_to_Client::eAuthorizeUpload;


	{ // Data Base check
	  if (!m_DataBase->IsOpen())
	    {
	      message = "Data Base is not ready";
	      D_SERVER_LOG(message);
	      str_size = message.size();
	      answer_byte = 0;
	    }
	  else
	    {
	      if (ORM::Image::GetImageByPathName(i_pathname) != NULL)
		{
		  message = "Image already in data base";
		  D_SERVER_LOG(message);
		  str_size = message.size();
		  answer_byte = 0;
		}
	      else
		answer_byte = 1;
	    }
	}

	out.writeRawData( (char*)&(query_byte), 1 );
	out.writeRawData( (char*)&(answer_byte), 1 );
	out.writeRawData( (char*)&str_size, 2 );
	out.writeRawData( message.c_str(), str_size );

	pCSocket->write(paquet);

	// if (answer_byte == 0)
	//   {
	//     delete pData;
	//     return (true);
	//   }
      } // /write

      // {
      // 	pCSocket->write( paquet );
      // }
      
      if (answer_byte == 1)
	{

	  int	byte_size_image = pClient->s_image_w * pClient->s_image_h * 3;

	  pClient->s_chunk_size = 0;
	  pClient->s_chunk_data = new char[ byte_size_image ];
	  memset( pClient->s_chunk_data, 0, byte_size_image );
	  pClient->s_chunk_cursor = 0;

	  pClient->s_previous_query = pClient->s_pendingPacket_query;
	}

      else
	{
	  pClient->s_pathname = "";
	  pClient->s_pathname_size = 0;
	  pClient->s_image_w = 0;
	  pClient->s_image_h = 0;

	  pClient->s_previous_query = Client_to_Server::eUnknowQuery;
	}

      pClient->s_pendingPacket_query = Client_to_Server::eUnknowQuery;
    } // /send

    delete pData;

  } // /get

  return (true);

  // pClient->s_pendingPacket_query = Client_to_Server::eUnknowQuery;
}

bool	Server::Command_Upload(t_client* pClient, QDataStream& in)
{
  {
    // D_SERVER_LOG( "Command_Upload" );
  }

  {
    if (pClient->s_previous_query != Client_to_Server::eAskUpload &&
	pClient->s_previous_query != Client_to_Server::eUpload )
      {
	D_SERVER_LOG( "must ask for the upload first" );
	pClient->AskSync( "sync needed : must ask for the upload first" );
	return (false);
      }
  }


  /// RECEIVE : size + DATA[size]
  /// SEND : nothing

  QTcpSocket*	pCSocket = pClient->s_pQTcpSocket;

  { // size

    int	min_size_to_receive = (int)sizeof(unsigned short);

    // D_SERVER_LOG("pCSocket->bytesAvailable() : " << pCSocket->bytesAvailable());

    // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
    if (pClient->s_chunk_size == 0)
      {
	// On n'a pas reçu la taille du message en entier
	if (pCSocket->bytesAvailable() < min_size_to_receive)
	  {
	    D_SERVER_LOG( "uncomplete size (" << pCSocket->bytesAvailable() << ")"
			  << " expected : " << min_size_to_receive );
	    return (false);
	  }

	// Si on a reçu la taille du message en entier, on la récupère
	in.readRawData( (char*)&(pClient->s_chunk_size), 2 );


	// if ( pClient->s_chunk_size == 0 || pClient->s_chunk_size > 1024 )
	if ( pClient->s_chunk_size == 0 )
	  {
	    D_SERVER_LOG( "bad values" );

	    pClient->AskSync( "sync needed : bad chunk size" );

	    return (false);
	  }

	int	image_size = pClient->s_image_w * pClient->s_image_h * 3;

	// D_SERVER_LOG( "image_w : " << pClient->s_image_w );
	// D_SERVER_LOG( "image_h : " << pClient->s_image_h );

	// D_SERVER_LOG( "image_size : " << image_size );
	// D_SERVER_LOG( "chunk_cursor : " << pClient->s_chunk_cursor << " / " << image_size );
	// D_SERVER_LOG( "chunk_size : " << pClient->s_chunk_size );
	// D_SERVER_LOG( "cursor + size : " << pClient->s_chunk_cursor + pClient->s_chunk_size );

	if (pClient->s_chunk_cursor + (int)pClient->s_chunk_size > image_size)
	  {
	    D_SERVER_LOG( "chunk larger than the image size" );

	    pClient->AskSync( "sync needed : too much data sended (> image size)" );

	    return (false);
	  }

	// D_SERVER_LOG("image width received : " << pClient->s_chunk_size);
      }

    // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
    if (pCSocket->bytesAvailable() < pClient->s_chunk_size)
      {

	D_SERVER_LOG( "uncomplete packet (" << pCSocket->bytesAvailable() << ")"
		      << " expected : " << pClient->s_chunk_size );

	// Si on n'a pas encore tout reçu, on arrête la méthode
	return (false);
      }

    // D_SERVER_LOG( "received " << pClient->s_chunk_size );

  } // /size


  { // get

    // char*	begin = pClient->s_chunk_data + pClient->s_chunk_cursor;
    char*	begin = &(pClient->s_chunk_data[ pClient->s_chunk_cursor ]);
    // memset( begin, 0, pClient->s_chunk_size );
    in.readRawData( begin, pClient->s_chunk_size );

    pClient->s_chunk_cursor += pClient->s_chunk_size;

    // unsigned int	image_size = pClient->s_image_w * pClient->s_image_h * 3;
    // D_SERVER_LOG( "chunk_cursor : " << pClient->s_chunk_cursor << " / " << image_size );

    {
      pClient->s_chunk_size = 0;
      pClient->s_previous_query = pClient->s_pendingPacket_query;
      pClient->s_pendingPacket_query = Client_to_Server::eUnknowQuery;
    }

  } // /get



  { // send

    std::string	message( "next chunk pleas" );

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    { // write

      unsigned short	str_size = message.size();

      unsigned char	query_byte = (unsigned char)Server_to_Client::eUploadNext;
      unsigned char	answer_byte = 1;

      out.writeRawData( (char*)&(query_byte), 1 );
      out.writeRawData( (char*)&(answer_byte), 1 );
      out.writeRawData( (char*)&str_size, 2 );

      out.writeRawData( message.c_str(), str_size );

    } // /write

    {
      pCSocket->write( paquet );

      // for ( t_clients_itr itr = m_mapClients.begin();
      // 	    itr != m_mapClients.end();
      // 	    ++itr )
      // 	itr->second->s_pQTcpSocket->write( paquet );
    }

  } // /send


  return (true);
}

bool	Server::Command_UploadFinished(t_client* pClient, QDataStream& in)
{
  bool	image_save = false;;

  static_cast<void>(in);

  {
    D_SERVER_LOG( "Command_UploadFinished" );
  }

  {
    if (pClient->s_previous_query != Client_to_Server::eUpload)
      {
	D_SERVER_LOG( "confirm an upload that never started" );
	pClient->AskSync( "sync needed : cannot confirm an upload that never started" );
	return (false);
      }
  }

  int	image_size = pClient->s_image_w * pClient->s_image_h * 3;
  if (pClient->s_chunk_cursor != image_size)
    {
      D_SERVER_LOG( "unfinished upload" );
      D_SERVER_LOG( "cursor : " << pClient->s_chunk_cursor );
      D_SERVER_LOG( "size : " << image_size );
      pClient->AskSync( "sync needed : ask for a confirmation of an unfinished upload" );
      return (false);
    }

  { // save

    QString	i_pathname(pClient->s_pathname.c_str());

    std::cerr << "W : " << pClient->s_image_w << std::endl;
    std::cerr << "H : " << pClient->s_image_h << std::endl;
    std::cerr << "name : " << pClient->s_pathname << std::endl;

    QImage  img_dest( (uchar*)pClient->s_chunk_data,
		      (int)pClient->s_image_w,
		      (int)pClient->s_image_h,
		      QImage::Format_RGB888 );

    if ( img_dest.save( QString(pClient->s_pathname.c_str()) ) )
      {
	std::cerr << "success" << std::endl;

	// ORM::Image::Create(NULL, i_pathname);


	//// TODO
	// Run_ProcessImage(QImage& qimg, bool update_gui = false);
	QtImageWindow::Run_ProcessImage(img_dest, pClient->s_pathname);
	//// TODO


	image_save = true;
      }
    else
      {
	std::cerr << "failure" << std::endl;
	image_save = false;
      }

  } // /save

  { // send

    std::string	message( "upload seem's ok" );

    QByteArray	paquet;
    QDataStream	out(&paquet, QIODevice::WriteOnly);

    { // write

      unsigned short	str_size = message.size();

      unsigned char	query_byte = (unsigned char)Server_to_Client::eConfirmUpload;
      unsigned char	answer_byte = 1;

      if (!image_save)
	{
	  message = "upload failed";
	  answer_byte = 0;
	}

      out.writeRawData( (char*)&(query_byte), 1 );
      out.writeRawData( (char*)&(answer_byte), 1 );
      out.writeRawData( (char*)&str_size, 2 );

      out.writeRawData( message.c_str(), str_size );

    } // /write

    {
      QTcpSocket*	pCSocket = pClient->s_pQTcpSocket;
      pCSocket->write( paquet );

      // for ( t_clients_itr itr = m_mapClients.begin();
      // 	    itr != m_mapClients.end();
      // 	    ++itr )
      // 	itr->second->s_pQTcpSocket->write( paquet );
    }

    {
      // pClient->s_chunk_size = 0;
      // pClient->s_chunk_data = new char[ pClient->s_image_w * pClient->s_image_h ];
      // pClient->s_chunk_cursor = 0;

      // s_previous_query = pClient->s_pendingPacket_query;
      // pClient->s_pendingPacket_query = Client_to_Server::eUnknowQuery;


      pClient->Reset();
    }

  } // /send

  return (true);
}

bool	Server::Command_Sync(t_client* pClient, QDataStream& in)
{
  static_cast<void>(in);

  {
    D_SERVER_LOG( "Command_UploadFinished" );
  }

  pClient->Reset();
  pClient->s_sync_needed = false;

  return (true);
}


Server::~Server()
{
  // for ( t_clients_itr itr = m_mapClients.begin();
  // 	itr != m_mapClients.end();
  // 	++itr )
  //   {
  //     // itr->second->s_pQTcpSocket->close();
  //     // delete itr->second;
  //   }

  // m_mapClients.clear();

  delete m_pQTcpServer;
}

void	Server::Listen_Stop()
{

  std::cerr << "Listen_Stop() begin" << std::endl;

  if (m_pQTcpServer == NULL)
    {

      int	choosen_port = m_pServeurPort->value();

      {
	D_SERVER_LOG( "Try to listen (" << choosen_port << ")" );
      }


      // Gestion du serveur
      m_pQTcpServer = new QTcpServer;

      // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585
      if ( !m_pQTcpServer->listen(QHostAddress::Any, choosen_port) )
	{
	  QString	qstr_serverStatus;

	  qstr_serverStatus += tr("Server OFF, Error :<br />");
	  qstr_serverStatus += m_pQTcpServer->errorString();


	  {
	    D_SERVER_LOG( "Listen failure :" << std::endl
			  << m_pQTcpServer->errorString().toStdString() )
	      }


	  m_pLabelStatus->setText( qstr_serverStatus );
	}
      else
	{
	  QString	qstr_serverStatus;

	  qstr_serverStatus += tr("Server ON, port : <strong>");
	  qstr_serverStatus += QString::number(m_pQTcpServer->serverPort());
	  qstr_serverStatus += tr("</strong>.");

	  {
	    D_SERVER_LOG( tr("Listen success").toStdString() );
	  }

	  // Si le serveur a été démarré correctement
	  m_pLabelStatus->setText( qstr_serverStatus );

	  connect( m_pQTcpServer, SIGNAL(newConnection()), this, SLOT(NewClient()) );


	  m_pPButtonListen->setText("Stop");

	  m_isShutingDown = false;
	}

    }
  else
    {
      m_isShutingDown = true;


      std::cerr << "delete clients" << std::endl;

      for ( t_clients_itr itr = m_mapClients.begin();
	    itr != m_mapClients.end();
	    ++itr )
	{
	  itr->second->s_pQTcpSocket->close();
	  delete itr->second;
	}

      std::cerr << "map clear" << std::endl;

      m_mapClients.clear();

      std::cerr << "clients deleted" << std::endl;

      std::cerr << "delete server" << std::endl;

      delete m_pQTcpServer, m_pQTcpServer = NULL;

      std::cerr << "server deleted" << std::endl;

      m_pLabelStatus->setText( "Server OFF" );
      m_pPButtonListen->setText("Listen");


      {
	// m_pTextEditLogs->append( tr("Server shutdown") );
	D_SERVER_LOG( tr("Server shutdown").toStdString() );
      }

    }

  std::cerr << "Listen_Stop() end" << std::endl;

}

void	Server::NewClient()
{
  if (m_isShutingDown)
    return;

  QTcpSocket*	pNewClient = m_pQTcpServer->nextPendingConnection();

  {
    D_SERVER_LOG( "New client (" << pNewClient->peerAddress().toString().toStdString() << ")" );
  }

  m_mapClients[ pNewClient ] = new t_client( pNewClient );

  connect( pNewClient, SIGNAL(readyRead()), this, SLOT(Receive()) );
  connect( pNewClient, SIGNAL(disconnected()), this, SLOT(RemoveClient()) );
}

void	Server::Receive()
{
  if (m_isShutingDown)
    return;

  // D_SERVER_LOG( "New packet received" );

  QTcpSocket*	pClientSocket = qobject_cast<QTcpSocket *>(sender());

  if (pClientSocket == NULL)
    {
      D_SERVER_LOG( "socket not retrieved" );
      return;
    }

  t_clients_itr	found_itr = m_mapClients.find( pClientSocket );
  if (found_itr == m_mapClients.end())
    {
      D_SERVER_LOG( "client not found" );
      return;
    }


  t_client*	pClient = found_itr->second;


  QDataStream	in(pClientSocket);


  bool	repeat = false;

  do
    {

      if ( pClient->s_pendingPacket_query == Client_to_Server::eUnknowQuery )
	{

	  if ( pClientSocket->bytesAvailable() < (int)sizeof(unsigned char))
	    {
	      // D_SERVER_LOG( "no even one octet for the query..." );
	      return;
	    }

	  char	query = 0;
	  in.readRawData( &query, 1 );

	  switch (query)
	    {

	    case Client_to_Server::eAskStatus :
	    case Client_to_Server::eAskUpload :
	    case Client_to_Server::eUpload :
	    case Client_to_Server::eUploadFinished :
	    case Client_to_Server::eSync :
	      pClient->s_pendingPacket_query = (Client_to_Server::e_Client_to_Server)query;
	      break;

	    case Client_to_Server::eUnknowQuery :
	    default :

	      {
		std::stringstream	sstr;
		sstr << "Invalid / unknow query : " << query;
		D_SERVER_LOG( sstr.str().c_str() );
	      }

	      return;

	    }

	} // if ( pClient->s_pendingPacket_query == Client_to_Server::eUnknowQuery )


      if (pClient->s_sync_needed == true)
	{
	  D_SERVER_LOG( "resync first" );
	  pClient->AskSync( "sync needed : resync first" );
	  return;
	}


      repeat = ( this->*( m_tabProtoCallback[ pClient->s_pendingPacket_query ] ) )( pClient, in );

    }
  while (repeat);

}

void	Server::RemoveClient()
{
  QTcpSocket*	pClientSocket = qobject_cast<QTcpSocket *>(sender());

  if (!m_isShutingDown)
    {
      if (pClientSocket == NULL)
	{
	  D_SERVER_LOG( "socket not retrieved" );
	  return;
	}

      t_clients_itr	found_itr = m_mapClients.find( pClientSocket );

      if (found_itr != m_mapClients.end())
	m_mapClients.erase( found_itr );
      else
	D_SERVER_LOG( "client not found" );
    }

  pClientSocket->deleteLater();

  if (!m_isShutingDown)
    D_SERVER_LOG( "client removed" );
}

void	Server::AppendLog( const std::string& log )
{
  std::cerr << "[LOG] : " << log << std::endl;

  {
    m_pStringListModelLogs->insertRow( 0 );

    QModelIndex	index = m_pStringListModelLogs->index( 0 );
    m_pStringListModelLogs->setData( index, QVariant( QString(log.c_str()) ) );
  }

  {
    int	row_count = m_pStringListModelLogs->rowCount();
    int	row_limit = 20;

    if (row_count > row_limit)
      m_pStringListModelLogs->removeRows( row_count - 1, 1 );
  }
}
