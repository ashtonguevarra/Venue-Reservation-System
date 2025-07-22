#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <limits>

using namespace std;

// ---------------------- Input Validation ----------------------
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

    string toString() const {
        stringstream ss;
        ss << id << "," << name << "," << location << "," << capacity << "," << (isAvailable ? "1" : "0") << "," << price;
        return ss.str();
    }

    static Venue fromString(const string& line) {
        stringstream ss(line);
        string token;
        vector<string> tokens;
        while (getline(ss, token, ',')) tokens.push_back(token);

        if (tokens.size() == 6) {
            return {stoi(tokens[0]), tokens[1], stoi(tokens[3]), tokens[4] == "1", tokens[2], stod(tokens[5])};
        }
        return {};
    }

    void display() const {
        cout << "ID: " << id << ", Name: " << name << ", Location: " << location
             << ", Capacity: " << capacity << ", Available: " << (isAvailable ? "Yes" : "No")
             << ", Price: $" << price << "\n";
    }
};

class VenueManager {
private:
    vector<Venue> venues;
    string filename = "Venues.txt";
    int nextId = 1;

public:
    VenueManager() {
        loadFromFile();
    }

    void addVenue() {
        Venue v;
        v.id = nextId++;
        v.name = inputAlpha("Enter venue name: ");
        v.location = inputAlpha("Enter location: ");
        v.capacity = inputInt("Enter capacity: ");
        v.price = inputDouble("Enter price: ");
        v.isAvailable = true;
        venues.push_back(v);
        saveToFile();
        cout << "Venue added!\n";
    }

    void updateVenue() {
        int id = inputInt("Enter venue ID to update: ");
        for (auto& v : venues) {
            if (v.id == id) {
                v.name = inputAlpha("Enter new name: ");
                v.location = inputAlpha("Enter new location: ");
                v.capacity = inputInt("Enter new capacity: ");
                v.price = inputDouble("Enter new price: ");
                v.isAvailable = inputInt("Available? (1 = Yes, 0 = No): ");
                saveToFile();
                cout << "Venue updated!\n";
                return;
            }
        }
        cout << "Venue not found.\n";
    }

    void deleteVenue() {
        int id = inputInt("Enter venue ID to delete: ");
        auto it = remove_if(venues.begin(), venues.end(), [id](const Venue& v) { return v.id == id; });
        if (it != venues.end()) {
            venues.erase(it, venues.end());
            saveToFile();
            cout << "Venue deleted.\n";
        } else {
            cout << "Venue not found.\n";
        }
    }

    void showVenues() const {
        if (venues.empty()) {
            cout << "No venues available.\n";
        } else {
            for (const auto& v : venues) v.display();
        }
    }

    bool isVenueAvailable(const string& name, const string& date) const {
        ifstream file("Bookings.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string cname, vname, bdate;
            getline(ss, cname, ',');
            getline(ss, vname, ',');
            getline(ss, bdate, ',');
            if (vname == name && bdate == date) return false;
        }
        return true;
    }

    Venue* findVenueByName(const string& name) {
        for (auto& v : venues) {
            if (v.name == name) return &v;
        }
        return nullptr;
    }

    void saveToFile() const {
        ofstream out(filename);
        for (const auto& v : venues) {
            out << v.toString() << "\n";
        }
    }

    void loadFromFile() {
        venues.clear();
        ifstream in(filename);
        string line;
        while (getline(in, line)) {
            Venue v = Venue::fromString(line);
            venues.push_back(v);
            if (v.id >= nextId) nextId = v.id + 1;
        }
    }
};

// ---------------------- Booking Struct ----------------------
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

