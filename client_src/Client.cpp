

#include "Client.hpp"

#include <iostream>
#include <sstream>


// Ctor of the client
FenClient::FenClient()
{
  setupUi(this);
  m_socket = new QTcpSocket(this);
  connect(m_socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
  connect(m_socket, SIGNAL(connected()), this, SLOT(connecte()));
  connect(m_socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
  connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));
  m_tailleMessage = 0;
  m_answerByte = 0;
  m_isquery = -1;

  m_protocol[Server_to_Client::eAnswerStatus] = &FenClient::Command_AnswerStatus;
  m_protocol[Server_to_Client::eAuthorizeUpload] = &FenClient::Command_Upload;
  m_protocol[Server_to_Client::eConfirmUpload] = &FenClient::Command_ConfirmUpload;
  m_protocol[Server_to_Client::eUploadNext] = &FenClient::Command_UploadNext;
  m_protocol[Server_to_Client::eError] = &FenClient::Command_Error;
}

void
FenClient::Command_Error(QString ReadMessage)
{
  QByteArray      paquet;
  QDataStream     out(&paquet, QIODevice::WriteOnly);
  unsigned char   key = Client_to_Server::eSync;

  listeMessages->append(tr("<em>ERROR From Server:</em>"));
  listeMessages->append(ReadMessage);
  std::cerr << "ERROR : " << ReadMessage.toStdString() << std::endl;
  out.writeRawData((char*)&key, 1);
  m_socket->write(paquet);
  listeMessages->append(tr("<em>Syncronisation finished</em>"));
}

void
FenClient::Command_AnswerStatus(QString msg)
{
  listeMessages->append(tr("<em>Status : </em>"));

  switch(m_answerByte)
    {
    case 0:
      listeMessages->append(tr("<em>Image is not in data base </em>"));
      break;
    case 1:
      listeMessages->append(tr("<em>Unprocessed </em>"));
      break;
    case 2:
      listeMessages->append(tr("<em>Processed Success </em>"));
      listeMessages->append(msg);
      break;
    case 3:
      listeMessages->append(tr("<em>Processed Failure </em>"));
      break;
    }
}

void
FenClient::Command_Upload(QString ReadMessage)
{
  listeMessages->append(tr("<em>Uploading :</em>"));
  listeMessages->append(ReadMessage);

  if (m_answerByte == 0)
    {
      std::cerr << "Quitting Upload" << std::endl;
      return ;
    }

  m_cursor = 0;

  pushButton->setEnabled(false);
  boutonEnvoyer->setEnabled(false);
  btnSend->setEnabled(false);

  QImage::Format	format = m_image.format();
  if (format != QImage::Format_RGB888)
    {
      QImage		tmpimage = m_image.convertToFormat(QImage::Format_RGB888);
      m_image = tmpimage;
    }

  Command_UploadNext(ReadMessage);
}

void
FenClient::Command_UploadNext(QString)
{
  unsigned short	chunksize = 1024;
  unsigned char		key = Client_to_Server::eUpload;

  unsigned char*	bitsimage = m_image.bits();
  unsigned int		imagetotalsize = m_image.width() * m_image.height() * 3;

  if (m_cursor == imagetotalsize)
    {
      finishedUploading();
      return;
    }

  QByteArray	paquet;
  QDataStream	out(&paquet, QIODevice::WriteOnly);

  unsigned short	curr_size = chunksize;

  if (m_cursor + chunksize > imagetotalsize)
    curr_size = imagetotalsize - m_cursor;

  unsigned char*	curr_begin = bitsimage + m_cursor;

  out.writeRawData((char*)&key, 1);
  out.writeRawData((char*)&curr_size, 2);
  out.writeRawData((char*)curr_begin, curr_size);
  m_socket->write(paquet);
  m_cursor += curr_size;
}

void
FenClient::Command_ConfirmUpload(QString ReadMessage)
{
  listeMessages->append(tr("<em>Uploading Finished ::</em>"));
  listeMessages->append(ReadMessage);

  pushButton->setEnabled(true);
  boutonEnvoyer->setEnabled(true);
  btnSend->setEnabled(true);
}

// Connection to the server
void
FenClient::on_boutonConnexion_clicked()
{
  listeMessages->append(tr("<em>Connection in progress...</em>"));
  boutonConnexion->setEnabled(false);
  pushButton->setEnabled(true);
  m_socket->abort();                                                  // Removing old connection
  m_socket->connectToHost(serveurIP->text(), serveurPort->value());   // Connecting to the server
}


namespace
{

  std::string	l_getFilename(const std::string& s)
  {
    char        sep = '/';
    char        sepExt='.';

#ifdef _WIN32
    sep = '\\';
#endif

    std::cerr << "l_getFilename : " << s << std::endl;

    size_t i = s.rfind(sep, s.length( ));

    // if (i != std::string::npos)
    {
      std::string fn(s);

      // if (i != std::string::npos)
      //        fn = (s.substr(i+1, s.length( ) - i));

      size_t j = fn.rfind( sepExt, fn.length() );

      if (j != std::string::npos)
        return fn.substr(0,j);
      else
        return fn;
    }
    // else
    //   return "";
  }

};



// Asking the Status of the pictures to the server
void
FenClient::on_boutonEnvoyer_clicked()
{
  std::string	str_file = message->text().toStdString();

  std::size_t	found = str_file.find_last_of("/");
  if (found != std::string::npos)
    str_file = str_file.substr(found + 1);


  ///
  str_file = l_getFilename( str_file );
  std::stringstream	sstr;
  sstr << str_file << ".png";
  str_file = sstr.str();
  ///


  QByteArray      paquet;
  QDataStream     out(&paquet, QIODevice::WriteOnly);

  const char*     messageToSend = str_file.c_str();
  unsigned short  tmp_size = str_file.size();
  unsigned char   key = Client_to_Server::eAskStatus;

  out.writeRawData((char*)&key, 1);
  out.writeRawData((char*)&tmp_size, 2);
  out.writeRawData(messageToSend, tmp_size);
  m_socket->write(paquet);
}

