

#include "TraceLogger.hpp"


// #include "Header.hpp"

// #include "Exception.hpp"

#include <iomanip>


// std::ostream*	TraceLog::m_pOutStream = NULL;
std::ostream*	TraceLog::m_pOutStream = &std::cerr;
std::ofstream*	TraceLog::m_pFileStream = NULL;

// sf::Mutex	TraceLog::m_Mutex;
QMutex		TraceLog::m_Mutex;

////////////////////////////////

TraceLog::TraceLog()
  : m_end(true), m_needEndl(true)
{
}

TraceLog::TraceLog(const TraceLog& toCopy)
  : m_end(true), m_needEndl(true)
{
  static_cast<void>(toCopy);
}

TraceLog::~TraceLog()
{
  if (m_needEndl)
    {
      (*m_pOutStream) << std::endl;
      m_pOutStream->flush();
    }

  if (m_end)
    {
      m_Mutex.unlock();
    }
}

////////////////////////////////

void		TraceLog::Initialize()
{
  // if (m_pFileStream != NULL)
  //   throw ExException("TraceLog already initialized");

  m_pOutStream = &std::cerr;
}

void		TraceLog::Initialize(std::ostream& OutStream)
{
  // if (m_pFileStream != NULL)
  //   throw ExException("TraceLog already initialized");

  m_pOutStream = &OutStream;
}

void		TraceLog::Initialize(const std::string& LogFile)
{
  // if (m_pFileStream != NULL)
  //   throw ExException("TraceLog already initialized");

  m_pFileStream = new std::ofstream;
  m_pFileStream->open(LogFile.c_str());

  if (m_pFileStream->is_open())
    m_pOutStream = m_pFileStream;
  else
    delete m_pFileStream, m_pFileStream = NULL;
}

void		TraceLog::Check() throw()
{
  // if (m_pOutStream == NULL)
  //   throw ExException("TraceLog uninitialized");
}

TraceLog	TraceLog::Log_1()
{
  m_Mutex.lock();

  Check();

  TraceLog	tmp;
  return (tmp);
}

TraceLog	TraceLog::Log_2(const char* file, const char* func, int line)
{
  // TraceLog	logger2;
  // return (logger2.NoSpace());

  m_Mutex.lock();

  Check();

  // (*m_pOutStream) << "[ " << file << " | " << func << " | " << line << " ] : ";

  (*m_pOutStream) << "[ "
  		  << std::setw(15) << file << " | "
  		  << std::setw(10) << func << " | "
  		  << std::setw(5) << line << "] : ";

  TraceLog	logger;
  return (logger);
}

void		TraceLog::Destroy()
{
  m_pOutStream = NULL;
  delete m_pFileStream, m_pFileStream = NULL;
}

////////////////////////////////

TraceLog&	TraceLog::NoSpace()
{
  Check();
  m_end = m_needEndl = false;
  m_pOutStream->flush();

  return (*this);
}

TraceLog&	TraceLog::Space()
{
  Check();
  (*m_pOutStream) << ' ';

  return (NoSpace());
}

////////////////////////////////

TraceLog	operator << (TraceLog logger, bool t)
{
  (*logger.m_pOutStream) << t;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, char t)
{
  (*logger.m_pOutStream) << t;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, signed short i)
{
  (*logger.m_pOutStream) << i;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, unsigned short i)
{
  (*logger.m_pOutStream) << i;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, signed int i)
{
  (*logger.m_pOutStream) << i;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, unsigned int i)
{
  (*logger.m_pOutStream) << i;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, signed long l)
{
  (*logger.m_pOutStream) << l;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, unsigned long l)
{
  (*logger.m_pOutStream) << l;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, float f)
{
  (*logger.m_pOutStream) << std::setprecision(7) << f;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, double f)
{
  (*logger.m_pOutStream) << std::setprecision(7) << f;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, const char * s)
{
  (*logger.m_pOutStream) << s;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, const std::string& s)
{
  (*logger.m_pOutStream) << s;
  return (logger.Space());
}

TraceLog	operator << (TraceLog logger, const void * p)
{
  (*logger.m_pOutStream) << std::hex << p;
  return (logger.Space());
}





TraceLog	tmp_Log(const char* file, const char* func, int line)
{
  return ( TraceLog::Log_2( file, func, line ) );
}


