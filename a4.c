#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif


/* --------- Matrix Functions --------- */

int * initMatrix(int size){
	int * matrix = malloc(sizeof(int )*size*size);
	int i = 0;
	for (i=0;i<(size*size);i++){
		//get random num
		int num = rand()%10;
		matrix[i] = num;
		
	}
	return matrix;
}

void printMatrix(int * matrix, int size){
	int i = 0;
	for (i=0;i<(size*size);i++){
		printf("%d", matrix[i]);
		if ((i+1)%size == 0)
			printf("\n");
	}	
}

void deleteMatrix(int * matrix, int size){
	free(matrix);
}

/* --------- Vector Functions --------- */

int * initVector(int size){
	int * vector = malloc(sizeof(int)*size);
	int i = 0;
	for (i=0;i<size;i++){
		//get random num
		int num = rand()%10;
		vector[i] = num;
	}
	return vector;
}

void printVector(int * vector, int size){
	int i = 0;
	for (i=0;i<size;i++){
		printf("%d\n", vector[i]);
	}
}

void deleteVector(int * vector, int size){
	free(vector);
}

int * multiply(int * matrix, int * vector, int size, int numThreads){

	char filename[12] = "multiply.cl";
    FILE * fp = fopen(filename, "r");
    if (fp == NULL){
        printf("could not open file\n");
        exit(0);
    }
    
    size_t program_size = sizeof(char)*500;
	char * codeStr = malloc(sizeof(char)*500);
    int len = fread(codeStr,1,sizeof(char)*500,fp);
    fclose(fp);
	//opencl setup
	// OpenCL specific variables
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	cl_device_id device_id = NULL;
	cl_platform_id platform_id = NULL;
	cl_program program = NULL;
    cl_kernel kernel = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;
    cl_int err;

	/* Get Platform and Device Info */
	err = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	err = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);
 
	/* Create OpenCL context */
	context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &err);

	program = clCreateProgramWithSource(context, 1, (const char**)&codeStr, &program_size, &err);
 
	/* Create Command Queue */
	command_queue = clCreateCommandQueue(context, device_id, 0, &err);

	int * product = malloc(sizeof(int)*size);
	/* Create Memory Buffer */
	cl_mem memMatrix = clCreateBuffer(context, CL_MEM_READ_WRITE, size * size * sizeof(int), NULL, &err);
	cl_mem memVector = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(int), NULL, &err);
	cl_mem memProduct = clCreateBuffer(context, CL_MEM_READ_WRITE, size * sizeof(int), NULL, &err);
	cl_mem memSize = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(int), NULL, &err);
	//cl_mem memNumT = clCreateBuffer(context, CL_MEM_READ_WRITE,  sizeof(int), NULL, &err);

	/* Write to memory buffer */
	err = clEnqueueWriteBuffer(command_queue,memMatrix, CL_TRUE, 0, size * size * sizeof(int), matrix, 0, NULL, NULL);
   	err = clEnqueueWriteBuffer(command_queue, memVector, CL_TRUE, 0, size * sizeof(int), vector, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(command_queue, memSize, CL_TRUE, 0, sizeof(int), &size, 0, NULL, NULL);
	//err = clEnqueueWriteBuffer(command_queue, memNumT, CL_TRUE, 0, sizeof(int), &numThreads, 0, NULL, NULL);
	
	/* Build Kernel */
	err = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

	/* Create OpenCL Kernel */
    kernel = clCreateKernel(program, "matrixMultiplication", &err);
 
    /* Set OpenCL Kernel Arguments */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&memMatrix);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&memVector);
	err = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&memProduct);
	err = clSetKernelArg(kernel, 3, sizeof(int), (void *)&memSize);
	//err = clSetKernelArg(kernel, 4, sizeof(int), (void *)&memNumT);
	
	/* Execute OpenCL Kernel */
	size_t globalThreads[2] = {size, size};
	size_t localThreads[2] = {numThreads, numThreads};
	 
	
	clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, globalThreads, localThreads, 0, NULL, NULL);

	/* Copy results from the memory buffer */
	err = clEnqueueReadBuffer(command_queue, memProduct, CL_TRUE, 0, size * sizeof(int),product, 0, NULL, NULL);
	

	// int i = 0;
	// for (i=0;i<size;i++){
	// 	printf("%d\n", *(product)+i);
	// }

    err = clFlush(command_queue);
    err = clFinish(command_queue);
    err = clReleaseKernel(kernel);
    err = clReleaseProgram(program);
    err = clReleaseMemObject(memMatrix);
    err = clReleaseMemObject(memVector);
    err = clReleaseMemObject(memProduct);
    err = clReleaseMemObject(memSize);
    //err = clReleaseMemObject(memNumT);
    err = clReleaseCommandQueue(command_queue);
    err = clReleaseContext(context);
    
    free(codeStr);
    
	return product;	
}


