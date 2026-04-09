#pragma once
#include <iostream>
#include <iomanip>

class Flight {
public:
    std::string flightNumber;
    std::string departure;
    std::string destination;
    std::string departureTime;
    std::string arrivalTime;
    std::string ticketCost;

    Flight(const std::string& num, const std::string& dep, const std::string& dest, const std::string& dtime, const std::string& atime, const std::string& tcost )
        : flightNumber(num), departure(dep), destination(dest), departureTime(dtime), arrivalTime(atime), ticketCost(tcost) {}

    void displayFlight() const {
        std::cout << std::setw(10) << flightNumber << " | "
                  << std::setw(15) << departure << " -> "
                  << std::setw(15) << destination << " | Departure: "
                  << departureTime << " | Arrival: " << arrivalTime << " | Ticket cost: " << ticketCost << "\n";
    }
};