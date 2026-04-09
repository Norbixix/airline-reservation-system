#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>

class SeatReservation {
    std::vector<std::vector<char>> seatLayout;

public:
    SeatReservation(int rows = 10, int cols = 6) : seatLayout(rows, std::vector<char>(cols, 'O')) {}

    void displaySeats() const {
        std::cout << "    ";
        for (size_t col = 0; col < seatLayout[0].size(); ++col) {
            std::cout << static_cast<char>('A' + col) << " ";
            if (col == 2) std::cout << "   "; 
        }
        std::cout << "\n";

        for (size_t row = 0; row < seatLayout.size(); ++row) {
            std::cout << std::setw(2) << (row + 1) << " ";

            for (size_t col = 0; col < seatLayout[row].size(); ++col) {
                std::cout << "|";  
                std::cout << seatLayout[row][col];
                if (col == 2) std::cout << "|  ";  
            }
            std::cout << "|\n";  
        }

        std::cout << "\nSeat Prices:\n";
        std::cout << "A, F: $10\nB, E: $8\nC, D: $4\n";
    }

    bool reserveSeat(int row, int col) {
        if (row < 0 || row >= (int)seatLayout.size() || col < 0 || col >= (int)seatLayout[row].size()) {
            throw std::out_of_range("Invalid seat position.");
        }
        if (seatLayout[row][col] == 'X') {
            return false;
        }
        seatLayout[row][col] = 'X';
        return true;
    }

    bool removeSeat(int row, int col) {
        if (row < 0 || row >= (int)seatLayout.size() || col < 0 || col >= (int)seatLayout[row].size()) {
            throw std::out_of_range("Invalid seat position.");
        }
        if (seatLayout[row][col] == 'O') {
            return false;
        }
        seatLayout[row][col] = 'O';
        return true;
    }

    void saveSeatsForFlight(const std::string& flightNumber) const {
        std::string fileName = flightNumber + "_seats.txt";
        std::ofstream file(fileName);
        for (const auto& row : seatLayout) {
            for (char seat : row) {
                file << seat;
            }
            file << std::endl;
        }
    }

    void loadSeatsForFlight(const std::string& flightNumber) {
        std::string fileName = flightNumber + "_seats.txt";
        std::ifstream file(fileName);
        std::string line;

        seatLayout.clear();
        while (std::getline(file, line)) {
            seatLayout.emplace_back(line.begin(), line.end());
        }

        if (seatLayout.empty()) {
            throw std::runtime_error("Seat file is empty or invalid.");
        }
    }
};