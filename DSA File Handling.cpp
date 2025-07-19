#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Venue {
public:
    string name;
    string location;
    double price;

    Venue(string n, string loc, double p) : name(n), location(loc), price(p) {}

    string toString() const {
        return name + "," + location + "," + formatPrice(price);
    }

private:
    string formatPrice(double price) const {
        // Function to convert double to string
        string result;
        bool isNegative = price < 0;
        if (isNegative) {
            price = -price; // Make price positive for processing
        }

        // Convert integer part
        int integerPart = static_cast<int>(price);
        double fractionalPart = price - integerPart;

        // Convert integer part to string
        do {
            result = char((integerPart % 10) + '0') + result;
            integerPart /= 10;
        } while (integerPart > 0);

        if (isNegative) {
            result = "-" + result;
        }

        // Convert fractional part to string
        if (fractionalPart > 0) {
            result += ".";
            fractionalPart *= 10; // Move to the first decimal place
            while (fractionalPart > 0) {
                int digit = static_cast<int>(fractionalPart);
                result += char(digit + '0');
                fractionalPart -= digit;
                fractionalPart *= 10; // Move to the next decimal place
            }
        }

        return result;
    }
};

class Customer {
public:
    string name;
    string contact;

    Customer(string n, string c) : name(n), contact(c) {}

    string toString() const {
        return name + "," + contact;
    }
};

class Booking {
public:
    string customerName;
    string venueName;
    string date;

    Booking(string cName, string vName, string d) 
        : customerName(cName), venueName(vName), date(d) {}

    string toString() const {
        return customerName + "," + venueName + "," + date;
    }
};

class VenueRentalSystem {
public:
    void saveVenue(const Venue& venue) {
        ofstream outFile("Venues.txt", ios::app);
        if (outFile.is_open()) {
            outFile << venue.toString() << "\n";
            outFile.close();
        }
    }

    void loadVenues() {
        ifstream inFile("Venues.txt");
        if (inFile.is_open()) {
            string line;
            cout << "\nLoaded Venues:\n";
            while (getline(inFile, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1 + 1);
                string name = line.substr(0, pos1);
                string location = line.substr(pos1 + 1, pos2 - pos1 - 1);
                double price = parsePrice(line.substr(pos2 + 1));
                cout << "Name: " << name << ", Location: " 
                     << location << ", Price: " << price << endl;
            }
            inFile.close();
        }
    }

    void saveCustomer(const Customer& customer) {
        ofstream outFile("Customers.txt", ios::app);
        if (outFile.is_open()) {
            outFile << customer.toString() << "\n";
            outFile.close();
        }
    }

    void loadCustomers() {
        ifstream inFile("Customers.txt");
        if (inFile.is_open()) {
            string line;
            cout << "\nLoaded Customers:\n";
            while (getline(inFile, line)) {
                size_t pos = line.find(',');
                string name = line.substr(0, pos);
                string contact = line.substr(pos + 1);
                cout << "Name: " << name << ", Contact: " << contact << endl;
            }
            inFile.close();
        }
    }

    void saveBooking(const Booking& booking) {
        ofstream outFile("Bookings.txt", ios::app);
        if (outFile.is_open()) {
            outFile << booking.toString() << "\n";
            outFile.close();
        }
    }

    void loadBookings() {
        ifstream inFile("Bookings.txt");
        if (inFile.is_open()) {
            string line;
            cout << "\nLoaded Bookings:\n";
            while (getline(inFile, line)) {
                size_t pos1 = line.find(',');
                size_t pos2 = line.find(',', pos1 + 1);
                string customerName = line.substr(0, pos1);
                string venueName = line.substr(pos1 + 1, pos2 - pos1 - 1);
                string date = line.substr(pos2 + 1);
                cout << "Customer: " << customerName << ", Venue: " 
                     << venueName << ", Date: " << date << endl;
            }
            inFile.close();
        }
    }

private:
    double parsePrice(const string& priceStr) const {
        // Function to convert string to double
        double result = 0.0;
        bool isNegative = false;
        size_t i = 0;

        // Check for negative sign
        if (priceStr[i] == '-') {
            isNegative = true;
            i++;
        }

        // Process integer part
        while (i < priceStr.length() && priceStr[i] != '.') {
            result = result * 10 + (priceStr[i] - '0');
            i++;
        }

        // Process fractional part
        if (i < priceStr.length() && priceStr[i] == '.') {
            i++;
            double decimalPlace = 0.1;
            while (i < priceStr.length()) {
                result += (priceStr[i] - '0') * decimalPlace;
                decimalPlace *= 0.1;
                i++;
            }
        }

        return isNegative ? -result : result;
    }
};

int main() {
    VenueRentalSystem system;

    // Example usage
    Venue venue("Conference Hall 1", "Conference Hall 2", 500.0);
    Customer customer("Test Man", "123-456-7890");
    Booking booking("Test Man", "Conference Hall", "2025-7-16");

    // Save data
    system.saveVenue(venue);
    system.saveCustomer(customer);
    system.saveBooking(booking);

    // Load data
    system.loadVenues();
    system.loadCustomers();
    system.loadBookings();

    return 0;
}

