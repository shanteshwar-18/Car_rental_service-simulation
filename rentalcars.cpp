#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <iomanip>

using namespace std;

// User Class Declaration and Implementation
class User {
private:
    string userId;
    string name;

public:
    // Constructor
    User(const string& userId, const string& name)
        : userId(userId), name(name) {}

    // Getter methods
    string getUserId() const { return userId; }
    string getName() const { return name; }

    // Display user details
    void displayUserDetails() const {
        cout << "User ID: " << userId << ", Name: " << name << endl;
    }
};

// Base Car Class (Abstract)

class Car {
private:
    string carId;
    string model;
    bool isAvailable;

public:
    // Constructor
    Car(const string& carId, const string& model)
        : carId(carId), model(model), isAvailable(true) {}

    // Virtual destructor for proper polymorphic destruction
    virtual ~Car() = default;

    // Getter methods
    string getCarId() const { return carId; }
    string getModel() const { return model; }
    bool getIsAvailable() const { return isAvailable; }

    // Car rental management
    void rentCar() { isAvailable = false; }
    void returnCar() { isAvailable = true; }

    // Pure virtual function - makes this class abstract
    virtual double getDailyRate() const = 0;

    // Virtual function for polymorphism
    virtual void displayDetails() const {
        cout << "Car ID: " << carId 
                  << ", Model: " << model 
                  << ", Available: " << (isAvailable ? "Yes" : "No")
                  << ", Daily Rate: $" << fixed << setprecision(2) 
                  << getDailyRate() << endl;
    }

    // Virtual function to get car type
    virtual string getCarType() const = 0;
};

// Economy Car Class (Derived)
class EconomyCar : public Car {
public:
    // Constructor calling base class constructor
    EconomyCar(const string& carId, const string& model)
        : Car(carId, model) {}

    // Override pure virtual function
    double getDailyRate() const override {
        return 50.0;  // Fixed rate for economy cars
    }

    // Override virtual function
    void displayDetails() const override {
        cout << "[ECONOMY] ";
        Car::displayDetails();
    }

    // Override car type
    string getCarType() const override {
        return "Economy";
    }
};

// Luxury Car Class (Derived)

class LuxuryCar : public Car {
public:
    // Constructor calling base class constructor
    LuxuryCar(const string& carId, const string& model)
        : Car(carId, model) {}

    // Override pure virtual function
    double getDailyRate() const override {
        return 150.0;  // Fixed rate for luxury cars
    }

    // Override virtual function
    void displayDetails() const override {
        cout << "[LUXURY] ";
        Car::displayDetails();
    }

    // Override car type
    string getCarType() const override {
        return "Luxury";
    }
};

// Rental Class Declaration and Implementation
class Rental {
private:
    string rentalId;
    User* user;          // Pointer to customer
    Car* car;            // Pointer to rented car
    int startDate;       // Day number for simulation
    int returnDate;
    double totalBill;
    bool isActive;       // Track if rental is active

public:
    // Constructor
    Rental(const string& rentalId, User* user, Car* car, int startDate)
        : rentalId(rentalId), user(user), car(car), startDate(startDate),
          returnDate(0), totalBill(0.0), isActive(true) {}

    // Getter methods
    string getRentalId() const { return rentalId; }
    Car* getCar() const { return car; }
    bool getIsActive() const { return isActive; }
    double getTotalBill() const { return totalBill; }

    // Calculate bill based on rental duration
    void calculateBill() {
        if (returnDate > startDate) {
            int days = returnDate - startDate;
            totalBill = days * car->getDailyRate();
        }
    }

    // Set return date and calculate bill
    void processReturn(int returnDay) {
        returnDate = returnDay;
        calculateBill();
        isActive = false;
        car->returnCar();  // Make car available again
    }

    // Display rental details
    void displayRentalDetails() const {
        cout << "\n=== Rental Details ===\n";
        cout << "Rental ID: " << rentalId << endl;
        cout << "Customer: " << user->getName() 
                  << " (ID: " << user->getUserId() << ")" << endl;
        cout << "Car: " << car->getModel() 
                  << " (ID: " << car->getCarId() << ")" << endl;
        cout << "Car Type: " << car->getCarType() << endl;
        cout << "Start Date: Day " << startDate << endl;
        
        if (!isActive) {
            cout << "Return Date: Day " << returnDate << endl;
            cout << "Rental Duration: " << (returnDate - startDate) << " days" << endl;
            cout << "Daily Rate: $" << fixed << setprecision(2) 
                      << car->getDailyRate() << endl;
            cout << "Total Bill: $" << fixed << setprecision(2) 
                      << totalBill << endl;
        } else {
            cout << "Status: Active (Not yet returned)" << endl;
        }
        cout << "========================\n" << endl;
    }
};

