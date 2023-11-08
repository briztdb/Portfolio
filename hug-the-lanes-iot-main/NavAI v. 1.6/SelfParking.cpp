#include <array>
#include <iostream>
#include <State>

using namespace std;

class SelfParkingModule {
 public:
  // Attributes
  struct ParkingSpace {
    int size;
    std::string location;
  } parkingSpace;
  array<int, 4> ultrasonicSensors;
  string parkingCamera;
  int steeringMotor;
  int speedMotor;
  bool parked;
  string currentLocation;

  // Methods
  ParkingSpace getAvailableSpace() { 
    return parkingSpace; 
  }

  void setUltrasonicSensors(array<int, 4> sensors) {
    ultrasonicSensors = sensors;
  }

  void setParkingCamera(string camera) { 
    parkingCamera = camera; 
  }

  void steer(int angle) { 
    steeringMotor = angle; 
  }

  void accelerate(int speed) { 
    speedMotor = speed; 
  }

  void park(State& state) {
    if (!state.is_obstacle_detected && !state.is_traffic_light_detected &&
        !state.is_pedestrian_detected) {
      std::cout << "Starting parking process...\n";
      // Perform self-parking algorithm
      state.steering_angle = 0.0;  // Set steering angle to straight
      state.throttle = 0.0;        // Set throttle to zero
      state.brake = 100.0;         // Apply brakes
      state.acceleration = -1.0;   // Move backwards into parking space
      state.parked = true;         // Set parked flag to true
      std::cout << "Vehicle parked successfully!\n";
    } else {
      std::cout << "Unable to park vehicle: obstacle detected!\n";
    }
  }

  void unpark(State& state) {
    if (state.parked) {
      std::cout << "Starting unparking process...\n";
      // Perform unparking algorithm
      state.steering_angle = 0.0;  // Set steering angle to straight
      state.throttle = 10.0;       // Apply slight throttle
      state.brake = 0.0;           // Release brakes
      state.acceleration = 1.0;    // Move forward out of parking space
      state.parked = false;        // Set parked flag to false
      std::cout << "Vehicle unparked successfully!\n";
    } else {
      std::cout << "Vehicle is not currently parked!\n";
    }
  }

  void updateLocation(string location) { 
    currentLocation = location; 
    }

  bool isParked() { 
    return parked; 
  }

  // Use case method
  void activateSelfParking() {
    cout << "Self-parking module activated." << endl;

    // Sensor data received (arbitrary values)
    array<int, 4> sensorData = {20, 40, 15, 50};

    // Check for available parking space
    if (sensorData[0] > 0) {
      parkingSpace.size = sensorData[0];
      parkingSpace.location = "Front";

      // Calculate distance and angle
      int distance = sensorData[1];
      int angle = 45;

      // Send signal to start parking maneuver
      cout << "Starting parking maneuver..." << endl;

      // Adjust steering and speed
      steer(angle);
      accelerate(10);

      // Check for obstacles or unsafe conditions
      if (ultrasonicSensors[0] < distance || ultrasonicSensors[1] < distance) {
        cout << "Obstacle detected. Cancelling parking process."
                  << endl;
        unpark();
        return;
      }

      // Park the vehicle
      park();
      cout << "Vehicle is parked safely in the designated spot."
                << endl;
    } else {
      cout << "No available parking space detected." << endl;
    }
  }
};