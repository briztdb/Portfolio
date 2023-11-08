#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <State>
using namespace std;

class Path { // class to represent Paths which are the units holding information about individual streets
    public:
        string getStreet() {
            return street;
        }
        string getDirection() {
            return direction;
        }
        void setStreet(string s) {
            street = s;
        }
        void setDirection(State vehicleState) {
            double heading = vehicleState.heading;
            if(heading >= 0 && heading < 90) {
                direction = "N";
            } else if(heading >= 90 && heading < 180) {
                direction = "W";
            } else if(heading >= 180 && heading < 270) {
                direction = "S";
            } else {
                direction = "E";
            }
        }
        string to_string() {
            return direction + " " + street;
        }
    private:
        string street;
        string direction;
};

class Route { // class to represent list of paths needed to take to reach a destination
    public:
        bool isEmpty;
        Path pop() {
            if(isEmpty) {
                throw "Empty Route cannot be popped from";
            }
            Path p = paths.back();
            paths.pop_back();
            return p;
        }
        void push(Path p) {
            paths.push_back(p);
        }
        vector<Path> getPaths() {
            return paths;
        }
        void setPaths(vector<Path> p) {
            paths = p;
            if(p.size() > 0) {
                isEmpty = false;
            } else {
                isEmpty = true;
            }
        }
    private:
        vector<Path> paths;
};

class Location { // class to represent specific locations 
    public:
        void setPreciseLocation(State vehicleState) {
            lat = vehicleState.latitude;
            lon = vehicleState.longitude;
        }
        vector<double> getPreciseLocation() {
            vector<double> coords;
            coords.push_back(lat);
            coords.push_back(lon);
            return coords;
        }
        void setAddress(string a) {
            address = a;
        }
        string getAddress() {
            return address;
        }
    private:
        string address;
        double lat, lon;
};

class NavPage {
    public:
        Location getCurrentLocation() {
            return curLoc;
        }
        void setCurrentLocation(Location l) {
            curLoc = l;
        }
        Location getDestination() {
            return dest;
        }
        void setDestination(Location d) {
            dest = d;
        }
        bool isRouteIP() {
            if(route.isEmpty) {
                return false;
            }
            return true;
        }
        void updateRoute(Route r) {
            route = r;
        }
    private:
        Route route;
        Location curLoc;
        Location dest;
};

class SysPage { // class to represent system view page mode and all its information
    public:
        void refreshInfo(State vehicleState) {
            speed = vehicleState.speed;
            charge = vehicleState.battery_level;
        }
        int getSpeed() {
            return speed;
        }
        double getCharge() {
            return charge;
        }
        bool isSoftUpdateAvail() {
            return softwareUpdateAvail;
        }
        void setSoftUpdate(bool b) {
            softwareUpdateAvail = b;
        }
    private:
        int speed; // int representing how fast the car is traveling in mph
        double charge; // float representing charge amt left in battery from 0-1
        bool softwareUpdateAvail = false; // bool representing if there is a software update available
};

class techPage { // class to represent technician interface
    public:
        void firstTimeLogin(string user, string pass) {
            if(pass.length() < 8) {
                throw "Password cannot be less than 8 characters"
            } else {
                uid = user;
                pwd = pass;
            }
        }
        void loginAttempt(string user, string pass) {
            if(uid = user && pwd = pass) {
                loggedin = true;
            }
        }
        void signOut() {
            loggedin = false;
        }
        void updateLogs(String filename, String log) {
            logs.open(filename);
            logs << log << "\n";
            logs.close();
        }
    private:
    string uid = "admin";
    string pwd = "admin";
    bool loggedin = false;
    fstream logs;

};

class HUD {
    public:
    NavPage nav;
    SysPage sys;
    techPage tech;
    
    int pageOpen; //int to represent which page is open currently: 1 for nav, 2 for sys, 3 for tech
    void buildPages() {
        NavPage n;
        nav = n;
        SysPage s;
        sys = s;
        pageOpen = true;
        techPage t;
        tech = t;
    }
    void swapPages(int i) {
        if(i > 3 || i < 1) {
            return;
        }
        pageOpen = i;
    }
    string currPage() {
        if(pageOpen == 1) {
            return "Navigation View Mode";
        } 
        if(pageOpen == 2) {
            return "System View Mode";
        }
        return "Technician View Mode";
    }
};