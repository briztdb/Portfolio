using namespace std;

   struct State {
     // Navigation related fields
     double latitude;   // Current latitude of the car
     double longitude;  // Current longitude of the car
     double altitude;   // Current altitude of the car in meters
     double speed;      // Current speed of the car in km/h
     double heading;    // Current heading of the car in degrees (0 = North)
     double distance_traveled;  // Total distance traveled by the car in km
     int satellites_used;       // Number of GPS satellites currently in use

     // Perception related fields
     bool is_obstacle_detected;       // Is there an obstacle in the car's path?
     bool is_traffic_light_detected;  // Is a traffic light in view of the car's
                                      // cameras?
     bool is_pedestrian_detected;     // Is a pedestrian in view of the car's
                                      // cameras?
     double distance_to_obstacle;  // Distance to the nearest obstacle in meters
     double obstacle_size;  // Size of the detected obstacle in meters (e.g.
                            // width, height)

     // Control related fields
     double steering_angle;  // Current steering angle of the car's wheels in
                             // degrees
     double throttle;        // Current throttle position (0-100%)
     double brake;           // Current brake position (0-100%)
     double acceleration;    // Current acceleration of the car in m/s^2

     // Battery related fields
     double battery_level;    // Current battery level in percent
     double charging_rate;    // Current charging rate in kW
     double estimated_range;  // Estimated range of the car in km based on
                              // current battery level

     // System related fields
     bool is_autonomous_mode_enabled;  // Is autonomous driving mode currently
                                       // enabled?
     bool is_vehicle_connected;        // Is the car currently connected to the
                                       // internet?
     bool is_software_update_available;  // Is a software update available for
                                         // the car's system?
   };

