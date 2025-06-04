#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <ostream>

#include "Payment.h"
#include "DBConnection.h"

#define BOLD        "\033[1m"
#define RESET       "\033[0m"
#define CYAN        "\033[36m"
#define YELLOW      "\033[33m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"

using namespace std;

// Default Constructor
Payment::Payment() {
    PaymentID = "";
    PaymentAmount = 0.0;
    PaymentType = "";
    ReservationID = "";
    PaymentStatus = "Pending";
    PaymentDate = "";
}

// Parameterized Constructor
Payment::Payment(string PaymentID, double PaymentAmount, string PaymentType, string ReservationID, string PaymentStatus, string PaymentDate) {
    this->PaymentID = PaymentID;
    this->PaymentAmount = PaymentAmount;
    this->PaymentType = PaymentType;
    this->ReservationID = ReservationID;
    this->PaymentStatus = PaymentStatus;
    this->PaymentDate = PaymentDate; // Initialize PaymentDate
}

string Payment::generateNewPaymentID() {
    DBConnection db;
    string latestPaymentID = "P0000001";  // Default value if no records exist
    string newPaymentID;

    try {
        // Prepare SQL statement to fetch the latest PaymentID
        db.prepareStatement("SELECT PaymentID FROM payment ORDER BY PaymentID DESC LIMIT 1;");
        db.QueryResult();

        if (db.res && db.res->next()) {
            latestPaymentID = db.res->getString("PaymentID");
        }

        // Extract numeric part, increment, and format the new PaymentID
        int numericPart = stoi(latestPaymentID.substr(1));  // Skip "P" and extract numeric part
        numericPart++;  // Increment the numeric part
        ostringstream oss;
        oss << "P" << setw(7) << setfill('0') << numericPart;  // Format with leading zeros
        newPaymentID = oss.str();
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return newPaymentID;
}


bool Payment::update() {
    try {
        DBConnection db;
        db.prepareStatement(
            "UPDATE payment SET PaymentAmount = ?, PaymentType = ?, PaymentStatus = ?, PaymentDate = ? WHERE ReservationID = ?");

        db.setString(1, to_string(this->PaymentAmount)); // PaymentAmount as string
        db.setString(2, this->PaymentType);             // PaymentType
        db.setString(3, this->PaymentStatus);           // PaymentStatus
        db.setString(4, this->PaymentDate);             // PaymentDate
        db.setString(5, this->ReservationID);           // ReservationID

        db.QueryStatement();

        return true;
    }
    catch (const std::exception& e) {
        cout << "Error while updating payment: " << e.what() << endl;
        return false;
    }
}

bool Payment::insert() {
    try {
        DBConnection db;
        db.prepareStatement(
            "INSERT INTO payment (PaymentID, PaymentAmount, PaymentType, ReservationID, PaymentStatus, PaymentDate) "
            "VALUES (?, ?, ?, ?, ?, ?)");

        db.setString(1, this->PaymentID);        // PaymentID
        db.setString(2, to_string(this->PaymentAmount)); // PaymentAmount
        db.setString(3, this->PaymentType);     // PaymentType
        db.setString(4, this->ReservationID);   // ReservationID
        db.setString(5, this->PaymentStatus);   // PaymentStatus
        db.setString(6, this->PaymentDate);     // PaymentDate

        db.QueryStatement();

        return true;
    }
    catch (const sql::SQLException& e) {
        cout << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")\n";
        return false;
    }
    catch (const std::exception& e) {
        cout << "Error: " << e.what() << "\n";
        return false;
    }
}

void Payment::cancelPaymentByReservationID(const string& reservationID) {
    DBConnection db;
    try {
        // Step 1: Fetch the PaymentID associated with the ReservationID
        db.prepareStatement("SELECT PaymentID FROM payment WHERE ReservationID = ?;");
        db.setString(1, reservationID);
        db.QueryResult();

        if (db.res && db.res->next()) {
            string paymentID = db.res->getString("PaymentID");

            // Step 2: Proceed to delete the payment entry based on PaymentID
            db.prepareStatement("DELETE FROM payment WHERE PaymentID = ?;");
            db.setString(1, paymentID);

            if (db.executeUpdate() > 0) {
                //cout << BOLD << CYAN << "Payment with  " << paymentID << " has been successfully refund." << RESET << endl;
            }
            else {
                cout << BOLD << RED << "Failed to cancel the reservation." << RESET << endl;
            }
        }
        else {
            // No payment associated with this ReservationID
            cout << BOLD << RED << "No payment found for Receipt No " << reservationID << "." << RESET << endl;
        }
    }
    catch (const sql::SQLException& e) {
        cerr << BOLD << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << RESET << endl;
    }
    catch (const std::exception& e) {
        cerr << BOLD << "Error: " << e.what() << RESET << endl;
    }
}

void Payment::annualSlotReport(string year) {
    DBConnection db;
    try {
        // Step 1: Fetch all payments for the specified year from the database
        // Assuming the PaymentDate format is 'YYYY-MM-DD' and we are extracting payments for a given year.
        string query = "SELECT PaymentAmount, PaymentDate FROM payment WHERE PaymentStatus = 'Completed' AND YEAR(PaymentDate) = ?";
        db.prepareStatement(query);
        db.setString(1, year);  // Bind the year parameter to the query
        db.QueryResult();

        // Maps to store the total payments per month
        map<int, double> monthlyPayments;
        double totalPayments = 0.0;

        // Step 2: Process the result set and aggregate payments by month
        while (db.res && db.res->next()) {
            double paymentAmount = db.res->getDouble("PaymentAmount");
            string paymentDate = db.res->getString("PaymentDate");

            // Extract month from PaymentDate (assuming the format is 'YYYY-MM-DD')
            int month = stoi(paymentDate.substr(5, 2)); // Extract month part

            // Add the payment amount to the respective month
            monthlyPayments[month] += paymentAmount;
            totalPayments += paymentAmount;
        }

        // Step 3: Create month names for the report
        const vector<string> monthNames = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        // Step 4: Print the report header
        cout << BOLD << CYAN << "\n\t\tParking Slot Income Report for Year " << year << RESET << endl;
        cout << BOLD << YELLOW << "\t-------------------------------------------------------------" << RESET << endl ;
        cout << BOLD<< left << setw(20)  << "\tMonth" 
             << setw(25) << "Total Payment (RM)" 
             << setw(20) << "Percentage (%)" << RESET << endl;
        cout << BOLD << YELLOW << "\t-------------------------------------------------------------" << RESET << endl;

        // Step 5: Print each month, its total payment, and percentage
        for (int month = 1; month <= 12; ++month) {
            double monthPayment = monthlyPayments[month];
            double percentage = (totalPayments > 0) ? (monthPayment / totalPayments) * 100 : 0;

            // Display the month, payment amount, and percentage without the "%" symbol
            cout << BOLD << "\t" << left << setw(20) << monthNames[month - 1]  // monthNames[month-1] for the correct month name
                 << setw(25) << fixed << setprecision(2) << monthPayment
                 << setw(20) << fixed << setprecision(2) << percentage << RESET << endl;
        }

        // Step 6: Print the total row at the bottom
        cout << BOLD << YELLOW << "\t-------------------------------------------------------------" << RESET << endl;
        cout << BOLD << left << setw(20) << "\tTotal"
             << setw(25) << fixed << setprecision(2) << totalPayments
             << setw(20) << "100" << RESET << endl;  
        cout << BOLD << YELLOW << "\t-------------------------------------------------------------" << RESET << endl;

        cout << BOLD << "Select an option..." << RESET << endl;

    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

void Payment::annualSlotReporttxt(int year, ostream& out) {
    DBConnection db;
    try {
        // Step 1: Fetch all payments for the specified year from the database
        string query = "SELECT PaymentAmount, PaymentDate FROM payment WHERE PaymentStatus = 'Completed' AND YEAR(PaymentDate) = ?";
        db.prepareStatement(query);
        db.setString(1, to_string(year));  // Bind the year parameter to the query
        db.QueryResult();

        // Maps to store the total payments per month
        map<int, double> monthlyPayments;
        double totalPayments = 0.0;

        // Step 2: Process the result set and aggregate payments by month
        while (db.res && db.res->next()) {
            double paymentAmount = db.res->getDouble("PaymentAmount");
            string paymentDate = db.res->getString("PaymentDate");

            int month = stoi(paymentDate.substr(5, 2)); // Extract month from 'YYYY-MM-DD'
            monthlyPayments[month] += paymentAmount;
            totalPayments += paymentAmount;
        }

        // Step 3: Create month names for the report
        const vector<string> monthNames = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };

        // Step 4: Print the report header
        out << "\n\t\tParking Slot Income Report for Year " << year << endl;
        out << "\t-------------------------------------------------------------" << endl;
        out << left << setw(20) << "\tMonth"
            << setw(25) << "Total Payment (RM)"
            << setw(20) << "Percentage (%)" << endl;
        out << "\t-------------------------------------------------------------" << endl;

        // Step 5: Print each month, its total payment, and percentage
        for (int month = 1; month <= 12; ++month) {
            double monthPayment = monthlyPayments[month];
            double percentage = (totalPayments > 0) ? (monthPayment / totalPayments) * 100 : 0;

            out << "\t" << left << setw(20) << monthNames[month - 1]
                << setw(25) << fixed << setprecision(2) << monthPayment
                << setw(20) << fixed << setprecision(2) << percentage << endl;
        }

        // Step 6: Print the total row at the bottom
        out << "\t-------------------------------------------------------------" << endl;
        out << left << setw(20) << "\tTotal"
            << setw(25) << fixed << setprecision(2) << totalPayments
            << setw(20) << "100" << endl;
        out << "\t-------------------------------------------------------------" << endl;
    }
    catch (const sql::SQLException& e) {
        cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }
}

// Getters
string Payment::getPaymentID() const {
    return PaymentID;
}

double Payment::getPaymentAmount() const {
    return PaymentAmount;
}

string Payment::getPaymentType() const {
    return PaymentType;
}

string Payment::getReservationID() const {
    return ReservationID;
}
string Payment::getPaymentStatus() const {
    return PaymentStatus;  // Return PaymentStatus
}
string Payment::getPaymentDate() const {
    return PaymentDate;
}

// Setters
void Payment::setPaymentID(const string& PaymentID) {
    this->PaymentID = PaymentID;
}

void Payment::setPaymentAmount(double PaymentAmount) {
    this->PaymentAmount = PaymentAmount;
}

void Payment::setPaymentType(const string& PaymentType) {
    this->PaymentType = PaymentType;
}

void Payment::setReservationID(const string& ReservationID) {
    this->ReservationID = ReservationID;
}

void Payment::setPaymentStatus(const string& PaymentStatus) {
    this->PaymentStatus = PaymentStatus;  // Set PaymentStatus
}
void Payment::setPaymentDate(const string& PaymentDate) {
    this->PaymentDate = PaymentDate;
}