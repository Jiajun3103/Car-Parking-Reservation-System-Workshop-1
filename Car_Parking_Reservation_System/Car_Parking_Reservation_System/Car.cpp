#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>
#include <vector>

#include "Car.h" 
#include "DBConnection.h"

using namespace std;

// Default Constructor
Car::Car() {
    CarID = "";
    UserID = "";
    CarModel = "";
    CarLicensePlate = "";
    CarDate = "";
}

// Parameterized Constructor
Car::Car(string CarID, string UserID, string CarModel, string CarLicensePlate, string CarDate) {
    this->CarID = CarID;
    this->UserID = UserID;
    this->CarModel = CarModel;
    this->CarLicensePlate = CarLicensePlate;
    this->CarDate = CarDate; 
}

string Car::generateNewCarID() {
    DBConnection db;
    string latestCarID = "C0000001";  // Default value if no records exist
    string newCarID;

    try {
        // Prepare SQL statement to fetch the latest CarID
        db.prepareStatement("SELECT CarID FROM Car ORDER BY CarID DESC LIMIT 1;");
        db.QueryResult();

        if (db.res && db.res->next()) {
            latestCarID = db.res->getString("CarID");
        }

        // Extract numeric part, increment, and format the new CarID
        int numericPart = std::stoi(latestCarID.substr(1));  // Extract numeric part (skip "C")
        numericPart++;  // Increment the numeric part
        std::ostringstream oss;
        oss << "C" << std::setw(7) << std::setfill('0') << numericPart;  // Format with leading zeros
        newCarID = oss.str();

    }
    catch (const sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return newCarID;
}

bool Car::insert() {
    DBConnection db;

    try {
        // Prepare the insert query with placeholders
        db.prepareStatement("INSERT INTO Car (CarID, UserID, CarModel, CarLicensePlate, CarDate) VALUES (?, ?, ?, ?, ?)");

        // Bind the values to the placeholders
        db.setString(1, this->CarID);
        db.setString(2, this->UserID);
        db.setString(3, this->CarModel);
        db.setString(4, this->CarLicensePlate);
        db.setString(5, this->CarDate); // Bind CarDate to the query

        // Execute the insert query
        if (db.executeUpdate() > 0) {  // Returns the number of affected rows
            return true;  // Insert successful
        }
        else {
            cout << "Insert query executed but affected no rows." << endl;
            return false;  // Insert failed
        }
    }
    catch (const std::exception& e) {
        cerr << "Error inserting car details: " << e.what() << endl;
        return false;
    }
}

bool Car::isUserIDValid(const string& userID) {
    DBConnection db;
    try {
        // Prepare SQL statement to check if UserID exists in the user table
        db.prepareStatement("SELECT COUNT(*) FROM user WHERE UserID = ?;");
        db.setString(1, userID);
        db.QueryResult();

        if (db.res && db.res->next()) {
            int count = db.res->getInt(1);
            return count > 0;  // If count > 0, the UserID exists
        }
        return false;  // If no such UserID exists
    }
    catch (const sql::SQLException& e) {
        std::cerr << "SQL Error: " << e.what() << std::endl;
        return false;
    }
}

// Getters
string Car::getCarID() {
    return CarID;
}

string Car::getUserID() {
    return UserID;
}

string Car::getCarModel() {
    return CarModel;
}

string Car::getCarLicensePlate() {
    return CarLicensePlate;
}

string Car::getCarDate() {
    return CarDate;
}

// Setters
void Car::setCarID(string CarID) {
    this->CarID = CarID;
}

void Car::setUserID(string UserID) {
    this->UserID = UserID;
}

void Car::setCarModel(string CarModel) {
    this->CarModel = CarModel;
}

void Car::setCarLicensePlate(string CarLicensePlate) {
    this->CarLicensePlate = CarLicensePlate;
}

void Car::setCarDate(string CarDate) {
    this->CarDate = CarDate;  
}