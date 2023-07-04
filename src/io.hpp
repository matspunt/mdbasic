#ifndef IO_HPP
#define IO_HPP

#include "system.hpp"
#include <vector>
#include <string>

//Writes coordinates and velocities to a GROMACS compliant .gro file
void write_coord_vel(const std::vector<Molecule>& system, const std::string& filename, const std::vector<double>& box_size);

#endif
