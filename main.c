#include "petsc.h"
#include "slepceps.h" 
#include <stdio.h>

int
main(int argc, char **args)
{
    // init PETSc
    PetscInitialize(&argc, &args, PETSC_NULL, PETSC_NULL);
    // work vars
    MPI_Comm comm = PETSC_COMM_WORLD;
    PetscViewer viewer;
    PetscInt nx = 4;
    Vec yeq, y,y0;
    Mat Ai,A_row;
    PetscScalar *y_sec_block, *y_sec_block_fst_row,*y_eq;
    PetscInt *ncols, *cols, *ncols_row, *cols_row;
    PetscScalar *vals,*vals_row;
    double mult_array[4];

    // create y_eq
    VecCreateMPI(comm, nx, nx, &yeq);
    // load yeq
    PetscViewerBinaryOpen(comm, "yeq_sec_block.petsc", FILE_MODE_READ, &viewer);
    VecLoad(yeq, viewer);
    PetscViewerDestroy(&viewer);
     // create A_row
    MatCreate(comm, &A_row);
    MatSetSizes(A_row, 1, nx, 1, nx);
    MatSetType(A_row, MATAIJ);
    // load Ai
    PetscViewerBinaryOpen(comm, "A_sec_block_fst_row.petsc", FILE_MODE_READ, &viewer);
    MatLoad(A_row, viewer);
    PetscViewerDestroy(&viewer);

    VecCreateMPI(comm, 1, 1, &y0);

    MatMult(A_row, yeq, y0);
    // write yout
    PetscViewerBinaryOpen(comm, "y_sec_block_fst_row_m3d.petsc", FILE_MODE_WRITE, &viewer);
    VecView(y0, viewer);
    PetscViewerDestroy(&viewer);

    // create y

    VecCreateMPI(comm, nx, nx, &y);
    // load y
    PetscViewerBinaryOpen(comm, "m3d_y_sec_block.petsc", FILE_MODE_READ, &viewer);
    VecLoad(y, viewer);
    PetscViewerDestroy(&viewer);


    // create Ai
    MatCreate(comm, &Ai);
    MatSetSizes(Ai, nx, nx, nx, nx);
    MatSetType(Ai, MATAIJ);
    // load Ai
    PetscViewerBinaryOpen(comm, "A_sec_block.petsc", FILE_MODE_READ, &viewer);
    MatLoad(Ai, viewer);
    PetscViewerDestroy(&viewer);


    // yout = Ai' * yeq
    MatMult(Ai, yeq, y);

      // write yout
    PetscViewerBinaryOpen(comm, "y_sec_block_m3d.petsc", FILE_MODE_WRITE, &viewer);
    VecView(y, viewer);
    PetscViewerDestroy(&viewer);



 
    


    VecGetArray(y,&y_sec_block);
    VecGetArray(y0,&y_sec_block_fst_row);
    VecGetArray(yeq,&y_eq);

    printf("Multypling a 4x4 Matrix A with a vector y and the first row of A with y.\n");
    printf("This gives different results in the firstt entry of the solution vector.\n");
    printf("y_4 ist the solution Ay and y_1 is the result of A[0,:]y.\n");

    printf("y_4[0]-y_1 %.20f\n", (y_sec_block[0]-y_sec_block_fst_row[0]));
    MatGetRow(Ai,0,&ncols,&cols,&vals);
    MatGetRow(A_row,0,&ncols_row,&cols_row,&vals_row);
    PetscScalar sum = 0.0;
    printf("Multypling the first row manually with a for loop, gives the result of y_1.\n");

    for(int i =0 ; i< 4;i++)
    {
        mult_array[i] = vals[i]*y_eq[i];
        sum += mult_array[i];
        //printf("firts row of A entry %d: %.20f\n", i,vals[i]-vals_row[i]);
        printf("sum %.20f\n", sum );
        printf("multvalue of entry %d %.20f\n", i, mult_array[i]);
    }
    
    printf("y_manually-y_4[0] %.20f\n", (sum-y_sec_block[0]));
    printf("y_manually-y_1 %.20f\n", (sum-y_sec_block_fst_row[0]));

    printf("\n");
    printf("One order of addition that returns the same result as y_4 is 2,3,0,1.\n");


    sum = 0.0;
    sum += mult_array[2];
    printf("2:%.20f\n", sum );
    sum += mult_array[3];
    printf("2+3:%.20f\n", sum );
    sum += mult_array[0];
    printf("2+3+0:%.20f\n", sum );
    sum += mult_array[1];
    printf("2+3+0+1:%.20f - y_4[0] =  %.20f\n", sum ,sum-y_sec_block[0] );
    printf("2+3+0+1:%.20f - y_1 =  %.20f\n", sum ,sum-y_sec_block_fst_row[0] );
    

    // for(int i = 0; i < 4; i++)
    // {
    //     for(int j = 0; j< 4; j++)
    //     {
    //         for(int k = 0; k<4;k++)
    //         {
    //             for(int l =0; l<4 ; l++)
    //             {   
    //                 if(i!=j && i!=k && i!=l && j!=k && j!=l && k!=l )
    //                 {
    //                     double sum = mult_array[i] +mult_array[j] + mult_array[k] +mult_array[l];
    //                     printf("%.20f\n", mult_array[i]);
    //                     printf("sum - y_sec_block: %d,%d,%d,%d : %.20f\n", i,j,k,l,sum-y_sec_block[0]);
    //                     printf("sum - y_sec_block_fst_row: %d,%d,%d,%d : %.20f\n", i,j,k,l,sum-y_sec_block_fst_row[0]);


    //                 }
   
    //             }
    //         }
    //     }
    // }
    //printf("multy-sec_block: %.20f\nmulty - sec_block_fst_row: %.20f\n", sum-y_sec_block[0] , sum-y_sec_block_fst_row[0]);

    // free yeq, y
    VecDestroy(&yeq);
    VecDestroy(&y);
    // free Ai
    MatDestroy(&Ai);

    return 0;
}
