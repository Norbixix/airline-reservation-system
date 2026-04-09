#include <iostream>
#include "Flight.hpp"
#include "Passenger.hpp"
#include "SeatReservation.hpp"
#include "Utilities.hpp"

const std::string ADMIN_PASSWORD = "admin";

void adminMenu(const std::vector<Flight>& flights) {
    while (true) {
        std::cout << "\n--- Admin Menu ---\n";
        std::string flightNum;
        std::cout << "Enter Flight Number to manage reservations (or type 'exit' to return to main menu): ";
        std::cin >> flightNum;

        if (flightNum == "exit") {
            break;
        }

        if (!isValidFlightNumber(flightNum, flights)) {
            std::cout << "Invalid flight number! Please enter a valid one.\n";
            continue;
        }

        int row;
        char col;
        bool validSeat = false;
        while (!validSeat) {
            std::cout << "Enter Seat to Remove (e.g., 1A): ";
            std::cin >> row >> col;
            col = std::toupper(col);

            if (row < 1 || row > 200 || col < 'A' || col > 'F') {
                std::cout << "Invalid seat input! Try again.\n";
                continue;
            }

            SeatReservation seatRes;
            try {
                seatRes.loadSeatsForFlight(flightNum);

                if (seatRes.removeSeat(row - 1, col - 'A')) {
                    removePassengerDetails(flightNum, row, col);
                    seatRes.saveSeatsForFlight(flightNum);
                    std::cout << "Seat " << row << col << " has been freed.\n";
                    validSeat = true;
                } else {
                    std::cout << "Seat is already free or invalid.\n";
                }

            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << "\n";
                break;
            }
        }
    }
}

void clientMenu(const std::vector<Flight>& flights) {
    std::cout << "\n--- Client Menu ---\n";
    while (true) {
        std::cout << "1. View Flights\n2. Book a Flight\n3. Exit\nChoose an option: ";
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            for (const auto& flight : flights) {
                flight.displayFlight();
            }
        } else if (choice == 2) {
            std::string flightNum;
            std::cout << "Enter Flight Number: ";
            std::cin >> flightNum;

            if (!isValidFlightNumber(flightNum, flights)) {
                std::cout << "Invalid flight number!\n";
                continue;
            }

            SeatReservation seatRes;
            try {
                seatRes.loadSeatsForFlight(flightNum);
            } catch (const std::exception& e) {
                std::cout << e.what() << "\nCreating new seat layout.\n";
                seatRes = SeatReservation();
            }

            std::string name, surname, id, address;
            int age;
            bool extraLuggage;

            do {
                std::cout << "Name: ";
                std::cin >> name;
            } while (!isValidName(name));

            do {
                std::cout << "Surname: ";
                std::cin >> surname;
            } while (!isValidName(surname));

            do {
                std::cout << "ID Number: ";
                std::cin >> id;
            } while (!isValidId(id));

            do {
                std::cout << "Age: ";
            } while (!getValidAge(age));

            //std::cin.ignore();
            std::cout << "Address: ";
            std::getline(std::cin, address);

            std::cout << "Do you want Extra Luggage - $12 (1 for yes, 0 for no): ";
            std::cin >> extraLuggage;

            Passenger passenger(name, surname, id, age, address, extraLuggage);

            std::cout << "\nSelect a seat:\n";
            while (true) {
                seatRes.displaySeats();
                int row;
                char seatCol;

                std::cout << "Row: ";
                while (!(std::cin >> row) || row < 1 || row > 200) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Invalid row. Rewrite the correct row: ";
                }
                std::cout << "Column: ";
                std::cin >> seatCol;
                seatCol = std::toupper(seatCol);

                if (seatCol < 'A' || seatCol > 'F') {
                    std::cout << "Invalid seat column. Rewrite the correct column: \n";
                    continue;
                }

                bool reserved = seatRes.reserveSeat(row - 1, seatCol - 'A');
                if (reserved) {
                    passenger.seatRow = row;
                    passenger.seatCol = seatCol;

                    double baseTicketCost = 0.0;
                    for (const auto& flight : flights) {
                        if (flight.flightNumber == flightNum) {
                            baseTicketCost = std::stod(flight.ticketCost);
                            break;
                        }
                    }

                    int seatCost = passenger.calculatePrice(seatCol);
                    double totalCost = baseTicketCost + seatCost;

                    std::cout << "\n--- Cost Details ---\n";
                    std::cout << "Base Ticket Cost: " << baseTicketCost << "$" << "\n";
                    std::cout << "Seat Cost (incl. extra luggage): " << seatCost << "$" << "\n";
                    std::cout << "Total Travel Cost: " << totalCost << "$" << "\n";
                    std::cout << "--- Passenger Details ---\n";
                    std::cout << passenger.toString() << "\n";
                    std::cout << "--- Flight Details ---\n";
                    for (const auto& flight : flights) {
                        if (flight.flightNumber == flightNum) {
                            flight.displayFlight();
                            break;
                        }
                    }

                    int confirm;
                    while (true) {
                        std::cout << "Do you want to confirm the booking? (1 for yes, 0 for no): ";
                        std::cin >> confirm;

                        if (std::cin.fail() || (confirm != 0 && confirm != 1)) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Invalid input. Please enter 1 for yes or 0 for no.\n";
                        } else {
                            break;
                        }
                    }

                    if (confirm == 1) {
                        std::cout << "Booking confirmed!\n";
                        savePassengerDetails(flightNum, passenger);
                        seatRes.saveSeatsForFlight(flightNum);
                        break;
                    } else {
                        std::cout << "Booking cancelled!\n";
                        seatRes.removeSeat(row - 1, seatCol - 'A');
                        break;
                    }
                } else {
                    std::cout << "Seat is already reserved! Try another one.\n";
                }
            }
        } else if (choice == 3) {
            break;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
}

int main() {
    const std::string flightFile = "flights.txt";
    std::vector<Flight> flights = loadFlights(flightFile);

    std::cout << "Welcome to the Airline Reservation System!\n";
    int role;
    while (true) {
        std::cout << "Select your role to log into the site\n";
        std::cout << "1. Client\n2. Admin\n";
        std::cout << "Enter 1 or 2: ";
        std::cin >> role;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number: 1 or 2.\n";
            continue;
        }
        if (role == 1) {
            clientMenu(flights);
            break;
        } else if (role == 2) {
            std::cout << "Enter admin password: ";
            std::string password;
            std::cin >> password;

            if (password == ADMIN_PASSWORD) {
                adminMenu(flights);
                break;
            } else {
                std::cout << "Incorrect password. Access denied.\n";
            }
        } else {
            std::cout << "Invalid role selection. Please try again.\n";
        }
    }

    return 0;
}
