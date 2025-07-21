#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Venue {
    int id;
    string name;
    int capacity;
    bool isAvailable;
};

void addVenue(vector<Venue*>& venues) {
    Venue* v = new Venue;
    cout << "Enter Venue ID: ";
    cin >> v->id;
    cin.ignore();
    cout << "Enter Venue Name: ";
    getline(cin, v->name);
    cout << "Enter Capacity: ";
    cin >> v->capacity;
    cout << "Is it available? (1 = Yes, 0 = No): ";
    cin >> v->isAvailable;

    venues.push_back(v);
    cout << "Venue added!\n";
}

void showVenues(vector<Venue*>& venues) {
    if (venues.empty()) {
        cout << "No venues found.\n";
        return;
    }

    cout << "\n--- Venue List ---\n";
    for (auto v : venues) {
        cout << "ID: " << v->id
             << " | Name: " << v->name
             << " | Capacity: " << v->capacity
             << " | Available: " << (v->isAvailable ? "Yes" : "No") << "\n";
    }
}


void updateVenue(vector<Venue*>& venues) {
    int id;
    cout << "Enter Venue ID to update: ";
    cin >> id;

    for (auto v : venues) {
        if (v->id == id) {
            cin.ignore();
            cout << "Enter new name: ";
            getline(cin, v->name);
            cout << "Enter new capacity: ";
            cin >> v->capacity;
            cout << "Is it available? (1 = Yes, 0 = No): ";
            cin >> v->isAvailable;
            cout << "Venue updated!\n";
            return;
        }
    }
    cout << "Venue not found.\n";
}

void deleteVenue(vector<Venue*>& venues) {
    int id;
    cout << "Enter Venue ID to delete: ";
    cin >> id;

    for (int i = 0; i < venues.size(); ++i) {
        if (venues[i]->id == id) {
            delete venues[i]; 
            venues.erase(venues.begin() + i);
            cout << "Venue deleted!\n";
            return;
        }
    }
    cout << "Venue not found.\n";
}

int main() {
    vector<Venue*> venues;
    int choice;

    do {
        cout << "\n--- Venue Manager ---\n";
        cout << "1. Add Venue\n";
        cout << "2. Show Venues\n";
        cout << "3. Update Venue\n";
        cout << "4. Delete Venue\n";
        cout << "5. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        switch (choice) {
            case 1: addVenue(venues); break;
            case 2: showVenues(venues); break;
            case 3: updateVenue(venues); break;
            case 4: deleteVenue(venues); break;
            case 5: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    
    for (auto v : venues) delete v;

    return 0;
}

