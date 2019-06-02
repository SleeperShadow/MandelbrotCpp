#include "Params.h"
#include <limits>
#include <string>
#include "cstring"

void ImageParams::initParameters(int argc, char** argv)
{
	int passedArgs = 0;
	while (passedArgs < argc)
	{
		if (strcmp(*argv, s) == 0 || strcmp(*argv, size) == 0)
		{
			++passedArgs;
			++argv;
			std::string dimensions{ *argv };
			auto xPos = dimensions.find('x');

			width = std::stoi(dimensions.substr(0, xPos));
			height = std::stoi(dimensions.substr(xPos + 1));
		}
		else if (strcmp(*argv, r) == 0 || strcmp(*argv, rect) == 0)
		{
			++passedArgs;
			++argv;
			std::string rectCoords{ *argv };
			auto left = rectCoords.substr(0, rectCoords.find(':'));
			rectCoords.erase(0, left.length() + 1);
			auto right = rectCoords.substr(0, rectCoords.find(':'));
			rectCoords.erase(0, right.length() + 1);
			auto bottom = rectCoords.substr(0, rectCoords.find(':'));
			rectCoords.erase(0, bottom.length() + 1);
			auto top = rectCoords.substr(0);

			xMin = std::stod(left);
			xMax = std::stod(right);
			yMin = std::stod(bottom);
			yMax = std::stod(top);

		}
		else if (strcmp(*argv, t) == 0 || strcmp(*argv, tasks) == 0)
		{
			++passedArgs;
			++argv;;
			numThreads = std::stoi(*argv);
			runningTasks.reserve(numThreads);
		}
		else if (strcmp(*argv, o) == 0 || strcmp(*argv, output) == 0)
		{
			++passedArgs;
			++argv;
			outputFile = std::string{ *argv };
		}
		else if (strcmp(*argv, q) == 0 || strcmp(*argv, quiet) == 0)
		{
			quietMode = true;
		}
		++passedArgs;
		++argv;
	}
}