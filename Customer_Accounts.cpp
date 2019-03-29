// Customer_Accounts.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// String length constants
const int NAME_SIZE = 21;
const int ADDR_SIZE = 31;
const int CITY_SIZE = 21;
const int STATE_SIZE = 3;
const int ZIP_SIZE = 6;
const int PHONE_SIZE = 13;
const int DATE_SIZE = 11;

// Customer data structure
struct Customer
{
	char name[NAME_SIZE];
	char address[ADDR_SIZE];
	char city[CITY_SIZE];
	char state[STATE_SIZE];				// TX
	char zip[ZIP_SIZE];					// 12345
	char phone[PHONE_SIZE];				// 123-456-7890
	double balance;
	char dateLastPayment[DATE_SIZE];	// MM-DD-YYYY
};

// Function prototypes
void getUserInput(Customer*);
void newRecords(Customer);
void displayRecord(char[NAME_SIZE]);
void deleteRecord(char[NAME_SIZE]);
void changeRecord(char[NAME_SIZE]);
void displayAllRecords();

int main()
{
	int userChoice;
	while (true)
	{
		// User menu
		cout << "1 - New Record\n";
		cout << "2 - Display Record\n";
		cout << "3 - Delete Record\n";
		cout << "4 - Change Record\n";
		cout << "5 - Display All Records\n";
		cout << "6 - Quit\n";

		// Take user input
		cin >> userChoice;
		cin.ignore();

		// Quit if 6
		if (userChoice == 6)
			break;
		
		// Perform action
		switch (userChoice)
		{
			// Enter new record
			case 1:
				Customer newCust;
				getUserInput(&newCust);
				newRecords(newCust);
				break;
			// Display record
			case 2: 
				char displayName[NAME_SIZE];
				cout << "Customer Name: ";
				cin.getline(displayName, NAME_SIZE);
				displayRecord(displayName);
				break;
			// Delete record
			case 3:
				char deleteName[NAME_SIZE];
				cout << "Customer Name: ";
				cin.getline(deleteName, NAME_SIZE);
				deleteRecord(deleteName);
				break;
			// Change record
			case 4:
				char changeName[NAME_SIZE];
				cout << "Customer Name: ";
				cin.getline(changeName, NAME_SIZE);
				changeRecord(changeName);
				break;
			// Display all records
			case 5:
				displayAllRecords();
				break;
		}
	}
	return 0;
}

// Gets user input for a record
void getUserInput(Customer *cust)
{
	// Ask user for field values
	cout << "Name: ";
	cin.getline(cust->name, NAME_SIZE);
	cout << "Street Address: ";
	cin.getline(cust->address, ADDR_SIZE);
	cout << "City: ";
	cin.getline(cust->city, CITY_SIZE);
	cout << "State (ex. TX): ";
	cin.getline(cust->state, STATE_SIZE);
	cout << "Zip: ";
	cin.getline(cust->zip, ZIP_SIZE);
	cout << "Phone: ";
	cin.getline(cust->phone, PHONE_SIZE);

	// Loop to require positive balance
	while (true)
	{
		cout << "Balance: $";
		cin >> cust->balance;
		cin.ignore();
		if (cust->balance >= 0)
			break;
	}
	cout << "Last Payment Date: ";
	cin.getline(cust->dateLastPayment, DATE_SIZE);
}

// Enter new records into the file
void newRecords(Customer cust)
{
	// Open file for binary writing
	fstream outFile;
	outFile.open("Customer.dat", ios::out | ios::app | ios::binary);
	if (outFile.fail())
	{
		cout << "ERROR: File not found.\n";
		return;
	}
	outFile.write(reinterpret_cast<char *>(&cust), sizeof(cust));
	outFile.close();
}

// Searches for a particular customer's record and displays it
void displayRecord(char custName[NAME_SIZE])
{
	// Open file for binary reading
	fstream inFile;
	inFile.open("Customer.dat", ios::in | ios::binary);
	if (inFile.fail())
	{
		cout << "ERROR: File not found.\n";
		return;
	}

	// Loop through each record until the name matches
	Customer cust;
	bool found = false;
	while (!inFile.eof() && !found)
	{
		inFile.read(reinterpret_cast<char *>(&cust), sizeof(cust));
		if (!strcmp(cust.name, custName))	// If match strcmp = 0
		{
			// Display record values
			cout << "Name: ";
			cout << cust.name << endl;
			cout << "Street Address: ";
			cout << cust.address << endl;
			cout << "City: ";
			cout << cust.city << endl;
			cout << "State (ex. TX): ";
			cout << cust.state << endl;
			cout << "Zip: ";
			cout << cust.zip << endl;
			cout << "Phone: ";
			cout << cust.phone << endl;
			cout << "Balance: $";
			cout << cust.balance << endl;
			cout << "Last payment date: ";
			cout << cust.dateLastPayment << endl;
			found = true;
		}
	}
	// Tell user if record not found
	if (!found)
		cout << "Record not found.";

	// Close file
	inFile.close();
}

