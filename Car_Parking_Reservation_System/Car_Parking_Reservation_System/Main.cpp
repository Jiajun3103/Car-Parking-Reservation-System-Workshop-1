// Database Header
#include "DBConnection.h"// Custom class for handling database operations

// Standard Input/Output
#include <iostream>    // For input and output operations (cin, cout)
#include <string>      // For string manipulation
#include <iomanip>     // For formatting output (setw, setprecision)
#include <cstdio>      // For standard input/output functions like sprintf
#include <algorithm>   // for std::find
#include <vector>      // For vector data structure
#include <map>         // For map data structure
#include <ostream>

// User Input and Validation
#include <cctype>      // For character handling (isdigit, isalpha)
#include <conio.h>     // For _getch() to capture keypress without pressing Enter (Windows-specific)
#include <regex>       // For input validation using regular expressions
#include <limits>      // for numeric_limits
// File and Error Handling
#include <fstream>     // For file input and output operations
#include <stdexcept>   // For exception handling (e.g., runtime_error)

// Database and Time Management
#include <stdlib.h>    // For standard library functions (like system calls)
#include <ctime>       // For time-related operations (e.g., current date and time)
#include <chrono>      // For precise time measurement and delays
#include <thread>      // For creating delays using std::this_thread::sleep_for
#include <sstream>     // For converting data to strings (e.g., formatting UserID)

// Local Header
#include "User.h"
#include "Parkingslot.h"
#include "Fine.h"
#include "Reservation.h"
#include "Payment.h"
#include "Car.h"

// ANSI escape codes for color
#define RESET "\033[0m"
#define BOLD "\033[1m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define CYAN "\033[36m"
#define YELLOW "\033[33m"

using namespace std;

void loginModule(User& user);//login module
bool validPhoneNumber(const string phoneNumber);// Validation for phone number
string getCurrentDate();//get current date

// User
// Main Interface Customer
char mainInterfaceUser(); 
// Make Reservation
void makeReservation(User& user, ParkingSlot& parkingSlot, Payment& payment, Car& car, Reservation& reservation); // Make Reservation
void displayParkingSlotMenu();
void handleTimeSlotSelection(ParkingSlot& parkingSlot, vector<int>& selectedTimeSlots, string slotName);
void cancelTimeSlot(vector<int>& selectedTimeSlots);
bool enterCarDetails(Car& car, User& user);
void handlePayment(Payment& payment, Reservation& reservation, float totalCost, string reservationID);
string getTimeSlotString(int timeSlotIndex);
string getPaymentType();

// Read Reservation
void showReservation(User& user, Reservation& reservation);

// Delete Reservation
void cancelReservation(User& user, Reservation& reservation, Payment& payment);

// Pay Fine
void payFine(User& user, Fine& fine);

// Edit Profile
void editProfile(User& user);
bool confirmUserIdentity(User& user);

// Admin
// Main Interface Admin
char mainInterfaceAdmin();

// Parking Slot Manangement
void manageSlot(User& admin, ParkingSlot& parkingSlot, Reservation& reservation);

// Set Fine 
void setFine(User& admin, Fine& fine, Reservation& reservation, Car& car);

// Analytics And Statistics
void analyticsStatistics();
void incomeMonthlySlot();
void monthlyFine();

// Annual Report
void slotReport(Payment& payment);
void saveAnnualSlotReportToFile(Payment& payment, int year);

// Manage Admin
void manageAdmin(User& user);
void editAdminProfile(User& user);
bool confirmAdminIdentity(User& user);
void addAdmin(User& user);
void deleteAdmin(User& user);

// Main Function
int main()
{
    DBConnection db;
    User user;

    // Menu selection option
    char menu = '~';
    ParkingSlot parkingSlot;
    Payment payment;
    Car car;
    Reservation reservation;
    Fine fine;
    while (1)
    {
        // User Login & Customer Registration
        loginModule(user);

        // For Customer
        if (user.fetchUserStatus() == "User")
        {
            while (1)
            {
                // Main menu
                menu = mainInterfaceUser();

                // Make reservation
                if (menu == '1')
                {
                    makeReservation(user, parkingSlot, payment, car, reservation);
                }
                // Show Reservation 
                else if (menu == '2')
                {
                    showReservation(user, reservation);
                }
                // Cancel Reservation
                else if (menu == '3')
                {
                    cancelReservation(user, reservation, payment);
                }
                // Pay fine
                else if (menu == '4')
                {
                    payFine(user, fine);
                }
                // Edit Profile
                else if (menu == '5')
                {
                    editProfile(user);
                }
                // Log-Out
                else if (menu == '6')
                {
                    break;
                }

            }

        }
        // For Admin
        else if (user.fetchUserStatus() == "Admin")
        {
            while (1)
            {
                // Main menu
                menu = mainInterfaceAdmin();

                // Manage the parking slot status
                if (menu == '1')
                {
                    manageSlot(user, parkingSlot, reservation);
                }
                // Set fine to user 
                else if (menu == '2')
                {
                    setFine(user, fine, reservation, car);
                }
                // Statistics & Analytics
                else if (menu == '3')
                {
                    analyticsStatistics();
                }
                // Annual Report
                else if(menu == '4')
                {
                    slotReport(payment);
                }
                else if (menu == '5')
                {
                    manageAdmin(user);
                }
                // Log-Out
                else if (menu == '6')
                {
                    break;
                }

            }

        }
    }

}

// Login Module & User Registration
void loginModule(User& user)
{
    char option = '~';   // User Input Option

    while (1)
    {
        system("cls");

        cout << BOLD << CYAN;
        cout << setw(40) << "\n\t\t\tDing Market" << RESET << endl;
        cout << YELLOW << "\t===============================================" << RESET << endl;
        cout << BOLD << "\t\tCar Parking Reservation System" << RESET << endl;
        cout << YELLOW << "\t===============================================" << RESET << endl;
        cout << BOLD << "\t[1] Login" << RESET << endl;
        cout << BOLD << "\t[2] Register" << RESET << endl;
        cout << BOLD << "\t[C] Exit" << RESET << endl;
        cout << YELLOW << "\t===============================================" << RESET << endl;

        switch (option)
        {
        case '~':
            cout << BOLD << "Select an option: " << RESET;
            option = tolower(_getch());
            break;

        case'1':
            break;

        case'2':
            break;

        case'c':
            break;

        default:
            cout << RED << "Invalid option. Please select again." << RESET << endl;
            option = tolower(_getch());
            break;

        }

        if (option == '1')
        {
            int FoundUser = 0;   // For Login

            option = '~';
            string username = "", password = "";

            while (1)
            {
                system("cls");

                cout << CYAN;
                cout << setw(40) << left << "\t=================================================" << RESET << endl;
                cout << BOLD << setw(32) << "\t\t          User Login          " << RESET << endl;
                cout << CYAN << setw(40) << "\t=================================================" << RESET << endl;
                cout << BOLD << setw(20) << "\t[1] Username   : " << RESET << YELLOW << username << RESET << endl;
                cout << BOLD << setw(20) << "\t[2] Password   : " << RESET << YELLOW << string(password.length(), '*') << RESET << endl << endl;
                cout << YELLOW << BOLD << setw(30) << "\t[C] Login" << endl;
                cout << setw(30) << "\t[R] Return" << RESET << endl << endl;
                cout << CYAN;
                cout << setw(40) << "\t=================================================" << RESET << endl;

                switch (option)
                {
                case '~':
                    cout << BOLD << setw(5) << "Select an option: " << RESET;
                    option = tolower(_getch());
                    break;

                case '1':
                    cout << YELLOW << "UserName: " << RESET;
                    getline(cin, username);
                    user.insertUserName(username);
                    option = '~';
                    break;

                case '2':
                    password.clear();
                    cout << YELLOW << "Password: " << RESET;
                    char ch;
                    while ((ch = _getch()) != '\r') { // Enter key is pressed
                        if (ch == '\b') { // Backspace is pressed
                            if (!password.empty()) {
                                password.pop_back();
                                cout << "\b \b"; // Erase character from console
                            }
                        }
                        else {
                            password.push_back(ch);
                            cout << '*';
                        }
                    }
                    cout << endl;
                    user.insertUserPassword(password);
                    option = '~';
                    break;

                case 'c':
                    if (user.login() == false)
                    {
                        cout << RED << setw(5) << "Invalid Information. Press a key to continue." << RESET;
                        _getch();
                        option = '~';
                        break;
                    }
                    else
                    {
                        FoundUser = 1;
                        break;
                    }

                case 'r':
                    break;

                default:
                    cout << RED << setw(5) << "Invalid option.Please select again. " << RESET;
                    option = tolower(_getch());
                    break;

                }

                if (FoundUser == 1 || option == 'r')
                {
                    if (option == 'r')
                    {
                        username = "";
                        password = "";
                        user.insertUserName(username);
                        user.insertUserPassword(password);
                    }

                    option = '~';
                    break;
                }

            }

            if (user.login() == true)   // Successfull Login
            {
                break;
            }

        }
        else if (option == '2')
        {
            int NewUser = 0;   // For Register

            option = '~';
            string details[5]; // Customer Information

            while (1)
            {
                system("cls");

                cout << CYAN;
                cout << setw(50) << left << "\t======================================================================================================" << RESET << endl;
                cout << BOLD << setw(42) << "\t\t\t\t\t        User Registration        " << RESET << endl;
                cout << CYAN << setw(50) << "\t======================================================================================================" << RESET << endl;
                cout << BOLD << setw(40) << "\t[1] Username (min.8 char & max.50 char)               : " << RESET << YELLOW << left << details[0] << RESET << endl;
                cout << BOLD << setw(40) << "\t[2] Phone No. (01xxxxxxxx)                            : " << RESET << YELLOW << left << details[1] << RESET << endl;
                cout << BOLD << setw(40) << "\t[3] Gender ('M' or 'F')                               : " << RESET << YELLOW << left << details[2] << RESET << endl;
                cout << BOLD << setw(40) << "\t[4] Password (min. 8 char, max. 20 char, No-spacing)  : " << RESET << YELLOW << string(details[3].length(), '*') << RESET << endl << endl;
                cout << YELLOW << BOLD << setw(35) << "\t[C] Register" << endl;
                cout << setw(35) << "\t[R] Return  " << RESET << endl << endl;
                cout << CYAN;
                cout << setw(50) << "\t======================================================================================================" << RESET << endl;

                switch (option)
                {
                case '~':
                    cout << BOLD << setw(5) << "Select an option: " << RESET;
                    option = tolower(_getch());
                    break;

                case '1':
                    cout << YELLOW << "Name (min.8 char & max.50 char): " << RESET;
                    getline(cin, details[0]);
                    while (details[0].length() < 8 || details[0].length() > 50)
                    {
                        cout << RED << "Error: Enter name again (min.8 char & max.50 char): " << RESET;
                        getline(cin, details[0]);
                    }
                    option = '~';
                    break;

                case '2':
                    cout << YELLOW << "Phone No. (01xxxxxxxx): " << RESET;
                    getline(cin, details[1]);
                    while (validPhoneNumber(details[1]) == false)
                    {
                        cout << RED << "Error: Enter phone no. again (01xxxxxxxx): " << RESET;
                        getline(cin, details[1]);
                    }
                    option = '~';
                    break;

                case '3':
                    cout << YELLOW << "Gender ('M' or 'F'): " << RESET;
                    getline(cin, details[2]);
                    while ((details[2][0] != 'm' && details[2][0] != 'f' && details[2][0] != 'M' && details[2][0] != 'F') || details[2].length() != 1)
                    {
                        cout << RED << "Error: Please enter gender again ('M' or 'F'): " << RESET;
                        getline(cin, details[2]);
                    }

                    if (details[2][0] == 'm')
                    {
                        details[2][0] = 'M';
                    }
                    else if (details[2][0] == 'f')
                    {
                        details[2][0] = 'F';
                    }

                    option = '~';
                    break;

                case '4':
                    details[3].clear();
                    cout << YELLOW << "Password (min.8 char, max.20 char & No-spacing): " << RESET;
                    char ch;

                    // Capture user input for the password
                    while ((ch = _getch()) != '\r') { // Enter key is pressed
                        if (ch == '\b') { // Backspace is pressed
                            if (!details[3].empty()) {
                                details[3].pop_back();
                                cout << "\b \b"; // Erase character from console
                            }
                        }
                        else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                            details[3].push_back(ch);
                            cout << '*';
                        }
                    }
                    cout << endl;

                    // Validate password length and spacing
                    while (details[3].length() < 8 || details[3].length() > 20 || details[3].find(' ') != string::npos) {
                        details[3].clear(); // Clear the password if it does not meet criteria
                        cout << RED << "Error: Password must be between 8 and 20 characters and contain no spaces. Enter password again: " << RESET;

                        // Capture user input for the password again
                        while ((ch = _getch()) != '\r') { // Enter key is pressed
                            if (ch == '\b') { // Backspace is pressed
                                if (!details[3].empty()) {
                                    details[3].pop_back();
                                    cout << "\b \b"; // Erase character from console
                                }
                            }
                            else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                                details[3].push_back(ch);
                                cout << '*';
                            }
                        }
                        cout << endl;
                    }

                    option = '~';
                    break;

                case 'c':
                {
                    if (details[0].empty() || details[1].empty() || details[2].empty() || details[3].empty())
                    {
                        cout << RED << setw(60) << "Invalid Information. Press any key to continue." << RESET;
                        _getch();
                        option = '~';
                    }
                    else
                    {
                        // Generate a new unique UserID
                        User tempUser;
                        string newUserID = tempUser.generateNewUserID();
                        details[4] = newUserID;

                        User newcustomer(details[4], details[3], details[0], details[1], details[2], "User");
                        if (newcustomer.newUser() == false)
                        {
                            cout << setw(105) << "UserID already exists. Press a key to continue.";
                            _getch();
                            option = '~';
                        }
                        else
                        {
                            cout << setw(127) << "Are you sure?  (Press 'Y' to confirm, 'N' to cancel).";
                            option = _getch();

                            while (option != 'y' && option != 'n')
                            {
                                cout << endl;
                                cout << setw(118) << BOLD << "Error input. Please select again (Press 'Y' to confirm, 'N' to cancel)." << RESET;
                                option = tolower(_getch());
                            }

                            if (option == 'n')
                            {

                                option = '~';

                            }
                            else if (option == 'y')
                            {
                                newcustomer.insert();
                                cout << GREEN << setw(80) << "\nRegistration successful! Press any key to continue." << RESET;
                                _getch();
                                NewUser = 1;
                                option = '~';
                            }

                        }
                    }

                    break;

                }

                case 'r':
                    break;

                default:
                    cout << RED << setw(5) << "Error input. Please select again." << RESET;
                    option = tolower(_getch());
                    break;

                }

                if (option == 'r' || NewUser == 1)
                {
                    option = '~';
                    break;
                }

            }

        }
        else if (option == 'c')
        {
            // Close System
            cout << endl;
            exit(0);
        }


    }

}

