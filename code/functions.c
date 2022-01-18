#include "functions.h"

//==============================================================================
// Read and load necessary data from a parameter file (parameters.txt)
//==============================================================================
void load_parameters(char *paramPath, int *_dim, int *_nbNodesByRow, int *_nbNodesByCol, int *_nbCells, int *_nbTypes, int **_nbCellsOfType, int *_nbStages, int *_nbSteps, double *_dt, double **_sigmaStages, double **_typeTension, double *_osc, double *_epsilonMin, double *_alpha, double *_epsilon0, int *_nbLocalNodes) {
	printf("[0] Loading parameters...\n");

	FILE* paramFile = NULL;
	paramFile = fopen(paramPath, "r");
	assert(paramFile != NULL);

	int dim, nbNodesByRow, nbNodesByCol;
	int nbCells, nbTypes, nbStages, nbSteps, nbLocalNodes;
	double dt, osc, epsilonMin, alpha, epsilon0;

	char line[128] = "";
	while (fgets(line, 128, paramFile) != NULL) {
		if (strstr(line, "$Dimension") != NULL) {
			fscanf(paramFile, "%d\n", &dim);
			*_dim = dim;
		}
		if (strstr(line, "$NodesByRow") != NULL) {
			fscanf(paramFile, "%d\n", &nbNodesByRow);
			*_nbNodesByRow = nbNodesByRow;
		}
		if (strstr(line, "$NodesByCol") != NULL) {
			fscanf(paramFile, "%d\n", &nbNodesByCol);
			*_nbNodesByCol = nbNodesByCol;
		}
		if (strstr(line, "$NumberStages") != NULL) {
			fscanf(paramFile, "%d\n", &nbStages);
			*_nbStages = nbStages;
		}
		if (strstr(line, "$NbStepsPerStage") != NULL) {
			fscanf(paramFile, "%d\n", &nbSteps);
			*_nbSteps = nbSteps;
		}
		if (strstr(line, "$TimeStepSize") != NULL) {
			fscanf(paramFile, "%lf\n", &dt);
			*_dt = dt;
		}
		if (strstr(line, "$NumberCells") != NULL) {
			fscanf(paramFile, "%d\n", &nbCells);
			*_nbCells = nbCells;
		}
		if (strstr(line, "$NumberTypes") != NULL) {
			fscanf(paramFile, "%d\n", &nbTypes);
			*_nbTypes = nbTypes;
		}
		if (strstr(line, "$NbCellsPerType") != NULL) {
			int *nbCellsOfType = (int *) malloc(nbTypes*sizeof(int));
			for (int i = 0; i < nbTypes; i++) {
				fscanf(paramFile, "%d\n", &nbCellsOfType[i]);
			}
			*_nbCellsOfType = nbCellsOfType;
		}
		if (strstr(line, "$SigmaPerStage") != NULL) {
			int nbSigmas = 0.5*nbTypes*(nbTypes+1);
			double *sigmaStages = (double *) malloc((nbStages+1)*nbSigmas*sizeof(double));
			for (int i = 0; i < nbStages+1; i++) {
				for (int j = 0; j < nbSigmas; j++) {
					fscanf(paramFile, "%lf\n", &sigmaStages[i*nbSigmas+j]);
				}
			}
			*_sigmaStages = sigmaStages;

			FILE *sigmaFile = NULL;
			sigmaFile = fopen("output/sigmavalues.txt", "w");
			assert(sigmaFile != NULL);
			for (int l = 0; l < nbSigmas; l++) {
				fprintf(sigmaFile, "%lf ", sigmaStages[l]);
			}
			fprintf(sigmaFile, "\n");
		  fclose(sigmaFile);

			double *typeTension = (double *) malloc(nbTypes*nbTypes*sizeof(double));
			int k = 0;
			for (int j = 0; j < nbTypes; j++) {
				for (int i = 0; i <= j; i++) {
					typeTension[i*nbTypes+j] = sigmaStages[k];
					typeTension[j*nbTypes+i] = sigmaStages[k];
					k += 1;
				}
			}
			*_typeTension = typeTension;
		}
		if (strstr(line, "$Fluctuation") != NULL) {
			fscanf(paramFile, "%lf\n", &osc);
			*_osc = osc;
		}
		if (strstr(line, "$ErrorTolerance") != NULL) {
			fscanf(paramFile, "%lf\n", &epsilonMin);
			*_epsilonMin = epsilonMin;
		}
    if (strstr(line, "$EpsilonScalingFactor") != NULL) {
			fscanf(paramFile, "%lf\n", &alpha);
			*_alpha = alpha;
		}
    if (strstr(line, "$InitialEpsilonValue") != NULL) {
			fscanf(paramFile, "%lf\n", &epsilon0);
			*_epsilon0 = epsilon0;
		}
		if (strstr(line, "$NumberLocalNodes") != NULL) {
			fscanf(paramFile, "%d\n", &nbLocalNodes);
			*_nbLocalNodes = nbLocalNodes;
		}
	}

	fclose(paramFile);
}

