#include <bits/stdc++.h> 
using namespace std; 
class CNN 
{ 
    public: 

    CNN(int n){
    	nLayers = n;

    	kernel = new float*[nLayers];
    	for(int l = 0; l < nLayers; l++){
    		/* Assuming kernel size to be 3x3 */
    		kernel[l] = new float[9];
    		for(int i = 0; i < 9; i++){
    			kernel[l][i] = 1.0f / 9.0f;
			}
		}
	}
	~CNN(){
		for(int l = 0; l < nLayers; l++){
			delete [] kernel[l];
		}
		delete [] kernel;
	}
    void convolve(int dim, char* mIn, int l, char* mOut, int zeroPad){
    	for(int i = 0; i < dim; i++){
    		for(int j = 0; j < dim ; j++){
    			float acc = 0;
    			for(int p = -1; p <= 1; p++){
    				for(int q = -1; q <= 1; q++){
    					float w = kernel[l][(p + 1) * 3 + (q + 1)];
    					acc += ((i+p < 0) || (i+p > dim - 1) || (j+q < 0) || (j+q > dim - 1)) ? 0 : w * mIn[(i+p) * dim + (j+q)];
					}
				}
				mOut[i * dim + j] = (int)round(acc);
			}
		}	
	}
	void zeropad(){
		return;	
	}
	void pool(int dim, char* mIn, int l, char* mOut, int zeroPad){
		int dimOut = dim - 1;
    	for(int i = 0; i < dimOut; i++){
    		for(int j = 0; j < dimOut; j++){
    			int max = 0;
    			for(int p = 0; p <= 1; p++){
    				for(int q = 0; q <= 1; q++){
    					max = std::max((int)mIn[(i+p) * (dim) + (j+q)], max);
					}
				}
				mOut[i * dimOut + j] = max;
			}
		}
	}
	int nLayers;

	private :

	float** kernel;
}; 

void getInput(int m, char *mIn){
	for(int i = 0; i < m; i++){
		for(int j = 0; j < m; j++){
			mIn[i*m + j] = rand() % 10;
		}
	}
	return;
}

void getOutput(int m1, int m2, char *mIn, char *mOut){
	for(int i = 0; i < m2; i++){
		for(int j = 0; j < m2; j++){
			int sum = mIn[2 * i * m1 + 2 * j];
			sum += mIn[2 * i * m1 + 2 * j + 1];
			sum += mIn[(2 * i + 1) * m1 + 2 * j];
			sum += mIn[(2 * i + 1) * m1 + 2 * j + 1];
			mOut[i * m2 + j] = (sum + 2) >> 2;
		}
	}
	return;
}

void print_data(int dim, char* mIn){
	for(int i = 0; i < dim; i++){
		for(int j = 0; j < dim; j++)
			printf("%d\t", mIn[i*dim + j]);
		printf("\n");
	}
	printf("\n");
}

int main() { 

	int inDim = 8;
	int outDim = 4;

	/* CNN */
    int noOfLayers = (inDim - outDim);
	CNN cnn(noOfLayers);

	do{
	    char *mInput = new char[inDim * inDim]; // [inDim][inDim];
	    char *mOutput = new char[outDim * outDim]; // [outDim][outDim];

	    /*Random input creation*/
	    getInput(inDim, mInput);
	    getOutput(inDim, outDim, mInput, mOutput);

		print_data(inDim, mInput);
		// print_data(outDim, mOutput);

		char *mIn = mInput;

		/* Forward propagation */
	    for(int l = 0; l < noOfLayers; l++){
			int dim = inDim - l;
			int dimNxt = inDim - l - 1;

	    	char *mTmp = new char[dim*dim];
	    	char *mOut = new char[dimNxt*dimNxt];

	    	cnn.convolve(dim, mIn, l, mTmp, 1);
	    	cnn.pool(dim, mTmp, l, mOut, 0);

	    	for(int i = 0; i < dimNxt; i++){
	    		for(int j = 0; j < dimNxt; j++){
	    			mIn[i*dimNxt + j] = mOut[i*dimNxt + j];
				}
			}

			print_data(dimNxt, mIn);
		}

	} while(0);
    return 0;
} 
