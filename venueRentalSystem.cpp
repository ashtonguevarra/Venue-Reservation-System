#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

int inputInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) break;
        else {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore();
    return value;
}

double inputDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) break;
        else {
            cout << "Invalid input. Please enter a valid price.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    cin.ignore();
    return value;
}

string inputAlpha(const string& prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        bool valid = true;
        for (size_t i = 0; i < value.length(); ++i) {
            if (!isalpha(value[i]) && !isspace(value[i])) {
                valid = false;
                break;
            }
        }
        if (valid && !value.empty()) break;
        cout << "Invalid input. Please enter only letters.\n";
    }
    return value;
}

string inputString(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// ---------------------- Venue Structure ----------------------
struct Venue {
    int id;
    string name;
    int capacity;
    bool isAvailable;
    string location;
    double price;
};

class VenueData {
public:
    string name;
    string location;
    double price;

    VenueData(string n, string loc, double p) : name(n), location(loc), price(p) {}

    string toString() const {
        return name + "," + location + "," + formatPrice(price);
    }

private:
    string formatPrice(double price) const {
        string result;
        bool isNegative = price < 0;
        if (isNegative) price = -price;

        int integerPart = static_cast<int>(price);
        double fractionalPart = price - integerPart;

        do {
            result = char((integerPart % 10) + '0') + result;
            integerPart /= 10;
        } while (integerPart > 0);

        if (isNegative) result = "-" + result;

        if (fractionalPart > 0) {
            result += ".";
            fractionalPart *= 10;
            int count = 0;
            while (fractionalPart > 0.001 && count < 5) {
                int digit = static_cast<int>(fractionalPart);
                result += char(digit + '0');
                fractionalPart -= digit;
                fractionalPart *= 10;
                count++;
            }
        }
        return result;
    }
};

// ---------------------- Booking Struct ----------------------
struct BookingNode {
    string customerName;
    string venueName;
    string date;
    int duration;
    double totalCost;
    BookingNode* next;
};

BookingNode* bookingHead = nullptr;

// ---------------------- Booking File ----------------------
class Booking {
public:
    string customerName;
    string venueName;
    string date;

    Booking(string cName, string vName, string d)
        : customerName(cName), venueName(vName), date(d) {
    }

    string toString() const {
        return customerName + "," + venueName + "," + date;
    }
};

class VenueRentalSystem {
public:
    void saveVenue(const VenueData& venue) {
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
                cout << "Name: " << name << ", Location: " << location << ", Price: " << price << endl;
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
                cout << "Customer: " << customerName << ", Venue: " << venueName << ", Date: " << date << endl;
            }
            inFile.close();
        }
    }

private:
    double parsePrice(const string& priceStr) const {
        double result = 0.0;
        bool isNegative = false;
        size_t i = 0;

        if (priceStr[i] == '-') {
            isNegative = true;
            i++;
        }

        while (i < priceStr.length() && priceStr[i] != '.') {
            result = result * 10 + (priceStr[i] - '0');
            i++;
        }

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

// ---------------------- Customer Class ----------------------
class Customer {
private:
    int id;
    string name;
    string email;
    string phone;
    string address;

public:
    Customer(int id = 0, const string& name = "", const string& email = "",
        const string& phone = "", const string& address = "")
        : id(id), name(name), email(email), phone(phone), address(address) {
    }

    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }

    void setId(int newId) { id = newId; }
    void setName(const string& newName) { name = newName; }
    void setEmail(const string& newEmail) { email = newEmail; }
    void setPhone(const string& newPhone) { phone = newPhone; }
    void setAddress(const string& newAddress) { address = newAddress; }

    void display() const {
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
        cout << "Phone: " << phone << endl;
        cout << "Address: " << address << endl;
        cout << "------------------------" << endl;
    }

    string toString() const {
        stringstream ss;
        ss << id << "," << name << "," << email << "," << phone << "," << address;
        return ss.str();
    }

    static Customer fromString(const string& line) {
        stringstream ss(line);
        string item;
        vector<string> tokens;

        while (getline(ss, item, ',')) {
            tokens.push_back(item);
        }

        if (tokens.size() >= 5) {
            return Customer(stoi(tokens[0]), tokens[1], tokens[2], tokens[3], tokens[4]);
        }
        return Customer();
    }
};

