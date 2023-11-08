#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <cmath>
#include <State>

using namespace std;

class Lane {
 public:
  // Attributes
  list<tuple<int, int>> left_boundary;
  list<tuple<int, int>> right_boundary;

  // Methods
  list<tuple<int, int>> getCenterLine() {
    list<tuple<int, int>> center_line;
    for (auto it1 = left_boundary.begin(), it2 = right_boundary.begin();
         it1 != left_boundary.end() && it2 != right_boundary.end();
         ++it1, ++it2) {
      center_line.push_back(make_tuple((get<0>(*it1) + get<0>(*it2)) / 2,
                                       (get<1>(*it1) + get<1>(*it2)) / 2));
    }
    return center_line;
  }
};

class LaneDetection {
 public:
  // Attributes
  list<Lane> lanes;

  // Methods
  void detectLanes() {
    // Implementation for detecting lanes
    // ...
    // Update lanes attribute
    lanes.clear();
    Lane lane1, lane2;  // Example lanes
    lanes.push_back(lane1);
    lanes.push_back(lane2);
  }

  list<tuple<int, int>> getLanePositions() {
    list<tuple<int, int>> lane_positions;
    for (const auto& lane : lanes) {
      lane_positions.push_back(make_tuple(get<0>(lane.left_boundary.front()),
                                          get<0>(lane.right_boundary.front())));
    }
    return lane_positions;
  }

  void LaneDetection::changeLanes() {
    double merge_threshold =
        5.0;  // distance threshold for merging lanes (in meters)
    vector<Lane> new_lanes;  // list to hold updated lanes

    for (int i = 0; i < lanes.size(); i++) {
      Lane current_lane = lanes[i];
      bool merged = false;

      for (int j = i + 1; j < lanes.size(); j++) {
        Lane other_lane = lanes[j];

        // check if lanes are close enough to be merged
        double distance = calculateDistance(current_lane.getCenterLine(),
                                      other_lane.getCenterLine());
        if (distance < merge_threshold) {
          // merge lanes
          Lane merged_lane = mergeLanes(current_lane, other_lane);
          new_lanes.push_back(merged_lane);
          merged = true;
          break;
        }
      }

      if (!merged) {
        // if current lane wasn't merged, add it to the list of updated lanes
        new_lanes.push_back(current_lane);
      }
    }

    // update lanes attribute
    lanes = new_lanes;
  }

  Lane getClosestLane() {
    Lane closest_lane;  // Example closest lane
    return closest_lane;
  }

  bool LaneDetection::isVehicleInLane(Lane lane, State vehicleState) {
    // get center line of the lane
    vector<pair<double, double>> centerLine = lane.getCenterLine();
    // get latitude and longitude of the vehicle
    double lat = vehicleState.latitude;
    double lon = vehicleState.longitude;

    // iterate through the center line and calculate the distance between each
    // point and the vehicle
    for (int i = 0; i < centerLine.size(); i++) {
      double dist = calculateDistance(lat, lon, centerLine[i].first,
                                      centerLine[i].second);
      // if the distance is less than the lane width, the vehicle is in the lane
      if (dist < laneWidth) {
        return true;
      }
    }

    // if no point on the center line is within the lane width of the vehicle,
    // the vehicle is not in the lane
    return false;
  }

  double LaneDetection::calculateDistance(double lat1, double lon1, double lat2,
                                          double lon2) {
    // Haversine formula for calculating distance between two lat-long
    // coordinates
    double R = 6371e3;  // earth's radius in meters
    double phi1 = toRadians(lat1);
    double phi2 = toRadians(lat2);
    double deltaPhi = toRadians(lat2 - lat1);
    double deltaLambda = toRadians(lon2 - lon1);
    double a =
        sin(deltaPhi / 2) * sin(deltaPhi / 2) +
        cos(phi1) * cos(phi2) * sin(deltaLambda / 2) * sin(deltaLambda / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    double d = R * c;
    return d;
  }

  double LaneDetection::toRadians(double degrees) {
    return degrees * M_PI / 180;
  }
};