int main(int argc, char * argv[]){
	
	if (argc < 2){
		printf("Please provide the number of threads and the size of the the matrix.\n");
		exit(0);
	}

	srand(time(NULL));
	if (strcmp(argv[1], "-g")==0){

		/* initialize matriceis */
		int * m1 = initMatrix(100);
		int * m2 = initMatrix(1000);
		int * m3 = initMatrix(10000);
		int * m4 = initMatrix(20000);
		/* initialize vectors */
		int * v1 = initVector(100);
		int * v2 = initVector(1000);
		int * v3 = initVector(10000);
		int * v4 = initVector(20000);

		printf("Size        Threads\n");
		printf("       1       2       4\n");
		printf("-------------------------\n");
		time_t t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12;

		time_t start = time(NULL);
		int * sum1 = multiply(m1,v1,100,1);
		time_t end = time(NULL);
		t1 = end - start;
		start = time(NULL);
		int * sum2 = multiply(m1,v1,100,2);
		end = time(NULL);
		t2 = end - start;
		start = time(NULL);
		int * sum3 = multiply(m1,v1,100,4);
		end = time(NULL);
		t3 = end - start;
		printf("100    %ld       %ld       %ld\n", t1,t2,t3);

		start = time(NULL);
		int * sum4 = multiply(m2,v2,1000,1);
		end = time(NULL);
		t4 = end - start;
		start = time(NULL);
		int * sum5 = multiply(m2,v2,1000,2);
		end = time(NULL);
		t5 = end - start;
		start = time(NULL);
		int * sum6 = multiply(m2,v2,1000,4);
		end = time(NULL);
		t6 = end - start;
		printf("1000   %ld       %ld       %ld\n", t4,t5,t6);

		start = time(NULL);
		int * sum7 = multiply(m3,v3,10000,1);
		end = time(NULL);
		t7 = end - start;
		start = time(NULL);
		int * sum8 = multiply(m3,v3,10000,2);
		end = time(NULL);
		t8 = end - start;
		start = time(NULL);
		int * sum9 = multiply(m3,v3,10000,4);
		end = time(NULL);
		t9 = end - start;
		printf("10000  %ld       %ld       %ld\n", t7,t8,t9);

		start = time(NULL);
		int * sum10 = multiply(m4,v4,20000,1);
		end = time(NULL);
		t10 = end - start;
		start = time(NULL);
		int * sum11 = multiply(m4,v4,20000,2);
		end = time(NULL);
		t11 = end - start;
		start = time(NULL);
		int * sum12 = multiply(m4,v4,20000,4);
		end = time(NULL);
		t12 = end - start;
		printf("20000  %ld       %ld       %ld\n", t10,t11,t12);

		deleteVector(sum1,100);
		deleteVector(sum2,100);
		deleteVector(sum3,100);
		deleteVector(sum4,1000);
		deleteVector(sum5,1000);
		deleteVector(sum6,1000);
		deleteVector(sum7,10000);
		deleteVector(sum8,10000);
		deleteVector(sum9,10000);
		deleteVector(sum10,20000);
		deleteVector(sum11,20000);
		deleteVector(sum12,20000);

	} else {
		if (argc != 3){
			printf("Please provide the number of threads and the size of the the matrix.\n");
			exit(0);
		}
		int numThreads = atoi(argv[1]);
		int size = atoi(argv[2]);
	
		int * matrix = initMatrix(size);
		int * vector = initVector(size);
        

		printMatrix(matrix, size);
		printf("\nX\n");
		printVector(vector,size);
		printf("\n=\n");
		int * product = multiply(matrix,vector,size,numThreads);
		//printVector(product,size);
		deleteMatrix(matrix, size);
		deleteVector(vector,size);
		deleteVector(product,size);
	}

	return 0;
}