// ---------------------- Customer Classes (Same as your original, kept unchanged) ----------------------
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
        : id(id), name(name), email(email), phone(phone), address(address) {}

    int getId() const { return id; }
    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }

    void display() const {
        cout << "ID: " << id << "\nName: " << name << "\nEmail: " << email
             << "\nPhone: " << phone << "\nAddress: " << address << "\n------------------------\n";
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
        while (getline(ss, item, ',')) tokens.push_back(item);
        if (tokens.size() >= 5) {
            return Customer(stoi(tokens[0]), tokens[1], tokens[2], tokens[3], tokens[4]);
        }
        return Customer();
    }
};

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
        string name = inputAlpha("Enter customer name: ");
        string email = inputString("Enter email: ");
        string phone = inputString("Enter phone: ");
        string address = inputString("Enter address: ");
        Customer newCustomer(nextId++, name, email, phone, address);
        customers.push_back(newCustomer);
        cout << "Customer added successfully with ID: " << newCustomer.getId() << endl;
    }

    void viewAllCustomers() const {
        if (customers.empty()) {
            cout << "No customers found.\n";
            return;
        }
        for (const auto& c : customers) c.display();
    }

    Customer* findCustomerByName(const string& name) {
        for (auto& c : customers) {
            if (c.getName() == name) return &c;
        }
        return nullptr;
    }

    void run() {
        int choice;
        do {
            cout << "\n=== Customer Menu ===\n1. Add Customer\n2. View Customers\n3. Back\nChoice: ";
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
        ofstream file(filename);
        for (const auto& c : customers) {
            file << c.toString() << endl;
        }
    }

    void loadFromFile() {
        ifstream file(filename);
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

// ---------------------- Booking and Report ----------------------
void makeBooking(VenueManager& vMgr, CustomerManager& cMgr) {
    string customerName = inputAlpha("Enter customer name: ");
    string venueName = inputAlpha("Enter venue name: ");
    string date = inputString("Enter date (YYYY-MM-DD): ");

    if (!cMgr.findCustomerByName(customerName)) {
        cout << "Customer not found.\n";
        return;
    }

    Venue* venue = vMgr.findVenueByName(venueName);
    if (!venue) {
        cout << "Venue not found.\n";
        return;
    }

    if (!vMgr.isVenueAvailable(venueName, date)) {
        cout << "Venue is already booked on that date.\n";
        return;
    }

    Booking b(customerName, venueName, date);
    ofstream file("Bookings.txt", ios::app);
    file << b.toString() << "\n";
    file.close();
    cout << "Booking confirmed!\n";
}

void generateReport() {
    ifstream file("Bookings.txt");
    string line;
    int count = 0;
    double revenue = 0;
    cout << "\n--- Booking Report ---\n";
    while (getline(file, line)) {
        cout << line << "\n";
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        string venueName = line.substr(pos1 + 1, pos2 - pos1 - 1);
        ifstream vfile("Venues.txt");
        string vline;
        while (getline(vfile, vline)) {
            Venue v = Venue::fromString(vline);
            if (v.name == venueName) {
                revenue += v.price;
                break;
            }
        }
        count++;
    }
    cout << "\nTotal Bookings: " << count << "\nTotal Revenue: $" << revenue << "\n";
}

// ---------------------- Main Menu ----------------------
void mainMenu() {
    VenueManager venueMgr;
    CustomerManager customerMgr;

    int choice;
    do {
        cout << "\n=== Main Menu ===\n"
             << "1. Manage Venues\n2. Manage Customers\n3. Make Booking\n4. View Report\n5. Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                int c;
                do {
                    cout << "\n--- Venue Management ---\n1. Add\n2. Update\n3. Delete\n4. Show\n5. Back\nChoice: ";
                    cin >> c;
                    switch (c) {
                        case 1: venueMgr.addVenue(); break;
                        case 2: venueMgr.updateVenue(); break;
                        case 3: venueMgr.deleteVenue(); break;
                        case 4: venueMgr.showVenues(); break;
                        case 5: break;
                        default: cout << "Invalid.\n";
                    }
                } while (c != 5);
                break;
            }
            case 2: customerMgr.run(); break;
            case 3: makeBooking(venueMgr, customerMgr); break;
            case 4: generateReport(); break;
            case 5: cout << "Goodbye!\n"; break;
            default: cout << "Invalid.\n";
        }
    } while (choice != 5);
}

// ---------------------- Entry Point ----------------------
int main() {
    mainMenu();
    return 0;
}

