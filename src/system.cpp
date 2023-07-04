#include "system.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

//A molecule is simply a collection of atoms, with its information public
Molecule::Molecule(double x, double y, double z) : atoms({Atom(x, y, z)}) {
}

//The system object is initialized by the 'Molecules::init_system' function
System::System(int numMols) {
    molecules.reserve(numMols);
    for (int i = 0; i < numMols; ++i) {
        molecules.emplace_back(0.0, 0.0, 0.0);
    }
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
    
    Atom atom(0.0, 0.0, 0.0); //init object to access member functions

    //Draw random coordinates inside of the defined box
    std::uniform_real_distribution<double> dis_x(0.0, box_size[0]);
    std::uniform_real_distribution<double> dis_y(0.0, box_size[1]);
    std::uniform_real_distribution<double> dis_z(0.0, box_size[2]);

    //Obtain the full number of M-B velocities to be allocated to the atoms later
    std::vector<double> velocities(3 * N_atoms);
    for (unsigned int i = 0; i < velocities.size(); ++i) {
        velocities[i] = atom.gen_velocities(T_init);
    }

    std::vector<Molecule> system;

    for (int i = 0; i < N_atoms; i++) {
        
        Molecule molecule(0.0, 0.0, 0.0); //init a molecule object
        // For LJ only systems, set j < 1
        for (int j = 0; j < 1; j++) {

            atom.x = dis_x(gen);
            atom.y = dis_y(gen);
            atom.z = dis_z(gen);

            // In the following logic, we assess any (partial) overlaps between generated atom coordinates
            // based on Euclidean distance, if within threshold, lower the Atom index by one
            bool overlap = false;
            for (const auto& molcoord : system) {
                for (const auto& atomcoord : molcoord.atoms) {
                    if (atom.euclidean_dist(atom, atomcoord, thres)) {
                        overlap = true;
                        break;
                    }
                }
                if (overlap) {
                    break;
                }
            }

            if (overlap) {
                j--;
                continue;
            }

            atom.vx = velocities[i];
            atom.vy = velocities[3 * i + 1]; //shift the indices to prevent duplicate vels being drawn
            atom.vz = velocities[3 * i + 2];

            molecule.atoms.push_back(atom);
        }
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
