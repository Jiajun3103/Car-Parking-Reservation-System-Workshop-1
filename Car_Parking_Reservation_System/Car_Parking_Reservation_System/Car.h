#pragma once
#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

class Car {
private:
    string CarID;
    string UserID;
    string CarModel;
    string CarLicensePlate;
    string CarDate;

public:
    Car();// default constructor
    Car(string CarID, string UserID, string CarModel, string CarLicensePlate, string CarDate);

    string generateNewCarID();
    bool insert();
    bool isUserIDValid(const string& userID);


    // getters 
    string getCarID();
    string getUserID();
    string getCarModel();
    string getCarLicensePlate();
    string getCarDate();


    // setters
    void setCarID(string CarID);
    void setUserID(string UserID);
    void setCarModel(string CarModel);
    void setCarLicensePlate(string CarLincensePlate);
    void setCarDate(string CarDate);
};

#endif // CAR_H
