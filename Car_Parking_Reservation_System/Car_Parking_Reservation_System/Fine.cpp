#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

#include "Fine.h"
#include "DBConnection.h"

using namespace std;


// Default Constructor
Fine::Fine() {
    FineID = "";
    UserID = "";
    FineAmount = 0.0;
    FineReason = "";
    FineStatus = "No"; // Default status is unpaid
    FineDate = ""; // Initialize FineDate as empty
}

// Parameterized Constructor
Fine::Fine(string FineID, string UserID, double FineAmount, string FineReason, string FineStatus, string FineDate) {
    this->FineID = FineID;
    this->UserID = UserID;
    this->FineAmount = FineAmount;
    this->FineReason = FineReason;
    this->FineStatus = FineStatus;
    this->FineDate = FineDate;
}

string Fine::generateNewFineID() {
    DBConnection db;
    string latestFineID = "F0000001"; // Default value if no records exist
    string newFineID;

    try {
        // Prepare SQL statement to fetch the latest FineID
        db.prepareStatement("SELECT FineID FROM fines ORDER BY FineID DESC LIMIT 1;");
        db.QueryResult();

        if (db.res && db.res->next()) {
            latestFineID = db.res->getString("FineID");
        }

        // Extract numeric part, increment, and format the new FineID
        int numericPart = stoi(latestFineID.substr(1));
        numericPart++;

        ostringstream oss;
        oss << "F" << setw(7) << setfill('0') << numericPart;
        newFineID = oss.str();
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return newFineID;
}

// Method to get the total unpaid fine for a specific user
double Fine::getTotalFine(const string& userID) {
    DBConnection db;
    double totalFine = 0.0;

    try {
        // Prepare SQL statement to fetch the sum of unpaid fines for the user
        db.prepareStatement(
            "SELECT SUM(FineAmount) AS TotalFine FROM fines WHERE UserID = ? AND FineStatus = 'No';");
        db.setString(1, userID);

        db.QueryResult();  // Execute the query

        if (db.res && db.res->next()) {
            totalFine = db.res->getDouble("TotalFine");  // Retrieves the total fine amount
        }
        else {
            cerr << "No unpaid fines found for user: " << userID << endl;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return totalFine;
}

// Retrieve fine history for the user
void Fine::displayFineHistory(const string& userID) {
    DBConnection db;

    try {
        db.prepareStatement("SELECT FineAmount, FineReason, FineStatus, FineDate FROM fines WHERE UserID = ?;");
        db.setString(1, userID);
        db.QueryResult();

        if (db.res) {
            while (db.res->next()) {
                cout << "Fine Amount: " << db.res->getDouble("FineAmount") << endl;
                cout << "Reason: " << db.res->getString("FineReason") << endl;
                cout << "Status: " << db.res->getString("FineStatus") << endl;
                cout << "Date: " << db.res->getString("FineDate") << endl;
                cout << "----------\n";
            }
        }
        else {
            cout << "No fines found for the user.\n";
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

string Fine::displayFineReason(const string& userID) {
    DBConnection db;
    string fineReason = "Unknown"; // Default value if no reason is found

    try {
        db.prepareStatement("SELECT FineReason FROM fines WHERE UserID = ? AND FineStatus = 'No';");
        db.setString(1, userID);
        db.QueryResult();

        if (db.res && db.res->next()) {
            fineReason = db.res->getString("FineReason");
        }
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return fineReason;
}

// Function to insert fine details into the database
bool Fine::insert() {
    DBConnection db;
    try {
        db.prepareStatement(
            "INSERT INTO fines (FineID, UserID, FineAmount, FineReason, FineStatus, FineDate) VALUES (?, ?, ?, ?, ?, ?);");

        db.setString(1, this->FineID);
        db.setString(2, this->UserID);
        db.setString(3, to_string(this->FineAmount));
        db.setString(4, this->FineReason);
        db.setString(5, this->FineStatus);
        db.setString(6, this->FineDate);

        db.executeUpdate();

        cout << "\nFine successfully inserted into the database." << endl;
        return true;
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return false;
    }
}

// Getters
string Fine::getFineID() const {
    return FineID;
}

string Fine::getUserID() const {
    return UserID;
}

double Fine::getFineAmount() const {
    return FineAmount;
}

string Fine::getFineReason() const {
    return FineReason;
}

string Fine::getFineStatus() const {
    return FineStatus;
}

string Fine::getFineDate() const {
    return FineDate;
}

// Setters
void Fine::setFineID(const string& FineID) {
    this->FineID = FineID;
}

void Fine::setUserID(const string& UserID) {
    this->UserID = UserID;
}

void Fine::setFineAmount(double FineAmount) {
    this->FineAmount = FineAmount;
}

void Fine::setFineReason(const string& FineReason) {
    this->FineReason = FineReason;
}

void Fine::setFineStatus(const string& FineStatus) {
    this->FineStatus = FineStatus;
}

void Fine::setFineDate(const string& FineDate) {
    this->FineDate = FineDate;
}