//==============================================================================
// Read and load initial configuration data (initconfig.txt)
//==============================================================================
void load_initconfig(char *initconfigPath, int nbNodes, int nbCells, int **_typeOfCell, int **_isAssigned, int **_volume, double **_phi) {
	printf("[0] Loading initial configuration...\n");

	FILE* initconfigFile = NULL;
	initconfigFile = fopen(initconfigPath, "r");
	assert(initconfigFile != NULL);

	char line[128] = "";
	while (fgets(line, 128, initconfigFile) != NULL) {
		if (strstr(line, "$TypeOfCells") != NULL) {
      int cell;
			int *typeOfCell = (int *) malloc(nbCells*sizeof(int));
			for (int p = 0; p < nbCells; p++) {
				fscanf(initconfigFile, "%d %d\n", &cell, &typeOfCell[p]);
			}
			*_typeOfCell = typeOfCell;
		}
		if (strstr(line, "$NodeCellRefNum") != NULL) {
			int cell;
			int *isAssigned = (int *) malloc(nbNodes*sizeof(int));
			int *volume = (int *) malloc(nbCells*sizeof(int));
			for (int p = 0; p < nbCells; p++) volume[p] = 0;

			double *phi = (double *) malloc(nbCells*nbNodes*sizeof(double));
			for (int k = 0; k < nbCells*nbNodes; k++) phi[k] = 0.0;

			for (int l = 0; l < nbNodes; l++) {
				fscanf(initconfigFile, "%d\n", &cell);
				phi[cell*nbNodes+l] = 1.0;
				volume[cell] += 1;
				isAssigned[l] = cell;
			}
			*_phi = phi;
			*_volume = volume;
			*_isAssigned = isAssigned;
		}
	}
	fclose(initconfigFile);
}

//==============================================================================
// Display parameter and configuration data
//==============================================================================
void display_data(int dim, int nbNodes, int nbCells, int nbTypes, double dt, double *typeTension) {
	printf("\tDimension: %d\n", dim);
	printf("\tNumber of Nodes: %d\n", nbNodes);
	printf("\tNumber of Cells: %d\n", nbCells);
	printf("\tNumber of Cell Types: %d\n", nbTypes);
	printf("\tTime Step Size: %lf\n", dt);

	printf("\tTension Values by Types:\n");
	for (int i = 0; i < nbTypes; i++) {
    for (int j = 0; j < nbTypes; j++) printf("\t  %.5lf", typeTension[i*nbTypes+j]);
    printf("\n");
  }
  printf("\n");
}

//==============================================================================
// Setup tension values by cell type
//==============================================================================
void setup_tension(int stage, int step, int nbSteps, int nbTypes, double *sigmaStages, double *typeTension) {
	// printf("[%d] Configuring tension values by cell type...\n", stage*nbSteps+step+1);

	FILE *sigmaFile = NULL;
	sigmaFile = fopen("output/sigmavalues.txt", "a");
	assert(sigmaFile != NULL);

  int nbSigmas = 0.5*nbTypes*(nbTypes+1);
	double *tensionList = (double *) malloc(nbSigmas*sizeof(double));

	for (int i = 0; i < nbSigmas; i++) {
		double slope = (sigmaStages[(stage+1)*nbSigmas+i] - sigmaStages[stage*nbSigmas+i])/nbSteps;
		tensionList[i] = sigmaStages[stage*nbSigmas+i] + slope*(step+1);
	}

	int k = 0;
	for (int j = 0; j < nbTypes; j++) {
		for (int i = 0; i <= j; i++) {
			typeTension[i*nbTypes+j] = tensionList[k];
			typeTension[j*nbTypes+i] = tensionList[k];
			k += 1;
		}
	}

	for (int l = 0; l < nbSigmas; l++) {
		fprintf(sigmaFile, "%lf ", tensionList[l]);
	}
	fprintf(sigmaFile, "\n");
	fclose(sigmaFile);

	// for (int i = 0; i < nbTypes; i++) {
	//   for (int j = 0; j < nbTypes; j++) printf("\t  %.5lf", typeTension[i*nbTypes+j]);
	//   printf("\n");
	// }

  free(tensionList);
}

