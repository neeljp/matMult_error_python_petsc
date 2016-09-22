###
```sh
$ ./main.exe 
Multypling a 4x4 Matrix A with a vector y and the first row of A with y.
This gives different results in the firstt entry of the solution vector.

y_4 ist the solution Ay and y_1 is the result of A[0,:]y.
y_4[0]-y_1 -0.00000000000000044409
Multypling the first row manually with a for loop, gives the result of y_1.
sum 2.16655995777766863242
multvalue of entry 0 2.16655995777766863242
sum 2.16970541238888303681
multvalue of entry 1 0.00314545461121454057
sum 2.16970695704416227656
multvalue of entry 2 0.00000154465527914900
sum 2.16970695708621708064
multvalue of entry 3 0.00000000004205461291
y_manually-y_4[0] 0.00000000000000044409
y_manually-y_1 0.00000000000000000000

One order of addition that returns the same result as y_4 is 2,3,0,1.
2:0.00000154465527914900
2+3:0.00000154469733376191
2+3+0:2.16656150247500223216
2+3+0+1:2.16970695708621663655 - y_4[0] =  0.00000000000000000000
2+3+0+1:2.16970695708621663655 - y_1 =  -0.00000000000000044409
```
```sh
$ python main.py 
norm y_py - y_m3d:  4.4408920985e-16
python first 5 values:  [ 2.16970116439824867172  2.1699684670590202451   2.17006950101934315001
  2.17007284988585791297  2.16970695708621708064]
m3d  first 5 values:  [ 2.16970116439824867172  2.1699684670590202451   2.17006950101934315001
  2.17007284988585791297  2.16970695708621663655]
forth entry error:  4.4408920985e-16
compare the multyplication of that entry:
 [ 0.]
compare the multyplication of the whole 4x4 block:
 [ -4.44089209850062616169e-16   0.00000000000000000000e+00
   0.00000000000000000000e+00   0.00000000000000000000e+00]
The error does not happen, if computing only the multyplication of the first row of the Matrix with the vector.

```