// Car Rental Management System Class
class CarRentalSystem {
private:
    vector<User> users;
    vector<unique_ptr<Car>> fleet;  // Using smart pointers for automatic memory management
    vector<Rental> rentals;
    int nextRentalId;

public:
    CarRentalSystem() : nextRentalId(1) {}

    // Add new car to fleet
    void addNewCar() {
        string carId, model;
        int carType;

        cout << "\n=== Add New Car ===\n";
        cout << "Enter Car ID: ";
        cin >> carId;
        
        // Check if car ID already exists
        for (const auto& car : fleet) {
            if (car->getCarId() == carId) {
                cout << "Error: Car with ID '" << carId << "' already exists!\n" << endl;
                return;
            }
        }

        cin.ignore(); // Clear input buffer
        cout << "Enter Car Model: ";
        getline(cin, model);
        
        cout << "Select Car Type:\n";
        cout << "1. Economy Car ($50/day)\n";
        cout << "2. Luxury Car ($150/day)\n";
        cout << "Enter choice (1-2): ";
        cin >> carType;

        try {
            if (carType == 1) {
                fleet.push_back(make_unique<EconomyCar>(carId, model));
                cout << "Economy car added successfully!\n" << endl;
            } else if (carType == 2) {
                fleet.push_back(make_unique<LuxuryCar>(carId, model));
                cout << "Luxury car added successfully!\n" << endl;
            } else {
                cout << "Invalid car type selected!\n" << endl;
            }
        } catch (const exception& e) {
            cout << "Error adding car: " << e.what() << endl;
        }
    }

    // Add new user
    void addNewUser() {
        string userId, name;

        cout << "\n=== Add New User ===\n";
        cout << "Enter User ID: ";
        cin >> userId;
        
        // Check if user ID already exists
        for (const auto& user : users) {
            if (user.getUserId() == userId) {
                cout << "Error: User with ID '" << userId << "' already exists!\n" << endl;
                return;
            }
        }

        cin.ignore(); // Clear input buffer
        cout << "Enter User Name: ";
        getline(cin, name);

        try {
            users.emplace_back(userId, name);
            cout << "User added successfully!\n" << endl;
        } catch (const exception& e) {
            cout << "Error adding user: " << e.what() << endl;
        }
    }

    // Rent a car
    void rentCar() {
        if (fleet.empty()) {
            cout << "No cars available in the fleet!\n" << endl;
            return;
        }
        
        if (users.empty()) {
            cout << "No users registered in the system!\n" << endl;
            return;
        }

        // Display available cars using polymorphism
        cout << "\n=== Available Cars ===\n";
        bool hasAvailableCars = false;
        for (const auto& car : fleet) {
            if (car->getIsAvailable()) {
                car->displayDetails();  // Polymorphic call
                hasAvailableCars = true;
            }
        }

        if (!hasAvailableCars) {
            cout << "No cars currently available for rent!\n" << endl;
            return;
        }

        string carId, userId;
        int startDate;

        cout << "\nEnter Car ID to rent: ";
        cin >> carId;
        cout << "Enter User ID: ";
        cin >> userId;
        cout << "Enter start date (day number): ";
        cin >> startDate;

        // Find the car
        Car* selectedCar = nullptr;
        for (const auto& car : fleet) {
            if (car->getCarId() == carId) {
                selectedCar = car.get();
                break;
            }
        }

        if (!selectedCar) {
            cout << "Error: Car with ID '" << carId << "' not found!\n" << endl;
            return;
        }

        if (!selectedCar->getIsAvailable()) {
            cout << "Error: Car is not available for rent!\n" << endl;
            return;
        }

        // Find the user
        User* selectedUser = nullptr;
        for (auto& user : users) {
            if (user.getUserId() == userId) {
                selectedUser = &user;
                break;
            }
        }

        if (!selectedUser) {
            cout << "Error: User with ID '" << userId << "' not found!\n" << endl;
            return;
        }

        try {
            // Create rental
            string rentalId = "R" + to_string(nextRentalId++);
            rentals.emplace_back(rentalId, selectedUser, selectedCar, startDate);
            selectedCar->rentCar();  // Set car as unavailable

            cout << "Car rented successfully!\n";
            cout << "Rental ID: " << rentalId << "\n" << endl;
        } catch (const exception& e) {
            cout << "Error processing rental: " << e.what() << endl;
        }
    }

