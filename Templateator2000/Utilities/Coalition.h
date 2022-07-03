#pragma once

#include "Utilities/Log.h"

class Coalition
{
public:
	enum Side
	{
		Neutral = 0, Red = 1, Blue = 2
	};

	static Side FromDcsCoalition(const int& dcs_coalition)
	{
		return static_cast<Side>(dcs_coalition);
	}

	static int ToDcsCoalition(const Side& coalition)
	{
		return coalition;
	}

	static Side FromString(const std::string& coalition)
	{
		if (coalition == "Neutral")
			return Neutral;
		if (coalition == "Red")
			return Red;
		if (coalition == "Blue")
			return Blue;

		LOG_ERROR("Unknown coalition {} !", coalition);
		throw std::runtime_error("Unknown coalition !");
	}

	static const std::string ToString(const Side& coalition)
	{
		switch (coalition)
		{
		case Neutral: return "Neutral";
		case Red: return "Red";
		case Blue: return "Blue";
		}
		return {};
	}
};
