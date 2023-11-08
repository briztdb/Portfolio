#include <iostream>
#include <string>
#include <State>

using namespace std;

class SystemUpdate {
private:
    string version;
    bool update_available;
    State& state;

public:
    SystemUpdate(State& s) : state(s) {
        version = "1.0";
        update_available = false;
    }

    // Checks for updates and updates the update_available attribute
    void checkForUpdate() {
        // Check for update logic...
        if (State.is_vehicle_connected && State.is_autonomous_mode_enabled) {
            update_available = true;
            cout << "Update available\n";
        }
        else {
            update_available = false;
            std::cout << "No update available\n";
        }
    }

    // Downloads and installs the available update
    void downloadUpdate() {
        if (update_available) {
            cout << "Downloading update...\n";
            // Download logic...
            cout << "Installing update...\n";
            // Install logic...
            version = "2.0";
            update_available = false;
            cout << "Update installed successfully!\n";
        }
        else {
            cout << "No update available to download\n";
        }
    }

    // Returns the current version of the system
    string getVersion() {
        return version;
    }

    // Sets the current version of the system to the specified value. (only to be used during testing)
    void setVersion(string new_version) {
        version = new_version;
    }

    // Checks to see if the system is connected to the internet
    bool isConnectedToTheInternet() {
        return state.is_vehicle_connected;
    }
};
