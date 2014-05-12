#ifndef	PROTOCOLE_HPP
#define	PROTOCOLE_HPP


struct	Client_to_Server
{

  enum	e_Client_to_Server
    {
      eUnknowQuery = -1,
      eAskStatus = 0,	// [query : byte] [pathname : string(ushort + data)]
      eAskUpload,	// [query : byte] [image_width : ushort] [image_height : ushort] [pathname : string(ushort + data)]
      eUpload,		// [query : byte] [chunk : ushort + data]
      eUploadFinished,	// [query : byte]
      eSync,
      eCount
    };

};

struct	Server_to_Client
{

  enum	e_Server_to_Client
    {
      eUnknowQuery = -1,
      eAnswerStatus = 0,	// [query : byte] [status : byte] [message : string(ushort + data)]
      eAuthorizeUpload,		// [query : byte] [value : byte] [error : string(ushort + data)]
      eUploadNext,		// [query : byte] [value : byte] [error : string(ushort + data)]  
      eConfirmUpload,		// [query : byte] [value : byte] [error : string(ushort + data)]
      eError,
      eCount
    };

};


#endif

