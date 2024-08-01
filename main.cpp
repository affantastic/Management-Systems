#include <iostream>
#include <fstream>
#include <ctime>

const int NUM_TABLES = 100;

using namespace std;

class Person {
protected:
    string name;
    int id;

public:
    Person() : name(""), id(0) {}
    Person(string name, int id) : name(name), id(id) {}

    void setName(string name) {
        this->name = name;
    }

    string getName() const {
        return name;
    }

    void setId(int id) {
        this->id = id;
    }

    int getId() const {
        return id;
    }

    virtual void showRole() const {
        cout << "I am a person." << endl;
    }

    // Overloading the equality operator
    bool operator==(const Person &other) const {
        return this->id == other.id;
    }

    // Overloading the insertion operator for output
    friend ostream& operator<<(ostream &os, const Person &person) {
        os << "Name: " << person.name << ", ID: " << person.id;
        return os;
    }
};

class Admin : public Person {
private:
    char tables[10][10];  // 10x10 array to represent 100 tables
    double revenue;

public:
    Admin() : revenue(0.0) {
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                tables[i][j] = 'X'; // 'X' means available, '0' means booked
    }

    void displayTables() {
        for (int i = 0; i < 10; ++i) {
            for (int j = 0; j < 10; ++j) {
                cout << tables[i][j] << " ";
            }
            cout << endl;
        }
    }

    bool bookTable(int row, int col) {
        if (tables[row][col] == 'X') {
            tables[row][col] = '0';
            return true;
        }
        return false;
    }

    void displayMenu() {
        cout << "Menu:\n"
             << "1.  Pizza     -  $10\n"
             << "2.  Burger    -  $5\n"
             << "3.  Salad     -  $7\n"
             << "4.  Pasta     -  $12\n"
             << "5.  Steak     -  $20\n"
             << "6.  Sushi     -  $15\n"
             << "7.  Tacos     -  $8\n"
             << "8.  Soup      -  $6\n"
             << "9.  Sandwich  -  $9\n"
             << "10. Dessert   -  $6\n";
    }

    void addRevenue(double amount) {
        revenue += amount;
    }

    double getRevenue() {
        return revenue;
    }

    void saveState() {
        ofstream file("admin_state.txt");
        if (file.is_open()) {
            file << revenue << endl;
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    file << tables[i][j];
                }
                file << endl;
            }
            file.close();
        }
    }

    void loadState() {
        ifstream file("admin_state.txt");
        if (file.is_open()) {
            file >> revenue;
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 10; ++j) {
                    file >> tables[i][j];
                }
            }
            file.close();
        }
    }

    void showRole() const override {
        cout << "I am an admin." << endl;
    }
};

class Waiter : public Person {
private:
    string orders[100];
    int orderCount;
    bool delivered;
    double tip;

public:
    Waiter() : orderCount(0), delivered(false), tip(0.0) {}

    void recordOrder(const string &order) {
        orders[orderCount++] = order;
        delivered = false;
    }

    void displayOrders() {
        if (orderCount == 0) {
            cout << "No orders taken yet!\n";
        } else {
            cout << "Orders:\n";
            for (int i = 0; i < orderCount; ++i) {
                cout << i + 1 << ". " << orders[i] << endl;
            }
        }
    }

    void serveOrder() {
        if (orderCount > 0) {
            cout << "Order served!\n";
            delivered = true;
            orderCount = 0; // Clear all orders after serving
        } else {
            cout << "No order to serve!\n";
        }
    }

    void deliverOrder() {
        delivered = true;
    }

    void takeTip(double amount) {
        if (delivered) {
            tip += amount;
            cout << "Tip received: $" << amount << endl;
        } else {
            cout << "Order not delivered yet, tip cannot be accepted!\n";
        }
    }

    bool isOrderDelivered() {
        return delivered;
    }

    double getTip() {
        return tip;
    }

