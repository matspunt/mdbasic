#ifndef ATOM_HPP
#define ATOM_HPP

#include <vector>
#include <random>
#include <chrono>

class Atom {
public:
    Atom(double x, double y, double z);
    double x, y, z;
    double vx, vy, vz;
    double ax, ay, az;
    double gen_velocities(double T_init);
    bool euclidean_dist(const Atom& atom1, const Atom& atom2, double thres);

private:
    std::vector<Atom*> neighborList;
};

#endif
