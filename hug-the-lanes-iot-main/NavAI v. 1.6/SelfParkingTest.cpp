#include <iostream>
#include <SelfParking.cpp>
using namespace std;

int main() {
  // create a SelfParkingModule object
  SelfParkingModule module;

  // test the getAvailableSpace() method
  module.parkingSpace.size = "Small";
  module.parkingSpace.location = "Lot B";
  cout << "Test 1: \t"
       << "Expected: "
       << "Small Lot B\t"
       << "Actual: " << module.getAvailableSpace().to_string() << endl;

  // test the setUltrasonicSensors() method
  UltrasonicSensor us1, us2, us3;
  us1.location = "Front";
  us2.location = "Left";
  us3.location = "Right";
  UltrasonicSensor sensors[] = {us1, us2, us3};
  module.setUltrasonicSensors(sensors, 3);
  cout << "Test 2: \t"
       << "Expected: "
       << "Front Left Right\t"
       << "Actual: ";
  for (int i = 0; i < module.ultrasonicSensorCount; i++) {
    cout << module.ultrasonicSensors[i].location << " ";
  }
  cout << endl;

  // test the setParkingCamera() method
  module.setParkingCamera("Rear");
  cout << "Test 3: \t"
       << "Expected: "
       << "Rear\t\t"
       << "Actual: " << module.parkingCamera.location << endl;

  // test the steer() method
  module.steer("Right");
  cout << "Test 4: \t"
       << "Expected: "
       << "Right\t\t"
       << "Actual: " << module.steeringMotor.direction << endl;

  // test the accelerate() method
  module.accelerate(50);
  cout << "Test 5: \t"
       << "Expected: "
       << "50\t\t\t"
       << "Actual: " << module.speedMotor.speed << endl;

  // test the park() method
  module.park();
  cout << "Test 6: \t"
       << "Expected: "
       << "1\t\t\t"
       << "Actual: " << module.parked << endl;

  // test the unpark() method
  module.unpark();
  cout << "Test 7: \t"
       << "Expected: "
       << "0\t\t\t"
       << "Actual: " << module.parked << endl;

  // test the updateLocation() method
  Location loc = {45.5231, -122.6765};
  module.updateLocation(loc);
  cout << "Test 8: \t"
       << "Expected: "
       << "45.5231 -122.6765\t"
       << "Actual: " << module.currentLocation.latitude << " "
       << module.currentLocation.longitude << endl;

  // test the isParked() method
  cout << "Test 9: \t"
       << "Expected: "
       << "0\t\t\t"
       << "Actual: " << module.isParked() << endl;
}