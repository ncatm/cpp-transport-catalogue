#include "stat_reader.h"

#include <iomanip>

namespace transport {

    void ProcessRequests(std::istream& in, const Catalogue& catalogue, std::ostream& out) {
        size_t requests_count;
        in >> requests_count;
        for (size_t i = 0; i < requests_count; ++i) {
            std::string keyword, line;
            in >> keyword;
            std::getline(in, line);
            if (keyword == "Bus") {
                PrintRoute(line, catalogue, out);
            }
            if (keyword == "Stop") {
                PrintStop(line, catalogue, out);
            }
        }
    }


    void PrintRoute(std::string& line, const Catalogue& catalogue, std::ostream& out) {
        std::string route_number = line.substr(1, line.npos);
        if (catalogue.FindRoute(route_number)) {
            out << "Bus " << route_number << ": " << catalogue.RouteInformation(route_number).stops_count << " stops on route, "
                << catalogue.RouteInformation(route_number).unique_stops_count << " unique stops, " << std::setprecision(6)
                << catalogue.RouteInformation(route_number).route_length << " route length, "
                << catalogue.RouteInformation(route_number).curvature << " curvature\n";
        }
        else {
            out << "Bus " << route_number << ": not found\n";
        }
    }

    void PrintStop(std::string& line, const Catalogue& catalogue, std::ostream& out) {
        std::string stop_name = line.substr(1, line.npos);
        if (catalogue.FindStop(stop_name)) {
            out << "Stop " << stop_name << ": ";
            std::set<std::string> buses = catalogue.GetBusesOnStop(stop_name);
            if (!buses.empty()) {
                out << "buses ";
                for (const auto& bus : buses) {
                    out << bus << " ";
                }
                out << "\n";
            }
            else {
                out << "no buses\n";
            }
        }
        else {
            out << "Stop " << stop_name << ": not found\n";
        }
    }


} // namespace transport