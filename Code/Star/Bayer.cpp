#include "Star.h"

#include <format>

namespace SCT
{

std::pair<BayerGreekLetter, unsigned char> ParseBayerFromName(const std::string& name)
{
	BayerGreekLetter greekLetter = None;
	if (name[0] == 'A')
	{
		greekLetter = Alpha;
	}
	else if (name[0] == 'B')
	{
		greekLetter = Beta;
	}
	else if (name[0] == 'C')
	{
		greekLetter = Chi;
	}
	else if (name[0] == 'D')
	{
		greekLetter = Delta;
	}
	else if (name[0] == 'E')
	{
		greekLetter = (name[1] == 't') ? Eta : Epsilon;
	}
	else if (name[0] == 'G')
	{
		greekLetter = Gamma;
	}
	else if (name[0] == 'I')
	{
		greekLetter = Iota;
	}
	else if (name[0] == 'K')
	{
		greekLetter = Kappa;
	}
	else if (name[0] == 'L')
	{
		greekLetter = Lambda;
	}
	else if (name[0] == 'M')
	{
		greekLetter = Mu;
	}
	else if (name[0] == 'N')
	{
		greekLetter = Nu;
	}
	else if (name[0] == 'O')
	{
		greekLetter = (name[2] == 'e') ? Omega : Omicron;
	}
	else if (name[0] == 'P')
	{
		if (name[1] == 'h')
		{
			greekLetter = Phi;
		}
		else if (name[1] == 's')
		{
			greekLetter = Psi;
		}
		else
		{
			greekLetter = Pi;
		}
	}
	else if (name[0] == 'R')
	{
		greekLetter = Rho;
	}
	else if (name[0] == 'S')
	{
		greekLetter = Sigma;
	}
	else if (name[0] == 'T')
	{
		greekLetter = (name[1] == 'h') ? Theta : Tau;
	}
	else if (name[0] == 'U')
	{
		greekLetter = Upsilon;
	}
	else if (name[0] == 'X')
	{
		greekLetter = Xi;
	}
	else if (name[0] == 'Z')
	{
		greekLetter = Zeta;
	}

	int index = 0;
	if (name[3] != ' ')
	{
		index = name[3] - '0';
	}

	return std::make_pair(greekLetter, index);
}

}
