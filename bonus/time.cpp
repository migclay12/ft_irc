#include "time.hpp"

std::string getCurrentDateTime()
{
	std::time_t now = std::time(0);              // current time
	std::tm* localTime = std::localtime(&now);   // convert to local time

	std::ostringstream oss;

	oss << std::setfill('0') << std::setw(2) << localTime->tm_hour      << ":"
		<< std::setfill('0') << std::setw(2) << localTime->tm_min       << ":"
		<< std::setfill('0') << std::setw(2) << localTime->tm_sec       << " "
		<< std::setfill('0') << std::setw(2) << localTime->tm_mday      << "-"
		<< std::setfill('0') << std::setw(2) << (1 + localTime->tm_mon) << "-"
		<< (1900 + localTime->tm_year);
	return (oss.str());
}