    void saveState() {
        ofstream file("waiter_state.txt");
        if (file.is_open()) {
            file << orderCount << endl;
            for (int i = 0; i < orderCount; ++i) {
                file << orders[i] << endl;
            }
            file << delivered << endl;
            file << tip << endl;
            file.close();
        }
    }

    void loadState() {
        ifstream file("waiter_state.txt");
        if (file.is_open()) {
            file >> orderCount;
            file.ignore(); // Ignore the newline character
            for (int i = 0; i < orderCount; ++i) {
                getline(file, orders[i]);
            }
            file >> delivered;
            file >> tip;
            file.close();
        }
    }

    void showRole() const override {
        cout << "I am a waiter." << endl;
    }

    // Overloading the addition assignment operator to add tips
    void operator+=(double amount) {
        tip += amount;
    }
};

class Chef : public Person {
public:
    void viewOrder(Waiter &waiter) {
        waiter.displayOrders();
        if (!waiter.isOrderDelivered()) {
            cout << "Preparing orders...\n";
            time_t start = time(0);
            while (difftime(time(0), start) < 15) {
                // simulate 15 minutes wait time
            }
            cout << "Orders ready!\n";
            waiter.deliverOrder();
        } else {
            cout << "No orders to prepare.\n";
        }
    }

    void showRole() const override {
        cout << "I am a chef." << endl;
    }
};

class Customer : public Person {
private:
    int rating;
    string reviewText;
    string orderList[100];
    int orderPrices[100];
    int orderCount;
    bool billPaid;

public:
    Customer() : rating(0), orderCount(0), billPaid(true) {
        cout << "Welcome to our restaurant!\n";
    }

    void viewMenu(Admin &admin) {
        admin.displayMenu();
    }

    void placeOrder(Waiter &waiter) {
        string menu[10][2] = {
            {"Pizza", "10"},
            {"Burger", "5"},
            {"Salad", "7"},
            {"Pasta", "12"},
            {"Steak", "20"},
            {"Sushi", "15"},
            {"Tacos", "8"},
            {"Soup", "6"},
            {"Sandwich", "9"},
            {"Dessert", "6"}
        };

        int order;
        char more;
        do {
            cout << "Enter order number (1-10): ";
            cin >> order;
            if (order < 1 || order > 10) {
                cout << "Invalid choice. Please try again.\n";
                continue;
            }
            string food = menu[order - 1][0];
            int price = stoi(menu[order - 1][1]);

            orderList[orderCount] = food;
            orderPrices[orderCount] = price;
            orderCount++;
            waiter.recordOrder(food);
            cout << "Order placed: " << food << endl;

            cout << "Do you want to add another item? (y/n): ";
            cin >> more;
        } while (more == 'y' || more == 'Y');

        // Display the current order
        cout << "Current Order:\n";
        for (int i = 0; i < orderCount; ++i) {
            cout << i + 1 << ". " << orderList[i] << " - $" << orderPrices[i] << endl;
        }
    }

    void giveRating(int rate) {
        if (rate >= 1 && rate <= 5) {
            rating = rate;
            cout << "Rating received: " << rating << "/5\n";
        } else {
            cout << "Invalid rating!\n";
        }
    }

    void giveReview(string text) {
        reviewText = text;
        cout << "Review received: " << reviewText << endl;
    }

    void generateBill() {
        if (orderCount == 0) {
            cout << "No orders placed. Cannot generate bill.\n";
            return;
        }
        int total = 0;
        for (int i = 0; i < orderCount; ++i) {
            total += orderPrices[i];
        }
        cout << "Total Bill: $" << total << endl;
        billPaid = false;
    }

    void payBill(Admin &admin) {
        if (orderCount == 0) {
            cout << "No orders placed. Cannot pay bill.\n";
            return;
        }
        if (!billPaid) {
            int total = 0;
            for (int i = 0; i < orderCount; ++i) {
                total += orderPrices[i];
            }
            admin.addRevenue(total);
            cout << "Bill paid successfully!\n";
            billPaid = true;
        } else {
            cout << "Bill already paid!\n";
        }
    }

