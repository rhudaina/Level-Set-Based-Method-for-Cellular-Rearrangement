//==============================================================================
/*  This program implements our proposed level set-based method for cellular
    rearrangement (by minimizing a volume-constrained weighted surface energy)

    Input: initial configuration data (initconfig.txt)
           parameter data file (parameters.txt)

    Output: solution files (assigned cell at each node)
*/
//==============================================================================
#include "functions.h"
#define PI 3.141592653589793238

int main(int argc, char const *argv[]) {
  //============================================================================
  clock_t time_start, time_end;
  double time_elapsed;
  time_start = clock();

  if (argc > 2) {
    //============================================================================
    // Initialize: setup parameters, initial configuration data, and FFTW plans
    //============================================================================
    int dim, nbNodesByRow, nbNodesByCol;
    int nbCells, nbTypes, nbStages, nbSteps, nbLocalNodes;
    double dt, osc, epsilonMin, alpha, epsilon0;

    int *nbCellsOfType = NULL;
    int *typeOfCell = NULL;
    int *isAssigned = NULL;
    int *volume = NULL;
    double *phi = NULL;
    double *sigmaStages = NULL;
    double *typeTension = NULL;

    // Load parameters
    char paramPath[228];
    strcpy(paramPath, argv[1]);
    load_parameters(paramPath, &dim, &nbNodesByRow, &nbNodesByCol, &nbCells, &nbTypes, &nbCellsOfType, &nbStages, &nbSteps, &dt, &sigmaStages, &typeTension, &osc, &epsilonMin, &alpha, &epsilon0, &nbLocalNodes);

    // Load initial configuration data
    char initconfigPath[228];
    strcpy(initconfigPath, argv[2]);
    int nbNodes = nbNodesByRow*nbNodesByCol;
    if (dim==3) nbNodes *= nbNodesByRow;
    load_initconfig(initconfigPath, nbNodes, nbCells, &typeOfCell, &isAssigned, &volume, &phi);

    // Build cell-by-cell sigma matrix only when constant throughout evolution
    double *sigma = (double *) malloc(nbCells*nbCells*sizeof(double));
    if (nbStages==1) build_sigma(0, nbCells, nbTypes, dt, osc, &sigma, typeTension, typeOfCell);

    // Preparing priority queue data
    int maxCellvolume = volume[0];
    for (int p = 1; p < nbCells; p++) {
      if (volume[p]>maxCellvolume) maxCellvolume = volume[p];
    }
    int *heapLen = (int *) malloc(nbCells*sizeof(int));
    int *bidkeyHeap = (int *) malloc(nbCells*maxCellvolume*sizeof(int));

    // Prepare auction variables
    int *currentVolume = (int *) malloc(nbCells*sizeof(int));
    double epsilonBar = epsilonMin/nbNodes;
    double *price = (double *) malloc(nbCells*sizeof(double));
    double *bid = (double *) malloc(nbNodes*sizeof(double));

    // Define FFTW plans
    int normCoef = nbCells*nbNodes;
    int nbCmplxNodes = ((int)(nbNodesByCol/2)+1)*nbNodesByRow;
    if (dim==3) nbCmplxNodes *= nbNodesByRow;
    int *nbPerDim = (int *) malloc((dim+1)*sizeof(int));
    for (int i = 1; i < dim+1; i++) nbPerDim[i] = nbNodesByRow;
    nbPerDim[0] = nbCells;
    if (dim==2) nbPerDim[2] = nbNodesByCol;

    double freqStepX = (nbNodesByCol-1.)/nbNodesByCol;
    double freqStepY = (nbNodesByCol-1.)/nbNodesByRow;
    fftw_complex *ft_phi = fftw_malloc(nbCells*nbCmplxNodes*sizeof(fftw_complex));
    fftw_plan forward;
    if (dim==2) forward = fftw_plan_dft_r2c_3d(nbCells, nbNodesByRow, nbNodesByCol, phi, ft_phi, FFTW_MEASURE);
    if (dim==3) forward = fftw_plan_dft_r2c(4, nbPerDim, phi, ft_phi, FFTW_MEASURE);
    fftw_plan backward;
    if (dim==2) backward = fftw_plan_dft_c2r_3d(nbCells, nbNodesByRow, nbNodesByCol, ft_phi, phi, FFTW_MEASURE);
    if (dim==3) backward = fftw_plan_dft_c2r(4, nbPerDim, ft_phi, phi, FFTW_MEASURE);

    // Export initial configuration to output
    export_solution(0, nbNodes, isAssigned);
    display_data(dim, nbNodes, nbCells, nbTypes, dt, typeTension);

    // ============================================================================
    // Run and export solution
    // ============================================================================
    printf("\nStarting evolution...\n");
    for (int l = 0; l < nbStages; l++) {
      for (int k = 0; k < nbSteps; k++) {
        // Build cell-by-cell sigma matrix varying throughout evolution
        if (nbStages>1) {
          setup_tension(l, k, nbSteps, nbTypes, sigmaStages, typeTension);
          build_sigma(l*nbSteps+k+1, nbCells, nbTypes, dt, osc, &sigma, typeTension, typeOfCell);
        }

        // Compute convolution using FFTW
        convolution(dim, nbNodes, nbNodesByRow, nbNodesByCol, nbCmplxNodes, nbCells, normCoef, PI, freqStepX, freqStepY, dt, sigma, phi, ft_phi, forward, backward);

        // Preserve cell connectivity through local auctions
        preserve_connectivity(dim, nbNodes, nbCells, nbNodesByRow, nbNodesByCol, nbLocalNodes, isAssigned, phi);

        // Preserve cell volumes using auction dynamics
        preserve_cellvolume(nbCells, nbNodes, maxCellvolume, heapLen, bidkeyHeap, isAssigned, currentVolume, volume, alpha, epsilon0, epsilonBar, phi, price, bid);
        //
        // Export solution to output
        export_solution(l*nbSteps+k+1, nbNodes, isAssigned);
      }
    }

    // ======================================================================
    // Deallocate memory
    // ======================================================================
    free(nbCellsOfType);
    free(typeOfCell);
    free(isAssigned);
    free(volume);
    free(phi);
    free(typeTension);
    free(sigma);
    free(heapLen);
    free(bidkeyHeap);
    free(currentVolume);
    free(price);
    free(bid);
    free(nbPerDim);

    fftw_free(ft_phi);
    fftw_destroy_plan(forward);
    fftw_destroy_plan(backward);
  }
  else {
    printf("[ERROR] Can not find required input files: parameters.txt, initconfig.txt, and meshdata.txt\n");
  }

  // ======================================================================
  // Measure time elapsed
  // ======================================================================
  printf("\nTIME:\n");
  time_end = clock();
  time_elapsed = (double) (time_end - time_start) / CLOCKS_PER_SEC;
  printf("%lf\n", time_elapsed);

  return 0;
}
