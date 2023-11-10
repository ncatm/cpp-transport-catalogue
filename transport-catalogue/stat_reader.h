#pragma once

#include "transport_catalogue.h"

namespace transport {
	void ProcessRequests(std::istream& in, const Catalogue& catalogue, std::ostream& out);
	void PrintRoute(std::string& line, const Catalogue& catalogue, std::ostream& out);
	void PrintStop(std::string& line, const Catalogue& catalogue, std::ostream& out);
} // namespace transport