// Validation for phone number
bool validPhoneNumber(const string phoneNumber)
{
    // Check length
    if (phoneNumber.length() < 10 || phoneNumber.length() > 12)
    {
        return false;
    }

    // Check first two digits are '01'
    if (phoneNumber.substr(0, 2) != "01")
    {
        return false;
    }

    // Check all characters are digits after the first two
    for (size_t i = 2; i < phoneNumber.length(); ++i)
    {
        if (!isdigit(phoneNumber[i]))
        {
            return false;
        }
    }

    return true;

}

// User
// Main Interface User
char mainInterfaceUser()
{
    char option = '~';   // User Input Option

    do
    {
        system("cls");
        cout << BOLD << CYAN << setw(40) << "\n\tCar Parking Reservation System" << RESET << endl;
        cout << YELLOW << "\t------------------------------------" << RESET << endl;
        cout << BOLD << "\t[1] Make Reservation " << RESET << endl;
        cout << BOLD << "\t[2] Show Reservation " << RESET << endl;
        cout << BOLD << "\t[3] Cancel Reservation " << RESET << endl;
        cout << BOLD << "\t[4] Fine " << RESET << endl;
        cout << BOLD << "\t[5] Edit Profile " << RESET << endl;
        cout << BOLD << "\t[6] Log-Out       " << RESET << endl;
        cout << YELLOW << "\t------------------------------------\n" << RESET << endl;

        switch (option)
        {
        case '~':
            cout << BOLD << setw(5) << "Select an option: " << RESET;
            option = tolower(_getch());
            break;

        case '1':
            break;

        case '2':
            break;

        case '3':
            break;

        case '4':
            break;

        case '5':
            break;

        case '6':
            break;

        default:
            cout << RED << "Error: Invalid option. Please select again." << RESET;
            option = tolower(_getch());
            break;
        }

    } while (option != '1' && option != '2' && option != '3' && option != '4' && option != '5' && option != '6');

    return option;
}

// Reservation
void makeReservation(User& user, ParkingSlot& parkingSlot, Payment& payment, Car& car, Reservation& reservation)
{
    char option = '~';   // User Input Option for Parking Slot Menu
    vector<int> selectedTimeSlots; // Store multiple time slots
    string reservedTimeSlots;
    string carModel, licensePlate; // Car details for payment

    while (true)
    {
        system("cls");

        cout << BOLD << CYAN << setw(40) << "\n\tCar Parking Reservation System" << RESET << endl;
        cout << YELLOW << "\t------------------------------" << RESET << endl;
        cout << BOLD << "\tChoose a parking slot:" << RESET << endl;
        cout << YELLOW << "\t------------------------------" << RESET << endl;

        for (int i = 1; i <= 9; i++)
        {
            cout << BOLD << CYAN << "\t[" << i << "] A00" << i << RESET << endl;
        }
        cout << BOLD << CYAN << "\t[R] Return to Main Menu" << RESET << endl << endl;
        cout << YELLOW << "\t------------------------------" << RESET << endl;

        cout << BOLD << "\tSelect an option: " << RESET;
        option = _getch();

        if (option >= '1' && option <= '9')
        {
            // Construct the slot name (e.g., A001, A002, etc.)
            string slotName = "A00" + to_string(option - '0');
            selectedTimeSlots.clear();  

            while (true)
            {
                system("cls");
                reservedTimeSlots = "";
                // Show parking slot details
                cout << BOLD << CYAN << "\n\tTime Slot Menu" << RESET << endl << endl;
                parkingSlot.showSlotInfo(slotName);

                if (!selectedTimeSlots.empty())
                {
                    // Loop through each selected time slot and append it to the string
                    for (size_t i = 0; i < selectedTimeSlots.size(); ++i)
                    {
                        // Append the time slot to the string
                        reservedTimeSlots += getTimeSlotString(selectedTimeSlots[i]);

                        // Add a comma after each slot except for the last one
                        if (i < selectedTimeSlots.size() - 1)
                        {
                            reservedTimeSlots += ", ";
                        }
                    }

                    // Display the neatly formatted concatenated time slots
                    cout << "\n\tSelected Time Slots: " << reservedTimeSlots;
                }
                else
                {
                    reservedTimeSlots = "None"; // If no slots are selected
                    cout << "\n\tSelected Time Slots: " << reservedTimeSlots;
                }

                // Display menu options
                cout << "\n\n\tOptions:" << endl;
                cout << "\t[A-L] Select a time slot" << endl;
                cout << "\t[Z] Cancel a selected time slot" << endl;
                cout << "\t[0] Finish selection" << endl;
                cout << "\t[R] Return to Select Parking Slot" << endl;

                cout << "\n\tEnter your choice: ";
                char choice = toupper(_getch());

                if (choice >= 'A' && choice <= 'L')
                {
                    // Convert letter A-L to corresponding time slot index (1-12)
                    int timeSlotIndex = choice - 'A' + 1;

                    if (parkingSlot.isAvailable(slotName, timeSlotIndex))
                    {
                        if (find(selectedTimeSlots.begin(), selectedTimeSlots.end(), timeSlotIndex) == selectedTimeSlots.end())
                        {
                            selectedTimeSlots.push_back(timeSlotIndex);
                            cout << "\n\tTime slot " << choice << " selected.";
                        }
                        else
                        {
                            cout << "\n\tTime slot " << choice << " is already selected.";
                        }
                    }
                    else
                    {
                        cout << "\n\tTime slot " << choice << " is not available.";
                    }
                    cout << "\n\tPress any key to continue...\n";
                    _getch();

                }
                else if (choice == 'Z')
                {
                    // Cancel a selected slot
                    if (!selectedTimeSlots.empty())
                    {
                        cout << "\n\tEnter the time slot to cancel (A-L): ";
                        char cancelSlot = toupper(_getch());
                        if (cancelSlot >= 'A' && cancelSlot <= 'L')
                        {
                            int timeSlotIndex = cancelSlot - 'A' + 1;
                            auto it = find(selectedTimeSlots.begin(), selectedTimeSlots.end(), timeSlotIndex);

                            if (it != selectedTimeSlots.end())
                            {
                                selectedTimeSlots.erase(it);
                                cout << "\n\tTime slot " << cancelSlot << " cancelled.";
                            }
                            else
                            {
                                cout << "\n\tTime slot " << cancelSlot << " was not selected.";
                            }
                        }
                        else
                        {
                            cout << "\n\tInvalid input.";
                        }
                    }
                    else
                    {
                        cout << "\n\tNo slots to cancel.";
                    }
                    cout << "\n\tPress any key to continue...\n";
                    _getch();

                }
                else if (choice == '0')
                {
                    if (!selectedTimeSlots.empty())
                    {
                        reservation.setReservedSlotName(slotName);
                        reservation.setReservedTimeSlots(reservedTimeSlots);
                    }
                    else
                    {
                        cout << "\n\tNo slots selected.";
                        cout << "\n\tPress any key to continue...\n";
                        _getch();
                    }

                }
                else if (choice == 'R')
                {
                    // Return to parking slot selection
                    selectedTimeSlots.clear(); // Empty the selected time slots
                    break; // Exit the loop
                }
                else
                {
                    cout << "\n\tInvalid choice.";
                    cout << "\n\tPress any key to continue...\n";
                    _getch();
                }


                if (!selectedTimeSlots.empty() && choice == '0')
                {
                    // Call enterCarDetails to collect car details
                    string carModel = "";
                    string licensePlate = "";
                    bool isConfirmed = false;

                    while (true)
                    {
                        system("cls");
                        cout << BOLD << CYAN << "\n\tCar Parking Reservation System\n" << RESET << endl;
                        cout << YELLOW << "\t------------------------------" << RESET << endl;
                        cout << BOLD << "\tCar Details Menu\n" << RESET;
                        cout << YELLOW << "\t------------------------------\n" << RESET;

                        // Display current car details
                        cout << "\t[1] Car Model: " << (carModel.empty() ? "Not Entered" : carModel) << endl;
                        cout << "\t[2] License Plate: " << (licensePlate.empty() ? "Not Entered" : licensePlate) << endl;
                        cout << "\t[C] Confirm" << endl;
                        cout << "\t[R] Return to Main Menu\n";

                        cout << "\n\tSelect an option:\n ";
                        char choice = toupper(_getch());

                        if (choice == '1')
                        {
                            bool valid = false;
                            while (!valid)
                            {
                                cout << "\tEnter Car Model (minimum 3 letters): ";
                                getline(cin, carModel);

                                // Check if the car model has at least 3 letters
                                if (carModel.length() >= 3 && all_of(carModel.begin(), carModel.end(), [](char c) { return isalnum(c) || c == ' '; }))
                                {
                                    valid = true; // Car model is valid
                                }
                                else
                                {
                                    cout << "\n\tInvalid car model! It must have at least 3 characters.";
                                    cout << "\n\tPlease try again.\n\n";
                                }
                            }
                        }
                        else if (choice == '2')
                        {
                            bool valid = false;
                            while (!valid)
                            {
                                cout << "\tEnter License Plate (3 letters + 1 to 4 digits): ";
                                getline(cin, licensePlate);

                                // Regular expression for 3 alphabets followed by 1 to 4 digits
                                regex licensePlatePattern("^[A-Z]{3}[0-9]{1,4}$", regex_constants::icase);

                                if (regex_match(licensePlate, licensePlatePattern))
                                {
                                    transform(licensePlate.begin(), licensePlate.begin() + 3, licensePlate.begin(), ::toupper);
                                    valid = true; 
                                }
                                else
                                {
                                    cout << "\n\tInvalid license plate format! It must be 3 letters followed by 1 to 4 digits.";
                                    cout << "\n\tPlease try again.\n\n";
                                }
                            }
                        }
                        else if (choice == 'C')
                        {
                            // Confirm details and set data into the car object
                            if (!carModel.empty() && !licensePlate.empty())
                            {
                                car.setCarModel(carModel);
                                car.setCarLicensePlate(licensePlate);
                                car.setUserID(user.fetchUserID());

                                isConfirmed = true;
                                cout << "\n\tCar details confirmed successfully!\n";
                                cout << "\tPress any key to continue...\n";
                                _getch();
                            }
                            else
                            {
                                cout << "\n\tBoth Car Model and License Plate must be filled before confirming.";
                                cout << "\n\tPress any key to continue...\n";
                                _getch();
                            }

                        }
                        else if (choice == 'R')
                        {
                            break; 
                        }
                        else
                        {
                            cout << "\n\tInvalid option. Please try again.";
                            cout << "\n\tPress any key to continue...\n";
                            _getch();
                        }


                        if (!carModel.empty() && !licensePlate.empty() && choice == 'C')
                        {
                            // Generate new CarID
                            string newCarID = car.generateNewCarID();
                            car.setCarID(newCarID);
                            car.setCarDate(getCurrentDate());

                            int totalHours = selectedTimeSlots.size();
                            float totalCost = totalHours * 1.0;

                            string reservationID = reservation.generateNewReservationID();
                            reservation.setReservationID(reservationID);
                            reservation.setUserID(user.fetchUserID());
                            reservation.setTotalTimeReserved(to_string(totalHours));
                            // Set the reservation date using the current date function
                            string currentDate = getCurrentDate(); // Set the reservation date
                            reservation.setReservationDate(currentDate);

                            while (1)
                            {
                                // Get valid payment type
                                string paymentType = getPaymentType();

                                // Insert the reservation into the database
                                if (reservation.insert())
                                {

                                    // Proceed with payment
                                    string newPaymentID = payment.generateNewPaymentID(); 
                                    payment.setPaymentID(newPaymentID); 
                                    payment.setPaymentAmount(totalCost);
                                    payment.setPaymentStatus("Pending");
                                    payment.setReservationID(reservationID); 
                                    payment.setPaymentDate(currentDate);

                                    // If paymentType is empty, handle the return
                                    if (paymentType.empty())
                                    {
                                        cout << "\tReturning to the previous step...\n";
                                        break;  
                                    }

                                    payment.setPaymentType(paymentType); 

                                    // Insert payment into the database
                                    if (payment.insert())
                                    {
                                        cout << "\n\tPayment details recorded successfully. Completing transaction...\n";
                                        payment.setPaymentStatus("Completed");

                                        if (payment.update())
                                        {
                                            cout << "\tPayment completed successfully. Thank you for using our service!\n";

                                            // After payment, update all selected slots as "Booked"
                                            for (int timeSlot : selectedTimeSlots)
                                            {
                                                string slotTime = getTimeSlotString(timeSlot);  // Convert index to time string
                                                if (parkingSlot.updateSlotStatus(slotName, slotTime, "Booked"))
                                                {
                                                    //cout << "Slot " << slotName << " at " << slotTime << " marked as 'Booked'.\n";
                                                }
                                                else
                                                {
                                                    cout << "\tFailed to update slot " << slotName << " at " << slotTime << " status.\n";
                                                }
                                            }

                                            // Insert car details into the database
                                            if (car.insert())
                                            {
                                                Car(newCarID, user.fetchUserID(), carModel, licensePlate,getCurrentDate());
                                                cout << "\tPress any key to continue...\n";
                                                _getch();
                                                return;
                                            }
                                            else
                                            {
                                                cout << "\tFailed to insert car details into the database. Please try again.\n";
                                            }
                                        }
                                        else
                                        {
                                            cout << "\tFailed to update payment status. Please contact support.\n";
                                        }
                                    }
                                    else
                                    {
                                        cout << "\tFailed to record payment details. Please try again.\n";
                                    }
                                }
                                else
                                {
                                    cout << "\tFailed to insert reservation details into the database. Aborting...\n";
                                }
                            }
                        }
                    }
                }
            }

        }
        else if (option == 'r' || option == 'R')
        {
            break; // Exit Parking Slot Menu and return to Main Menu
        }
        else
        {
            cout << "Invalid selection. Please try again.\n";
            cout << "Press any key to continue...\n";
            _getch();
        }
    }
}

