import numpy as np
import petsc_io as io 
np.set_printoptions(precision=20)

y_m3d = io.read_PETSc_vec("m3d_y.petsc")
y_py = io.read_PETSc_vec("py_y.petsc")

print("norm y_py - y_m3d: ", np.linalg.norm(y_py[:5]-y_m3d[:5])) 
print("python first 5 values: ", y_py[:5])
print("m3d  first 5 values: ", y_m3d[:5])
print("forth entry error: ", y_py[4] -y_m3d[4] )
 

y_eq = io.read_PETSc_vec("yeq_sec_block.petsc")
A = io.read_PETSc_mat("A_sec_block_fst_row.petsc")
y = A.dot(y_eq)
io.write_PETSc_vec(y,"y_sec_block_fst_row_py.petsc")

y_m3d = io.read_PETSc_vec("y_sec_block_fst_row_m3d.petsc")

print ("compare the multyplication of that entry:\n", y_m3d -y )


A = io.read_PETSc_mat("A_sec_block.petsc")
y = np.zeros(4)
y = A.dot(y_eq)
io.write_PETSc_vec(y,"y_sec_block_py.petsc")

y_m3d = io.read_PETSc_vec("y_sec_block_m3d.petsc")

print ("compare the multyplication of the whole 4x4 block:\n", y_m3d -y )

print("The error does not happen, if computing only the multyplication of the first row of the Matrix with the vector.")