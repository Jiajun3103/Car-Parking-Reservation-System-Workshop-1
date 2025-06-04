#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <ctime>

#include "User.h"
#include "DBConnection.h"

using namespace std;

// Constructor & Initialisation
User::User() {
	//initialize
	UserID = "";
	UserPassword = "";
	UserName = "";
	UserPhoneNo = "";
	UserGender = "";
	UserStatus = "";
}
User::User(string UserID)
{
	this->UserID = UserID;
}
User::User(string UserID, string UserPassword, string UserName, string UserPhoneNo, string UserGender, string UserStatus) {
	this->UserID = UserID;
	this->UserPassword = UserPassword;
	this->UserName = UserName;
	this->UserPhoneNo = UserPhoneNo;
	this->UserGender = UserGender;
	this->UserStatus = UserStatus;
}

// User Login with authentication to database
bool User::login() {

	DBConnection db;
	db.prepareStatement("SELECT * FROM user WHERE UserName=? AND UserPassword=?");
	db.stmt->setString(1, UserName);
	db.stmt->setString(2, UserPassword);
	db.QueryResult();
	if (db.res->rowsCount() == 1) {
		while (db.res->next()) {
			UserID = db.res->getString("UserID");
			UserPassword = db.res->getString("UserPassword");
			UserName = db.res->getString("UserName");
			UserPhoneNo = db.res->getString("UserPhoneNo");
			UserGender = db.res->getString("UserGender");
			UserStatus = db.res->getString("UserStatus");
		}
		db.~DBConnection();
		return true;
	}
	else {

		db.~DBConnection();
		return false;
	}
}
// Create new user in database
void User::insert() {

	DBConnection db;//instantiate

	db.prepareStatement("Insert into user (UserID,UserPassword,UserName,UserPhoneNo,UserGender,UserStatus) VALUES (?,?,?,?,?,?)");
	db.stmt->setString(1, UserID);
	db.stmt->setString(2, UserPassword);
	db.stmt->setString(3, UserName);
	db.stmt->setString(4, UserPhoneNo);
	db.stmt->setString(5, UserGender);
	db.stmt->setString(6, UserStatus);

	db.QueryStatement();
	db.~DBConnection();
}

// Check if UserID already exist in database
bool User::newUser() {
	DBConnection db;
	db.prepareStatement("SELECT UserID FROM user WHERE UserID=?");
	db.stmt->setString(1, UserID);
	db.QueryResult();

	if (db.res->rowsCount() > 0) {
		// UserID already exists in the database
		db.~DBConnection();

		return false;
	}
	else {
		// UserID does not exist, proceed with registration
		db.~DBConnection();

		return true;
	}
}
// Update and edit user information
void User::update()
{
	// Instantiate DBConnection
	DBConnection db;

	// Prepare the SQL update statement
	db.prepareStatement("UPDATE user SET UserPassword = ?, UserName = ?, UserPhoneNo = ?, UserGender = ?, UserStatus = ? WHERE UserID = ?");

	// Set the parameters for the SQL update statement
	db.stmt->setString(1, UserPassword);
	db.stmt->setString(2, UserName);
	db.stmt->setString(3, UserPhoneNo);
	db.stmt->setString(4, UserGender);
	db.stmt->setString(5, UserStatus);
	db.stmt->setString(6, UserID);

	// Execute the query
	db.QueryStatement();

	// Destruct the DBConnection object to clean up resources
	db.~DBConnection();
}

// Generate new user id
string User::generateNewUserID() {
	DBConnection db;
	std::string latestUserID = "U0000000"; // Default value if no records exist
	std::string newUserID;

	try {
		// Prepare SQL statement to fetch the latest UserID
		db.prepareStatement("SELECT UserID FROM user ORDER BY UserID DESC LIMIT 1;");
		db.QueryResult();

		if (db.res && db.res->next()) {
			latestUserID = db.res->getString("UserID");
		}

		// Extract numeric part, increment, and format the new UserID
		int numericPart = std::stoi(latestUserID.substr(1));
		numericPart++;
		std::ostringstream oss;
		oss << "U" << std::setw(7) << std::setfill('0') << numericPart;
		newUserID = oss.str();
		db.~DBConnection();
	}
	catch (const sql::SQLException& e) {
		std::cerr << "SQL Error: " << e.what() << " (Code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << ")" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return newUserID;
}

// Insert attributes
void User::insertUserID(string UserID)
{
	this->UserID = UserID;
}
void User::insertUserPassword(string UserPassword)
{
	this->UserPassword = UserPassword;
}
void User::insertUserName(string UserName)
{
	this->UserName = UserName;
}
void User::insertUserPhoneNo(string UserPhoneNo)
{
	this->UserPhoneNo = UserPhoneNo;
}
void User::insertUserGender(string UserGender)
{
	this->UserGender = UserGender;
}
void User::insertUserStatus(string UserStatus)
{
	this->UserStatus = UserStatus;
}

// Return attributes
string User::fetchUserID()
{
	return UserID;
}
string User::fetchUserPassword()
{
	return UserPassword;
}
string User::fetchUserName()
{
	return UserName;
}
string User::fetchUserPhoneNo()
{
	return UserPhoneNo;
}
string User::fetchUserGender()
{
	return UserGender;
}
string User::fetchUserStatus()
{
	return UserStatus;
}

// Destructor
User::~User() {

}