// Time Slot
string getTimeSlotString(int timeSlotIndex)
{
    switch (timeSlotIndex)
    {
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
    default: return ""; // Error case, should not happen
    }
}

// Select Payment Type
string getPaymentType()
{
    int paymentTypeChoice;
    string paymentType;

    while (true)
    {
        system("cls");
        cout << BOLD << CYAN << setw(40) << "\n\tSelect Payment Type" << RESET << endl;
        cout << YELLOW << "\t----------------------------" << RESET << endl;
        cout << BOLD << "\t[1] Credit Card" << RESET << endl;
        cout << BOLD << "\t[2] E-Wallet" << RESET << endl;
        cout << BOLD << "\t[3] Bank Transfer" << RESET << endl;
        cout << BOLD << "\t[R] Return" << RESET << endl;
        cout << YELLOW << "\t----------------------------" << RESET << endl;

        cout << BOLD << "\n\tEnter your choice (1-3) or press 'r' to return: " << RESET;

        // Wait for user input
        char userInput = tolower(_getch());  // Capture a single character input

        // If the user input is a number, process the selection
        if (userInput >= '1' && userInput <= '3')
        {
            paymentTypeChoice = userInput - '0'; // Convert char to integer

            switch (paymentTypeChoice)
            {
            case 1:
                paymentType = "Credit Card";
                return paymentType;
            case 2:
                paymentType = "E-Wallet";
                return paymentType;
            case 3:
                paymentType = "Bank Transfer";
                return paymentType;
            }
        }
        else if (userInput == 'r')
        {
            // Return if user presses 'r' to go back
            return "";
        }
        else
        {
            cout << RED << "\n\tInvalid choice. Please select a valid option.\n" << RESET;
            _getch();  
        }
    }
}

//Show Reservation
void showReservation(User& user, Reservation& reservation) {

    system("cls");
    cout << "\n\n";
    cout << YELLOW << "\t==========================================================================================================================\n" << RESET;
    cout << BOLD << "\t\t\t\t\t\t         RESERVATION DETAILS              \n" << RESET;
    cout << YELLOW <<"\t==========================================================================================================================\n"<< RESET;

    // Display user information
    cout << BOLD;
    cout << "\tUser Information: " << endl;
    //cout << "\tUser ID:          " << user.fetchUserID() << endl;
    cout << "\tName:             " << user.fetchUserName() << endl;
    cout << "\tPhone Number:     " << user.fetchUserPhoneNo() << endl;
    cout << "\tGender:           " << user.fetchUserGender() << endl;
    cout << "\tStatus:           " << user.fetchUserStatus() << endl;
    cout << RESET;

    reservation.showReservationsByUser(user.fetchUserID());

    cout << YELLOW << "\t==========================================================================================================================\n" << RESET;
    cout << BOLD << "\t\t\t\t\t\t      End of Reservation Details\n" << RESET;
    cout << YELLOW << "\t==========================================================================================================================\n" << RESET;


    cout << BOLD << "\n\nPress any key to return...\n" << RESET;
    _getch();
}

//Cancel Reservation
void cancelReservation(User& user, Reservation& reservation, Payment& payment) {
    string reservationID;

    while (true) {
        system("cls");
        cout << BOLD << CYAN << "\t\t\t\t\t\t\tCancel Reservation" << RESET << endl;
        cout << YELLOW << "\t------------------------------------------------------------------------------------------------------------------------" << RESET << endl;
        reservation.showReservationsByUserAndDate(user.fetchUserID());
        cout << YELLOW << "\t------------------------------------------------------------------------------------------------------------------------" << RESET << endl;
        cout << BOLD << "\t[1] Receipt No: " << (reservationID.empty() ? "Not entered" : reservationID) << RESET << endl << endl;
        cout << CYAN << BOLD << "\t[C] Confirm" << RESET << "  " << CYAN << BOLD 
            << "\n\t[R] Return" << RESET << endl;
        cout << YELLOW << "\t------------------------------------------------------------------------------------------------------------------------" << RESET << endl;
        cout << BOLD << "Select an option: " << RESET;

        char choice = _getch();  
        choice = toupper(choice); 

        if (choice == '1') {
            cout << BOLD << CYAN << "\nEnter Receipt No: " << RESET;
            cin >> reservationID;
        }
        else if (choice == 'C') {
            if (reservationID.empty()) {
                cout << BOLD << RED << "\nPlease enter a Receipt No before confirming." << RESET << endl;
                system("pause");
                continue;
            }

            DBConnection db;
            try {
                db.prepareStatement("SELECT r.UserID, r.ReservationDate, p.PaymentID FROM reservations r "
                    "LEFT JOIN payment p ON r.ReservationID = p.ReservationID "
                    "WHERE r.ReservationID = ?;");
                db.setString(1, reservationID);
                db.QueryResult();

                if (db.res && db.res->next()) {
                    string reservationUserID = db.res->getString("UserID");
                    string reservationDate = db.res->getString("ReservationDate");
                    string currentUserID = user.fetchUserID();

                    if (reservationUserID != currentUserID) {
                        cout << BOLD << RED << "\nYou are not authorized to cancel this reservation." << RESET << endl;
                        system("pause");
                        continue;
                    }

                    string currentDate = getCurrentDate();
                    if (reservationDate < currentDate) {
                        cout << BOLD << RED << "\nCannot cancel a reservation that has already passed." << RESET << endl;
                        system("pause");
                        continue;
                    }

                    payment.cancelPaymentByReservationID(reservationID);
                    if (reservation.cancelReservation(reservationID)) {
                        cout << BOLD << CYAN << "\nReservation has been successfully canceled." << RESET << endl;
                    }
                    else {
                        cout << BOLD << RED << "\nFailed to cancel the reservation. Check the ID and try again." << RESET << endl;
                    }
                }
                else {
                    cout << BOLD << RED << "\nNo receipt No found with " << reservationID << "." << RESET << endl;
                }
            }
            catch (const sql::SQLException& e) {
                cerr << BOLD << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << RESET << endl;
            }
            catch (const std::exception& e) {
                cerr << BOLD << "Error: " << e.what() << RESET << endl;
            }
            system("pause");
        }
        else if (choice == 'R') {
            cout << BOLD << CYAN << "\nReturning to the main menu..." << RESET << endl;
            break;
        }
        else {
            cout << BOLD << RED << "\nInvalid selection. Please try again." << RESET << endl;
            system("pause");
        }
    }
}

