#pragma once 

#include <string>
#include <math.h>
#include <random>
#include <chrono>

namespace Utils
{
    std::string split(std::string const& str, std::string const& del);

	float rad(float deg);

	template<typename T>
	T random(T min, T max) {
		// Use a seed based on current time
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		static std::default_random_engine generator(seed);

		// Create a distribution
		std::uniform_real_distribution<T> distribution(min, max);

		// Generate a random number
		static T previous = distribution(generator);
		T current = distribution(generator);

		// Ensure the new number is different from the previous one
		while (current == previous) {
			current = distribution(generator);
		}
		previous = current;
		return current;
	}
}
