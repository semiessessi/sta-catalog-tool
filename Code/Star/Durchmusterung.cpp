#include "Durchmusterung.h"

namespace SCT
{

const Durchmusterung Durchmusterung::Empty;

Durchmusterung::Durchmusterung()
: m_number(0)
, m_zone(0)
, m_sign(0)
, m_component(0)
, m_catalog(BD)
{

}

Durchmusterung::Durchmusterung(const Durchmusterung& other)
: m_number(other.m_number)
, m_zone(other.m_zone)
, m_sign(other.m_sign)
, m_component(other.m_component)
, m_catalog(other.m_catalog)
{
}

Durchmusterung Durchmusterung::FromHDString(const std::string& string)
{
	Durchmusterung returnValue;

	returnValue.m_catalog = BD;
	if (string[0] == 'A')
	{
		returnValue.m_catalog = AG;
	}
	else if (string[0] == 'C')
	{
		if (string[1] == 'D')
		{
			returnValue.m_catalog = CD;
		}
		else
		{
			returnValue.m_catalog = CP;
		}
	}

	returnValue.m_sign = string[2] == '-';

	returnValue.m_zone = std::stoi(string.substr(3, 2));
	returnValue.m_number = std::stoi(string.substr(5, 5));

	if (string.size() > 11)
	{
		if (string[11] != ' ')
		{
			returnValue.m_component = string[11] - 'A' + 1;
		}
	}

	return returnValue;
}

Durchmusterung Durchmusterung::FromHipString(const std::string& string)
{
	Durchmusterung returnValue;

	returnValue.m_catalog = BD;
	if (string[0] == 'A')
	{
		returnValue.m_catalog = AG;
	}
	else if (string[0] == 'C')
	{
		if (string[1] == 'D')
		{
			returnValue.m_catalog = CD;
		}
		else
		{
			returnValue.m_catalog = CP;
		}
	}

	returnValue.m_sign = string[2] == '-';

	returnValue.m_zone = std::stoi(string.substr(3, 2));
	returnValue.m_number = std::stoi(string.substr(5, 5));

	return returnValue;
}

unsigned int Durchmusterung::GetDataNoComponent() const
{
	Durchmusterung copy(GetComponentless());
	return *reinterpret_cast<const unsigned int*>(&copy);
}

Durchmusterung Durchmusterung::GetComponentless() const
{
	Durchmusterung copy(*this);
	copy.m_component = 0;
	return copy;
}

}
