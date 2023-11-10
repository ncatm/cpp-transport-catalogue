#include "transport_catalogue.h"

namespace transport {

    void Catalogue::AddRoute(const Route& route) {
        all_routes_.push_back(route);
        for (const auto& route_stop : route.stops) {
            for (auto& stop_ : all_stops_) {
                if (stop_.name == route_stop) stop_.buses.insert(route.bus_name);
            }
        }
        route_name_to_route[all_routes_.back().bus_name] = &all_routes_.back();
    }

    void Catalogue::AddStop(const Stop& stop) {
        all_stops_.push_back(stop);
        stopname_to_stop_[all_stops_.back().name] = &all_stops_.back();
    }

    const Route* Catalogue::FindRoute(const std::string& route_number) const {
        return route_name_to_route.count(route_number) ? route_name_to_route.at(route_number) : nullptr;
    }

    Stop* Catalogue::FindStop(const std::string& stop_name) const {
        return stopname_to_stop_.count(stop_name) ? stopname_to_stop_.at(stop_name) : nullptr;
    }

    const RouteInfo Catalogue::RouteInformation(const std::string& route_number) const {
        RouteInfo route_info{};
        const Route* bus = FindRoute(route_number);

        if (!bus) throw std::invalid_argument("bus not found");
        if (bus->circular_route) route_info.stops_count = bus->stops.size();
        else route_info.stops_count = bus->stops.size() * 2 - 1;

        int route_length = 0;
        double geographic_length = 0.0;

        for (auto iter = bus->stops.begin(); iter + 1 != bus->stops.end(); ++iter) {
            auto from = stopname_to_stop_.find(*iter)->second;
            auto to = stopname_to_stop_.find((*(iter + 1)))->second;
            if (bus->circular_route) {
                route_length += GetDistance(from, to);
                geographic_length += ComputeDistance(stopname_to_stop_.find(*iter)->second->coordinates,
                    stopname_to_stop_.find(*(iter + 1))->second->coordinates);
            }
            else {
                route_length += GetDistance(from, to) + GetDistance(to, from);
                geographic_length += ComputeDistance(stopname_to_stop_.find(*iter)->second->coordinates,
                    stopname_to_stop_.find(*(iter + 1))->second->coordinates) * 2;
            }
        }
        if (bus->circular_route) {
            auto from = stopname_to_stop_.find(*(bus->stops.end() - 1))->second;
            auto to = stopname_to_stop_.find(*(bus->stops.begin()))->second;
            route_length += GetDistance(from, to);
            geographic_length += ComputeDistance(from->coordinates, to->coordinates);
        }

        route_info.unique_stops_count = UniqueStopsCount(route_number);
        route_info.route_length = route_length;
        route_info.curvature = route_length / geographic_length;

        return route_info;
    }

    size_t Catalogue::UniqueStopsCount(const std::string& route_number) const {
        std::unordered_set<std::string> unique_stops;
        for (const auto& stop : route_name_to_route.at(route_number)->stops) {
            unique_stops.insert(stop);
        }
        return unique_stops.size();
    }

    const std::set<std::string> Catalogue::GetBusesOnStop(const std::string& stop_name) const {
        return stopname_to_stop_.at(stop_name)->buses;
    }

    void Catalogue::SetDistance(Stop* from, Stop* to, int distance) {
        from->stop_distances[to->name] = distance;
    }

    int Catalogue::GetDistance(const Stop* from, const Stop* to) const {
        if (from->stop_distances.count(to->name)) return from->stop_distances.at(to->name);
        else if (to->stop_distances.count(from->name)) return to->stop_distances.at(from->name);
        else return 0;
    }

} // namespace transport