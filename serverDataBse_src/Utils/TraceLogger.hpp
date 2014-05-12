

#ifndef	TRACELOGGER_HPP
#define	TRACELOGGER_HPP


// #include "Header.hpp"
#include <QMutex>

#include <iostream>
#include <fstream>
#include <cstring>


#include <sstream>



class	TraceLog
{
private :

  static std::ostream*	m_pOutStream;
  static std::ofstream*	m_pFileStream;
  // static sf::Mutex	m_Mutex;
  static QMutex		m_Mutex;

  bool	m_end;
  bool	m_needEndl;

public :

  TraceLog();
  TraceLog(const TraceLog& toCopy);
  ~TraceLog();

public :

  static void		Initialize();
  static void		Initialize(std::ostream& OutStream);
  static void		Initialize(const std::string& LogFile);

  static void		Check() throw();
  static TraceLog	Log_1(void);
  static TraceLog	Log_2(const char* file, const char* func, int line);

  static void		Destroy();

public :

  TraceLog&	NoSpace();
  TraceLog&	Space();

private :

  friend TraceLog	operator << (TraceLog logger, bool t);
  friend TraceLog	operator << (TraceLog logger, char t);
  friend TraceLog	operator << (TraceLog logger, signed short i);
  friend TraceLog	operator << (TraceLog logger, unsigned short i);
  friend TraceLog	operator << (TraceLog logger, signed int i);
  friend TraceLog	operator << (TraceLog logger, unsigned int i);
  friend TraceLog	operator << (TraceLog logger, signed long l);
  friend TraceLog	operator << (TraceLog logger, unsigned long l);
  friend TraceLog	operator << (TraceLog logger, float f);
  friend TraceLog	operator << (TraceLog logger, double f);
  friend TraceLog	operator << (TraceLog logger, const char* s);
  friend TraceLog	operator << (TraceLog logger, const std::string& s);
  friend TraceLog	operator << (TraceLog logger, const void* p);

};


TraceLog	operator << (TraceLog logger, bool t);
TraceLog	operator << (TraceLog logger, char t);
TraceLog	operator << (TraceLog logger, signed short i);
TraceLog	operator << (TraceLog logger, unsigned short i);
TraceLog	operator << (TraceLog logger, signed int i);
TraceLog	operator << (TraceLog logger, unsigned int i);
TraceLog	operator << (TraceLog logger, signed long l);
TraceLog	operator << (TraceLog logger, unsigned long l);
TraceLog	operator << (TraceLog logger, float f);
TraceLog	operator << (TraceLog logger, double f);
TraceLog	operator << (TraceLog logger, const char* s);
TraceLog	operator << (TraceLog logger, const std::string& s);
TraceLog	operator << (TraceLog logger, const void* p);


TraceLog	tmp_Log(const char* file, const char* func, int line);


// #define ExFILE ( strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : (__FILE__) )
// #define Log()	Log_2(ExFILE, __func__, __LINE__)

// #define Log()	Log_2(__FILE__, __func__, __LINE__)
#define LOG(log)	tmp_Log(__FILE__, __func__, __LINE__) << log
// #define LOG(log)	(void)0

// #define Log()	Log_1()


#endif

