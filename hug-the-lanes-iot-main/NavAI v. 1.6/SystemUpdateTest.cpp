#include <iostream>
#include <SystemUpdate.cpp>
using namespace std;

int main() {
  State s;
  SystemUpdate sysUpdate(s);
  cout << "Beginning of tests for SystemUpdate: " << endl;
  // Test 1: checkForUpdate when update is available
  s.is_vehicle_connected = true;
  s.is_autonomous_mode_enabled = true;
  sysUpdate.checkForUpdate();
  cout << "Test 1: \t"
       << "Expected: "
       << "Update available"
       << "\tActual: ";
  // check output with expected
  // output must be captured since it was not returned
  ostringstream output1;
  sysUpdate.checkForUpdate();
  cout << output1.str() << std::endl;
  assert(output1.str() == "Update available\n");

  // Test 2: checkForUpdate when update is not available
  s.is_vehicle_connected = true;
  s.is_autonomous_mode_enabled = false;
  sysUpdate.checkForUpdate();
  cout << "Test 2: \t"
       << "Expected: "
       << "No update available"
       << "\tActual: ";
  // check output with expected
  // output must be captured since it was not returned
  std::ostringstream output2;
  sysUpdate.checkForUpdate();
  cout << output2.str() << endl;
  assert(output2.str() == "No update available\n");

  // Test 3: downloadUpdate when update is available
  sysUpdate.setVersion("1.0");
  s.is_vehicle_connected = true;
  s.is_autonomous_mode_enabled = true;
  sysUpdate.checkForUpdate();
  sysUpdate.downloadUpdate();
  cout << "Test 3: \t"
       << "Expected: "
       << "Update installed successfully!"
       << "\tActual: ";
  // check output with expected
  // output must be captured since it was not returned
  ostringstream output3;
  sysUpdate.downloadUpdate();
  cout << output3.str() << endl;
  assert(output3.str() == "Update installed successfully!\n");
  cout << "Test 3.1: \t"
       << "Expected: "
       << "2.0"
       << "\tActual: " << sysUpdate.getVersion() << endl;

  // Test 4: downloadUpdate when update is not available
  sysUpdate.setVersion("1.0");
  s.is_vehicle_connected = true;
  s.is_autonomous_mode_enabled = false;
  sysUpdate.checkForUpdate();
  sysUpdate.downloadUpdate();
  cout << "Test 4: \t"
       << "Expected: "
       << "No update available to download"
       << "\tActual: ";
  // check output with expected
  // output must be captured since it was not returned
  ostringstream output4;
  sysUpdate.downloadUpdate();
  cout << output4.str() << endl;
  assert(output4.str() == "No update available to download\n");
  cout << "Test 4.1: \t"
       << "Expected: "
       << "1.0"
       << "\tActual: " << sysUpdate.getVersion() << endl;

  // Test 5: isConnectedToTheInternet when the system is connected
  s.is_vehicle_connected = true;
  cout << "Test 5: \t"
       << "Expected: "
       << "1"
       << "\t\tActual: " << sysUpdate.isConnectedToTheInternet() << endl;
}
