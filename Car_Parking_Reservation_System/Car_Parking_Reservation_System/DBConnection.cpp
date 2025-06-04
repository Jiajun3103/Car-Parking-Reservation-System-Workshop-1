#include "DBConnection.h"
#include <conio.h>
#include "DBConnection.h"
#include "DBConnection.h"
#include <conio.h>

using namespace std;
using namespace sql;

DBConnection::DBConnection() {

	try {
		mysql::MySQL_Driver* driver = mysql::get_mysql_driver_instance();

		//connection = driver->connect("tcp://127.0.0.1:3306", "root", "");
		connection = driver->connect("localhost", "root", "abc123");
		//database address, username, password

		connection->setSchema("car_parking_reservation_system"); //database name
		stmt = nullptr;
		res = nullptr;
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() == 0) {
			system("cls");
			cout << "Unable to connect to database";
			_getche();
			exit(0);
			return;
		}
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getche();
	}
}

DBConnection::~DBConnection() {
	try {
		if (connection) {
			connection->close();
		}
		if (stmt) {
			stmt->close();
		}
		if (res) {
			res->close();
		}
	}
	catch (sql::SQLException& e) {
		if (e.getErrorCode() != 0) {
			cout << "# ERR: SQLException in " << __FILE__;
			cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;
			_getche();
		}
	}
}

void DBConnection::prepareStatement(string query) {
	stmt = connection->prepareStatement(query);
}

void DBConnection::QueryStatement() {

	try {
		stmt->executeUpdate();
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getche();
	}
}

void DBConnection::QueryResult() {

	try {
		res = stmt->executeQuery();
	}
	catch (sql::SQLException& e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
		_getche();
	}
}

int DBConnection::getGeneratedId() {
	prepareStatement("SELECT LAST_INSERT_ID();");
	QueryResult();
	int lastInsertId = -1;
	if (res->rowsCount() > 0) {
		while (res->next()) {
			lastInsertId = res->getInt64("LAST_INSERT_ID()");
		}
	}
	return lastInsertId;
}

void DBConnection::setString(int parameterIndex, const std::string& value) {
	if (stmt) {
		stmt->setString(parameterIndex, value);
	}
	else {
		throw std::runtime_error("Prepared statement is not initialized.");
	}
}

int DBConnection::executeUpdate() {
	try {
		if (stmt) {
			return stmt->executeUpdate(); // Executes the SQL statement and returns the number of affected rows
		}
		else {
			throw std::runtime_error("Prepared statement is not initialized.");
		}
	}
	catch (const sql::SQLException& e) {
		std::cerr << "# ERR: SQLException in " << __FILE__;
		std::cerr << "(" << __FUNCTION__ << ") on line " << __LINE__ << std::endl;
		std::cerr << "# ERR: " << e.what();
		std::cerr << " (MySQL error code: " << e.getErrorCode();
		std::cerr << ", SQLState: " << e.getSQLState() << " )" << std::endl;
		return -1; // Indicate failure
	}
}
