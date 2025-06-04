#pragma once
#ifndef PAYMENT_H
#define PAYMENT_H

#include <string>
#include <ostream>
using namespace std;

class Payment {
private:
    string PaymentID;         // Unique identifier for payment
    double PaymentAmount;     // Amount paid
    string PaymentType;       // Payment method (Credit Card, E-Wallet, Bank Transfer)
    string ReservationID;     // Associated Reservation ID
    string PaymentStatus;
    string PaymentDate;
public:
    // Constructors
    Payment();  // Default constructor
    Payment(string PaymentID, double PaymentAmount, string PaymentType, string ReservationID, string PaymentStatus, string PaymentDate);

    string generateNewPaymentID();
    bool update();
    bool insert();
    void cancelPaymentByReservationID(const string& reservationID);
    void annualSlotReport(string year);
    void annualSlotReporttxt(int year, ostream& out = cout);

    // Getters
    string getPaymentID() const;
    double getPaymentAmount() const;
    string getPaymentType() const;
    string getReservationID() const;
    string getPaymentStatus() const;
    string getPaymentDate() const;

    // Setters
    void setPaymentID(const string& PaymentID);
    void setPaymentAmount(double PaymentAmount);
    void setPaymentType(const string& PaymentType);
    void setReservationID(const string& ReservationID);
    void setPaymentStatus(const string& PaymentStatus);
    void setPaymentDate(const string& PaymentDate);
};

#endif // PAYMENT_H

