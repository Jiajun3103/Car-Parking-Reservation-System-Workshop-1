#pragma once
#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
using namespace std;

class Reservation {
private:
    // Menber Variables
    string ReservationID;
    string UserID;
    string TotalTimeReserved;
    string ReservationDate;
    string ReservedSlotName;
    string ReservedTimeSlots;

public:
    // Constructors
    Reservation();
    Reservation(const string& ReservationID, const string& UserID, const string& TotalTimeReserved,
        const string& ReservationDate, const string& ReservedSlotName, const string& ReservedTimeSlots);

    // Methods
    bool insert();  // Insert a new reservation into the database
    string generateNewReservationID();
    void showReservationsByUser(const string& userID);
    void showReservationsByUserAndDate(const string& userID);
    bool cancelReservation(const string& reservationID);
    string getCurrentDate();

    // Getters
    string getReservationID() const;
    string getUserID() const;
    string getTotalTimeReserved() const;
    string getReservationDate() const;
    string getReservedSlotName() const;
    string getReservedTimeSlots() const;

    // Setters
    void setReservationID(const string& ReservationID);
    void setUserID(const string& UserID);
    void setTotalTimeReserved(const string& TotalTimeReserved);
    void setReservationDate(const string& ReservationDate);
    void setReservedSlotName(const string& ReservedSlotName);
    void setReservedTimeSlots(const string& ReservedTimeSlots);
};

#endif // RESERVATION_H