// Pay Fine
void payFine(User& user, Fine& fine)
{
    // Get the total fine for the user
    double totalFine = fine.getTotalFine(user.fetchUserID());

    // Fetch the fine reason for the user from the database
    string fineReason = fine.displayFineReason(user.fetchUserID()); // Assuming getFineReason retrieves the reason


    // Check if the user has unpaid fines
    if (totalFine > 0)
    {
        system("cls");
        cout << BOLD << CYAN << "\n\t\tFine Payment" << RESET << endl;
        cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;

        // Display the fine details
        cout << BOLD << "\tTotal unpaid fine: RM" << fixed << setprecision(2) << totalFine << RESET << endl;
        cout << BOLD << "\tReason: " << fineReason << RESET << endl;

        // Prompt the user for payment
        cout << BOLD << "\n\tDo you want to pay the fine? (Y/N): " << RESET;
        char paymentChoice = tolower(_getch());

        if (paymentChoice == 'Y' || paymentChoice == 'y')
        {
            cout << BOLD << CYAN << setw(40) << "\n\tSelect Payment Type" << RESET << endl;
            cout << YELLOW << "\t----------------------------" << RESET << endl;
            cout << BOLD << "\t[1] Credit Card" << RESET << endl;
            cout << BOLD << "\t[2] E-Wallet" << RESET << endl;
            cout << BOLD << "\t[3] Bank Transfer" << RESET << endl;
            cout << BOLD << "\t[R] Return" << RESET << endl;
            cout << YELLOW << "\t----------------------------" << RESET << endl;

            cout << CYAN << "\tEnter your choice: " << RESET;
            char paymentMethodChoice;
            paymentMethodChoice = tolower(_getch());

            // Check if the user wants to return to the main menu by pressing 'R'
            if (paymentMethodChoice == 'R' || paymentMethodChoice == 'r') {
                cout << RED << "\n\tPayment canceled. Returning to menu.\n" << RESET;
                _getch();
                return;  
            }

            string paymentMethodStr;
            switch (paymentMethodChoice)
            {
            case '1':
                paymentMethodStr = "Credit Card";
                break;
            case '2':
                paymentMethodStr = "E-Wallet";
                break;
            case '3':
                paymentMethodStr = "Bank Transfer";
                break;
            default:
                cout << RED << "\n\tInvalid selection. Payment method will not be processed.\n" << RESET;
                return;
            }

            // Show selected payment method
            cout << GREEN << "\n\tYou have selected " << paymentMethodStr << " as your payment method.\n" << RESET;
            cout << CYAN << "\n\tProcessing payment...\n" << RESET;

            cout << GREEN << "\n\tPayment processed successfully through " << paymentMethodStr << ".\n" << RESET;

            // Update FineStatus in the database to mark the fines as paid
            try
            {
                DBConnection db;
                db.prepareStatement("UPDATE fines SET FineStatus = 'Yes' WHERE UserID = ? AND FineStatus = 'No';");
                db.setString(1, user.fetchUserID());
                db.QueryResult();  

                cout << GREEN << "\n\tFine payment successful!" << RESET << endl;
                _getch();  
            }
            catch (const sql::SQLException& e)
            {
                cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << endl;
            }
            catch (const exception& e)
            {
                cerr << "Error: " << e.what() << endl;
            }
        }
        else
        {
            cout << RED << "\n\tPayment canceled. Returning to menu.\n" << RESET;
            _getch();  
        }
    }
    else
    {
        cout << BOLD << "\n\n\tYou have no unpaid fines.\n" << RESET;
        _getch();  
    }
}

// Edit Profile
void editProfile(User& user)
{
    if (confirmUserIdentity(user) == true)
    {
        char option = '~';   

        int NewEdit = 0;   // Confirm Edited
        string details[4];

        details[0] = user.fetchUserName();
        details[1] = user.fetchUserPhoneNo();
        details[2] = user.fetchUserGender();
        details[3] = user.fetchUserPassword();

        while (1)
        {

            system("cls");

            cout << CYAN;
            cout << setw(50) << left << "\t------------------------------------------------------------------------" << RESET << endl;
            cout << BOLD << setw(42) << "\t\t\t        Customer Profile        " << RESET << endl;
            cout << CYAN << setw(50) << "\t------------------------------------------------------------------------" << RESET << endl;
            cout << BOLD << setw(40) << "\t[1] Username (min.8 char & max.50 char)               :" << RESET << YELLOW << left << details[0] << RESET << endl;
            cout << BOLD << setw(40) << "\t[2] Phone No. (01xxxxxxxx)                            :" << RESET << YELLOW << left << details[1] << RESET << endl;
            cout << BOLD << setw(40) << "\t[3] Gender ('M' or 'F')                               :" << RESET << YELLOW << left << details[2] << RESET << endl;
            cout << BOLD << setw(40) << "\t[4] Password (min. 8 char, max. 20 char, No-spacing)  :" << RESET << YELLOW << string(details[3].length(), '*') << RESET << endl << endl;
            cout << YELLOW << BOLD << setw(35) << "\t[C] Confirm" << endl;
            cout << setw(35) << "\t[R] Return  " << RESET << endl << endl;
            cout << CYAN;
            cout << setw(50) << "\t------------------------------------------------------------------------" << RESET << endl;


            switch (option)
            {
            case '~':
                cout << BOLD << "Select an option: " << RESET;
                option = _getch();
                break;

            case '1':
                cout << CYAN << "Name (min.8 char & max.50 char): " << RESET;
                getline(cin, details[0]);
                while (details[0].length() < 8 || details[0].length() > 50)
                {
                    cout << RED << "Error input. Enter name again (min.8 char & max.50 char): " << RESET;
                    getline(cin, details[0]);
                }
                option = '~';
                break;

            case '2':
                cout << CYAN << "Phone No. (01xxxxxxxx): " << RESET;
                getline(cin, details[1]);
                while (validPhoneNumber(details[1]) == false)
                {
                    cout << RED << "Error input. Enter phone no. again (01xxxxxxxx): " << RESET;
                    getline(cin, details[1]);
                }
                option = '~';
                break;

            case '3':
                cout << CYAN << "Gender ('M' or 'F'): " << RESET;
                getline(cin, details[2]);
                while (details[2][0] != 'm' && details[2][0] != 'f' || details[2].length() != 1)
                {
                    cout << RED << "Error input. Please enter gender again ('M' or 'F'): " << RESET;
                    getline(cin, details[2]);
                }

                if (details[2][0] == 'm')
                {
                    details[2][0] = 'M';
                }
                else if (details[2][0] == 'f')
                {
                    details[2][0] = 'F';
                }

                option = '~';
                break;

            case '4':
                details[3].clear();
                cout << CYAN << "Password (min. 8 char, max. 20 char, No-spacing): " << RESET;
                char ch;
                while ((ch = _getch()) != '\r') { // Enter key is pressed
                    if (ch == '\b') { // Backspace is pressed
                        if (!details[3].empty()) {
                            details[3].pop_back();
                            cout << "\b \b"; // Erase character from console
                        }
                    }
                    else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                        details[3].push_back(ch);
                        cout << '*';
                    }
                }
                cout << endl;

                // Check if the password meets the required conditions (min. 8, max. 20, no spaces)
                while (details[3].length() < 8 || details[3].length() > 20 || details[3].find(' ') != string::npos) {
                    cout << RED << "Error input. Enter password again (min. 8 char, max. 20 char, No-spacing): " << RESET;
                    details[3].clear();
                    while ((ch = _getch()) != '\r') { // Enter key is pressed
                        if (ch == '\b') { // Backspace is pressed
                            if (!details[3].empty()) {
                                details[3].pop_back();
                                cout << "\b \b"; // Erase character from console
                            }
                        }
                        else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                            details[3].push_back(ch);
                            cout << '*';
                        }
                    }
                    cout << endl;
                }
                option = '~';
                break;

            case 'c':
                if (details[0].empty() || details[1].empty() || details[2].empty() || details[3].empty())
                {
                    cout << RED << setw(105) << "Invalid Information. Press a key to continue." << RESET;
                    _getch();
                    option = '~';
                }
                else
                {
                    user.insertUserName(details[0]);
                    user.insertUserPhoneNo(details[1]);
                    user.insertUserGender(details[2]);
                    user.insertUserPassword(details[3]);

                    user.update();
                    cout << GREEN << setw(105) << "Profile updated successfully!" << RESET << endl;
                    NewEdit = 1;
                }

                break;

            case 'r':
                break;

            default:
                cout << RED << "Error input. Please select again." << RESET;
                option = _getch();
                break;
            }

            if (option == 'r' || NewEdit == 1)
            {
                option = '~';
                break;
            }

        }

    }
    else
    {
        cout << RED << "\nUser identity verification failed. Returning to menu." << RESET << endl;
        _getch();
    }

}

// Authenticate user identity
bool confirmUserIdentity(User& user)
{
    char option = '~';
    string password = "";

    int correctUser = 0;
    while (1)
    {
        system("cls");

        cout << CYAN;
        cout << setw(50) << left << "\t==================================================" << RESET << endl;
        cout << BOLD << setw(42) << "\t        Confirm User Identity        " << RESET << endl;
        cout << CYAN << setw(50) << "\t==================================================" << RESET << endl;
        cout << BOLD << setw(40) << "\t[1] Current Password :  " << RESET << YELLOW << left << string(password.length(), '*') << RESET << endl << endl;
        cout << YELLOW << BOLD << setw(35) << "\t[C] Confirm" << endl;
        cout << setw(35) << "\t[R] Return  " << RESET << endl << endl;
        cout << CYAN;
        cout << setw(50) << "\t==================================================" << RESET << endl;


        switch (option)
        {
        case '~':
            cout << BOLD << setw(5) << "Select an option: " << RESET;
            option = tolower(_getch());
            break;

        case '1':
            password.clear();
            cout << YELLOW << "Enter current user password to confirm identity: " << RESET;
            char ch;
            while ((ch = _getch()) != '\r') { // Enter key is pressed
                if (ch == '\b') { // Backspace is pressed
                    if (!password.empty()) {
                        password.pop_back();
                        cout << "\b \b"; // Erase character from console
                    }
                }
                else if (password.length() < 20 && ch != ' ') { // Restrict max length and no spaces
                    password.push_back(ch);
                    cout << '*';
                }
            }
            cout << endl;
            option = '~';
            break;

        case 'c':
            if (password == user.fetchUserPassword())
            {
                correctUser = 1;
                break;
            }
            else
            {
                cout << RED << "Invalid Password. Press a key to continue." << RESET;
                _getch();
                option = '~';
                break;
            }

        case 'r':
            break;

        default:
            cout << RED << "Error input. Please select again." << RESET;
            option = _getch();
            break;

        }

        if (option == 'r')
        {
            return false;
            break;
        }
        else if (correctUser == 1)
        {
            return true;
            break;
        }

    }
}

// Return current date
string getCurrentDate()
{
    auto now = chrono::system_clock::now();
    time_t in_time_t = chrono::system_clock::to_time_t(now);

    struct tm buf;
    localtime_s(&buf, &in_time_t);

    stringstream ss;
    ss << put_time(&buf, "%Y-%m-%d");
    return ss.str();
}

