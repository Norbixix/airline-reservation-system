#include <algorithm>
#include <fstream>
#include <regex>
#include <vector>
#include <iostream>
#include <limits>
#include "Flight.hpp"
#include "Passenger.hpp"

bool isValidName(const std::string& name) {
    std::regex pattern("^[A-Za-z\\-\\s]+$");
    return std::regex_match(name, pattern);
}

bool isValidAge(int age) {
    return age >= 0 && age <= 130;
}

bool isValidId(const std::string& id) {
    std::regex pattern("^[A-Za-z0-9]+$");
    return std::regex_match(id, pattern);
}

bool getValidAge(int &age) {
    while (true) {
        std::cin >> age;
        if (std::cin.fail() || !isValidAge(age)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid age. Please enter a valid age (0-130): ";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return true;
        }
    }
}


bool isValidFlightNumber(const std::string& flightNum, const std::vector<Flight>& flights) {
    return std::any_of(flights.begin(), flights.end(), [&flightNum](const Flight& flight) {
        return flight.flightNumber == flightNum;
    });
}

std::vector<Flight> loadFlights(const std::string& fileName) {
    std::vector<Flight> flights;
    std::ifstream file(fileName);
    std::string num, dep, dest, dtime, atime, tcost;
    while (file >> num >> dep >> dest >> dtime >> atime >> tcost) {
        flights.emplace_back(num, dep, dest, dtime, atime, tcost);
    }
    return flights;
}

void savePassengerDetails(const std::string& flightNumber, const Passenger& passenger) {
    std::string fileName = flightNumber + "_passengers.txt";
    std::ofstream file(fileName, std::ios::app);
    if (file.is_open()) {
        file << passenger.toString() << std::endl;
    } else {
        throw std::runtime_error("Failed to open passenger file.");
    }
}

void removePassengerDetails(const std::string& flightNumber, int row, char col) {
    std::string fileName = flightNumber + "_passengers.txt";
    std::ifstream inputFile(fileName);
    std::ofstream tempFile("temp.txt");

    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find(std::to_string(row) + col) != std::string::npos) {
            continue;
        }
        tempFile << line << std::endl;
    }

    inputFile.close();
    tempFile.close();

    std::remove(fileName.c_str());
    std::rename("temp.txt", fileName.c_str());
}