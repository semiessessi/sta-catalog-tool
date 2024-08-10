#pragma once 

#ifndef DURCHMUSTERUNG_H
#define DURCHMUSTERUNG_H

#include <string>

namespace SCT
{

#pragma pack(push, 1)

class Durchmusterung
{

	enum Catalog : unsigned int
	{
		BD = 0,
		CP = 1,
		CD = 2,
		AG = 3
	};

public:

	static const Durchmusterung Empty;

	Durchmusterung();
	Durchmusterung(const Durchmusterung& other);

	bool operator==(const Durchmusterung& other) const
	{
		return GetData() == other.GetData();
	}

	bool operator!=(const Durchmusterung& other) const
	{
		return GetData() != other.GetData();
	}

	static Durchmusterung FromHDString(const std::string& string);
	static Durchmusterung FromHipString(const std::string& string);

	unsigned int GetData() const { return *reinterpret_cast<const unsigned int*>(this); }
	unsigned int GetDataNoComponent() const;
	Durchmusterung GetComponentless() const;

private:

	struct
	{
		unsigned int m_number : 16;
		unsigned int m_zone : 7;
		unsigned int m_sign : 1;
		unsigned int m_component : 6;
		Catalog m_catalog : 2;
	};
};

#pragma pack(pop)

}

template <>
struct std::hash<SCT::Durchmusterung>
{
	std::size_t operator()(const SCT::Durchmusterung& k) const
	{
		using std::size_t;
		using std::hash;
		using std::string;

		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return hash<unsigned int>()(k.GetData());
	}
};

#endif