// Admin
// Main Interface Admin
char mainInterfaceAdmin()
{
    char option = '~';   

    do
    {
        system("cls");
        cout << BOLD << CYAN << setw(40) << "\n\tAdmin Interface" << RESET << endl;
        cout << YELLOW << "\t------------------------------------\n" << RESET << endl;
        cout << BOLD << "\t[1] Manage Parking Slot " << RESET << endl;
        cout << BOLD << "\t[2] Set Fine " << RESET << endl;
        cout << BOLD << "\t[3] Statistics & Analytics " << RESET << endl;
        cout << BOLD << "\t[4] Annual Report       " << RESET << endl;
        cout << BOLD << "\t[5] Manage Admin       " << RESET << endl;
        cout << BOLD << "\t[6] Log-Out       " << RESET << endl;
        cout << YELLOW << "\n\t------------------------------------\n" << RESET << endl;

        switch (option)
        {
        case '~':
            cout << BOLD << "Select an option." << RESET;
            option = tolower(_getch());
            break;

        case '1':
            break;

        case '2':
            break;

        case '3':
            break;

        case '4':
            break;

        case '5':
            break;

        case '6':
            cout << RED << "\nLogging out...\n" << RESET;
            system("pause");
            break;

        default:
            cout << RED << "Error option. Please select again." << RESET;
            option = tolower(_getch());
            break;
        }

    } while (option != '1' && option != '2' && option != '3' && option != '4' && option != '5' && option != '6');

    return option;
}

// Parking Slot Management
void manageSlot(User& admin, ParkingSlot& parkingSlot, Reservation& reservation) {
    DBConnection db;
    string choice = "0";

    while (choice != "3") {
        system("cls"); 

        try {
            cout << BOLD << CYAN << "\n\tParking Slot Management" << RESET << endl;
            cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;

            db.prepareStatement("SELECT * FROM parkingslot;");
            db.QueryResult();

            cout << BOLD << "\n\t+--------+----------+-------------+----------------+" << RESET << endl;
            cout << BOLD << "\t| SlotID | SlotName | SlotTime    | SlotStatus     |" << RESET << endl;
            cout << BOLD << "\t+--------+----------+-------------+----------------+" << RESET << endl;

            while (db.res && db.res->next()) {
                string id = db.res->getString("SlotID");
                string name = db.res->getString("SlotName");
                string time = db.res->getString("SlotTime");
                string status = db.res->getString("SlotStatus");

                cout << "\t| " << setw(6) << id
                    << " | " << setw(8) << name
                    << " | " << setw(11) << time
                    << " | " << setw(10) << (status == "Available" ? GREEN : RED) << status << RESET << " |" << endl;
            }

            cout << BOLD << "\t+--------+----------+-------------+----------------+\n" << RESET;

            // Display admin options
            cout << YELLOW << "\n\tAdmin Options:\n" << RESET;
            cout << BOLD << "\t1. Update Slot Status\n" << RESET;
            cout << BOLD << "\t2. Refresh Slot Status\n" << RESET;
            cout << BOLD << "\t3. Exit\n" << RESET;
            cout << CYAN << "\n\tEnter your choice: " << RESET;
            //cin >> choice;
            getline(cin, choice);
            //cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == "1") {
                string slotID, newStatus;
                string statusChoice;

                // Prompt admin for SlotID
                cout << CYAN << "\n\tEnter SlotID to update: " << RESET;
                //cin >> slotID;
                getline(cin, slotID);

                // Provide options for status
                cout << CYAN << "\n\tSelect new status:\n" << RESET;
                cout << GREEN << "\t1. Available\n" << RESET;
                cout << RED << "\t2. Booked\n" << RESET;
                cout << CYAN << "\n\tEnter your choice (1/2): " << RESET;
                //cin >> statusChoice;
                getline(cin, statusChoice);
                //cin.ignore(numeric_limits<streamsize>::max(), '\n');

                // Validate and assign the new status based on the choice
                if (statusChoice == "1") {
                    newStatus = "Available";
                }
                else if (statusChoice == "2") {
                    newStatus = "Booked";
                }
                else {
                    cout << RED << "\n\tInvalid choice! Please select '1' for Available or '2' for Booked.\n" << RESET;
                    system("pause");
                    continue;
                }

                // Update slot status in the database
                db.prepareStatement("UPDATE parkingslot SET SlotStatus = ? WHERE SlotID = ?;");
                db.stmt->setString(1, newStatus);
                db.stmt->setString(2, slotID);

                int rowsAffected = db.executeUpdate();

                if (rowsAffected > 0) {
                    cout << GREEN << "\n\tSlot status updated successfully!\n" << RESET;
                }
                else {
                    cout << RED << "\n\tFailed to update slot status. Ensure the SlotID exists.\n" << RESET;
                }
                system("pause");
            }
            else if (choice == "2") {
                // Refresh all slots to "Available"
                db.prepareStatement("UPDATE parkingslot SET SlotStatus = 'Available';");
                int rowsAffected = db.executeUpdate();

                if (rowsAffected > 0) {
                    cout << GREEN << "\n\tAll slot statuses refreshed to 'Available' successfully!\n" << RESET;
                }
                else {
                    cout << RED << "\n\tFailed to refresh slot statuses.\n" << RESET;
                }
                system("pause");
            }
            else if (choice == "3") {
                //cout << RED << "\n\tExiting manageSlot interface...\n" << RESET;
                //system("pause");
                break;
            }
            else {
                cout << RED << "\n\tInvalid choice! Please try again.\n" << RESET;
                system("pause");
            }
        }
        catch (const sql::SQLException& e) {
            cerr << RED << "\n\tSQL Error: " << e.what()
                << " (Code: " << e.getErrorCode()
                << ", SQLState: " << e.getSQLState() << ")" << RESET << endl;
            system("pause");
        }
        catch (const exception& e) {
            cerr << RED << "\n\tError: " << e.what() << RESET << endl;
            system("pause");
        }
    }
}

