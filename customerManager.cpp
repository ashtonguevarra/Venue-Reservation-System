#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;

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
        return to_string(id) + "," + name + "," + email + "," + phone + "," + address;
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
        cout << "Enter customer name: ";
        cin.ignore();
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

        cout << "\n=== All Customer Profiles ===" << endl;
        for (const auto& customer : customers) {
            customer.display();
        }
    }

    void searchByName() const {
        string searchName;
        cout << "Enter customer name to search: ";
        cin.ignore();
        getline(cin, searchName);

        cout << "\n=== Search Results ===" << endl;
        bool found = false;

        for (const auto& customer : customers) {
            if (customer.getName().find(searchName) != string::npos) {
                customer.display();
                found = true;
            }
        }

        if (!found) {
            cout << "No customers found with name containing: " << searchName << endl;
        }
    }

    void searchById() const {
        int searchId;
        cout << "Enter customer ID to search: ";
        cin >> searchId;

        auto it = find_if(customers.begin(), customers.end(),
            [searchId](const Customer& c) { return c.getId() == searchId; });

        if (it != customers.end()) {
            cout << "\n=== Customer Found ===" << endl;
            it->display();
        }
        else {
            cout << "Customer with ID " << searchId << " not found." << endl;
        }
    }

    void deleteCustomer() {
        int deleteId;
        cout << "Enter customer ID to delete: ";
        cin >> deleteId;

        auto it = find_if(customers.begin(), customers.end(),
            [deleteId](const Customer& c) { return c.getId() == deleteId; });

        if (it != customers.end()) {
            cout << "Customer found: " << it->getName() << endl;
            char confirm;
            cout << "Are you sure you want to delete? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                customers.erase(it);
                cout << "Customer deleted successfully." << endl;
            }
        }
        else {
            cout << "Customer with ID " << deleteId << " not found." << endl;
        }
    }

    void updateCustomer() {
        int updateId;
        cout << "Enter customer ID to update: ";
        cin >> updateId;

        auto it = find_if(customers.begin(), customers.end(),
            [updateId](const Customer& c) { return c.getId() == updateId; });

        if (it != customers.end()) {
            cout << "Current customer information:" << endl;
            it->display();

            string name, email, phone, address;
            cout << "\nEnter new information (press Enter to keep current):" << endl;

            cout << "Name [" << it->getName() << "]: ";
            cin.ignore();
            getline(cin, name);
            if (!name.empty()) it->setName(name);

            cout << "Email [" << it->getEmail() << "]: ";
            getline(cin, email);
            if (!email.empty()) it->setEmail(email);

            cout << "Phone [" << it->getPhone() << "]: ";
            getline(cin, phone);
            if (!phone.empty()) it->setPhone(phone);

            cout << "Address [" << it->getAddress() << "]: ";
            getline(cin, address);
            if (!address.empty()) it->setAddress(address);

            cout << "Customer updated successfully!" << endl;
        }
        else {
            cout << "Customer with ID " << updateId << " not found." << endl;
        }
    }

    void saveToFile() const {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& customer : customers) {
                file << customer.toString() << endl;
            }
            file.close();
        }
    }

    void loadFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (!line.empty()) {
                    Customer customer = Customer::fromString(line);
                    if (customer.getId() > 0) {
                        customers.push_back(customer);
                        if (customer.getId() >= nextId) {
                            nextId = customer.getId() + 1;
                        }
                    }
                }
            }
            file.close();
        }
    }

    void displayMenu() {
        cout << "\n=== Customer Manager System ===" << endl;
        cout << "1. Add Customer" << endl;
        cout << "2. View All Customers" << endl;
        cout << "3. Search by Name" << endl;
        cout << "4. Search by ID" << endl;
        cout << "5. Update Customer" << endl;
        cout << "6. Delete Customer" << endl;
        cout << "7. Save and Exit" << endl;
        cout << "Enter your choice: ";
    }

    void run() {
        int choice;
        do {
            displayMenu();
            cin >> choice;

            switch (choice) {
            case 1:
                addCustomer();
                break;
            case 2:
                viewAllCustomers();
                break;
            case 3:
                searchByName();
                break;
            case 4:
                searchById();
                break;
            case 5:
                updateCustomer();
                break;
            case 6:
                deleteCustomer();
                break;
            case 7:
                saveToFile();
                cout << "Data saved. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
            }
        } while (choice != 7);
    }
};

int main() {
    CustomerManager manager("customers.txt");
    manager.run();
    return 0;
}