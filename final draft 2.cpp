#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

struct Record {
    char name[100];
    char sc_no[10];
    char consumer_id[10];
    char counter[100];
    float unit;
    double amount;
};

vector<Record> records;

// Function to calculate the amount based on the unit
double calculateAmount(float unit) {
    if (unit <= 50)
        return 5.0 * unit;
    else if (unit <= 100)
        return 25 + 11.0 * (unit - 50);
    else if (unit <= 200)
        return 100 + 11.5 * (unit - 100);
    else
        return 250 + 13.0 * (unit - 200);
}

// Function to check if the SC number is unique
bool isUniqueSCNo(const char* sc_no) {
    for (const auto& record : records) {
        if (strcmp(record.sc_no, sc_no) == 0) {
            return false;
        }
    }
    return true;
}

// Function to add a record
void addRecord() {
    Record newRecord;

    cout << "Enter Full Name: ";
    cin.ignore();
    cin.getline(newRecord.name, 100);

    while (true) {
        cout << "Enter Service Connection Number (SC No.): ";
        cin.getline(newRecord.sc_no, 10);
        if (isUniqueSCNo(newRecord.sc_no)) {
            break;
        } else {
            cout << "SC No. already exists. Please enter a unique SC No." << endl;
        }
    }

    cout << "Enter Customer ID: ";
    cin.getline(newRecord.consumer_id, 10);

    cout << "Enter Counter: ";
    cin.getline(newRecord.counter, 100);

    cout << "Enter Unit: ";
    cin >> newRecord.unit;

    newRecord.amount = calculateAmount(newRecord.unit);
    records.push_back(newRecord);

    ofstream outFile("records.txt", ios::app);
    if (outFile.is_open()) {
        outFile << newRecord.name << ","
                << newRecord.sc_no << ","
                << newRecord.consumer_id << ","
                << newRecord.counter << ","
                << newRecord.unit << ","
                << newRecord.amount << "\n";
        outFile.close();
    }
}

// Function to search a record by SC No.
void searchRecord() {
    char sc_no[10];
    cout << "Enter Service Connection Number (SC No.) to search: ";
    cin.ignore();
    cin.getline(sc_no, 10);

    for (const auto& record : records) {
        if (strcmp(record.sc_no, sc_no) == 0) {
            cout << "Record Found:\n";
            cout << "Name: " << record.name << "\n";
            cout << "SC No.: " << record.sc_no << "\n";
            cout << "Consumer ID: " << record.consumer_id << "\n";
            cout << "Counter: " << record.counter << "\n";
            cout << "Unit: " << record.unit << "\n";
            cout << "Amount: " << record.amount << "\n";
            return;
        }
    }
    cout << "Record not found.\n";
}

// Function to edit a record
void editRecord() {
    char sc_no[10];
    cout << "Enter Service Connection Number (SC No.) to edit: ";
    cin.ignore();
    cin.getline(sc_no, 10);

    for (auto& record : records) {
        if (strcmp(record.sc_no, sc_no) == 0) {
            int choice;
            cout << "Record Found. What do you want to update?\n";
            cout << "1. Name\n";
            cout << "2. Counter\n";
            cout << "3. Unit\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1:
                    cout << "Enter new Name: ";
                    cin.getline(record.name, 100);
                    break;
                case 2:
                    cout << "Enter new Counter: ";
                    cin.getline(record.counter, 100);
                    break;
                case 3:
                    cout << "Enter new Unit: ";
                    cin >> record.unit;
                    record.amount = calculateAmount(record.unit);
                    break;
                default:
                    cout << "Invalid choice.\n";
                    return;
            }
            cout << "Record updated successfully.\n";
            return;
        }
    }
    cout << "Record not found. Do you want to add new data instead? (y/n): ";
    char addNew;
    cin >> addNew;
    if (addNew == 'y' || addNew == 'Y') {
        addRecord();
    }
}

// Function to delete a record
void deleteRecord() {
    char sc_no[10];
    cout << "Enter Service Connection Number (SC No.) to delete: ";
    cin.ignore();
    cin.getline(sc_no, 10);

    for (auto it = records.begin(); it != records.end(); ++it) {
        if (strcmp(it->sc_no, sc_no) == 0) {
            records.erase(it);
            cout << "Record deleted successfully.\n";
            return;
        }
    }
    cout << "Record not found.\n";
}

// Function to display all records
void displayRecords() {
    cout << left << setw(20) << "Name"
         << setw(15) << "SC No."
         << setw(15) << "Consumer ID"
         << setw(20) << "Counter"
         << setw(10) << "Unit"
         << setw(10) << "Amount" << "\n";

    for (const auto& record : records) {
        cout << left << setw(20) << record.name
             << setw(15) << record.sc_no
             << setw(15) << record.consumer_id
             << setw(20) << record.counter
             << setw(10) << record.unit
             << setw(10) << record.amount << "\n";
    }
}

// Function to load records from file
void loadRecords() {
    ifstream inFile("records.txt");
    if (inFile.is_open()) {
        Record record;
        while (inFile.getline(record.name, 100, ',') &&
               inFile.getline(record.sc_no, 10, ',') &&
               inFile.getline(record.consumer_id, 10, ',') &&
               inFile.getline(record.counter, 100, ',') &&
               inFile >> record.unit &&
               inFile.get() &&
               inFile >> record.amount &&
               inFile.get()) {
            records.push_back(record);
        }
        inFile.close();
    }
}

// Main function
int main() {
    loadRecords();
    int choice;

    do {
        cout << "1. Add Record\n";
        cout << "2. Search Record\n";
        cout << "3. Edit Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Display All Records\n";
        cout << "6. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addRecord();
                break;
            case 2:
                searchRecord();
                break;
            case 3:
                editRecord();
                break;
            case 4:
                deleteRecord();
                break;
            case 5:
                displayRecords();
                break;
            case 6:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
