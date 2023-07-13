#ifndef ATOM_HPP
#define ATOM_HPP

#include <vector>
#include <random>
#include <chrono>

class Atom {
public:
    Atom(double x, double y, double z, double vx, double vy, double vz);
    double x, y, z;
    double vx, vy, vz;
    double ax, ay, az;
    static double gen_velocities(double T_init);
    static bool euclidean_dist(const Atom& atom1, const Atom& atom2, double thres);

private:
    std::vector<Atom*> neighborList;
};

#endif
