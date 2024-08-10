#include "SpectralClass.h"

namespace SCT
{
const SpectralClass SpectralClass::Empty;

SpectralClass::SpectralClass()
: m_type(Unknown)
, m_index(0)
{

}

SpectralClass SpectralClass::FromString(const std::string& source)
{
	std::string trimmed = source;
	while (trimmed.back() == ' ')
	{
		trimmed.pop_back();
		if (trimmed.size() == 0)
		{
			return SpectralClass::Empty;
		}
	}

	SpectralClass returnValue;
	returnValue.m_type = Unknown;
	if (source[0] == 'O')
	{
		returnValue.m_type = O;
	}
	else if (source[0] == 'B')
	{
		returnValue.m_type = B;
	}
	else if (source[0] == 'A')
	{
		returnValue.m_type = A;
	}
	else if (source[0] == 'F')
	{
		returnValue.m_type = F;
	}
	else if (source[0] == 'G')
	{
		returnValue.m_type = G;
	}
	else if (source[0] == 'K')
	{
		returnValue.m_type = K;
	}
	else if (source[0] == 'M')
	{
		returnValue.m_type = M;
	}

	if (trimmed.length() == 1)
	{
		return returnValue;
	}

	int index = 0;
	if ((source[1] >= '0')
		&& (source[1] <= '9'))
	{
		index = source[1] - '0';
	}

	returnValue.m_index = index;

	return returnValue;
}

std::string SpectralClass::ToString() const
{
	std::string returnValue;
	switch (m_type)
	{
		case O: returnValue += "O"; break;
		case B: returnValue += "B"; break;
		case A: returnValue += "A"; break;
		case F: returnValue += "F"; break;
		case G: returnValue += "G"; break;
		case K: returnValue += "K"; break;
		case M: returnValue += "M"; break;
		default: return "??";
	}

	returnValue += std::to_string(m_index);
	return returnValue;
}

}