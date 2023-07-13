#include "system.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

//A molecule is simply a collection of atoms, with its information public
Molecule::Molecule(double x, double y, double z, double vx, double vy, double vz) : atoms({Atom(x, y, z, vx, vy, vz)}) {
}

System::System(int numMols) {
}

/**
 * Initializes a system of atoms with random coords. Assigns random velocities
 * using 'gen_velocities'
 *
 * @param box_size A vector containing the x, y, and z dimensions of the box.
 * @param N_atoms The total number of atoms in the system.
 * @return The initialized system (vector of Molecules)
 *
 */
std::vector<Molecule> Molecule::init_system(const std::vector<double>& box_size, int N_atoms, double thres, double T_init) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dis_x(0.0, box_size[0]);
    std::uniform_real_distribution<double> dis_y(0.0, box_size[1]);
    std::uniform_real_distribution<double> dis_z(0.0, box_size[2]);

    std::vector<double> velocities(3 * N_atoms);
    for (unsigned int i = 0; i < velocities.size(); ++i) {
        velocities[i] = Atom::gen_velocities(T_init);
    }

    std::vector<Molecule> system;

    for (int i = 0; i < N_atoms; i++) {
        Atom atom(dis_x(gen), dis_y(gen), dis_z(gen), 0.0, 0.0, 0.0); //initialize with empty velocities

        bool overlap = false;
        for (const auto& molcoord : system) {
            for (const auto& atomcoord : molcoord.atoms) {
                if (Atom::euclidean_dist(atomcoord, atom, thres)) {
                    overlap = true;
                    break;
                }
            }
            if (overlap) {
                break;
            }
        }

        if (overlap) {
            i--;
            continue;
        }

        atom.vx = velocities[i];
        atom.vy = velocities[3 * i + 1];
        atom.vz = velocities[3 * i + 2];

        Molecule molecule(atom.x, atom.y, atom.z, atom.vx, atom.vy, atom.vz);
        system.push_back(molecule);
    }

    return system;
}


/**
 * Writes the coordinates and velocities of a system to a file in GRO format 
 * (Potentially include step and time here when simulating?)
 * @param system The vector of Molecule structs representing the system.
 * @param filename The name of the output file.
 * @param box_size A vector containing the x, y, and z dimensions of the box.
 * 
 */
void System::write_coord_vel(const std::vector<Molecule>& system, const std::string& filename, const std::vector<double>& box_size) {
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
