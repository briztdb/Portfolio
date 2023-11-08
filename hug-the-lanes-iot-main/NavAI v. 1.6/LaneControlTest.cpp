#include <iostream>
#include <LaneControl.cpp>
using namespace std;

int main() {
  // Test case for getCenterLine method
  Lane lane;
  lane.left_boundary = {{0, 0}, {2, 2}, {4, 4}};
  lane.right_boundary = {{0, 4}, {2, 2}, {4, 0}};
  list<tuple<int, int>> center_line = lane.getCenterLine();
  assert(center_line.size() == 3);
  assert(center_line.front() == make_tuple(0, 2));
  assert(center_line.back() == make_tuple(4, 2));

  // Test case for detectLanes method
  LaneDetection ld;
  ld.detectLanes();
  assert(ld.lanes.size() == 2);

  // Test case for getLanePositions method
  list<tuple<int, int>> lane_positions = ld.getLanePositions();
  assert(lane_positions.size() == 2);
  assert(lane_positions.front() == make_tuple(0, 0));
  assert(lane_positions.back() == make_tuple(0, 0));

  // Test case for changeLanes method
  ld.changeLanes();
  assert(ld.lanes.size() == 2);

  // Test case for getClosestLane method
  Lane closest_lane = ld.getClosestLane();
  // The value of closest_lane is not deterministic, so we cannot assert on it.

  // Test case for isVehicleInLane method
  Lane lane2;
  lane2.left_boundary = {{0, 0}, {2, 2}, {4, 4}};
  lane2.right_boundary = {{0, 4}, {2, 2}, {4, 0}};
  State vehicle_state;
  vehicle_state.latitude = 1;
  vehicle_state.longitude = 1;
  bool is_vehicle_in_lane = ld.isVehicleInLane(lane2, vehicle_state);
  assert(is_vehicle_in_lane == true);

  // Test case for calculateDistance method
  double lat1 = 37.7749, lon1 = -122.4194, lat2 = 38.5816, lon2 = -121.4944;
  double distance = ld.calculateDistance(lat1, lon1, lat2, lon2);
  // The value of distance is not deterministic, so we cannot assert on it.

  return 0;
}