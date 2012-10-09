#include "globalHeader.hpp"
#include <Eigen>

using namespace std;
using namespace Eigen;
CvMat* PCA(CvMat* SampleMatrix);
CvMat* LDA(CvMat* SampleMatrix,int MCount);