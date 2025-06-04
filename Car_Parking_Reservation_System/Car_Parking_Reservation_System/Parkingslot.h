#pragma once
#ifndef PARKINGSLOT_H
#define PARKINGSLOT_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class ParkingSlot {
private:
    string SlotID;
    string SlotName;
    string SlotTime;
    string SlotStatus;

public:
    ParkingSlot();// default constructor
    ParkingSlot(string SlotID, string SlotName, string SlotTime, string SlotStatus);

    static vector<ParkingSlot> fetchAvailableSlots(); // Fetch parking slots
    void displayInfo() const; // Display SlotName
    string generateNewSlotID();
    void showSlotInfo(string& slotName);
    bool isAvailable(const string& slotName, int timeSlotIndex);
    static string getTimeSlotString(int timeSlotIndex);
    string fetchSlotIDByTime(const string& slotName, const string& slotTime);
    bool updateSlotStatus(const string& slotName, const string& slotTime, const string& newStatus);

    // getters 
    string getSlotID();
    string getSlotName();
    string getSlotTime();
    string getSlotStatus();
    string getSlotTime(int timeSlotIndex);

    // setters
    void setSlotID(string SlotID);
    void setSlotName(string SlotName);
    void setSlotTime(string SlotTime);
    void setSlotStatus(string SlotStatus);
};

#endif // PARKINGSLOT_H
