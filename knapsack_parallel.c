// A Dynamic Programming based
// solution for 0-1 Knapsack problem

#include <stdio.h>
#include <malloc.h>
#include <omp.h>
#include <stdlib.h>
#include <sys/time.h>


double tempo, tempo_total;

/*  Retorna tempo em milisegundos

    Forma de uso:
 
    double tempo;
    tempo = timestamp();
    <trecho de programa do qual se deseja medir tempo>
    tempo = timestamp() - tempo;
*/

double timestamp(void) {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}


// A utility function that returns
// maximum of two integers
int max(int a, int b)
{ 
    return (a > b) ? a : b; 
}
  
// Returns the maximum value that
// can be put in a knapsack of capacity W
int knapSack(int W, int wt[], int val[], int n)
{
    
    int i, w;
    int K[2][W + 1];
    // We know we are always using the the current row or
    // the previous row of the array/vector . Thereby we can
    // improve it further by using a 2D array but with only
    // 2 rows i%2 will be giving the index inside the bounds
    // of 2d array K


            
    for (i = 0; i <= n; i++) {

        tempo = timestamp(); // cálculo do tempo de execução paralelo

        #pragma omp parallel for schedule(auto) private(w) // paralelização
        for (w = 0; w <= W; w++) {
            if (i == 0 || w == 0) {
                K[i % 2][w] = 0;
            } else if (wt[i - 1] <= w) {
                int maior = max(val[i - 1] + K[(i - 1) % 2][w - wt[i - 1]], K[(i - 1) % 2][w]);
                K[i % 2][w] = maior;
            } else {
                K[i % 2][w] = K[(i - 1) % 2][w];
            }
        }

        tempo =  timestamp() - tempo;

        tempo_total += tempo;
    }

    printf("Tempo paralelo: %f ms\n", tempo_total);
    
    return K[n % 2][W];

}

// Driver Code
int main()
{
    tempo = timestamp();
    int n, W;

	scanf("%d %d", &n, &W);
	int *val = (int*) calloc(n, sizeof(int));
	int *wt = (int*) calloc(n, sizeof(int));

	int i;
	for (i = 0; i < n; ++i) {
		scanf("%d %d", &(val[i]), &(wt[i])); 
	}

    printf("Resultado Parallel com %d threads: %d", omp_get_max_threads(), knapSack(W, wt, val, n));
    return 0;
}