//==============================================================================
// Build sigma matrix (tension by cells)
//==============================================================================
void build_sigma(int step, int nbCells, int nbTypes, double dt, double osc, double **_sigma, double *typeTension, int *typeOfCell) {
	// printf("[%d] Building cell-by-cell tension matrix...\n", step);
  double *sigma = (double *) malloc(nbCells*nbCells*sizeof(double));

	double val = exp(-16*(dt*step-1.0));
	if (val<1.0) osc = val*osc;

  for (int i = 0; i < nbCells; i++) {
    for (int j = 0; j < nbCells; j++) {
      if (i == j) sigma[i*nbCells+j] = 0.0;
      else sigma[i*nbCells+j] = typeTension[typeOfCell[i]*nbTypes+typeOfCell[j]]*(1.0 + osc*sin((double)((7*i+9*j)%(16*nbCells)/nbCells)*step*dt));
		}
  }
  *_sigma = sigma;
}

//==============================================================================
// Compute convolution using FFTW3 for a 2D configuration
//==============================================================================
void convolution(int dim, int nbNodes, int nbNodesByRow, int nbNodesByCol, int nbCmplxNodes, int nbCells, int normCoef, double PI, double freqStepX, double freqStepY, double dt, double *sigma, double *phi, fftw_complex *ft_phi, fftw_plan forward, fftw_plan backward) {
	// Define phi-function
	double *phi_copy = (double *) malloc(nbCells*nbNodes*sizeof(double));
	for (int k = 0; k < nbCells*nbNodes; k++) {
		phi_copy[k] = phi[k];
		phi[k] = 0.;
	}
	for (int i = 0; i < nbCells; i++) {
		for (int j = 0; j < nbCells; j++) {
			for (int l = 0; l < nbNodes; l++) {
				phi[i*nbNodes+l] = phi[i*nbNodes+l] + sigma[i*nbCells+j]*phi_copy[j*nbNodes+l];
			}
		}
	}

	// Compute the convolution
	fftw_execute(forward);
	int i, j, k;
	double freqStep;
	for (int p = 0; p < nbCells; p++) {
		for (int l = 0; l < nbCmplxNodes; l++) {
			i = l%((int)(nbNodesByCol/2)+1);
      j = (l/((int)(nbNodesByCol/2)+1));
			if (dim==3) j%=nbNodesByRow;
			if (j >= ((int)(nbNodesByRow/2)+1)) j = nbNodesByRow-j;
			if (dim==3) {
      	k = l/(((int)(nbNodesByRow/2)+1)*nbNodesByRow);
				if (k >= ((int)(nbNodesByRow/2)+1)) k = nbNodesByRow-k;
			}
			if (dim==2) freqStep = freqStepX*freqStepX*i*i + freqStepY*freqStepY*j*j;
			if (dim==3) freqStep = freqStepX*freqStepX*freqStepX*(i*i + j*j + k*k);
      ft_phi[p*nbCmplxNodes + l] = exp(-4*PI*PI*dt*freqStep)*ft_phi[p*nbCmplxNodes + l]/normCoef;
		}
	}
	fftw_execute(backward);

	free(phi_copy);
}

