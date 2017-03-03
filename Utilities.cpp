#include <random>
#include <utility>

// Return a random int from min to max, inclusive
int randInt(int min, int max)
{
	if (max < min)
		std::swap(max, min);
	static std::random_device rd;
	static std::mt19937 generator(rd());
	std::uniform_int_distribution<> distro(min, max);
	return distro(generator);
}