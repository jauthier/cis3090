__kernel void matrixMultiplication(__global int * matrix, __global int * vector, __global int * product,int size){
    int id = get_local_id(0);
    int num = 0;
    for (int i=0;i<size;i++){ // for the num of elements in the vector
        num = num + (matrix[id*size+i]*vector[i]);
    }
    product[id] = num;
}