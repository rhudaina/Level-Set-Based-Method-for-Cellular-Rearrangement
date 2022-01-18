#ifndef FUNCTIONS_HEADER
#define FUNCTIONS_HEADER

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <complex.h>
#include <fftw3.h>

// #define min(a,b) (((a)<(b))?(a):(b))

// Read and load necessary data from a parameter file (parameters.txt)
void load_parameters(char *paramPath, int *_dim, int *_nbNodesByRow, int *_nbNodesByCol, int *_nbCells, int *_nbTypes, int **_nbCellsOfType, int *_nbStages, int *_nbSteps, double *_dt, double **_sigmaStages, double **_typeTension, double *_osc, double *_epsilonMin, double *_alpha, double *_epsilon0, int *_nbLocalNodes);

// Read and load initial configuration data (initconfig.txt)
void load_initconfig(char *initconfigPath, int nbNodes, int nbCells, int **_typeOfCell, int **_isAssigned, int **_volume, double **_phi);

// Display parameter and configuration data
void display_data(int dim, int nbNodes, int nbCells, int nbTypes, double dt, double *typeTension);

// Setup tension values by cell type
void setup_tension(int stage, int step, int nbSteps, int nbTypes, double *sigmaStages, double *typeTension);

// Build sigma matrix (tension by cells)
void build_sigma(int step, int nbCells, int nbTypes, double dt, double osc, double **_sigma, double *typeTension, int *typeOfCell);

// Compute convolution using FFTW3 for a 2D configuration
void convolution(int dim, int nbNodes, int nbNodesByRow, int nbNodesByCol, int nbCmplxNodes, int nbCells, int normCoef, double PI, double freqStepX, double freqStepY, double dt, double *sigma, double *phi, fftw_complex *ft_phi, fftw_plan forward, fftw_plan backward);

// Preserve cell connectivity by flagging neighboring cells
void preserve_connectivity(int dim, int nbNodes, int nbCells, int nbNodesByRow, int nbNodesByCol, int nbLocalNodes, int *isAssigned, double *phi);

// Insert key to bid heap of given cell
void insert_bid(int key, int cell, int maxCellvolume, int *heapLen, int *bidkeyHeap, double *bid);

// Replace smallest bidder in the cell by given node
void heapify_bid(int cell, int maxCellvolume, int *heapLen, int *bidkeyHeap, double *bid);

// Run standard auction dynamics with priority queue
void preserve_cellvolume(int nbCells, int nbNodes, int maxCellvolume, int *heapLen, int *bidkeyHeap, int *isAssigned, int *currentVolume, int *volume, double alpha, double epsilon0, double epsilonBar, double *phi, double *price, double *bid);

// Export solution to corresponding 'output' folder
void export_solution(int step, int nbNodes, int *isAssigned);

#endif
