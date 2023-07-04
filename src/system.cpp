#include "system.hpp"
#include <cmath>
#include <random>
#include <chrono>

/**
 * This function draws a single random velocity from Maxwell-Boltzmann distribution
 * based on a specified temperature in Kelvin, set by @param T_init
 */ 
double gen_velocities(double T_init) {
    
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    const double kB = 0.0083144621; // kJ mol^-1 K^-1

    //We can represent M-B as a gamma distribution, with k_shape = 3/2 and θ_scale = kB*T
    std::gamma_distribution<double> maxboltz(3.0 / 2.0, (kB * T_init));

    return maxboltz(generator);
}

/*This helper function for 'init_system' ensures no overlaps are possible when populating the box*/
bool check_overlap(const Atom& atom1, const Atom& atom2, double thres) {
    double dx = atom1.x - atom2.x;
    double dy = atom1.y - atom2.y;
    double dz = atom1.z - atom2.z;
    //calculate Euclidean distance between particles
    double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    return dist < thres;
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
std::vector<Molecule> init_system(const std::vector<double>& box_size, int N_atoms, double thres, double T_init) {
    std::vector<Molecule> system;

    std::random_device rd;
    std::mt19937 gen(rd());
    
    //Draw random coordinates inside of the defined box
    std::uniform_real_distribution<double> dis_x(0.0, box_size[0]);
    std::uniform_real_distribution<double> dis_y(0.0, box_size[1]);
    std::uniform_real_distribution<double> dis_z(0.0, box_size[2]);

    //Obtain the full number of M-B velocities to be allocated to the atoms later
    std::vector<double> velocities(3 * N_atoms);
    for (int i = 0; i < velocities.size(); ++i) {
        velocities[i] = gen_velocities(T_init);
    }

    for (int i = 0; i < N_atoms; i++) {
        Molecule molecule;
        // For LJ only systems, set j < 1
        for (int j = 0; j < 1; j++) {
            Atom atom;

            atom.x = dis_x(gen);
            atom.y = dis_y(gen);
            atom.z = dis_z(gen);

            // In the following logic, we assess any (partial) overlaps between generated atom coordinates
            // based on Euclidean distance, if within treshold, lower the Atom index by one
            bool overlap = false;
            for (const auto& molcoord : system) {
                for (const auto& atomcoord : molcoord.atoms) {
                    if (check_overlap(atom, atomcoord, thres)) {
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
