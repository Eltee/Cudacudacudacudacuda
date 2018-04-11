#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "cuda_runtime.h"

int main(int argc, char *argv[])
{

	if (argc != 5)
	{
		printf("Pas assez d'arguments.\n");
		exit(1);
	}

	int n, m, np, nbproc, i, j, k;
	double h, td;
	//dickbutt


	int init, dim;



	sscanf(argv[1], "%i", &n);
	sscanf(argv[2], "%i", &m);
	sscanf(argv[3], "%i", &np);
	td = atof(argv[4]);
	h = atof(argv[5]);

	//Sequentiel

	codeSequentiel(n, m, np, td, h);


	//Parallele
	double dim = sqrt(n*m);
	dim3 DimGrid(n, m, np);
	dim3 DimBlock(dim, dim, 1);
	float * mat;
	float * matDest;

	cudaMalloc((void **)&mat, n*m *np * sizeof(float));
	cudaMemcpy(matDest, mat, n*m * sizeof(float), cudaMemcpyHostToDevice);


	CodeParallele <<<DimGrid, DimBlock >>> (n, m, np, td, h, matDest);

	cudaFree(matDest);

}

__global__ void CodeParallele(int n, int m, int np, double td, double h, float matDest) {


	







}


void codeSequentiel(int n, int m, int np, double td, double h) {

	printf("Lignes: %d \r\n", n);
	printf("Colonnes: %d \r\n", m);
	printf("Nombre de pas de temps: %d \r\n", np);
	printf("Temps discrétisé: %f \r\n", td);
	printf("Taille d'un côté d'une subdivision: %f \r\n", h);

	int i, j, k;
	double plaque[n][m][np];
	for (k = 0; k < np; k++) {
		for (i = 0; i < n; i++) {
			for (j = 0; j < m; j++) {
				plaque[i][j][k] = 0.0;
			}
		}
	}

	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			plaque[i][j][0] = i * (n - i - 1) * j * (m - j - 1);
		}
	}

	printf("Matrice initiale séquentielle: \r\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			printf("%6.1f", plaque[i][j][0]);
		}
		printf("\r\n");
	}

	double timeStart, timeEnd, Texec;
	//struct timeval tp;
	//gettimeofday(&tp, NULL); // Début du chronomètre
	//timeStart = (double)(tp.tv_sec) + (double)(tp.tv_usec) / 1e6;

	for (k = 1; k < np; k++) {
		for (i = n - 2; i >= 1; i--) {
			for (j = 1; j < m - 1; j++) {
				plaque[i][j][k] = ((1 - (4 * (td / (h*h)))) * plaque[i][j][k - 1]) + ((td / (h*h)) * (plaque[i - 1][j][k - 1] + plaque[i + 1][j][k - 1] + plaque[i][j - 1][k - 1] + plaque[i][j + 1][k - 1]));
				//usleep(5);
			}
		}
	}

	//gettimeofday(&tp, NULL); // Fin du chronomètre
	//timeEnd = (double)(tp.tv_sec) + (double)(tp.tv_usec) / 1e6;
	Texec = timeEnd - timeStart; //Temps d'exécution en secondes

	printf("Temps d'execution séquentiel: %f \r\n", Texec);
	printf("Matrice finale séquentielle: \r\n");
	for (i = 0; i < n; i++) {
		for (j = 0; j < m; j++) {
			printf("%6.1f", plaque[i][j][np - 1]);
		}
		printf("\r\n");
	}
}