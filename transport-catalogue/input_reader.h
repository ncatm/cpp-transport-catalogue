#pragma once

#include "transport_catalogue.h"

#include <iostream>

namespace transport {
	void FillCatalogue(Catalogue& catalogue);
	Route FillRoute(std::string& line);
	Stop FillStop(std::string& line);
	void FillStopDistances(std::string& line, Catalogue& catalogue);
} // namespace transport