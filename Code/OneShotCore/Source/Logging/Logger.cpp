#include "CoreHeaders.h"
#include "Logging/Logger.h"

CLogger::CLogger(std::string sLoggerName) :
m_sLoggerName(sLoggerName)
{
}

std::string CLogger::GetCurrentLocalTime()
{
	std::time_t ttNow = m_SystemClock.to_time_t(m_SystemClock.now());
	char* pNow = OSE_NEW_ARRAY(char, 26);
	struct tm buf;
	::gmtime_s(&buf, &ttNow);
	std::strftime(pNow, 26, "%d/%m/%y - %H:%M:%S", &buf);
	std::string sNow = std::string(pNow);
	OSE_DELETE_ARRAY(pNow);

	return sNow;
}