//==============================================================================
// Preserve cell connectivity by flagging neighboring cells
//==============================================================================
void preserve_connectivity(int dim, int nbNodes, int nbCells, int nbNodesByRow, int nbNodesByCol, int nbLocalNodes, int *isAssigned, double *phi) {
	int nbNodesHeight = 1;
	if (dim==3) nbNodesHeight = nbNodesByRow;

	// flag neighboring cells (row-wise) of each node
	int *sumRow = (int *) malloc(nbCells*nbNodes*sizeof(int));
	for (int i = 0; i < nbCells*nbNodes; i++) sumRow[i] = 0;
	// start with leftmost node of each row
	for (int h = 0; h < nbNodesHeight; h++) {
		for (int r = 0; r < nbNodesByRow; r++) {
			int l = r*nbNodesByCol + h*nbNodesByRow*nbNodesByCol;
			for (int i = -nbLocalNodes; i < nbLocalNodes+1; i++) {
				int tmp = l+i +nbNodesByCol*((l+i)<l);
				sumRow[isAssigned[tmp]*nbNodes+l] += 1;
			}
		}
	}
	// move to other nodes on each row
	for (int h = 0; h < nbNodesHeight; h++) {
		for (int r = 0; r < nbNodesByRow; r++) {
			for (int c = 1; c < nbNodesByCol; c++) {
				int l = r*nbNodesByCol + c + h*nbNodesByRow*nbNodesByCol;
				int tmp1 = l-1-nbLocalNodes +nbNodesByCol*((l-1-nbLocalNodes)<(r*nbNodesByCol + h*nbNodesByRow*nbNodesByCol));
				int tmp2 = l+nbLocalNodes -nbNodesByCol*((l+nbLocalNodes)>((r+1)*nbNodesByCol-1 + h*nbNodesByRow*nbNodesByCol));
				for (int p = 0; p < nbCells; p++) sumRow[p*nbNodes + l] = sumRow[p*nbNodes + l-1];
				sumRow[isAssigned[tmp1]*nbNodes+l] -= 1;
				sumRow[isAssigned[tmp2]*nbNodes+l] += 1;
			}
		}
	}

	// flag cells in square neighborhood of each node (using row info)
	int *sumSquare = (int *) malloc(nbCells*nbNodes*sizeof(int));
	for (int i = 0; i < nbCells*nbNodes; i++) sumSquare[i] = 0;
	// start with first node of each column
	for (int h = 0; h < nbNodesHeight; h++) {
		for (int c = 0; c < nbNodesByCol; c++) {
			int l = c + h*nbNodesByRow*nbNodesByCol;
			for (int i = -nbLocalNodes; i < nbLocalNodes+1; i++) {
				int tmp = l+i*nbNodesByCol + nbNodesByRow*nbNodesByCol*((l+i*nbNodesByCol)<h*nbNodesByRow*nbNodesByCol);
				for (int p = 0; p < nbCells; p++) sumSquare[p*nbNodes + l] += sumRow[p*nbNodes + tmp];
			}
		}
	}
	// move to other nodes on each column
	for (int h = 0; h < nbNodesHeight; h++) {
		for (int r = 1; r < nbNodesByRow; r++) {
			for (int c = 0; c < nbNodesByCol; c++) {
				int l = r*nbNodesByCol + c + h*nbNodesByRow*nbNodesByCol;
				int tmp1 = l-(nbLocalNodes+1)*nbNodesByCol + nbNodesByRow*nbNodesByCol*((l-(nbLocalNodes+1)*nbNodesByCol)<h*nbNodesByRow*nbNodesByCol);
				int tmp2 = l+nbLocalNodes*nbNodesByCol - nbNodesByRow*nbNodesByCol*((l+nbLocalNodes*nbNodesByCol)>=(h+1)*nbNodesByRow*nbNodesByCol);
				for (int p = 0; p < nbCells; p++) {
					sumSquare[p*nbNodes + l] = sumSquare[p*nbNodes + (r-1)*nbNodesByCol + c + h*nbNodesByRow*nbNodesByCol];
					sumSquare[p*nbNodes + l] -= sumRow[p*nbNodes + tmp1];
					sumSquare[p*nbNodes + l] += sumRow[p*nbNodes + tmp2];
				}
			}
		}
	}
	free(sumRow);

	// flag cells in cubic neighborhood of each node (using square info)
	if (nbNodesHeight>1) {
		int *sumCube = (int *) malloc(nbCells*nbNodes*sizeof(int));
		for (int i = 0; i < nbCells*nbNodes; i++) sumCube[i] = 0;
		// start with base nodes
		for (int l = 0; l < nbNodesByRow*nbNodesByCol; l++) {
			for (int i = -nbLocalNodes; i < nbLocalNodes+1; i++) {
					int tmp = l+i*nbNodesByRow*nbNodesByCol + nbNodes*((l+i*nbNodesByRow*nbNodesByCol)<0);
					for (int p = 0; p < nbCells; p++) sumCube[p*nbNodes + l] += sumSquare[p*nbNodes + tmp];
			}
		}
		// move to other nodes
		for (int i = 0; i < nbNodesByRow*nbNodesByCol; i++) {
			for (int h = 1; h < nbNodesHeight; h++) {
				int l = i + h*nbNodesByRow*nbNodesByCol;
				int tmp1 = l-(nbLocalNodes+1)*nbNodesByRow*nbNodesByCol + nbNodes*((l-(nbLocalNodes+1)*nbNodesByRow*nbNodesByCol)<0);
				int tmp2 = l+nbLocalNodes*nbNodesByRow*nbNodesByCol - nbNodes*((l+nbLocalNodes*nbNodesByRow*nbNodesByCol)>=nbNodes);
				for (int p = 0; p < nbCells; p++) {
					sumCube[p*nbNodes + l] = sumCube[p*nbNodes + l-nbNodesByRow*nbNodesByCol];
					sumCube[p*nbNodes + l] -= sumSquare[p*nbNodes + tmp1];
					sumCube[p*nbNodes + l] += sumSquare[p*nbNodes + tmp2];
				}
			}
		}

		// set phi large for cells away from nodal neighborhood
		for (int l = 0; l < nbCells*nbNodes; l++) {
			if (sumCube[l]==0) phi[l] = 1000.0;
		}
		free(sumCube);
	}
	else {
		for (int l = 0; l < nbCells*nbNodes; l++) {
			if (sumSquare[l]==0) phi[l] = 1000.0;
		}
	}

	free(sumSquare);
}


