#include "io.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

/**
 * Writes the coordinates and velocities of a system to a file in GRO format 
 * (Potentially include step and time here when simulating?)
 * @param system The vector of Molecule structs representing the system.
 * @param filename The name of the output file.
 * @param box_size A vector containing the x, y, and z dimensions of the box.
 * 
 */
void write_coord_vel(const std::vector<Molecule>& system, const std::string& filename, const std::vector<double>& box_size) {
    std::ofstream groFile(filename);
    if (!groFile) {
        std::cerr << "Cannot create .GRO, please try again. " << filename << std::endl;
        return;
    }

    groFile << "Coords & vels" << std::endl;
    groFile << std::setw(5) << system.size() << std::endl;
    groFile << std::fixed << std::setprecision(3);

    int atomIndex = 1;

    for (const auto& molecule : system) {
        for (const auto& atom : molecule.atoms) {
            groFile << std::setw(6) << "ATOM";
            groFile << std::setw(6) << "M1";
            groFile << std::setw(5) << atomIndex;
            groFile << std::setw(8) << std::setprecision(3) << atom.x << " ";
            groFile << std::setw(8) << std::setprecision(3) << atom.y << " ";
            groFile << std::setw(8) << std::setprecision(3) << atom.z << " ";
            groFile << std::setw(8) << std::setprecision(3) << atom.vx << " ";
            groFile << std::setw(8) << std::setprecision(3) << atom.vy << " ";
            groFile << std::setw(8) << std::setprecision(3) << atom.vz << " \n";

            atomIndex++;
        }
    }

    groFile << std::setw(8) << std::setprecision(5) << box_size[0];
    groFile << std::setw(8) << std::setprecision(5) << box_size[1];
    groFile << std::setw(8) << std::setprecision(5) << box_size[2] << std::endl;

    groFile.close();
}
