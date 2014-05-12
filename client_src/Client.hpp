

#ifndef	HEADER_FENCLIENT
#define	HEADER_FENCLIENT


# include   <QtNetwork>
# include   <QString>
# include   <QFileDialog>
# include   <iostream>

# include   "Protocole.hpp"
# include   "ui_FenClient.h"


class FenClient : public QWidget, private Ui::FenClient
{
  Q_OBJECT

public:
  FenClient();

private slots:
  void  on_boutonConnexion_clicked();
  void  on_boutonEnvoyer_clicked();
  void  on_message_returnPressed();
  void  on_pushButton_clicked();
  void  on_btnSend_clicked();

  void  finishedUploading();
  void  donneesRecues();
  void  connecte();
  void  deconnecte();
  void  erreurSocket(QAbstractSocket::SocketError erreur);

private:
  typedef void		(FenClient::*pFunc_client)(QString ReadMessage);
  pFunc_client		m_protocol[Client_to_Server::eCount];

  void			Command_AnswerStatus(QString ReadMessage);
  void			Command_ConfirmUpload(QString ReadMessage);
  void			Command_Upload(QString ReadMessage);
  void			Command_Error(QString ReadMessage);
  void			Command_UploadNext(QString ReadMessage);

  QTcpSocket		*m_socket;
  QImage		m_image;
  unsigned int		m_cursor;
  unsigned char		m_answerByte;
  unsigned short	m_tailleMessage;
  int			m_isquery;
};


#endif