// Browsing the file the search an image
void
FenClient::on_pushButton_clicked()
{
  message->clear();
  message->setFocus();

  QStringList	fileNames = QFileDialog::getOpenFileNames( this,
							   tr("Open File"),
							   "/path/to/file/",
							   tr("Image (*.jpg *.bmp *.png)") );

  if (!fileNames.empty())
    {
      QString	text = fileNames.first();
      message->setText(text);
      if (m_image.load(message->text()) == false)
	  listeMessages->append(tr("<em>ERROR : The file has failed to be loaded</em>"));
      else
        {
	  FenClient::imageView->setPixmap( QPixmap::fromImage(m_image) );
	  FenClient::imageView->show();

	  listeMessages->append(tr("<em>The file was loaded successfully</em>"));
	  boutonEnvoyer->setEnabled(true);
	  btnSend->setEnabled(true);
        }
    }
}


// Asking for the authorisation to send the pictures to the server
void
FenClient::on_btnSend_clicked()
{
  std::string	str_file = message->text().toStdString();

  std::size_t	found = str_file.find_last_of("/");
  if (found != std::string::npos)
    str_file = str_file.substr(found + 1);

  QByteArray      paquet;
  QDataStream     out(&paquet, QIODevice::WriteOnly);


  ///
  str_file = l_getFilename( str_file );
  std::stringstream	sstr;
  sstr << str_file << ".png";
  str_file = sstr.str();
  ///


  const char*     messageToSend = str_file.c_str();
  unsigned short  tmp_size = str_file.size();

  std::cerr << "IMAGE : " << messageToSend << std::endl;

  unsigned short  image_width = m_image.size().width();
  unsigned short  image_height = m_image.size().height();
  unsigned char   key = Client_to_Server::eAskUpload;

  out.writeRawData((char*)&key, 1);
  out.writeRawData((char*)&image_width, 2);
  out.writeRawData((char*)&image_height, 2);
  out.writeRawData((char*)&tmp_size, 2);
  out.writeRawData(messageToSend, tmp_size);
  m_socket->write(paquet);
}

// The ENTER button is the same as the status button
void
FenClient::on_message_returnPressed()
{
  on_boutonEnvoyer_clicked();
}

// We receive data from the server
void
FenClient::donneesRecues()
{
  QDataStream	in(m_socket);

  if (m_isquery == Server_to_Client::eUnknowQuery)
    {
      if (m_socket->bytesAvailable() < (int)sizeof(unsigned short))
	return;
      char query = 0;
      in.readRawData(&query, 1);

      switch (query)
	{
	case Server_to_Client::eAnswerStatus:
	case Server_to_Client::eAuthorizeUpload:
	case Server_to_Client::eConfirmUpload:
	case Server_to_Client::eError:
	case Server_to_Client::eUploadNext:
	  m_isquery = (Server_to_Client::e_Server_to_Client)query;
	  break;

	case Server_to_Client::eUnknowQuery :
	default:
	  return;
	}
    }

  if (m_tailleMessage == 0)
    {
      if (m_socket->bytesAvailable() < (int)sizeof(unsigned char ) * 3)
	return;
      in.readRawData((char*)&(m_answerByte), 1);
      in.readRawData((char*)&(m_tailleMessage), 2);
    }

  if (m_socket->bytesAvailable() < (int)m_tailleMessage)
    return;

  QString ReadMessage;
  char*   pData = new char[m_tailleMessage + 1];
  memset(pData, 0, m_tailleMessage + 1);
  in.readRawData(pData, m_tailleMessage);
  ReadMessage = QString(pData);
  delete(pData);
  m_tailleMessage = 0;

  (this->*(m_protocol[m_isquery]))(ReadMessage);

  m_isquery = Server_to_Client::eUnknowQuery;
}

// Sending the bit of end of Upload
void
FenClient::finishedUploading()
{
  QByteArray	paquet;
  QDataStream	out(&paquet, QIODevice::WriteOnly);
  unsigned char	key = Client_to_Server::eUploadFinished;

  out.writeRawData((char*)&key, 1);

  m_socket->write(paquet);
}

// The connection with the server is done
void
FenClient::connecte()
{
  listeMessages->append(tr("<em>Connection established!</em>"));
  boutonConnexion->setEnabled(true);
}

// Deconnection from the server
void
FenClient::deconnecte()
{
  listeMessages->append(tr("<em>Disconnected from the server</em>"));
}

// Error with the server
void
FenClient::erreurSocket(QAbstractSocket::SocketError erreur)
{
  switch(erreur)
    {
    case QAbstractSocket::HostNotFoundError:
      listeMessages->append(tr("<em>ERROR : No server found, please check the ip address or the port.</em>"));
      break;
    case QAbstractSocket::ConnectionRefusedError:
      listeMessages->append(tr("<em>ERROR : Connection refused please check the server</em>"));
      break;
    case QAbstractSocket::RemoteHostClosedError:
      listeMessages->append(tr("<em>ERROR : Connection closed by the server.</em>"));
      break;
    default:
      listeMessages->append(tr("<em>ERROR : ") + m_socket->errorString() + tr("</em>"));
    }
  boutonConnexion->setEnabled(true);
  pushButton->setEnabled(false);
  boutonEnvoyer->setEnabled(false);
  btnSend->setEnabled(false);
  imageView->clear();
  message->clear();
  message->setFocus();
}

