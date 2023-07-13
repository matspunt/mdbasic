#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "atom.hpp"
#include <vector>

class Molecule {
public:
    Molecule(double x, double y, double z, double vx, double vy, double vz);
    std::vector<Atom> atoms;
    static std::vector<Molecule> init_system(const std::vector<double>& box_size, int N_atoms, double thres, double T_init);

private:
};

class System {
public:
    System(int numMols);
    static void write_coord_vel(const std::vector<Molecule>& system, const std::string& filename, const std::vector<double>& box_size);

private:
    std::vector<Molecule> molecules;
};

#endif
