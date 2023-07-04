#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>

// An atom owns Cartesian coordinates, velocities and acceleration information and stores its neighbours in a vector

struct Atom {
    double x,y,z;
    double vx, vy, vz;
    double ax, ay, az;
    std::vector<Atom*> neighborList;
};

//A molecule is a self-contained collection of Atoms. In the case of argon, a Molecule contains a single Atom.
struct Molecule {
    std::vector<Atom> atoms;
};

//Checks overlaps between atoms upon system building
bool check_overlap(const Atom& atom1, const Atom& atom2, double thres);

//Draws velocities from M-B distribution
double gen_velocities(double T_init);

//Sets up system with N atoms, random coordinates and random velocities
std::vector<Molecule> init_system(const std::vector<double>& box_size, int N_atoms, double thres, double T_init);

#endif