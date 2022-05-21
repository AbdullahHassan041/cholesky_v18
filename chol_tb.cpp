
/********* testbench of Cholesky **********/
//; $SIZE = param_define("SIZE", 4);
#include "chol.h"
#include "hls/linear_algebra/utils/x_hls_matrix_utils.h"
//#include "hls/linear_algebra/utils/x_hls_matrix_tb_utils.h"
using namespace hls;

int main ()
{
	int Cholesky_success = 0;
	int fail = 0;
	int i, j;
	matrix_t A[4][4]={{6,15,55,25},{15,55,225,70},{55,225,979,90},{25,70,90,1000}};
	matrix_t L[matrix_size][matrix_size];
	matrix_t Ar[matrix_size][matrix_size];



    for(i = 0; i < 5; i++)
    {
    	Cholesky_success |= top(A, L);
    }
	// Now re-create A: Ar = L * L'
    hls::matrix_multiply<hls::NoTranspose,hls::Transpose,matrix_size,matrix_size,matrix_size,matrix_size,matrix_size,matrix_size,matrix_t,matrix_t>(L, L, Ar);

    printf("A = \n");
    hls::print_matrix<matrix_size, matrix_size, matrix_t, hls::NoTranspose>(A, "   ");

    printf("L = \n");
    hls::print_matrix<matrix_size, matrix_size, matrix_t, hls::NoTranspose>(L, "   ");

    printf("A reconstructed = \n");
    hls::print_matrix<matrix_size, matrix_size, matrix_t, hls::NoTranspose>(Ar, "   ");

    // Generate error ratio and compare against threshold value
    // - LAPACK error measurement method
    // - Threshold taken from LAPACK test functions
    //int -> float
    float error = 0;
    for(i = 0; i < matrix_size; i++)
    	for(j = 0; j < matrix_size; j++)
    		error += hls::abs(A[i][j]- Ar[i][j]);
    if(error > 5.0)
    	fail = 1;
    return (Cholesky_success||fail);
}