// Displays all fields for a customer
void deleteRecord(char custName[NAME_SIZE])
{
	// Open file for binary reading and writing
	fstream inFile, outFile, temp;
	inFile.open("Customer.dat", ios::in | ios::binary);
	temp.open("Temporary.dat", ios::out | ios::binary);
	if (inFile.fail() || temp.fail())
	{
		cout << "ERROR: File not found.\n";
		return;
	}

	// Loop through each record until the name matches
	Customer cust;
	while (!inFile.eof())
	{
		inFile.read(reinterpret_cast<char *>(&cust), sizeof(cust));
		// If the record is not a match, add it to temp
		if (strcmp(cust.name, custName))	// If match strcmp = 0
			temp.write(reinterpret_cast<char *>(&cust), sizeof(cust));
	}

	// Close both files and reopen with opposite in and out
	inFile.close();
	temp.close();
	outFile.open("Customer.dat", ios::out | ios::binary | ios::);
	temp.open("Temporary.dat", ios::in | ios::binary);
	if (outFile.fail() || temp.fail())
	{
		cout << "ERROR: File not found.\n";
		return;
	}

	// Write all of temp to outFile
	while (!temp.eof())
	{
		temp.read(reinterpret_cast<char *>(&cust), sizeof(cust));
		outFile.write(reinterpret_cast<char *>(&cust), sizeof(cust));
	}

	// Close both files
	outFile.close();
	temp.close();
}

// Searches for a customer's record and changes it
void changeRecord(char custName[NAME_SIZE])
{
	// Open inFile and temp
	fstream inFile, outFile, temp;
	inFile.open("Customer.dat", ios::in | ios::binary);
	temp.open("Temporary.dat", ios::out | ios::binary);
	if (inFile.fail() | temp.fail())
	{
		cout << "ERROR: File not found.\n";
		return;
	}

	// Write inFile to temp, except for desired customer
	// change data before writing cust
	Customer cust;
	bool found = false;
	while (!inFile.eof() && !found)
	{
		inFile.read(reinterpret_cast<char*>(&cust), sizeof(cust));
		if (!strcmp(cust.name, custName))	// Returns 0 when equal
		{
			// Ask user for new customer data
			cout << "Enter changes\n";
			cout << "Name: ";
			cin.getline(cust.name, NAME_SIZE);
			cout << "Street Address: ";
			cin.getline(cust.address, ADDR_SIZE);
			cout << "City: ";
			cin.getline(cust.city, CITY_SIZE);
			cout << "State (ex. TX): ";
			cin.getline(cust.state, STATE_SIZE);
			cout << "Zip: ";
			cin.getline(cust.zip, ZIP_SIZE);
			cout << "Phone: ";
			cin.getline(cust.phone, PHONE_SIZE);
			found = true;
		}
		// Write cust to temp
		temp.write(reinterpret_cast<char*>(&cust), sizeof(cust));
	}
	// Tell user if record not found
	if (!found)
	{
		cout << "Record not found.\n";
		return;
	}
	// Close both files and reopen outFile and temp
	inFile.close();
	temp.close();
	outFile.open("Customer.dat", ios::out | ios::binary);
	temp.open("Temporary.dat", ios::in | ios::binary);

	// Write all of temp to outFile
	while (!temp.eof())
	{
		temp.read(reinterpret_cast<char*>(&cust), sizeof(cust));
		outFile.write(reinterpret_cast<char*>(&cust), sizeof(cust));
	}

	// Close both files
	temp.close();
	outFile.close();
}

// Displays all records
void displayAllRecords()
{
	fstream inFile;
	inFile.open("Customer.dat", ios::in | ios::binary);
	// Loop through all records and display each
	Customer cust;
	while (!inFile.eof())
	{
		// Read record to cust
		inFile.read(reinterpret_cast<char*>(&cust), sizeof(cust));
		
		// Display record values
		cout << "Name: ";
		cout << cust.name << endl;
		cout << "Street Address: ";
		cout << cust.address << endl;
		cout << "City: ";
		cout << cust.city << endl;
		cout << "State (ex. TX): ";
		cout << cust.state << endl;
		cout << "Zip: ";
		cout << cust.zip << endl;
		cout << "Phone: ";
		cout << cust.phone << endl;
		cout << "Balance: $";
		cout << cust.balance << endl;
		cout << "Last payment date: ";
		cout << cust.dateLastPayment << endl;
		cout << endl;
	}
	// Close file
	inFile.close();
}