// Function to set a fine for a user
void setFine(User& admin, Fine& fine, Reservation& reservation, Car& car)
{
    string userID;
    double fineAmount = 0.0;
    string fineReason;
    char choice;

    while (true)
    {
        system("cls");
        cout << BOLD << CYAN << "\n\tSet Fine Menu" << RESET << endl;
        cout << YELLOW << "\t------------------------------------\n" << RESET;
        cout << BOLD << "\t[1] User ID      : " << RESET << userID << endl;
        cout << BOLD << "\t[2] Fine Amount  : " << RESET << (fineAmount > 0 ? to_string(fineAmount) : "Not Set") << endl;
        cout << BOLD << "\t[3] Fine Reason  : " << RESET << (!fineReason.empty() ? fineReason : "Not Set") << endl << endl;

        cout << BOLD << YELLOW << "\t[S] Show Reservation Detail Today\n" << RESET;
        cout << BOLD << YELLOW << "\t[C] Confirm\n" << RESET;
        cout << BOLD << YELLOW << "\t[R] Return\n" << RESET;
        cout << YELLOW << "\t------------------------------------\n" << RESET;
        cout << BOLD << "\nEnter your choice: " << RESET;
        choice = _getch();

        if (choice == '1')
        {
            cout << CYAN << "\nEnter User ID: " << RESET;
            getline(cin, userID);
        }
        else if (choice == '2')
        {
            string input;
            bool valid = false;
            do
            {
                cout << CYAN << "\nEnter Fine Amount: " << RESET;
                getline(cin, input);
                valid = true;
                for (char c : input)
                {
                    if (!isdigit(c) && c != '.' && c != '-')
                    {
                        valid = false;
                        break;
                    }
                }
                if (valid)
                {
                    try
                    {
                        fineAmount = stod(input);
                        if (fineAmount <= 0)
                        {
                            cout << RED << "Invalid amount. Please enter a positive number." << RESET << endl;
                            valid = false;
                        }
                    }
                    catch (...)
                    {
                        valid = false;
                    }
                }
                if (!valid)
                {
                    cout << RED << "Invalid input. Please enter a valid positive number." << RESET << endl;
                }
            } while (!valid);
            fineAmount = floor(fineAmount * 100 + 0.5) / 100; // Round to 2 decimal places
        }
        else if (choice == '3')
        {
            cout << CYAN << "\nEnter Fine Reason: " << RESET;
            getline(cin, fineReason);

            while (fineReason.length() > 45) {
                cout << RED << "\n\tError: Fine reason cannot exceed 45 characters.\n" << RESET;
                cout << CYAN << "\nEnter Fine Reason (max 45 characters): " << RESET;
                getline(cin, fineReason);
            }

        }
        else if (toupper(choice) == 'S') {
            while (true) {
                system("cls");
                string today = getCurrentDate();

                try {
                    DBConnection db;

                    string query =
                        "SELECT DISTINCT user.UserID, user.UserName, user.UserPhoneNo, "
                        "car.CarModel, car.CarLicensePlate, "
                        "reservations.TotalTimeReserved, reservations.ReservedSlotName, "
                        "reservations.ReservedTimeSlot "
                        "FROM reservations "
                        "JOIN user ON reservations.UserID = user.UserID "
                        "LEFT JOIN car ON reservations.UserID = car.UserID "
                        "WHERE reservations.ReservationDate = ? AND car.CarDate = ?";

                    db.prepareStatement(query);
                    db.setString(1, today); 
                    db.setString(2, today); 

                    db.QueryResult();

                    if (db.res->rowsCount() == 0) {
                        cout << "No reservations found for today (" << today << ")." << endl;
                        system("pause");
                        break;
                    }

                    cout << endl;
                    cout << BOLD << CYAN << "\t\t\t\t\t\tShow Reservation Detail Today" << RESET << endl;
                    cout << YELLOW << string(140, '-') << RESET << endl;
                    cout << BOLD << left << setw(15) << "User ID"
                        << setw(20) << "User Name"
                        << setw(15) << "Phone No"
                        << setw(20) << "Car Model"
                        << setw(15) << "License Plate"
                        << setw(15) << "Time Reserved"
                        << setw(20) << "Slot Name"
                        << "Time Slots" << RESET << endl;
                    cout << YELLOW << string(140, '-') << RESET << endl;

                    while (db.res->next()) {
                        cout << BOLD << left << setw(15) << db.res->getString("UserID")
                            << setw(20) << db.res->getString("UserName")
                            << setw(15) << db.res->getString("UserPhoneNo")
                            << setw(20) << db.res->getString("CarModel")
                            << setw(15) << db.res->getString("CarLicensePlate")
                            << setw(15) << db.res->getString("TotalTimeReserved")
                            << setw(20) << db.res->getString("ReservedSlotName")
                            << db.res->getString("ReservedTimeSlot") << RESET << endl;
                    }
                    cout << YELLOW << string(140, '-') << RESET << endl;
                    cout << endl << endl;
                    system("pause");
                    break;
                }
                catch (const sql::SQLException& e) {
                    cerr << "# ERR: SQLException in " << __FILE__;
                    cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
                    cerr << "# ERR: " << e.what();
                    cerr << " (MySQL error code: " << e.getErrorCode();
                    cerr << ", SQLState: " << e.getSQLState() << " )" << endl;
                }
                catch (const exception& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        }
        else if (toupper(choice) == 'C')
        {
            if (userID.empty() || fineAmount <= 0 || fineReason.empty())
            {
                cout << RED << "\nAll fields must be filled before confirming!\n" << RESET;
                system("pause");
            }
            else
            {
                // Generate a new Fine ID
                string newFineID = fine.generateNewFineID();
                string currentDate = getCurrentDate(); 

                // Set the fine details
                fine.setFineID(newFineID);
                fine.setUserID(userID);
                fine.setFineAmount(fineAmount);
                fine.setFineReason(fineReason);
                fine.setFineStatus("No"); 
                fine.setFineDate(currentDate); 

                if (fine.insert())
                {
                    //Fine successfully set for User ID
                }
                else
                {
                    cerr << RED << "\nFailed to set the fine. Please try again.\n" << RESET;
                }
                system("pause");
                break;
            }
        }
        else if (toupper(choice) == 'R')
        {
            cout << CYAN << "\nReturning to the previous menu...\n" << RESET;
            break;
        }
        else
        {
            cout << RED << "\nInvalid choice. Please try again.\n" << RESET;
            system("pause");
        }
    }
}

// Statistics & Analytics
void analyticsStatistics()
{
    char option = '~';   

    while (1)
    {
        system("cls");

        cout << BOLD << CYAN << setw(60) << "\n\tStatistics & Analytics" << RESET << endl;
        cout << YELLOW << "\t----------------------------------------" << RESET << endl;

        cout << BOLD << "\t[A] Monthly Income From Parking Slots" << RESET << endl;
        cout << BOLD << "\t[B] Total Fine Received" << RESET << endl;
        cout << BOLD << "\t[R] Return to Main Menu" << RESET << endl;
        cout << YELLOW << "\t----------------------------------------\n" << RESET << endl;

        switch (option)
        {
        case '~':
            cout << BOLD << "\tSelect an option: " << RESET;
            option = tolower(_getch());
            break;

        case'a':
            incomeMonthlySlot();
            option = '~';
            break;

        case'b':
            monthlyFine();
            option = '~';
            break;

        case'r':
            cout << RED << "\n\tReturning to the main menu...\n" << RESET;
            system("pause");
            break;

        default:
            cout << RED << "\tError: Invalid option. Please try again." << RESET;
            option = _getch();
            break;

        }

        if (option == 'r')
        {
            break;
        }

    }

}

void incomeMonthlySlot() {
    DBConnection db;

    char option = '~'; 

    while (true) {
        system("cls");

        cout << BOLD << CYAN << setw(60) << "\n\tMonthly Income (Payment)" << RESET << endl;
        cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;

        string year;
        cout << BOLD << CYAN << "\n\tEnter year to view income (e.g., 2025) or [R] to return: " << RESET;
        //cin >> year;
        getline(cin, year);
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(year[0]) == 'r' && year.length() == 1) {
            cout << RED << "\n\tReturning to the main menu...\n" << RESET;
            system("pause");
            return;
        }

        if (year.length() != 4 || !all_of(year.begin(), year.end(), ::isdigit)) {
            cout << RED << "\n\tInvalid year format. Please enter a valid year (e.g., 2025).\n" << RESET;
            system("pause");
            continue;
        }

        // Check if the year exists in the database
        string yearCheckQuery = "SELECT DISTINCT SUBSTR(PaymentDate, 1, 4) AS Year FROM payment WHERE PaymentStatus = 'Completed'";
        db.prepareStatement(yearCheckQuery);
        db.QueryResult();

        bool yearExists = false;
        while (db.res->next()) {
            if (db.res->getString("Year") == year) {
                yearExists = true;
                break;
            }
        }

        if (!yearExists) {
            cout << RED << "\n\tInvalid year. No data found for the year " << year << ".\n" << RESET;
            system("pause");
            continue;
        }

        // Clear the screen before showing the histogram
        system("cls");

        cout << BOLD << CYAN << setw(60) << "\n\tMonthly Income (Payment)" << RESET << endl;
        cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;

        // SQL query to calculate monthly income for the given year
        string incomeQuery = "SELECT COALESCE(SUM(PaymentAmount), 0) AS TotalIncome, months.Month "
            "FROM (SELECT '01' AS Month UNION SELECT '02' UNION SELECT '03' UNION SELECT '04' "
            "UNION SELECT '05' UNION SELECT '06' UNION SELECT '07' UNION SELECT '08' UNION SELECT '09' "
            "UNION SELECT '10' UNION SELECT '11' UNION SELECT '12') AS months "
            "LEFT JOIN payment ON months.Month = SUBSTR(PaymentDate, 6, 2) "
            "AND SUBSTR(PaymentDate, 1, 4) = ? AND PaymentStatus = 'Completed' "
            "GROUP BY months.Month";

        db.prepareStatement(incomeQuery); // Prepare the query
        db.setString(1, year);            // Bind the year parameter
        db.QueryResult();                 // Execute the query

        double maxIncome = 0;
        double totalIncome = 0; // Variable to hold total income
        vector<pair<string, double>> monthlyIncome; // Store income and month for further processing

        while (db.res->next()) {
            string month = db.res->getString("Month");
            double monthIncome = db.res->getDouble("TotalIncome");
            monthlyIncome.emplace_back(month, monthIncome);
            totalIncome += monthIncome; // Add to total income
            maxIncome = max(maxIncome, monthIncome); // Find the highest income for scaling
        }

        // Display histogram with scaling
        cout << endl << BOLD << "Monthly Income Histogram for Year " << year << RESET << endl;
        const int maxBarWidth = 50; // Maximum width for histogram bars

        for (const auto& entry : monthlyIncome) {
            string month = entry.first;
            double income = entry.second;
            int barWidth = static_cast<int>((income / maxIncome) * maxBarWidth); // Scale the bar width

            // Calculate the percentage
            double percentage = (totalIncome > 0) ? (income / totalIncome) * 100 : 0;

            // Check if the bar width is 0
            if (barWidth > 0) {
                // Display histogram bar with scaling
                cout << BOLD << "Month " << setw(2) << month << " : " << RESET;
                cout << "\033[42m" << string(barWidth, ' ') << RESET; // Green background bar
                cout << " " << YELLOW << "RM " << fixed << setprecision(2) << income << " (" << fixed << setprecision(2) << percentage << "%)" << RESET << endl << endl;
            }
            else {
                // If barWidth is 0, display the month with no bar
                cout << BOLD << "Month " << setw(2) << month << " : " << RESET;
                cout << " " << YELLOW << "RM " << fixed << setprecision(2) << income << " (" << fixed << setprecision(2) << percentage << "%)" << RESET << endl << endl;
            }
        }

        // Variables for statistical calculations
        double minIncome = DBL_MAX; // Set initial minimum to a very high value
        double sumOfSquares = 0.0; // For variance calculation

        for (const auto& entry : monthlyIncome) {
            double income = entry.second;
            minIncome = min(minIncome, income); // Find the minimum income
            sumOfSquares += pow(income - (totalIncome / 12), 2); // Sum of squares for variance
        }

        double averageIncome = totalIncome / 12; // Calculate average income
        double variance = sumOfSquares / 12;     // Calculate variance
        double stdDeviation = sqrt(variance);    // Calculate standard deviation

        // Footer and Information
        cout << CYAN << "\n\tNote: Histogram bar length is proportional to income.\n" << RESET;
        cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;

        // Display total income
        cout << BOLD << CYAN << "\n\tTotal Income for the Year " << year << ": " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << totalIncome << RESET << endl;

        // Display additional statistics
        cout << BOLD << CYAN << "\n\tMaximum Monthly Income: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << maxIncome << RESET << endl;

        cout << BOLD << CYAN << "\n\tMinimum Monthly Income: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << minIncome << RESET << endl;

        cout << BOLD << CYAN << "\n\tAverage Monthly Income: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << averageIncome << RESET << endl;

        //cout << BOLD << CYAN << "\n\tIncome Variance: " << RESET;
        //cout << YELLOW << "RM " << fixed << setprecision(2) << variance << RESET << endl;

        cout << BOLD << CYAN << "\n\tStandard Deviation of Income: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << stdDeviation << RESET << endl;
        cout << YELLOW << "\n\t------------------------------------------------------------" << RESET << endl;

        cout << BOLD << "\tPress any key to return to the input year interface...\n" << RESET;
        system("pause");
    }
}

void monthlyFine() {
    DBConnection db; 

    while (true) {
        system("cls");

        cout << BOLD << CYAN << setw(60) << "\n\tMonthly Fines (Collected)" << RESET << endl;
        cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;

        string year;
        cout << BOLD << CYAN << "\n\tEnter year to view fines (e.g., 2025) or [R] to return: " << RESET;
        getline(cin, year);
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (tolower(year[0]) == 'r' && year.length() == 1) {
            cout << RED << "\n\tReturning to the main menu...\n" << RESET;
            system("pause");
            return;
        }

        if (year.length() != 4 || !all_of(year.begin(), year.end(), ::isdigit)) {
            cout << RED << "\n\tInvalid year format. Please enter a valid year (e.g., 2025).\n" << RESET;
            system("pause");
            continue;
        }

        // Check if the year exists in the database
        string yearCheckQuery = "SELECT DISTINCT SUBSTR(PaymentDate, 1, 4) AS Year FROM payment WHERE PaymentStatus = 'Completed'";
        db.prepareStatement(yearCheckQuery);
        db.QueryResult();

        bool yearExists = false;
        while (db.res->next()) {
            if (db.res->getString("Year") == year) {
                yearExists = true;
                break;
            }
        }

        if (!yearExists) {
            cout << RED << "\n\tInvalid year. No data found for the year " << year << ".\n" << RESET;
            system("pause");
            continue;
        }

        system("cls");

        cout << BOLD << CYAN << setw(60) << "\n\tMonthly Fines (Collected)" << RESET << endl;
        cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;
        string fineQuery = "SELECT COALESCE(SUM(FineAmount), 0) AS TotalFines, months.Month "
            "FROM (SELECT '01' AS Month UNION SELECT '02' UNION SELECT '03' UNION SELECT '04' "
            "UNION SELECT '05' UNION SELECT '06' UNION SELECT '07' UNION SELECT '08' UNION SELECT '09' "
            "UNION SELECT '10' UNION SELECT '11' UNION SELECT '12') AS months "
            "LEFT JOIN fines ON months.Month = SUBSTR(FineDate, 6, 2) "
            "AND SUBSTR(FineDate, 1, 4) = ? AND FineStatus = 'Yes' "
            "GROUP BY months.Month";

        db.prepareStatement(fineQuery); 
        db.setString(1, year);    
        db.QueryResult();              

        bool hasData = false; // Flag to check if data exists for the year
        vector<pair<string, double>> monthlyFines; // Store fines and months for further processing
        double maxFine = 0;
        double minFine = DBL_MAX; // Set initial minimum to a very high value
        double totalFines = 0; // Variable to hold the total fines
        double sumOfSquares = 0.0; // For variance calculation

        while (db.res->next()) {
            hasData = true; // Data exists for the year
            string month = db.res->getString("Month");
            double fines = db.res->getDouble("TotalFines");
            monthlyFines.emplace_back(month, fines);
            totalFines += fines; // Add to total fines
            maxFine = max(maxFine, fines); // Find the highest fine for scaling
            minFine = min(minFine, fines); // Find the lowest fine
            sumOfSquares += pow(fines - (totalFines / 12), 2); // Sum of squares for variance
        }

        if (!hasData) { // If no data found for the year
            cout << RED << "\n\tNo fines recorded for the year " << year << ".\n" << RESET;
            system("pause");
            continue;
        }

        // Display histogram with scaling
        cout << endl << BOLD << "Monthly Fines Collected for Year " << year << RESET << endl;
        const int maxBarWidth = 50; // Maximum width for histogram bars

        for (const auto& entry : monthlyFines) {
            string month = entry.first;
            double fines = entry.second;
            int barWidth = static_cast<int>((fines / maxFine) * maxBarWidth); // Scale the bar width

            // Calculate the percentage
            double percentage = (totalFines > 0) ? (fines / totalFines) * 100 : 0;

            // Check if the bar width is 0
            if (barWidth > 0) {
                // Display histogram bar with scaling
                cout << BOLD << "Month " << setw(2) << month << " : " << RESET;
                cout << "\033[42m" << string(barWidth, ' ') << RESET; // Green background bar
                cout << " " << YELLOW << "RM " << fixed << setprecision(2) << fines << " (" << fixed << setprecision(2) << percentage << "%)" << RESET << endl << endl;
            }
            else {
                // If barWidth is 0, display the month with no bar
                cout << BOLD << "Month " << setw(2) << month << " : " << RESET;
                cout << " " << YELLOW << "RM " << fixed << setprecision(2) << fines << " (" << fixed << setprecision(2) << percentage << "%)" << RESET << endl << endl;
            }
        }


        // Calculate statistics
        double averageFine = totalFines / 12; // Calculate average fine
        double variance = sumOfSquares / 12;   // Calculate variance
        double stdDeviation = sqrt(variance);  // Calculate standard deviation

        cout << CYAN << "\n\tNote: Histogram bar length is proportional to fines collected.\n" << RESET;
        cout << YELLOW << "\t------------------------------------------------------------" << RESET << endl;

        cout << BOLD << CYAN << "\n\tTotal Fine Collected for the Year " << year << ": " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << totalFines << RESET << endl;

        // Display additional statistics
        cout << BOLD << CYAN << "\n\tMaximum Monthly Fine: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << maxFine << RESET << endl;

        cout << BOLD << CYAN << "\n\tMinimum Monthly Fine: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << minFine << RESET << endl;

        cout << BOLD << CYAN << "\n\tAverage Monthly Fine: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << averageFine << RESET << endl;

        //cout << BOLD << CYAN << "\n\tIncome Variance: " << RESET;
        //cout << YELLOW << "RM " << fixed << setprecision(2) << variance << RESET << endl;

        cout << BOLD << CYAN << "\n\tStandard Deviation of Income: " << RESET;
        cout << YELLOW << "RM " << fixed << setprecision(2) << stdDeviation << RESET << endl;

        cout << YELLOW << "\n\t------------------------------------------------------------" << RESET << endl;
        cout << BOLD << "\tPress any key to return to the input year interface...\n" << RESET;
        system("pause");
    }
}

void slotReport(Payment& payment) {
    DBConnection db;  

    while (true) {
        system("cls");

        // Prompt user to input a year or return to the main menu
        string year;
        cout << BOLD << CYAN << "\n\tEnter year to view annual report (e.g., 2024) or [R] to return: " << RESET;
        getline(cin, year);
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Allow the user to return to the previous menu
        if (tolower(year[0]) == 'r' && year.length() == 1) {
            cout << RED << "\n\tReturning to the main menu...\n" << RESET;
            system("pause");
            return;
        }

        // Validate year input
        if (year.length() != 4 || !all_of(year.begin(), year.end(), ::isdigit)) {
            cout << RED << "\n\tInvalid year format. Please enter a valid year (e.g., 2024).\n" << RESET;
            system("pause");
            continue;
        }

        // Check if the year exists in the database
        string yearCheckQuery = "SELECT DISTINCT SUBSTR(PaymentDate, 1, 4) AS Year FROM payment WHERE PaymentStatus = 'Completed'";
        db.prepareStatement(yearCheckQuery);
        db.QueryResult();

        bool yearExists = false;
        while (db.res && db.res->next()) {
            if (db.res->getString("Year") == year) {
                yearExists = true;
                break;
            }
        }

        if (!yearExists) {
            cout << RED << "\n\tInvalid year. No data found for the year " << year << ".\n" << RESET;
            system("pause");
            continue;
        }

        // Proceed to display the report for the valid year
        system("cls");
        
        const string loadingText = "\tGenerating report for year " + year + " ... ";
        const int totalSteps = 25;  
        const char progressChar = '-';  
        const char fillChar = '#';      

        // Display the loading message at the top
        cout << BOLD << CYAN << loadingText;
        cout.flush();  // Ensure it's printed immediately

        for (int i = 0; i <= totalSteps; ++i) {
            system("cls");

            cout << BOLD << CYAN << loadingText;
            cout.flush();  

            cout << "[";

            for (int j = 0; j < i; ++j) {
                cout << GREEN << fillChar << RESET;
            }

            for (int j = i; j < totalSteps; ++j) {
                cout << progressChar;
            }

            cout << "]" << flush;  
            this_thread::sleep_for(chrono::milliseconds(100));  
        }

        cout << "\n" << RESET;

        while (true) {
            system("cls");
            payment.annualSlotReport(year);
            cout << BOLD << "[P] save as text.file\n";
            cout << "[R] Return" << RESET;

            char choice = _getch();
            if (tolower(choice) == 'p') {
                saveAnnualSlotReportToFile(payment, stoi(year));
                system("pause");
            }
            else if (tolower(choice) == 'r') {
                cout << BOLD << "\nReturning to the main menu..." << RESET << endl;
                return;
            }
            else {
                cout << BOLD << "\nInvalid choice. Please select again..." << RESET << endl;
                system("pause");
            }
        }
    }
}

void saveAnnualSlotReportToFile(Payment& payment, int year) {
    string filename = "Annual_Slot_Report_" + to_string(year) + ".txt";
    ofstream outFile(filename);

    if (!outFile) {
        cerr << "Error: Could not create file " << filename << endl;
        return;
    }

    payment.annualSlotReporttxt(year, outFile); 
    outFile.close();

    cout << BOLD << "Report successfully saved to " << filename << RESET << endl;
}

// Admin Management
void manageAdmin(User& user)
{
    char option = '~';

    while (1)
    {
        system("cls");

        cout << BOLD << CYAN << setw(60) << "\n\tAdmin Management" << RESET << endl;
        cout << YELLOW << "\t----------------------------------------" << RESET << endl;

        cout << BOLD << "\t[1] Edit Admin Detail" << RESET << endl;
        cout << BOLD << "\t[2] Add Admin" << RESET << endl;
        cout << BOLD << "\t[3] Delete Admin" << RESET << endl;
        cout << BOLD << "\t[R] Return to Main Menu" << RESET << endl;
        cout << YELLOW << "\t----------------------------------------\n" << RESET << endl;

        switch (option)
        {
        case '~':
            cout << BOLD << "\tSelect an option: " << RESET;
            option = tolower(_getch());
            break;

        case'1':
            editAdminProfile(user);
            option = '~';
            break;

        case'2':
            addAdmin(user);
            option = '~';
            break;

        case'3':
            deleteAdmin(user);
            option = '~';
            break;

        case'r':
            cout << RED << "\n\tReturning to the main menu...\n" << RESET;
            system("pause");
            break;

        default:
            cout << RED << "\tError: Invalid option. Please try again." << RESET;
            option = _getch();
            break;

        }

        if (option == 'r')
        {
            break;
        }

    }
}

// Edit Admin Profile
void editAdminProfile(User& user)
{
    if (confirmAdminIdentity(user) == true)
    {
        char option = '~';

        int NewEdit = 0;   // Confirm Edited
        string details[4];

        details[0] = user.fetchUserName();
        details[1] = user.fetchUserPhoneNo();
        details[2] = user.fetchUserGender();
        details[3] = user.fetchUserPassword();

        while (1)
        {

            system("cls");

            cout << CYAN;
            cout << setw(50) << left << "\t------------------------------------------------------------------------" << RESET << endl;
            cout << BOLD << setw(42) << "\t\t\t        Admin Profile        " << RESET << endl;
            cout << CYAN << setw(50) << "\t------------------------------------------------------------------------" << RESET << endl;
            cout << BOLD << setw(40) << "\t[1] Username (min.8 char & max.50 char)               :" << RESET << YELLOW << left << details[0] << RESET << endl;
            cout << BOLD << setw(40) << "\t[2] Phone No. (01xxxxxxxx)                            :" << RESET << YELLOW << left << details[1] << RESET << endl;
            cout << BOLD << setw(40) << "\t[3] Gender ('M' or 'F')                               :" << RESET << YELLOW << left << details[2] << RESET << endl;
            cout << BOLD << setw(40) << "\t[4] Password (min. 8 char, max. 20 char, No-spacing)  :" << RESET << YELLOW << string(details[3].length(), '*') << RESET << endl << endl;
            cout << YELLOW << BOLD << setw(35) << "\t[C] Confirm" << endl;
            cout << setw(35) << "\t[R] Return  " << RESET << endl << endl;
            cout << CYAN;
            cout << setw(50) << "\t------------------------------------------------------------------------" << RESET << endl;


            switch (option)
            {
            case '~':
                cout << BOLD << "Select an option: " << RESET;
                option = _getch();
                break;

            case '1':
                cout << CYAN << "Name (min.8 char & max.50 char): " << RESET;
                getline(cin, details[0]);
                while (details[0].length() < 8 || details[0].length() > 50)
                {
                    cout << RED << "Error input. Enter name again (min.8 char & max.50 char): " << RESET;
                    getline(cin, details[0]);
                }
                option = '~';
                break;

            case '2':
                cout << CYAN << "Phone No. (01xxxxxxxx): " << RESET;
                getline(cin, details[1]);
                while (validPhoneNumber(details[1]) == false)
                {
                    cout << RED << "Error input. Enter phone no. again (01xxxxxxxx): " << RESET;
                    getline(cin, details[1]);
                }
                option = '~';
                break;

            case '3':
                cout << CYAN << "Gender ('M' or 'F'): " << RESET;
                getline(cin, details[2]);
                while (details[2][0] != 'm' && details[2][0] != 'f' || details[2].length() != 1)
                {
                    cout << RED << "Error input. Please enter gender again ('M' or 'F'): " << RESET;
                    getline(cin, details[2]);
                }

                if (details[2][0] == 'm')
                {
                    details[2][0] = 'M';
                }
                else if (details[2][0] == 'f')
                {
                    details[2][0] = 'F';
                }

                option = '~';
                break;

            case '4':
                details[3].clear();
                cout << CYAN << "Password (min. 8 char, max. 20 char, No-spacing): " << RESET;
                char ch;
                while ((ch = _getch()) != '\r') { // Enter key is pressed
                    if (ch == '\b') { // Backspace is pressed
                        if (!details[3].empty()) {
                            details[3].pop_back();
                            cout << "\b \b"; // Erase character from console
                        }
                    }
                    else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                        details[3].push_back(ch);
                        cout << '*';
                    }
                }
                cout << endl;

                // Check if the password meets the required conditions (min. 8, max. 20, no spaces)
                while (details[3].length() < 8 || details[3].length() > 20 || details[3].find(' ') != string::npos) {
                    cout << RED << "Error input. Enter password again (min. 8 char, max. 20 char, No-spacing): " << RESET;
                    details[3].clear();
                    while ((ch = _getch()) != '\r') { // Enter key is pressed
                        if (ch == '\b') { // Backspace is pressed
                            if (!details[3].empty()) {
                                details[3].pop_back();
                                cout << "\b \b"; // Erase character from console
                            }
                        }
                        else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                            details[3].push_back(ch);
                            cout << '*';
                        }
                    }
                    cout << endl;
                }
                option = '~';
                break;

            case 'c':
                if (details[0].empty() || details[1].empty() || details[2].empty() || details[3].empty())
                {
                    cout << RED << setw(105) << "Invalid Information. Press a key to continue." << RESET;
                    _getch();
                    option = '~';
                }
                else
                {
                    user.insertUserName(details[0]);
                    user.insertUserPhoneNo(details[1]);
                    user.insertUserGender(details[2]);
                    user.insertUserPassword(details[3]);

                    user.update();
                    cout << GREEN << setw(105) << "Profile updated successfully!" << RESET << endl;
                    NewEdit = 1;
                }
                _getch();
                break;

            case 'r':
                break;

            default:
                cout << RED << "Error input. Please select again." << RESET;
                option = _getch();
                break;
            }

            if (option == 'r' || NewEdit == 1)
            {
                option = '~';
                break;
            }

        }

    }
    else
    {
        cout << RED << "\nUser identity verification failed. Returning to menu." << RESET << endl;
        _getch();
    }

}

