# mdbasic

This is a molecular dynamics engine programmed in C++, which focuses on minimal functionality. The sole goal of the project is to understand MD better, from both a theory and implementation perspective, not to write anything competitive (I am too poor a programmer for that anyway). 
In addition, the goal is to implement parallelization to learn more about important HPC frameworks. By implementing parallelization calls (starting kernels, memory allocation etc.) in wrapper functions, it shouldn't be too difficult (fingers crossed) to implement support for multiple abstraction layers such as SYCL or Kokkos but the initial focus is on OpenMP and HIP.

The code should reproduce properties of Lennard-Jones fluids or noble gases such as argon, before implementing bonded topologies which introduce a whole new layer of complexity.  

## Implementation goals:

Required features:

- [x] Initialize coordinates for arbitrary box sizes and number of particles, draw velocities from Maxwell-Boltzmann distribution. 
- [x] Implement coordinate and trajectory writers. 
- [ ] Implement buffered cell/neighbour lists and minimum image convention (for MIC PBC see: https://www.degruyter.com/document/doi/10.1524/zpch.2013.0311/html)
- [ ] Calculate the derivative of the Lennard-Jones potential, and use this to determine the force on each particle (F)
- [ ] Implement the Velocity-Verlet or two step leapfrog algorithm to calculate a = F / m where the mass = 39.962 amu for argon
- [ ] Implement removal of center of mass motion (comm motion should be conserved)
- [ ] Implement a steepest descent minimization, based on the force calculation of the Lennard-Jones potential. 
- [ ] Implement calculation of potential and kinetic energy, show conservation of energy and linear momentum (NVE ensemble). 
- [ ] Implement thermostat and barostat (weak coupling/Berendsen), show conservation of temperature (NVT) & pressure (NPT). 
- [ ] Implement parallelization (focus on single node or desktop grade hardware, OpenMP for CPU, HIP for GPU)

Cool but will make life difficult:

- Implement holonomic constraints and harmonic bonds and angles. 
- Implement MPI for inter node parallelization
- Electrostatics, Ewald summation.
- Fancier thermostats (Bussi and/or Nosé-Hoover thermostat), barostats (Parrinello-Rahman). 

Awesome but never going to happen:

- Different water models, reproduce thermodynamic properties water. 
- Alchemical perturbation calculations (free energies).
- Support for better classical potentials such as Morse potential. 
- Different bonded potentials, e.g. Ryckaert-Bellemans dihedrals
- Analytics (RDF, distance calculations etc.)

## Concerning symplectic integrators

- Ch4. Frenkel & Smit contains description of basic predictor-corrector schemes. 
- Nice graphical showcase of leapfrog: https://www.physics.drexel.edu/~steve/Courses/Comp_Phys/Integrators/leapfrog/

### Concerning neighbour lists

Overview of neighbour lists and cell lists (also, there is basic pseudocode in the book by Frenkel & Smit):

https://en.wikipedia.org/wiki/Cell_lists

https://scicomp.stackexchange.com/questions/29393/md-simulation-reference-for-the-neighbors-list-method

### I/O and file formats

- Adhere to GROMACS formats. Coords as .gro/.xtc, topology as .itp/.top and .xvg files for physical property calculations.
- By using GROMACS formats, VMD can be simply used to visualize the system. 

### Units and physical constants

- For units, adhere to GROMACS: https://manual.gromacs.org/documentation/2023/reference-manual/definitions.html
- The Boltzmann constant is defined as 0.0083144621 $kJ mol^{-1} K^{-1}$
- 1 amu = 1.6606538 * 10^-27 kg

### LJ parameters and argon

- Exhaustive collection of LJ fluid properties here: (https://pubs.acs.org/doi/10.1021/acs.jcim.9b00620)

- Some different parameters for argon here: http://www.sklogwiki.org/SklogWiki/index.php/Argon

- The following link contains some discussion of efficient software implementation of LJ calculation:

https://math.stackexchange.com/questions/1742524/numerical-force-due-to-lennard-jones-potential

- Note: do not forget LJ corrections when utilizing cutoff: shift to zero by subtracting value at cut-off (simplest also implemented in GROMACS), or other correction methods (check Rapaport book). 