//==============================================================================
// Insert key to bid heap of given cell
//==============================================================================
void insert_bid(int key, int cell, int maxCellvolume, int *heapLen, int *bidkeyHeap, double *bid) {
  int i = heapLen[cell];
  bidkeyHeap[cell*maxCellvolume + i] = key;
  heapLen[cell]+=1;

  // Swim up to maintain heap property
  while (i>0 && bid[bidkeyHeap[cell*maxCellvolume + i]] < bid[bidkeyHeap[cell*maxCellvolume + (i-1)/2]]) {
    // Swap parent and current node
		int nodkey = bidkeyHeap[cell*maxCellvolume + i];
    bidkeyHeap[cell*maxCellvolume + i] = bidkeyHeap[cell*maxCellvolume + (i-1)/2];
    bidkeyHeap[cell*maxCellvolume + (i-1)/2] = nodkey;

    // Update to its parent
    i = (i-1)/2;
  }
}


//==============================================================================
// Replace smallest bidder in the cell by given node
//==============================================================================
void heapify_bid(int cell, int maxCellvolume, int *heapLen, int *bidkeyHeap, double *bid) {
  // Sink down to maintain heap property
  int i = 0;
  int j = 1;
  int nodkey = bidkeyHeap[cell*maxCellvolume + i];
  int chdkey = bidkeyHeap[cell*maxCellvolume + j];
  if (bid[bidkeyHeap[cell*maxCellvolume + j+1]] < bid[chdkey]) j+=1;

  while (j<heapLen[cell] && bid[nodkey] > bid[bidkeyHeap[cell*maxCellvolume + j]]) {
    // Swap smaller child and current node
    bidkeyHeap[cell*maxCellvolume + i] = bidkeyHeap[cell*maxCellvolume + j];
    bidkeyHeap[cell*maxCellvolume + j] = nodkey;

    // Update to its child
    i = j;
		j = 2*i+1;
    nodkey = bidkeyHeap[cell*maxCellvolume + i];
		chdkey = bidkeyHeap[cell*maxCellvolume + j];

    // Check if there are two children
    if (j+1<heapLen[cell] && bid[bidkeyHeap[cell*maxCellvolume + j+1]] < bid[chdkey]) j+=1;
  }
}


