//heeloo gois kayo na bahala

#include <iostream>
#include <vector>
using namespace std;

struct Venue {
    string name;
    double pricePerHour;
};

struct Customer {
    string name;
    string id;  
};

struct Booking {
    Customer customer;
    Venue venue;
    string date;
    int duration;
    Booking *next;
};


Booking *bookingHead = nullptr;


vector<Venue> venues = {
    {"FEU TECH Student Plaza", 1000.0},
    {"FEU TECH Multipurpose Room", 500.0},
    {"FEU Gym", 300.0},
    {"FEU Tayuman", 800.0},
    {"FEU Auditorium", 3500.0},
    {"FEU TECH Rooftop", 2200.0},
    {"FEU Library Hall", 1800.0},
    {"FEU TECH Pool", 4500.0}
};

vector<Customer> customers = {
    {"Kurt", "202400001"},
    {"Ash", "202400002"},
    {"Ryu", "202400003"},
    {"Moraleda", "202400004"},
    {"Machica", "202400005"}
};


Customer *findCustomerByName(const string& name) {
    for (auto& cust : customers) {
        if (cust.name == name) {
            return &cust;
        }
    }
    return nullptr;
}


Venue *findVenueByName(const string& name) {
    for (auto& v : venues) {
        if (v.name == name) {
            return &v;
        }
    }
    return nullptr;
}


bool isVenueAvailable(const string& venueName, const string& date) {
    Booking *curr = bookingHead;
    while (curr != nullptr) {
        if (curr->venue.name == venueName && curr->date == date) {
            return false;
        }
        curr = curr->next;
    }
    return true;
}


void makeBooking() {
    string custName, venueName, date;
    int duration;

    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, custName);

    
    Customer *customer = nullptr;
    for (auto& c : customers) {
        if (c.name == custName) {
            customer = &c;
            break;
        }
    }

    
    if (!customer) {
        cout << "Customer not found. Would you like to register this name? (y/n): ";
        char option;
        cin >> option;
        cin.ignore();
        if (tolower(option) == 'y') {
            string id;
            cout << "Enter a unique customer ID for " << custName << ": ";
            getline(cin, id);
            customers.push_back({custName, id});
            customer = &customers.back();
            cout << "Customer registered successfully.\n";
        } else {
            cout << "Booking cancelled.\n";
            return;
        }
    }

    cout << "Enter Venue Name: ";
    getline(cin, venueName);
    Venue *venue = findVenueByName(venueName);
    if (!venue) {
        cout << "Venue not found!\n";
        return;
    }

    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> date;

    if (!isVenueAvailable(venueName, date)) {
        cout << "Venue is already booked on this date!\n";
        return;
    }

    cout << "Enter Duration (in hours): ";
    cin >> duration;

    Booking *newBooking = new Booking{*customer, *venue, date, duration, nullptr};

    if (bookingHead == nullptr) {
        bookingHead = newBooking;
    } else {
        Booking *temp = bookingHead;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newBooking;
    }

    cout << "Booking successful!\n";
}


void cancelBooking() {
    string custName, venueName, date;

    cout << "Enter Customer Name: ";
    cin.ignore();
    getline(cin, custName);
    cout << "Enter Venue Name: ";
    getline(cin, venueName);
    cout << "Enter Date (YYYY-MM-DD): ";
    cin >> date;

    Booking *curr = bookingHead;
    Booking *prev = nullptr;

    while (curr != nullptr) {
        if (curr->customer.name == custName && curr->venue.name == venueName && curr->date == date) {
            if (prev == nullptr) {
                bookingHead = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            cout << "Booking cancelled.\n";
            return;
        }
        prev = curr;
        curr = curr->next;
    }

    cout << "Booking not found.\n";
}


void generateReport() {
    Booking *curr = bookingHead;
    double totalRevenue = 0;
    int count = 0;

    if (!curr) {
        cout << "No bookings found.\n";
        return;
    }

    cout << "\n===== Booking Report =====\n";
    while (curr != nullptr) {
        cout << "Customer: " << curr->customer.name
             << " \nVenue: " << curr->venue.name
             << " \nDate: " << curr->date
             << " \nTime Duration: " << curr->duration
             << " \nTotal: ?" << (curr->venue.pricePerHour * curr->duration) << "\n\n";

        totalRevenue += curr->venue.pricePerHour * curr->duration;
        count++;
        curr = curr->next;
    }

    cout << "----------------------\n";
    cout << "Total Bookings: " << count << "\n";
    cout << "Total Revenue: ?" << totalRevenue << "\n\n";
}


void mainMenu() {
    int choice;
    do {
        cout << "\n===== Booking System =====\n";
        cout << "1. Make a Booking\n";
        cout << "2. Cancel a Booking\n";
        cout << "3. View Booking Report\n";
        cout << "4. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: makeBooking(); break;
            case 2: cancelBooking(); break;
            case 3: generateReport(); break;
            case 4: cout << "Exiting Booking System\n"; break;
            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 4);
}

int main() {
    mainMenu();
}