    // Return a car
    void returnCar() {
        if (rentals.empty()) {
            cout << "No active rentals found!\n" << endl;
            return;
        }

        // Display active rentals
        cout << "\n=== Active Rentals ===\n";
        bool hasActiveRentals = false;
        for (const auto& rental : rentals) {
            if (rental.getIsActive()) {
                cout << "Rental ID: " << rental.getRentalId() 
                          << ", Car ID: " << rental.getCar()->getCarId() 
                          << ", Model: " << rental.getCar()->getModel() << endl;
                hasActiveRentals = true;
            }
        }

        if (!hasActiveRentals) {
            cout << "No active rentals to return!\n" << endl;
            return;
        }

        string searchId;
        int returnDate;

        cout << "\nEnter Rental ID or Car ID: ";
        cin >> searchId;
        cout << "Enter return date (day number): ";
        cin >> returnDate;

        // Find the rental
        Rental* selectedRental = nullptr;
        for (auto& rental : rentals) {
            if (rental.getIsActive() && 
                (rental.getRentalId() == searchId || rental.getCar()->getCarId() == searchId)) {
                selectedRental = &rental;
                break;
            }
        }

        if (!selectedRental) {
            cout << "Error: No active rental found with ID '" << searchId << "'!\n" << endl;
            return;
        }

        try {
            selectedRental->processReturn(returnDate);
            cout << "Car returned successfully!\n";
            selectedRental->displayRentalDetails();
        } catch (const exception& e) {
            cout << "Error processing return: " << e.what() << endl;
        }
    }

    // View all cars using polymorphism
    void viewAllCars() const {
        if (fleet.empty()) {
            cout << "No cars in the fleet!\n" << endl;
            return;
        }

        cout << "\n=== All Cars in Fleet ===\n";
        for (const auto& car : fleet) {
            car->displayDetails();  // Polymorphic call
        }
        cout << endl;
    }

    // View all users
    void viewAllUsers() const {
        if (users.empty()) {
            cout << "No users registered!\n" << endl;
            return;
        }

        cout << "\n=== All Registered Users ===\n";
        for (const auto& user : users) {
            user.displayUserDetails();
        }
        cout << endl;
    }

    // View all rentals
    void viewAllRentals() const {
        if (rentals.empty()) {
            cout << "No rentals found!\n" << endl;
            return;
        }

        cout << "\n=== All Rentals ===\n";
        for (const auto& rental : rentals) {
            rental.displayRentalDetails();
        }
    }

    // Display main menu
    void displayMenu() const {
        
        cout << "-----------Car Rental Service System--------\n";
        cout << "1. Add New Car\n";
        cout << "2. Add New User\n";
        cout << "3. Rent a Car\n";
        cout << "4. Return a Car\n";
        cout << "5. View All Cars\n";
        cout << "6. View All Users\n";
        cout << "7. View All Rentals\n";
        cout << "8. Exit\n";
        cout << "Enter your choice (1-8): ";
    }

    // Run the main application
    void run() {
        int choice;
        
        cout << "Welcome to the Car Rental Service Simulator!\n";
        
        while (true) {
            displayMenu();
            cin >> choice;

            // Input validation
            if (cin.fail()) {
                cin.clear();
                cin.ignore(10000, '\n');
                cout << "Invalid input! Please enter a number.\n" << endl;
                continue;
            }

            switch (choice) {
                case 1:
                    addNewCar();
                    break;
                case 2:
                    addNewUser();
                    break;
                case 3:
                    rentCar();
                    break;
                case 4:
                    returnCar();
                    break;
                case 5:
                    viewAllCars();
                    break;
                case 6:
                    viewAllUsers();
                    break;
                case 7:
                    viewAllRentals();
                    break;
                case 8:
                    cout << "Thank you for using Car Rental Service!\n";
                    cout << "Goodbye!\n";
                    return;
                default:
                    cout << "Invalid choice! Please enter a number between 1-8.\n" << endl;
                    break;
            }
        }
    }
};

// Main Function
int main() {
    try {
        CarRentalSystem system;
        system.run();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "Unknown fatal error occurred!" << endl;
        return 1;
    }

    return 0;
}