//==============================================================================
// Run standard auction dynamics with priority queue
//==============================================================================
void preserve_cellvolume(int nbCells, int nbNodes, int maxCellvolume, int *heapLen, int *bidkeyHeap, int *isAssigned, int *currentVolume, int *volume, double alpha, double epsilon0, double epsilonBar, double *phi, double *price, double *bid) {
	int nbAssignedNodes;
  double epsilon = epsilon0;

  // Initialize cell prices
  for (int p = 0; p < nbCells; p++) price[p] = 0.;

  // Convert to a maximation problem
	for (int n = 0; n < nbCells*nbNodes; n++) phi[n] = 1.-phi[n];

  while (epsilon >= epsilonBar) {
    // printf("\teps: %.3e\n",epsilon);
    // Initialize: membership auction
    nbAssignedNodes = 0;
    for (int l = 0; l < nbNodes; l++) {
      isAssigned[l] = -1; // -1 means the node is not assigned
    }
    for (int p = 0; p < nbCells; p++) {
      currentVolume[p] = 0;
    }

		// Initialize heap data
		for (int p = 0; p < nbCells; p++) heapLen[p] = 0;
		for (int p = 0; p < nbCells*maxCellvolume; p++) bidkeyHeap[p] = -1;

    // Begin membership auction
		int l = 0;
    while (nbAssignedNodes < nbNodes) {
      // Find an unassigned node
      while (l < nbNodes && isAssigned[l] != -1)  l += 1;
      if (l == nbNodes) {
        l = 0;
        while (l < nbNodes && isAssigned[l] != -1) l += 1;
        if (l >= nbNodes) {
          printf("\n[ERROR] Check auction dynamics.\n");
          exit(-1);
        }
      }

      int pStar = 0;	// preferred cell
      int pNext = 1;	// next preferred cell
      if (phi[pStar*nbNodes+l]-price[pStar] < phi[pNext*nbNodes+l]-price[pNext]) {
        pStar = 1;
        pNext = 0;
      }
      for (int p = 2; p < nbCells; p++) {
        if (phi[p*nbNodes+l]-price[p] > phi[pStar*nbNodes+l]-price[pStar]) {
          pNext = pStar;
          pStar = p;
        }
        else if ((phi[p*nbNodes+l]-price[p] <= phi[pStar*nbNodes+l]-price[pStar]) && (phi[p*nbNodes+l]-price[p] > phi[pNext*nbNodes+l]-price[pNext]) ) {
          pNext = p;
        }
      }

      // Calculate bid of corresponding node
      bid[l] = epsilon + phi[pStar*nbNodes+l] - phi[pNext*nbNodes+l] + price[pNext];

      // Assignment
      if (currentVolume[pStar] == volume[pStar]) {
        // Replace smallest bidder y by node l
				isAssigned[bidkeyHeap[pStar*maxCellvolume]] = -1;

				isAssigned[l] = pStar;
			  bidkeyHeap[pStar*maxCellvolume] = l;
        heapify_bid(pStar, maxCellvolume, heapLen, bidkeyHeap, bid);

        price[pStar] = bid[bidkeyHeap[pStar*maxCellvolume]];
      }
      else {
        insert_bid(l, pStar, maxCellvolume, heapLen, bidkeyHeap, bid);
			  isAssigned[l] = pStar;
        currentVolume[pStar] += 1;
        nbAssignedNodes += 1;

        if (currentVolume[pStar] == volume[pStar]) {
          price[pStar] = bid[bidkeyHeap[pStar*maxCellvolume]];
        }
      }
    }

    // Implement epsilon scaling
    epsilon = epsilon/alpha;
    if (epsilon < epsilonBar) {
			// Reinitialize phi
      for (int k = 0; k < nbCells*nbNodes; k++) {
        phi[k] = 0.0;
      }
      // Save new configuration
      for (int l = 0; l < nbNodes; l++) {
        int cell = isAssigned[l];
        phi[cell*nbNodes+l] = 1.0;
      }
    }
  }
}


//==============================================================================
// Export solution to corresponding 'output' folder
//==============================================================================
void export_solution(int step, int nbNodes, int *isAssigned) {
  char fileName[228];
	sprintf(fileName, "output/sol%04d.txt", step);
	printf("[%d] Exporting solution to '%s'\n", step, fileName);

  FILE *solFile = NULL;
  solFile = fopen(fileName, "w");
  assert(solFile != NULL);

	for (int l = 0; l < nbNodes; l++) {
		fprintf(solFile, "%d\n", isAssigned[l]);
	}

  fclose(solFile);
}
