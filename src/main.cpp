#include "atom.hpp"
#include "system.hpp"
#include <iostream>
#include <vector>

int main() {
    std::vector<double> box_size{5.0, 5.0, 5.0}; // cubic box, 125 nm^3
    int N_atoms = 80;
    double T_init = 310; // in Kelvin

    double atom_overlap_spacing = 0.2; // ensure rel. spacing of 0.2 nm between each atom.

    std::vector<Molecule> system = Molecule::init_system(box_size, N_atoms, atom_overlap_spacing, T_init);

    std::string gro_file = "output.gro";
    System::write_coord_vel(system, gro_file, box_size);

    std::cout << "Generated coords: " << gro_file << "\n";

    return 0;
}
