#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>
#include <map>

#include "Parkingslot.h"
#include "DBConnection.h"

// ANSI escape codes for color
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"

ParkingSlot::ParkingSlot() {
    SlotID = "";
    SlotName = "";
    SlotTime = "";
    SlotStatus = "";
}

ParkingSlot::ParkingSlot(string SlotID, string SlotName, string SlotTime, string SlotStatus){
    this->SlotID = SlotID;
    this->SlotName = SlotName;
    this->SlotTime = SlotTime;
    this->SlotStatus = SlotStatus;
}

//vector<ParkingSlot> availableSlots = ParkingSlot::fetchAvailableSlots(); // Fetch all parking slots
vector<ParkingSlot> ParkingSlot::fetchAvailableSlots() {
    vector<ParkingSlot> slots;
    DBConnection db;

    try {
        db.prepareStatement("SELECT SlotID, SlotName, SlotStatus FROM parkingslot;");
        db.QueryResult();

        while (db.res && db.res->next()) {
            string id = db.res->getString("SlotID");
            string name = db.res->getString("SlotName");
            string time = db.res->getString("SlotTime");
            string status = db.res->getString("SlotStatus");
            slots.emplace_back(id, name, time, status);
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return slots;
}

////show SlotName in here get from database
//for (const auto& slot : availableSlots) {
//    slot.displayInfo(); // Show only SlotName
//}
void ParkingSlot::displayInfo() const {
    cout << "Slot Name: " << SlotName << endl; // Display only SlotName
}

string ParkingSlot::generateNewSlotID() {
	DBConnection db;
	std::string latestSlotID = "S001"; // Default value if no records exist
	std::string newSlotID;

	try {
		// Prepare SQL statement to fetch the latest SlotID
		db.prepareStatement("SELECT SlotID FROM parkingslot ORDER BY SlotID DESC LIMIT 1;");
		db.QueryResult();

		if (db.res && db.res->next()) {
			latestSlotID = db.res->getString("SlotID");
		}

		// Extract numeric part, increment, and format the new SlotID
		int numericPart = std::stoi(latestSlotID.substr(1));
		numericPart++;
		std::ostringstream oss;
		oss << "U" << std::setw(7) << std::setfill('0') << numericPart;
		newSlotID = oss.str();
	}
	catch (const sql::SQLException& e) {
		std::cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return newSlotID;
}

// Show slot information
void ParkingSlot::showSlotInfo(string& slotName) {
    DBConnection db;

    try {
        // Query to fetch time slots and their statuses for the specified parking slot
        db.prepareStatement("SELECT SlotTime, SlotStatus FROM parkingslot WHERE SlotName = ?;");
        db.stmt->setString(1, slotName);
        db.QueryResult();

        system("cls");
        cout << BOLD << CYAN << setw(40) << "\n\tParking Slot Information" << RESET << endl;
        cout << YELLOW << "\t------------------------------" << RESET << endl;
        cout << BOLD << "\tParking Slot: " << RESET << slotName << endl;
        cout << "\tTime\t\t\tStatus" << endl;
        cout << YELLOW << "\t------------------------------" << RESET << endl;

        // Define the alphabet to map time slots to letters (A-L)
        const char timeSlotLabels[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' };

        int index = 0; // Start index from 0 to map to the correct letter

        // Loop through the results and display SlotTime and SlotStatus
        while (db.res && db.res->next()) {
            string time = db.res->getString("SlotTime");
            string status = db.res->getString("SlotStatus");

            // Display slot with letter label (A-L)
            if (index < 12) {
                cout << BOLD << "\t" << "[" << timeSlotLabels[index] << "] " << RESET << time
                    << "\t\t" << status << endl;
            }
            ++index; // Increment index for each row
        }

        // If no results were found, display a message
        if (index == 0) {
            cout << RED << "\n\tNo time slots found for this parking slot." << RESET << endl;
        }

        cout << YELLOW << "\n\t------------------------------" << RESET << endl;

    }
    catch (const sql::SQLException& e) {
        cerr << RED << "\nSQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << RESET << endl;
    }
    catch (const exception& e) {
        cerr << RED << "\nError: " << e.what() << RESET << endl;
    }
}

// Check if a parking slot is available
bool ParkingSlot::isAvailable(const string& slotName, int timeSlotIndex) {
    DBConnection db;
    string status;
    string timeSlotString = getTimeSlotString(timeSlotIndex);  // Convert timeSlotIndex to the corresponding time string

    if (timeSlotString.empty()) {
        cerr << "Invalid time slot index: " << timeSlotIndex << endl;
        return false;
    }

    try {
        // Query to check availability of a specific time slot
        db.prepareStatement("SELECT SlotStatus FROM parkingslot WHERE SlotName = ? AND SlotTime = ?;");
        db.stmt->setString(1, slotName);
        db.stmt->setString(2, timeSlotString);  // Use the time string instead of index
        db.QueryResult();

        if (db.res && db.res->next()) {
            status = db.res->getString("SlotStatus");
            cout << "Status for " << timeSlotString << ": " << status << endl; // Debugging line
        }

        return status == "Available";  // Check if status is "Available"
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
        return false;
    }
}

string ParkingSlot::getTimeSlotString(int timeSlotIndex) {
    switch (timeSlotIndex) {
    case 1: return "10am-11am";
    case 2: return "11am-12pm";
    case 3: return "12pm-1pm";
    case 4: return "1pm-2pm";
    case 5: return "2pm-3pm";
    case 6: return "3pm-4pm";
    case 7: return "4pm-5pm";
    case 8: return "5pm-6pm";
    case 9: return "6pm-7pm";
    case 10: return "7pm-8pm";
    case 11: return "8pm-9pm";
    case 12: return "9pm-10pm";
    default: return ""; // Return empty if invalid index
    }
}

string ParkingSlot::fetchSlotIDByTime(const string& slotName, const string& slotTime) {
    DBConnection db;
    string slotID;

    try {
        db.prepareStatement("SELECT SlotID FROM parkingslot WHERE SlotName = ? AND SlotTime = ?;");
        db.stmt->setString(1, slotName);
        db.stmt->setString(2, slotTime);
        db.QueryResult();

        if (db.res && db.res->next()) {
            slotID = db.res->getString("SlotID");
        }
        else {
            cout << "No slot found for the given slot name and time: " << slotName << ", Time " << slotTime << ".\n";
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }

    return slotID;
}

bool ParkingSlot::updateSlotStatus(const string& slotName, const string& slotTime, const string& newStatus) {
    DBConnection db;

    try {
        // Update query using SlotName and SlotTime
        db.prepareStatement("UPDATE parkingslot SET SlotStatus = ? WHERE SlotName = ? AND SlotTime = ?;");
        db.stmt->setString(1, newStatus);  // New status to update (e.g., "Booked")
        db.stmt->setString(2, slotName);  // SlotName to match
        db.stmt->setString(3, slotTime);  // SlotTime to match
        int rowsAffected = db.executeUpdate();

        if (rowsAffected > 0) {
            return true;  // Successfully updated the slot status
        }
        else {
            cout << "Failed to update slot status. Slot may not exist or is already updated." << endl;
            return false;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
        return false;
    }
}


// getters
string ParkingSlot::getSlotID() {
    return SlotID;
}
string ParkingSlot::getSlotName() {
    return SlotName;
}
string ParkingSlot::getSlotTime() {
    return SlotTime;
}
string ParkingSlot::getSlotStatus() {
    return SlotStatus;
}
string ParkingSlot::getSlotTime(int timeSlotIndex) {
    // Example: Generate a time string based on the index
    return "Time " + to_string(timeSlotIndex);
}

// setters
void ParkingSlot::setSlotID(string SlotID) {
    this->SlotID = SlotID;
}
void ParkingSlot::setSlotName(string SlotName) {
    this->SlotName = SlotName;
}
void ParkingSlot::setSlotTime(string SlotTime) {
    this->SlotTime = SlotTime;
}
void ParkingSlot::setSlotStatus(string SlotStatus) {
    this->SlotStatus = SlotStatus;
}
