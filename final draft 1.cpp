#include <iostream>
#include <fstream>
#include <iomanip>
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

void calculateAmount(Record &rec) {
    if (rec.unit <= 50)
        rec.amount = 5.0 * rec.unit;
    else if (rec.unit <= 100)
        rec.amount = 25 + 11.0 * (rec.unit - 50);
    else if (rec.unit <= 200)
        rec.amount = 100 + 11.5 * (rec.unit - 100);
    else
        rec.amount = 250 + 13.0 * (rec.unit - 200);
}

void addRecord() {
    Record rec;
    cout << "Enter full name: ";
    cin.ignore();
    cin.getline(rec.name, 100);
    while (true) {
        cout << "Enter service connection number (sc_no): ";
        cin.getline(rec.sc_no, 10);
        ifstream infile("records.txt");
        bool exists = false;
        Record temp;
        while (infile.read((char*)&temp, sizeof(temp))) {
            if (strcmp(temp.sc_no, rec.sc_no) == 0) {
                exists = true;
                break;
            }
        }
        infile.close();
        if (exists) {
            cout << "Service connection number already exists. Please enter a unique number.\n";
        } else {
            break;
        }
    }
    cout << "Enter consumer ID: ";
    cin.getline(rec.consumer_id, 10);
    cout << "Enter counter: ";
    cin.getline(rec.counter, 100);
    cout << "Enter unit: ";
    cin >> rec.unit;
    calculateAmount(rec);

    ofstream outfile("records.txt", ios::app);
    outfile.write((char*)&rec, sizeof(rec));
    outfile.close();
    cout << "Record added successfully.\n";
}

void searchRecord() {
    char sc_no[10];
    cout << "Enter service connection number to search: ";
    cin.ignore();
    cin.getline(sc_no, 10);
    ifstream infile("records.txt");
    Record rec;
    bool found = false;
    while (infile.read((char*)&rec, sizeof(rec))) {
        if (strcmp(rec.sc_no, sc_no) == 0) {
            found = true;
            cout << "Name: " << rec.name << "\n";
            cout << "Service Connection Number: " << rec.sc_no << "\n";
            cout << "Consumer ID: " << rec.consumer_id << "\n";
            cout << "Counter: " << rec.counter << "\n";
            cout << "Unit: " << rec.unit << "\n";
            cout << "Amount: " << rec.amount << "\n";
            break;
        }
    }
    infile.close();
    if (!found) {
        cout << "Record not found.\n";
    }
}

void editRecord() {
    char sc_no[10];
    cout << "Enter service connection number to edit: ";
    cin.ignore();
    cin.getline(sc_no, 10);
    fstream file("records.txt", ios::in | ios::out | ios::binary);
    Record rec;
    bool found = false;
    while (file.read((char*)&rec, sizeof(rec))) {
        if (strcmp(rec.sc_no, sc_no) == 0) {
            found = true;
            cout << "What do you want to update?\n";
            cout << "1. Name\n";
            cout << "2. Counter\n";
            cout << "3. Unit\n";
            int choice;
            cin >> choice;
            cin.ignore();
            switch (choice) {
                case 1:
                    cout << "Enter new name: ";
                    cin.getline(rec.name, 100);
                    break;
                case 2:
                    cout << "Enter new counter: ";
                    cin.getline(rec.counter, 100);
                    break;
                case 3:
                    cout << "Enter new unit: ";
                    cin >> rec.unit;
                    calculateAmount(rec);
                    break;
                default:
                    cout << "Invalid choice.\n";
                    return;
            }
            file.seekp(-sizeof(rec), ios::cur);
            file.write((char*)&rec, sizeof(rec));
            cout << "Record updated successfully.\n";
            break;
        }
    }
    file.close();
    if (!found) {
        cout << "Record not found.\n";
    }
}

void deleteRecord() {
    char sc_no[10];
    cout << "Enter service connection number to delete: ";
    cin.ignore();
    cin.getline(sc_no, 10);
    ifstream infile("records.txt");
    ofstream tempFile("temp.txt");
    Record rec;
    bool found = false;
    while (infile.read((char*)&rec, sizeof(rec))) {
        if (strcmp(rec.sc_no, sc_no) != 0) {
            tempFile.write((char*)&rec, sizeof(rec));
        } else {
            found = true;
        }
    }
    infile.close();
    tempFile.close();
    remove("records.txt");
    rename("temp.txt", "records.txt");
    if (found) {
        cout << "Record deleted successfully.\n";
    } else {
        cout << "Record not found.\n";
    }
}

void displayRecords() {
    ifstream infile("records.txt");
    Record rec;
    cout << left << setw(20) << "Name" << setw(15) << "SC No" << setw(15) << "Consumer ID" << setw(20) << "Counter" << setw(10) << "Unit" << setw(10) << "Amount" << "\n";
    cout << "------------------------------------------------------------------------------------------\n";
    while (infile.read((char*)&rec, sizeof(rec))) {
        cout << left << setw(20) << rec.name << setw(15) << rec.sc_no << setw(15) << rec.consumer_id << setw(20) << rec.counter << setw(10) << rec.unit << setw(10) << rec.amount << "\n";
    }
    infile.close();
}

int main() {
    int choice;
    while (true) {
        cout << "\nMenu:\n";
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
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
