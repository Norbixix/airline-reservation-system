#pragma once
#include <iostream>
#include <string>

class Passenger {
public:
    std::string name;
    std::string surname;
    std::string idNumber;
    std::string address;
    int age;
    bool extraLuggage;
    int seatRow;
    char seatCol;

    Passenger(const std::string& n, const std::string& s, const std::string& id, int a, const std::string& addr, bool luggage)
        : name(n), surname(s), idNumber(id), age(a), address(addr), extraLuggage(luggage), seatRow(0), seatCol(' ') {}

    int calculatePrice(char seatCol) const {
        int price = 0;
        if (seatCol == 'A' || seatCol == 'F') {
            price = 10;
        } else if (seatCol == 'B' || seatCol == 'E') {
            price = 8;
        } else if (seatCol == 'C' || seatCol == 'D') {
            price = 4;
        }

        if (extraLuggage) {
            price += 12;
        }

        return price;
    }

    std::string toString() const {
        return name + " " + surname + ", ID: " + idNumber + ", Age: " + std::to_string(age) + ", Address: " + address + ", Extra Luggage: " + (extraLuggage ? "Yes" : "No") + ", Seat: " + std::to_string(seatRow) + seatCol;
    }
};