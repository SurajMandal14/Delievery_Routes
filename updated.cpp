#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>

using namespace std;

struct DeliveryLocation {
    int id;
    double x, y;
};

double calculateDistance(const DeliveryLocation& location1, const DeliveryLocation& location2) {
    double xDiff = location1.x - location2.x;
    double yDiff = location1.y - location2.y;
    double distance = sqrt((xDiff * xDiff) + (yDiff * yDiff));
    return distance;
}

double calculateRouteDistance(const vector<int>& route, const vector<vector<double>>& distances) {
    double totalDistance = 0.0;
    for (size_t i = 0; i < route.size() - 1; ++i) {
        int from = route[i];
        int to = route[i + 1];
        totalDistance += distances[from][to];
    }
    return totalDistance;
}

vector<int> findOptimalRoute(vector<vector<double>>& distances) {
    int numLocations = distances.size();
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
            double distance = distances[currentLocation][location];
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
                double newDistance = calculateRouteDistance(newRoute, distances);
                if (newDistance < calculateRouteDistance(optimalRoute, distances)) {
                    optimalRoute = newRoute;
                    improved = true;
                }
            }
        }
    }

    return optimalRoute;
}

int main() {
    cout << "\n\n*******************   DS-II PROJECT   ****************************\n\n";
    cout << "***************  OPTIMIZING DELIVERY ROUTES  ******************\n\n";

    int numLocations;
    cout << "Enter the number of delivery locations: ";
    cin >> numLocations;

    vector<DeliveryLocation> locations(numLocations);

    for (int i = 0; i < numLocations; ++i) {
        double x, y;
        cout << "Enter coordinates (x y) for location " << i << ": ";
        cin >> x >> y;
        locations[i] = { i, x, y };
    }

    vector<vector<double>> distances(numLocations, vector<double>(numLocations, 0.0));

    for (int i = 0; i < numLocations; ++i) {
        for (int j = i + 1; j < numLocations; ++j) {
            double distance = calculateDistance(locations[i], locations[j]);
            cout << "Distance between location " << i << " and location " << j << ": " << distance << endl;
            distances[i][j] = distance;
            distances[j][i] = distance;
        }
    }

    vector<int> optimalRoute = findOptimalRoute(distances);
    double totalDistance = calculateRouteDistance(optimalRoute, distances);

    cout << "Optimal Route: ";
    for (int location : optimalRoute) {
        cout << location << " ";
    }
    cout << endl;
    cout << "Total Distance: " << totalDistance << endl;

    return 0;
}
