#include "atom.hpp"

Atom::Atom(double x, double y, double z)
    : x(x), y(y), z(z), vx(0.0), vy(0.0), vz(0.0), ax(0.0), ay(0.0), az(0.0) {
}

/**
 * This function draws a single random velocity from Maxwell-Boltzmann distribution
 * based on a specified temperature in Kelvin, set by @param T_init
 */ 
double Atom::gen_velocities(double T_init) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);

    const double kB = 0.0083144621; // kJ mol^-1 K^-1

    // We can represent M-B as a gamma distribution, with k_shape = 3/2 and θ_scale = kB*T
    std::gamma_distribution<double> maxboltz(3.0 / 2.0, (kB * T_init));

    return maxboltz(generator);
}

/*This helper function for 'Molecule::init_system' ensures no overlaps are possible when populating the box*/
bool Atom::euclidean_dist(const Atom& atom1, const Atom& atom2, double thres) {

    double dx = atom1.x - atom2.x;
    double dy = atom1.y - atom2.y;
    double dz = atom1.z - atom2.z;

    //calculate Euclidean distance between particles
    double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
    return dist < thres;
}