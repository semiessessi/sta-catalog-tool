#include "Catalog/Catalog.h"
#include "Core/Parameter.h"
#include "Core/Project_Config.h"
#include "Maths/Astronomy.h"

#include <string>

static bool bTest = false;

namespace SCT
{

bool IsTest()
{
	return bTest;
}

int Test(const SCL::ParameterInstance&, const std::string&)
{
	bTest = true;
	return 0;
}

int DoTests()
{
	if (!IsTest())
	{
		return 0;
	}

	puts("Running tests...");
	puts("");
	int returnCode = 0;

	{
		double error = 0.0f;
		error = ToJulianCenturiesSinceJ2000(kJ2000);
		if (abs(error) > 0.000000001)
		{
			puts("Failed test of J2000 being zero point for Julian centuries");
			returnCode = -1;
		}
		else
		{
			puts("Passed test of J2000 being zero point for Julian centuries");
		}
	}

#define TEST_ERROR(name, value, expected, limit) \
	{ \
		const double error = abs(value - expected); \
		if (error > limit) \
		{ \
			printf("ERROR: Test \"" name "\" failed! Error: %f, required %f\n", error, limit); \
			returnCode = -1; \
		} \
		else \
		{ \
			printf("Test \"" name "\" passed. Error: %f, required %f\n", error, limit); \
		} \
	}

	{
		// example from p. 135 Meeus
		const double raJ2000 = 41.054063;
		const double decJ2000 = 49.227750;
		// the book rounds.
		const double jdt = 2462088.69; //2462088.5 + 13.0/24.0 + 19.0/(24.0 * 60.0);
		const auto testValues = RADecJ2000ToCurrentEpochDegrees(jdt, raJ2000, decJ2000);
		const double ra = testValues.first;		// 41.547214
		const double dec = testValues.second;	// 49.348483
		// the book seems to use lower precision arithmetic than double..
		const double expectedRa = 41.547214;
		const double expectedDec = 49.348483;
		const double errorLimit = (0.1 / 3600); // 100mas

		TEST_ERROR("RA from J2000 to nearby epoch (p.135 Meeus)", ra, expectedRa, errorLimit);
		TEST_ERROR("Declination from J2000 to nearby epoch (p.135 Meeus)", dec, expectedDec, errorLimit);
	}

	{
		// example from p. 135 Meeus
		const double raJ2000 = 41.547214;
		const double decJ2000 = 49.348483;
		// the book rounds.
		const double jdt = 2462088.69; //2462088.5 + 13.0/24.0 + 19.0/(24.0 * 60.0);
		const auto testValues = RADecCurrentEpochToJ2000Degrees(jdt, raJ2000, decJ2000);
		const double ra = testValues.first;		// 41.547214
		const double dec = testValues.second;	// 49.348483
		// the book seems to use lower precision arithmetic than double..
		const double expectedRa = 41.054063;
		const double expectedDec = 49.227750;
		const double errorLimit = (0.1 / 3600); // 100mas

		TEST_ERROR("RA from nearby epoch to J2000 (reversed p.135 Meeus)", ra, expectedRa, errorLimit);
		TEST_ERROR("Declination from nearby epoch to J2000 (reversed p.135 Meeus)", dec, expectedDec, errorLimit);
	}

	// display some rows
	puts(StarSystem::DisplayTableHeader().c_str());
	int i = 0;
	Catalog::ForEachStar([&](SCT::StarSystem starSystem)
		{
			if (i > 30000)
			{
				if (((i % 61337) != 0)
					&& ((i % 27111) != 0))
				{
					++i;
					return;
				}
			}
			else if (i > 3000)
			{
				if (((i % 6311) != 0)
					&& ((i % 2197) != 0))
				{
					++i;
					return;
				}
			}
			else
			{
				if (((i % 577) != 0)
					&& ((i % 831) != 0))
				{
					++i;
					return;
				}
			}

			printf("%s", starSystem.ToDisplayTableRows().c_str());
			++i;
		});

	return returnCode;
}

}
