#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <chrono>

#include "Reservation.h"
#include "DBConnection.h"

#define BOLD        "\033[1m"
#define RESET       "\033[0m"
#define CYAN        "\033[36m"
#define YELLOW      "\033[33m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"

using namespace std;

// Default Constructor
Reservation::Reservation() {
    ReservationID = "";
    UserID = "";
    TotalTimeReserved = "";
    ReservationDate = "";
    ReservedSlotName = "";
    ReservedTimeSlots = ""; 
}

// Parameterized Constructor
Reservation::Reservation(const string& ReservationID, const string& UserID, const string& TotalTimeReserved,
    const string& ReservationDate, const string& ReservedSlotName, const string& ReservedTimeSlots) {
    this->ReservationID = ReservationID;
    this->UserID = UserID;
    this->TotalTimeReserved = TotalTimeReserved;
    this->ReservationDate = ReservationDate;
    this->ReservedSlotName = ReservedSlotName;
    this->ReservedTimeSlots = ReservedTimeSlots; 
}

// Method to insert a reservation into the database
bool Reservation::insert() {
    DBConnection db;
    try {
        string query = "INSERT INTO reservations (ReservationID, UserID, TotalTimeReserved, ReservationDate, ReservedSlotName, ReservedTimeSlot) "
            "VALUES (?, ?, ?, ?, ?, ?);";

        db.prepareStatement(query);
        db.setString(1, ReservationID);
        db.setString(2, UserID);
        db.setString(3, TotalTimeReserved);
        db.setString(4, ReservationDate);
        db.setString(5, ReservedSlotName);

        // Use ReservedTimeSlots directly (already a concatenated string)
        db.setString(6, ReservedTimeSlots);

        if (db.executeUpdate() > 0) {
            return true;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
    return false;
}

string Reservation::generateNewReservationID() {
    DBConnection db;
    string latestReservationID = "R0000001";  // Default value if no records exist
    string newReservationID;

    try {
        bool isDuplicate = true;
        while (isDuplicate) {
            db.prepareStatement("SELECT ReservationID FROM reservations ORDER BY ReservationID DESC LIMIT 1;");
            db.QueryResult();

            if (db.res && db.res->next()) {
                latestReservationID = db.res->getString("ReservationID");
            }

            int numericPart = std::stoi(latestReservationID.substr(1));
            numericPart++;
            std::ostringstream oss;
            oss << "R" << std::setw(7) << std::setfill('0') << numericPart;
            newReservationID = oss.str();

            db.prepareStatement("SELECT COUNT(*) AS count FROM reservations WHERE ReservationID = ?;");
            db.setString(1, newReservationID);
            db.QueryResult();

            if (db.res && db.res->next()) {
                int count = db.res->getInt("count");
                isDuplicate = (count > 0);
            }
            else {
                isDuplicate = false;
            }
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return newReservationID;
}

// Method to show reservations by a specific user
void Reservation::showReservationsByUser(const string& userID) {
    DBConnection db;
    try {
        db.prepareStatement("SELECT * FROM reservations WHERE UserID = ?");
        db.setString(1, userID);
        db.QueryResult();

        vector<Reservation> reservations;
        while (db.res && db.res->next()) {
            // Extract reservation details and add to vector
            Reservation reservation(
                db.res->getString("ReservationID"),
                db.res->getString("UserID"),
                db.res->getString("TotalTimeReserved"),
                db.res->getString("ReservationDate"),
                db.res->getString("ReservedSlotName"),
                db.res->getString("ReservedTimeSlot")
            );
            reservations.push_back(reservation);
        }

        if (reservations.empty()) {
            cout << BOLD << "\n\tNo reservations done yet" << RESET << endl;
        }
        else {
            // Calculate the table width for a balanced separator
            const int columnWidth = 25; // Increased width to compensate for removal of User ID
            const int totalWidth = columnWidth * 5 + 5; // Adjust for 5 columns now

            // Print header with bold formatting
            cout << BOLD << string(totalWidth, '-') << RESET << endl;
            cout << BOLD
                << "| " << setw(columnWidth - 1) << left << "Receipt No"
                << "| " << setw(columnWidth - 1) << left << "Total Time Reserved"
                << "| " << setw(columnWidth - 1) << left << "Reservation Date"
                << "| " << setw(columnWidth - 1) << left << "Reserved Slot Name"
                << "| " << setw(columnWidth - 1) << left << "Reserved Time Slot"
                << "|" << RESET << endl;
            cout << BOLD << string(totalWidth, '-') << RESET << endl;

            // Print each reservation's details in a table-like format
            for (const auto& res : reservations) {
                cout << BOLD
                    << "| " << setw(columnWidth - 1) << left << res.getReservationID()
                    << "| " << setw(columnWidth - 1) << left << (res.getTotalTimeReserved() + " hour(s)")
                    << "| " << setw(columnWidth - 1) << left << res.getReservationDate()
                    << "| " << setw(columnWidth - 1) << left << res.getReservedSlotName()
                    << "| " << setw(columnWidth - 1) << left << res.getReservedTimeSlots()
                    << "|" << RESET << endl;
            }
            cout << BOLD << string(totalWidth, '-') << RESET << endl;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << BOLD << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << RESET << endl;
    }
    catch (const std::exception& e) {
        cerr << BOLD << "Error: " << e.what() << RESET << endl;
    }
}

// Method to show reservations by a specific user and the current date
void Reservation::showReservationsByUserAndDate(const string& userID) {
    DBConnection db;
    string currentDate = getCurrentDate();  // Assuming this function retrieves the current date in "YYYY-MM-DD" format

    try {
        db.prepareStatement("SELECT * FROM reservations WHERE UserID = ? AND ReservationDate >= ? ORDER BY ReservationDate ASC");
        db.setString(1, userID);
        db.setString(2, currentDate);
        db.QueryResult();

        vector<Reservation> reservations;
        while (db.res && db.res->next()) {
            Reservation reservation(
                db.res->getString("ReservationID"),
                db.res->getString("UserID"),
                db.res->getString("TotalTimeReserved"),
                db.res->getString("ReservationDate"),
                db.res->getString("ReservedSlotName"),
                db.res->getString("ReservedTimeSlot")
            );
            reservations.push_back(reservation);
        }

        if (reservations.empty()) {
            cout << BOLD << RED << "\tNo reservations found." << RESET << endl;
        }
        else {
            const int columnWidth = 25;
            const int totalWidth = columnWidth * 5 + 5;

            cout << BOLD << string(totalWidth, '-') << RESET << endl;
            cout << BOLD
                << "| " << setw(columnWidth - 1) << left << "Receipt No"
                << "| " << setw(columnWidth - 1) << left << "Total Time Reserved"
                << "| " << setw(columnWidth - 1) << left << "Reservation Date"
                << "| " << setw(columnWidth - 1) << left << "Reserved Slot Name"
                << "| " << setw(columnWidth - 1) << left << "Reserved Time Slot"
                << "|" << RESET << endl;
            cout << BOLD << string(totalWidth, '-') << RESET << endl;

            for (const auto& res : reservations) {
                cout << BOLD
                    << "| " << setw(columnWidth - 1) << left << res.getReservationID()
                    << "| " << setw(columnWidth - 1) << left << (res.getTotalTimeReserved() + " hour(s)")
                    << "| " << setw(columnWidth - 1) << left << res.getReservationDate()
                    << "| " << setw(columnWidth - 1) << left << res.getReservedSlotName()
                    << "| " << setw(columnWidth - 1) << left << res.getReservedTimeSlots()
                    << "|" << RESET << endl;
            }
            cout << BOLD << string(totalWidth, '-') << RESET << endl;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << BOLD << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << RESET << endl;
    }
    catch (const std::exception& e) {
        cerr << BOLD << "Error: " << e.what() << RESET << endl;
    }
}

// Method to cancel a reservation by ReservationID
bool Reservation::cancelReservation(const string& reservationID) {
    DBConnection db;
    try {
        // Check if the reservation exists before attempting deletion
        db.prepareStatement("SELECT ReservationID FROM reservations WHERE ReservationID = ?;");
        db.setString(1, reservationID);
        db.QueryResult();

        if (db.res && db.res->next()) {

            // Proceed to delete the reservation
            db.prepareStatement("DELETE FROM reservations WHERE ReservationID = ?;");
            db.setString(1, reservationID);

            if (db.executeUpdate() > 0) {
                //cout << BOLD << CYAN << "Reservation with ID " << reservationID << " has been successfully canceled." << RESET << endl;
                return true;
            }
            else {
                cout << BOLD << RED << "Failed to cancel reservation with  " << reservationID << "." << RESET << endl;
            }
        }
        else {
            cout << BOLD << RED << "No reservation found with " << reservationID << "." << RESET << endl;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << BOLD << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << RESET << endl;
    }
    catch (const std::exception& e) {
        cerr << BOLD << "Error: " << e.what() << RESET << endl;
    }

    return false;
}


string Reservation::getCurrentDate() {
    auto now = chrono::system_clock::now();
    time_t in_time_t = chrono::system_clock::to_time_t(now);

    struct tm buf;
    localtime_s(&buf, &in_time_t);

    stringstream ss;
    ss << put_time(&buf, "%Y-%m-%d");
    return ss.str();
}

// Getters
string Reservation::getReservationID() const {
    return ReservationID;
}

string Reservation::getUserID() const {
    return UserID;
}

string Reservation::getTotalTimeReserved() const {
    return TotalTimeReserved;
}

string Reservation::getReservationDate() const {
    return ReservationDate;
}

string Reservation::getReservedSlotName() const {
    return ReservedSlotName;
}

string Reservation::getReservedTimeSlots() const {
    return ReservedTimeSlots; 
}

// Setters
void Reservation::setReservationID(const string& ReservationID) {
    this->ReservationID = ReservationID;
}

void Reservation::setUserID(const string& UserID) {
    this->UserID = UserID;
}

void Reservation::setTotalTimeReserved(const string& TotalTimeReserved) {
    this->TotalTimeReserved = TotalTimeReserved;
}

void Reservation::setReservationDate(const string& ReservationDate) {
    this->ReservationDate = ReservationDate;
}

void Reservation::setReservedSlotName(const string& ReservedSlotName) {
    this->ReservedSlotName = ReservedSlotName;
}

void Reservation::setReservedTimeSlots(const string& ReservedTimeSlots) {
    this->ReservedTimeSlots = ReservedTimeSlots;
}
