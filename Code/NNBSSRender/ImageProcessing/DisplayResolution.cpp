#include "DisplayResolution.h"

#include <NNBSSBridge/MainBridge.h>

void SNNBSSDisplayResolution::GetResolution(const ResolutionAbbreviation& abbreviation, ResolutionWidth& resWidth, ResolutionHeight& resHeight)
{
	auto it = std::find_if(Resolutions.begin(), Resolutions.end(),
		[&abbreviation](const std::pair<ResolutionAbbreviation, std::tuple<ResolutionWidth, ResolutionHeight>>& element) { return element.first == abbreviation; });

	if (it != std::end(Resolutions))
	{
		// structured binding
		const auto& [key, value] { *it };

		resWidth = std::get<0>(value);
		resHeight = std::get<1>(value);
	}
	else
	{
		NNBSSErrorShow("%s is not a valid resolution abbreviation!",100, abbreviation);
	}
}

std::string SNNBSSDisplayResolution::GetResolutionAbbreviation(const ResolutionWidth& resWidth, const ResolutionHeight& resHeight)
{
	for (const auto& p : Resolutions)
	{
		if (std::get<0>(p.second) == resWidth && std::get<1>(p.second) == resHeight)
		{
			return p.first;
		}
	}

#if 0
	NNBSSErrorShow("%sx%s is not registered resolution!", 100, std::to_string(resWidth).c_str(), std::to_string(resHeight).c_str());
#endif 
	return std::string();
}
