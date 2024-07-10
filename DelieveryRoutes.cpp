#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

class DeliveryLocation {
public:
    int id;
    double x, y;

    DeliveryLocation(int id, double x, double y) : id(id), x(x), y(y) {}
};

class DeliveryGraph {
public:
    vector<DeliveryLocation> locations;
    vector<vector<double>> distances;

    DeliveryGraph(const vector<DeliveryLocation>& locations)
        : locations(locations), distances(locations.size(), vector<double>(locations.size(), 0.0)) {}

    void addDistance(int from, int to, double distance) {
        distances[from][to] = distance;
        distances[to][from] = distance;
    }
};

double calculateDistance(const DeliveryLocation& location1, const DeliveryLocation& location2) {
    double xDiff = location1.x - location2.x;
    double yDiff = location1.y - location2.y;
    double distance = sqrt((xDiff * xDiff) + (yDiff * yDiff));
    return distance;
}

double calculateRouteDistance(const vector<int>& route, const DeliveryGraph& graph) {
    double totalDistance = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        int from = route[i];
        int to = route[i + 1];
        totalDistance += graph.distances[from][to];
    }
    return totalDistance;
}

vector<int> findOptimalRoute(const DeliveryGraph& graph) {
    int numLocations = graph.locations.size();
    vector<int> unvisitedLocations(numLocations);
    for (int i = 0; i < numLocations; ++i) {
        unvisitedLocations[i] = i;
    }
    vector<int> optimalRoute;
    optimalRoute.reserve(numLocations); 
    int currentLocation = 0;
    unvisitedLocations.erase(remove(unvisitedLocations.begin(), unvisitedLocations.end(), currentLocation), unvisitedLocations.end());
    optimalRoute.push_back(currentLocation);

    while (!unvisitedLocations.empty()) {
        double minDistance = numeric_limits<double>::max();
        int nearestLocation = -1;

        for (const int& location : unvisitedLocations) {
            double distance = graph.distances[currentLocation][location];
            if (distance < minDistance) {
                minDistance = distance;
                nearestLocation = location;
            }
        }

        optimalRoute.push_back(nearestLocation);
        unvisitedLocations.erase(remove(unvisitedLocations.begin(), unvisitedLocations.end(), nearestLocation), unvisitedLocations.end());
        currentLocation = nearestLocation;
    }
    optimalRoute.push_back(0);

    bool improved = true;
    while (improved) {
        improved = false;
        for (size_t i = 1; i < optimalRoute.size() - 2; ++i) {
            for (size_t j = i + 1; j < optimalRoute.size() - 1; ++j) {
                vector<int> newRoute(optimalRoute);
                reverse(newRoute.begin() + i, newRoute.begin() + j + 1);
                double newDistance = calculateRouteDistance(newRoute, graph);
                if (newDistance < calculateRouteDistance(optimalRoute, graph)) {
                    optimalRoute = newRoute;
                    improved = true;
                }
            }
        }
    }

    return optimalRoute;
}

int main() {
    cout<<"\n\n                                        *******************   DS-II PROJECT   ****************************\n\n";
    cout<<"\n                                        ***************  OPTIMIZING DELIVERY ROUTES  ******************\n\n";

    int numLocations;
    cout << "Enter the number of delivery locations: ";
    cin >> numLocations;

    vector<DeliveryLocation> locations;
    locations.reserve(numLocations); 

    for (int i = 0; i < numLocations; ++i) {
        double x, y;
        cout << "Enter coordinates (x y) for location " << i << ": ";
        cin >> x >> y;
        locations.emplace_back(i, x, y);
    }

    DeliveryGraph graph(locations);

    for (int i = 0; i < numLocations; ++i) {
        for (int j = i + 1; j < numLocations; ++j) {
            double distance = calculateDistance(locations[i], locations[j]);
            cout << "Distance between location " << i << " and location " << j << ": " << distance << endl;
            graph.addDistance(i, j, distance);
        }
    }

    vector<int> optimalRoute = findOptimalRoute(graph);
    double totalDistance = calculateRouteDistance(optimalRoute, graph);

    cout << "Optimal Route: ";
    for (int location : optimalRoute) {
        cout << location << " ";
    }
    cout << endl;
    cout << "Total Distance: " << totalDistance << endl;

    return 0;
}