    bool isBillPaid() {
        return billPaid;
    }

    void saveState() {
        ofstream file("customer_state.txt");
        if (file.is_open()) {
            file << rating << endl;
            file << reviewText << endl;
            file << orderCount << endl;
            for (int i = 0; i < orderCount; ++i) {
                file << orderList[i] << " " << orderPrices[i] << endl;
            }
            file << billPaid << endl;
            file.close();
        }
    }

    void loadState() {
        ifstream file("customer_state.txt");
        if (file.is_open()) {
            file >> rating;
            file.ignore();
            getline(file, reviewText);
            file >> orderCount;
            for (int i = 0; i < orderCount; ++i) {
                file >> orderList[i] >> orderPrices[i];
            }
            file >> billPaid;
            file.close();
        }
    }

    void showRole() const override {
        cout << "I am a customer." << endl;
    }
};

int main() {
    Admin admin;
    Waiter waiter;
    Chef chef;
    Customer customer;

    // Load the state from files
    admin.loadState();
    waiter.loadState();
    customer.loadState();

    int choice;
    do {
        cout << "\nRestaurant Management System\n";
        cout << "1. Admin - Display Tables\n";
        cout << "2. Admin - Book Table\n";
        cout << "3. Admin - Display Menu\n";
        cout << "4. Customer - Generate Bill\n";
        cout << "5. Customer - Pay Bill\n";
        cout << "6. Waiter - Serve Order\n";
        cout << "7. Waiter - Take Tip\n";
        cout << "8. Chef - View Order\n";
        cout << "9. Customer - View Menu\n";
        cout << "10. Customer - Place Order\n";
        cout << "11. Customer - Give Rating\n";
        cout << "12. Customer - Give Review\n";
        cout << "13. Show Roles\n";
        cout << "14. Save State\n";
        cout << "15. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                admin.displayTables();
                break;
            case 2: {
                int row, col;
                cout << "Enter row and column to book (0-9 each): ";
                cin >> row >> col;
                if (admin.bookTable(row, col)) {
                    cout << "Table booked successfully!\n";
                } else {
                    cout << "Table already booked!\n";
                }
                break;
            }
            case 3:
                admin.displayMenu();
                break;
            case 4:
                customer.generateBill();
                break;
            case 5:
                customer.payBill(admin);
                break;
            case 6:
                waiter.serveOrder();
                break;
            case 7: {
                double tipAmount;
                cout << "Enter tip amount: $";
                cin >> tipAmount;
                waiter.takeTip(tipAmount);
                break;
            }
            case 8:
                chef.viewOrder(waiter);
                break;
            case 9:
                customer.viewMenu(admin);
                break;
            case 10:
                customer.placeOrder(waiter);
                break;
            case 11: {
                if (waiter.isOrderDelivered()) {
                    int rating;
                    cout << "Enter rating (1-5): ";
                    cin >> rating;
                    customer.giveRating(rating);
                } else {
                    cout << "Order needs to be served first to give a rating!\n";
                }
                break;
            }
            case 12: {
                if (waiter.isOrderDelivered()) {
                    string review;
                    cout << "Enter review: ";
                    cin.ignore(); // Clearing input buffer
                    getline(cin, review);
                    customer.giveReview(review);
                } else {
                    cout << "Order needs to be served first to give a review!\n";
                }
                break;
            }
            case 13: {
                Person *persons[] = {&admin, &waiter, &chef, &customer};
                for (Person *person : persons) {
                    person->showRole();
                }
                break;
            }
            case 14:
                // Save the state to files
                admin.saveState();
                waiter.saveState();
                customer.saveState();
                cout << "State saved successfully!\n";
                break;
            case 15:
                cout << "Exiting system...\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 15);

    return 0;
}