// Authenticate admin identity
bool confirmAdminIdentity(User& user)
{
    char option = '~';
    string password = "";

    int correctUser = 0;
    while (1)
    {
        system("cls");

        cout << CYAN;
        cout << setw(50) << left << "\t==================================================" << RESET << endl;
        cout << BOLD << setw(42) << "\t        Confirm Admin Identity        " << RESET << endl;
        cout << CYAN << setw(50) << "\t==================================================" << RESET << endl;
        cout << BOLD << setw(40) << "\t[1] Current Password :  " << RESET << YELLOW << left << string(password.length(), '*') << RESET << endl << endl;
        cout << YELLOW << BOLD << setw(35) << "\t[C] Confirm" << endl;
        cout << setw(35) << "\t[R] Return  " << RESET << endl << endl;
        cout << CYAN;
        cout << setw(50) << "\t==================================================" << RESET << endl;


        switch (option)
        {
        case '~':
            cout << BOLD << setw(5) << "Select an option: " << RESET;
            option = tolower(_getch());
            break;

        case '1':
            password.clear();
            cout << YELLOW << "\nEnter current admin password to confirm identity: " << RESET;
            char ch;
            while ((ch = _getch()) != '\r') { // Enter key is pressed
                if (ch == '\b') { // Backspace is pressed
                    if (!password.empty()) {
                        password.pop_back();
                        cout << "\b \b"; // Erase character from console
                    }
                }
                else if (password.length() < 20 && ch != ' ') { // Restrict max length and no spaces
                    password.push_back(ch);
                    cout << '*';
                }
            }
            cout << endl;
            option = '~';
            break;

        case 'c':
            if (password == user.fetchUserPassword())
            {
                correctUser = 1;
                break;
            }
            else
            {
                cout << RED << "Invalid Password. Press a key to continue." << RESET;
                _getch();
                option = '~';
                break;
            }

        case 'r':
            break;

        default:
            cout << RED << "Error input. Please select again." << RESET;
            option = _getch();
            break;

        }

        if (option == 'r')
        {
            return false;
            break;
        }
        else if (correctUser == 1)
        {
            return true;
            break;
        }

    }
}

