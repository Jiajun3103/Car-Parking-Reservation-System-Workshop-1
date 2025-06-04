#pragma once
#ifndef FINE_H
#define FINE_H

#include <string>
using namespace std;

class Fine {
private:
    string FineID;
    string UserID;
    double FineAmount;
    string FineReason;
    string FineStatus;
    string FineDate;

public:
    // Constructors
    Fine();  // Default constructor
    Fine(string FineID, string UserID, double FineAmount, string FineReason, string FineStatus, string FineDate);

    string generateNewFineID();
    double getTotalFine(const string& userID);
    void displayFineHistory(const string& userID);
    string displayFineReason(const string& userID);
    bool insert();

    // Getters
    string getFineID() const;
    string getUserID() const;
    double getFineAmount() const;
    string getFineReason() const;
    string getFineStatus() const;
    string getFineDate() const;

    // Setters
    void setFineID(const string& FineID);
    void setUserID(const string& UserID);
    void setFineAmount(double FineAmount);
    void setFineReason(const string& FineReason);
    void setFineStatus(const string& FineStatus);
    void setFineDate(const string& FineDate);
};

#endif // FINE_H

