#pragma once

# include <cstdlib>
# include <ctime>
# include <stdexcept>
# include <vector>
# include <string>
# include <sstream>

namespace Jokes {
	void						initialize();
	std::vector<std::string>	get_joke_vect();
};
