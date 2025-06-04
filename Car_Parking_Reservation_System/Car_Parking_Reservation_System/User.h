#pragma once
#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User
{
private:
	// Member variables
	string UserID, UserPassword, UserName, UserPhoneNo, UserGender, UserStatus;

public:
	User(); // Default Constructor
	User(string UserID); // Constructor with UserID parameter
	User(string UserID, string UserPassword, string UserName, string UserPhoneNo, string UserGender, string UserStatus); // Constructor with all user parameter

	bool login(); // User Login with authentication to database
	void insert(); // Create new user in database
	bool newUser(); // Check if UserID already exist in database
	void update(); // Update and edit user information
	string generateNewUserID();

	// Insert attributes
	void insertUserID(string UserID);
	void insertUserPassword(string UserPassword);
	void insertUserName(string UserName);
	void insertUserPhoneNo(string UserPhoneNo);
	void insertUserGender(string UserGender);
	void insertUserStatus(string UserStatus);

	// Return attributes
	string fetchUserID();
	string fetchUserPassword();
	string fetchUserName();
	string fetchUserPhoneNo();
	string fetchUserGender();
	string fetchUserStatus();

	~User(); // Destructor

};

#endif