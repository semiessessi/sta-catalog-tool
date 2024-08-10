#include "Star.h"

#include <format>

namespace SCT
{

std::string ComponentStar::ToDisplayTableRow(const double ra, const double dec) const
{
	std::string returnValue;
	std::string HR = std::to_string(m_HR);
	while (HR.size() < 4)
	{
		HR.insert(HR.begin(), ' ');
	}
	returnValue += HR;
	returnValue += " ";

	std::string HD = std::to_string(m_HD);
	while (HD.size() < 6)
	{
		HD.insert(HD.begin(), ' ');
	}
	returnValue += HD;
	returnValue += " ";

	std::string SAO = std::to_string(m_SAO);
	while (SAO.size() < 6)
	{
		SAO.insert(SAO.begin(), ' ');
	}
	returnValue += SAO;
	returnValue += " ";

	std::string Hip = std::to_string(m_Hip);
	while (Hip.size() < 6)
	{
		Hip.insert(Hip.begin(), ' ');
	}
	returnValue += Hip;
	returnValue += " ";

	if (m_visualMagnitude < 50.0)
	{
		std::string vmag = std::format("{:.2f}", m_visualMagnitude);
		//std::to_string(m_visualMagnitude);
		while (vmag.size() < 5)
		{
			vmag.insert(vmag.begin(), ' ');
		}
		returnValue += vmag;
	}
	else
	{
		returnValue += "     ";
	}

	returnValue += " ";

	returnValue += m_spectralClass.ToString();

	returnValue += " ";

	std::string raString = std::format("{:.4f} ", ra);
	while (raString.size() < 10)
	{
		raString.insert(raString.begin(), ' ');
	}
	returnValue += raString;
	returnValue += " ";

	std::string decString = std::format("{:.4f} ", dec);
	while (decString.size() < 10)
	{
		decString.insert(decString.begin(), ' ');
	}
	returnValue += decString;

	return returnValue;
}

std::string StarSystem::DisplayTableHeader()
{
	std::string returnValue;
	returnValue += " HR     HD    SAO    Hip   Vmag SC   RA (deg)  Dec (deg)\n";
	returnValue += "------------------------------------------------------------------";
	return returnValue;
}

std::string StarSystem::ToDisplayTableRows() const
{
	std::string returnValue;
	for (size_t i = 0; i < m_components.size(); ++i)
	{
		returnValue += m_components[0].ToDisplayTableRow(m_RA.ToDegrees(), m_Dec.ToDegrees());
		returnValue += "\n";
	}
	return returnValue;
}

}
