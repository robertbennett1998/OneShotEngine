#include "CoreHeaders.h"
#include "Logging/Logger.h"

CLogger::CLogger(std::string sLoggerName) :
m_sLoggerName(sLoggerName)
{
}

bool CLogger::AddSibling(std::shared_ptr<CLogger> logger)
{
	if (!m_Siblings.empty())
	{
		for (auto& sib : m_Siblings)
			if (sib == logger)
				return false;
	}

	m_Siblings.push_back(logger);

	return true;
}
bool CLogger::RemoveSibling(std::shared_ptr<CLogger> logger)
{
	if (!m_Siblings.empty())
	{
		for (UINT i = 0; i < m_Siblings.size(); i++)
		{
			if (m_Siblings[i] == logger)
			{
				m_Siblings.erase(m_Siblings.begin() + i);
				return true;
			}
		}
	}

	return false;
}

std::string CLogger::GetCurrentLocalTime()
{
	std::time_t ttNow = m_SystemClock.to_time_t(m_SystemClock.now());
	char now[26];
	struct tm buf;
	::gmtime_s(&buf, &ttNow);
	std::strftime(now, 26, "%d/%m/%y - %H:%M:%S", &buf);
	std::string sNow = std::string(now);

	return sNow;
}
