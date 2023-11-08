#include <iostream>
#include <HUDclasses.cpp>
using namespace std;

int main() {
    // tests for NavPage
    HUD hud1;
    hud1.buildPages();
    NavPage nav = hud1.nav;
    SysPage sys = hud1.sys;
    cout << "Beginning of tests for NavPage: " << endl;
    cout << "Test 1: \t" << "Expected: " << "System View Mode\t" << "Actual: " << hud1.currPage() << endl; 
    hud1.swapPages();
    cout << "Test 2: \t" << "Expected: " << "Navigation View Mode\t" << "Actual: " << hud1.currPage() << endl; 

    Route r;
    Path p1;
    Path p2;
    Path p3;

    p1.setStreet("Broadway");
    p2.setStreet("Blue Sky Lane");
    p3.setStreet("Orangeburg Road");
    p3.setDirection("N ");
    r.push(p1);
    r.push(p2);
    r.push(p3);
    cout << "Test 3: \t" << "Expected: " << "N Orangeburg Road\t" << "Actual: " << r.pop().to_string() << endl;
    cout << "Test 4: \t" << "Expected: " << "Blue Sky Lane\t\t" << "Actual: " << r.pop().to_string() << endl;
    r.push(p3);
    cout << "Test 5: \t" << "Expected: " << "N Orangeburg Road\t" << "Actual: " << r.pop().to_string() << endl;
    cout << "Test 6: \t" << "Expected: " << "Broadway\t\t" << "Actual: " << r.pop().to_string() << endl;
    cout << "Test 7: \t" << "Expected: " << "0\t\t\t" << "Actual: " << r.isEmpty << endl;
    cout << "End of tests for NavPage ---------------------------------------------------" << endl;
    // Tests for SysPage
    cout << "\nBeginning of tests for SysPage: " << endl;
    sys.refreshInfo(4, .67);
    cout << "Test 1: \t" << "Expected: " << "4\t\t\t" << "Actual: " << sys.getSpeed() << endl;
    cout << "Test 2: \t" << "Expected: " << "67.000000%\t\t" << "Actual: " << sys.getChargePercent() << endl;
    cout << "Test 3: \t" << "Expected: " << "0\t\t\t" << "Actual: " << sys.isSoftUpdateAvail() << endl;
    sys.setSoftUpdate(true);
    cout << "Test 4: \t" << "Expected: " << "1\t\t\t" << "Actual: " << sys.isSoftUpdateAvail() << endl;

    
}