// ---------------------- Customer Manager ----------------------
class CustomerManager {
private:
    vector<Customer> customers;
    string filename;
    int nextId;

public:
    CustomerManager(const string& file = "customers.txt") : filename(file), nextId(1) {
        loadFromFile();
    }

    ~CustomerManager() {
        saveToFile();
    }

    void addCustomer() {
        string name, email, phone, address;

        cout << "\n=== Add New Customer ===" << endl;
        cin.ignore();
        cout << "Enter customer name: ";
        getline(cin, name);
        cout << "Enter email: ";
        getline(cin, email);
        cout << "Enter phone: ";
        getline(cin, phone);
        cout << "Enter address: ";
        getline(cin, address);

        Customer newCustomer(nextId++, name, email, phone, address);
        customers.push_back(newCustomer);

        cout << "Customer added successfully with ID: " << newCustomer.getId() << endl;
    }

    void viewAllCustomers() const {
        if (customers.empty()) {
            cout << "No customers found." << endl;
            return;
        }

        for (size_t i = 0; i < customers.size(); ++i) {
            customers[i].display();
        }
    }

    void run() {
        int choice;
        do {
            cout << "\n=== Customer Menu ===\n1. Add Customer\n2. View Customers\n3. Back to Main\nChoice: ";
            cin >> choice;
            switch (choice) {
            case 1: addCustomer(); break;
            case 2: viewAllCustomers(); break;
            case 3: return;
            default: cout << "Invalid. Try again.\n";
            }
        } while (choice != 3);
    }

    void saveToFile() const {
        ofstream file(filename.c_str());
        for (size_t i = 0; i < customers.size(); ++i) {
            file << customers[i].toString() << endl;
        }
    }

    void loadFromFile() {
        ifstream file(filename.c_str());
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                Customer c = Customer::fromString(line);
                customers.push_back(c);
                if (c.getId() >= nextId) nextId = c.getId() + 1;
            }
        }
    }
};

// ---------------------- Main ----------------------
int main() {
    vector<Venue*> venues;
    VenueRentalSystem venueSystem;
    CustomerManager customerManager;

    int choice;
    do {
        cout << "\n~-~ Main Menu ~-~\n";
        cout << "1. Manage Venues\n2. Manage Customers\n3. Book Venue\n4. Load Bookings\n5. Exit\n\nChoice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            int vChoice;
            do {
                cout << "\n--- Venue Management ---\n1. Add Venue\n2. View Venues\n3. Back\nChoice: ";
                cin >> vChoice;
                if (vChoice == 1) {
                    Venue* v = new Venue;
                    v->id = inputInt("Enter ID: ");
                    v->name = inputAlpha("Enter Name: ");
                    v->location = inputAlpha("Enter Location: ");
                    v->price = inputDouble("Enter Price: ");
                    v->capacity = inputInt("Enter Capacity: ");
                    v->isAvailable = inputInt("Available? (1 for Yes, 0 for No): ");
                    venues.push_back(v);
                    venueSystem.saveVenue(VenueData(v->name, v->location, v->price));
                }
                else if (vChoice == 2) {
                    venueSystem.loadVenues();
                }
            } while (vChoice != 3);
            break;
        }
        case 2:
            customerManager.run();
            break;
        case 3: {
            string cname, vname, date;
            cname = inputAlpha("Enter customer name: ");
            vname = inputAlpha("Enter venue name: ");
            date = inputString("Enter date (YYYY-MM-DD): ");
            venueSystem.saveBooking(Booking(cname, vname, date));
            break;
        }
        case 4:
            venueSystem.loadBookings();
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 5);

    for (size_t i = 0; i < venues.size(); ++i) delete venues[i];
    return 0;
}