// Add a new Admin
void addAdmin(User& user)
{
    int NewUser = 0;   // For Register

    char option = '~';
    string details[5]; // Customer Information

    while (1)
    {
        system("cls");

        cout << CYAN;
        cout << setw(50) << left << "\t======================================================================================================" << RESET << endl;
        cout << BOLD << setw(42) << "\t\t\t\t\t        Add Admin        " << RESET << endl;
        cout << CYAN << setw(50) << "\t======================================================================================================" << RESET << endl;
        cout << BOLD << setw(40) << "\t[1] Username (min.8 char & max.50 char)               : " << RESET << YELLOW << left << details[0] << RESET << endl;
        cout << BOLD << setw(40) << "\t[2] Phone No. (01xxxxxxxx)                            : " << RESET << YELLOW << left << details[1] << RESET << endl;
        cout << BOLD << setw(40) << "\t[3] Gender ('M' or 'F')                               : " << RESET << YELLOW << left << details[2] << RESET << endl;
        cout << BOLD << setw(40) << "\t[4] Password (min. 8 char, max. 20 char, No-spacing)  : " << RESET << YELLOW << string(details[3].length(), '*') << RESET << endl << endl;
        cout << YELLOW << BOLD << setw(35) << "\t[C] Add" << endl;
        cout << setw(35) << "\t[R] Return  " << RESET << endl << endl;
        cout << CYAN;
        cout << setw(50) << "\t======================================================================================================" << RESET << endl;

        switch (option)
        {
        case '~':
            cout << BOLD << setw(5) << "Select an option: " << RESET;
            option = tolower(_getch());
            break;

        case '1':
            cout << YELLOW << "Name (min.8 char & max.50 char): " << RESET;
            getline(cin, details[0]);
            while (details[0].length() < 8 || details[0].length() > 50)
            {
                cout << RED << "Error: Enter name again (min.8 char & max.50 char): " << RESET;
                getline(cin, details[0]);
            }
            option = '~';
            break;

        case '2':
            cout << YELLOW << "Phone No. (01xxxxxxxx): " << RESET;
            getline(cin, details[1]);
            while (validPhoneNumber(details[1]) == false)
            {
                cout << RED << "Error: Enter phone no. again (01xxxxxxxx): " << RESET;
                getline(cin, details[1]);
            }
            option = '~';
            break;

        case '3':
            cout << YELLOW << "Gender ('M' or 'F'): " << RESET;
            getline(cin, details[2]);
            while ((details[2][0] != 'm' && details[2][0] != 'f' && details[2][0] != 'M' && details[2][0] != 'F') || details[2].length() != 1)
            {
                cout << RED << "Error: Please enter gender again ('M' or 'F'): " << RESET;
                getline(cin, details[2]);
            }

            if (details[2][0] == 'm')
            {
                details[2][0] = 'M';
            }
            else if (details[2][0] == 'f')
            {
                details[2][0] = 'F';
            }

            option = '~';
            break;

        case '4':
            details[3].clear();
            cout << YELLOW << "Password (min.8 char, max.20 char & No-spacing): " << RESET;
            char ch;

            // Capture user input for the password
            while ((ch = _getch()) != '\r') { // Enter key is pressed
                if (ch == '\b') { // Backspace is pressed
                    if (!details[3].empty()) {
                        details[3].pop_back();
                        cout << "\b \b"; // Erase character from console
                    }
                }
                else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                    details[3].push_back(ch);
                    cout << '*';
                }
            }
            cout << endl;

            // Validate password length and spacing
            while (details[3].length() < 8 || details[3].length() > 20 || details[3].find(' ') != string::npos) {
                details[3].clear(); // Clear the password if it does not meet criteria
                cout << RED << "Error: Password must be between 8 and 20 characters and contain no spaces. Enter password again: " << RESET;

                // Capture user input for the password again
                while ((ch = _getch()) != '\r') { // Enter key is pressed
                    if (ch == '\b') { // Backspace is pressed
                        if (!details[3].empty()) {
                            details[3].pop_back();
                            cout << "\b \b"; // Erase character from console
                        }
                    }
                    else if (details[3].length() < 20 && ch != ' ') { // Restrict max length and no spaces
                        details[3].push_back(ch);
                        cout << '*';
                    }
                }
                cout << endl;
            }

            option = '~';
            break;

        case 'c':
        {
            if (details[0].empty() || details[1].empty() || details[2].empty() || details[3].empty())
            {
                cout << RED << setw(60) << "Invalid Information. Press any key to continue." << RESET;
                _getch();
                option = '~';
            }
            else
            {
                // Generate a new unique UserID
                User tempUser;
                string newUserID = tempUser.generateNewUserID();
                details[4] = newUserID;

                User newcustomer(details[4], details[3], details[0], details[1], details[2], "Admin");
                if (newcustomer.newUser() == false)
                {
                    cout << setw(105) << "UserID already exists. Press a key to continue.";
                    _getch();
                    option = '~';
                }
                else
                {
                    cout << setw(127) << "Are you sure?  (Press 'Y' to confirm, 'N' to cancel).";
                    option = _getch();

                    while (option != 'y' && option != 'n')
                    {
                        cout << endl;
                        cout << setw(118) << BOLD << "Error input. Please select again (Press 'Y' to confirm, 'N' to cancel)." << RESET;
                        option = tolower(_getch());
                    }

                    if (option == 'n')
                    {

                        option = '~';

                    }
                    else if (option == 'y')
                    {
                        newcustomer.insert();
                        cout << GREEN << setw(80) << "\nAdd admin successful! Press any key to continue." << RESET;
                        _getch();
                        NewUser = 1;
                        option = '~';
                    }

                }
            }

            break;

        }

        case 'r':
            break;

        default:
            cout << RED << setw(5) << "Error input. Please select again." << RESET;
            option = tolower(_getch());
            break;

        }

        if (option == 'r' || NewUser == 1)
        {
            option = '~';
            break;
        }

    }

}

// Delete A Admin
void deleteAdmin(User& user)
{
    char option = '~';
    string username = "";
    string password = "";

    while (1)
    {
        system("cls");

        cout << CYAN;
        cout << setw(50) << left << "\t==================================================" << RESET << endl;
        cout << BOLD << setw(42) << "\t        Delete Admin Interface        " << RESET << endl;
        cout << CYAN << setw(50) << "\t==================================================" << RESET << endl;
        cout << BOLD << setw(20) << "\t[1] Username   : " << RESET << YELLOW << username << RESET << endl;
        cout << BOLD << setw(20) << "\t[2] Password   : " << RESET << YELLOW << string(password.length(), '*') << RESET << endl << endl;
        cout << YELLOW << BOLD << setw(30) << "\t[C] Confirm" << endl;
        cout << setw(30) << "\t[R] Return" << RESET << endl << endl;
        cout << CYAN;
        cout << setw(50) << "\t==================================================" << RESET << endl;

        cout << BOLD << setw(5) << "Select an option: " << RESET;
        option = tolower(_getch());

        switch (option)
        {
        case '1':
            cout << YELLOW << "\nEnter Username: " << RESET;
            getline(cin, username);
            break;

        case '2':
            password.clear();
            cout << YELLOW << "\nEnter Password: " << RESET;
            char ch;
            while ((ch = _getch()) != '\r') // Enter key is pressed
            {
                if (ch == '\b') // Backspace
                {
                    if (!password.empty())
                    {
                        password.pop_back();
                        cout << "\b \b";
                    }
                }
                else
                {
                    password.push_back(ch);
                    cout << '*';
                }
            }
            cout << endl;
            break;

        case 'c':
        {
            if (username.empty() || password.empty())
            {
                cout << RED << "Error: Username or Password cannot be empty. Press any key to continue." << RESET << endl;
                _getch();
                break;
            }

            // Verify credentials
            User tempUser;
            tempUser.insertUserName(username);
            tempUser.insertUserPassword(password);

            if (!tempUser.login())
            {
                cout << RED << "Error: Invalid credentials. Press any key to continue." << RESET << endl;
                _getch();
                break;
            }

            if (tempUser.fetchUserStatus() != "Admin")
            {
                cout << RED << "Error: User is not an Admin. Press any key to continue." << RESET << endl;
                _getch();
                break;
            }

            // Confirm deletion
            cout << YELLOW << "Are you sure you want to delete this Admin? (Y/N): " << RESET;
            char confirm = tolower(_getch());
            if (confirm == 'y')
            {
                // Delete admin from database
                DBConnection db;
                db.prepareStatement("DELETE FROM user WHERE UserID = ?");
                db.stmt->setString(1, tempUser.fetchUserID());
                db.QueryStatement();

                cout << GREEN << "\nAdmin successfully deleted. Press any key to continue." << RESET << endl;
                _getch();
                return;
            }
            else
            {
                cout << CYAN << "\nDeletion canceled. Press any key to continue." << RESET << endl;
                _getch();
            }
            break;
        }

        case 'r':
            return;

        default:
            cout << RED << "Error: Invalid input. Press any key to try again." << RESET << endl;
            _getch();
            break;
        }
    }
}