#pragma once 

#ifndef SPECTRAL_CLASS_H
#define SPECTRAL_CLASS_H

#include <string>

namespace SCT
{

#pragma pack(push, 1)
__declspec(align(1))
class SpectralClass
{
	enum Type : unsigned char
	{
		Unknown,
		O,
		B,
		A,
		F,
		G,
		K,
		M
	};

public:

	static const SpectralClass Empty;

	SpectralClass();
	static SpectralClass FromString(const std::string& source);

	bool operator==(const SpectralClass& other) const
	{
		return (m_type == other.m_type)
			&& (m_index == other.m_index);
	}

	std::string ToString() const;

private:

	struct
	{
		Type m_type : 4;
		unsigned char m_index : 4;
	};
};

#pragma pack(pop)

}

#endif
