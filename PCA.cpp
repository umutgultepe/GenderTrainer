#include "PCA.hpp"


MatrixXf cvToEigen(CvMat* cvMatrix)

{
  CvSize sz=cvGetMatSize(cvMatrix);
  
//  printf("Height and Width:%d,%d\n",sz.height,sz.width);
  MatrixXf mat(sz.height,sz.width);
  
  
  
  for (int i=0;i<sz.height;i++)
    for (int j=0;j<sz.width;j++)
      mat(i,j)=cvGetReal2D(cvMatrix,i,j);
    
    
    return mat;
  
  
  
  
}




void EigenToCV(SelfAdjointEigenSolver<MatrixXf> EigenVectors,CvMat* &cvVectors,int index,int featureCount)
{
  
  VectorXf vecDum;
  cvVectors=cvCreateMat(featureCount,index,CV_32F);
  
  for (int i=0;i<index;i++)
  {
    vecDum=EigenVectors.eigenvectors().col(i);
    for (int j=0;j<featureCount;j++)
      cvSetReal2D(cvVectors,j,i,vecDum(j));
    
      
//    cvSetReal1D(cvVals,featureCount-i-1,EigenVectors.eigenvalues()[i]);
  }
}









CvMat* LDA(CvMat* SampleMatrix, int MCount)
{
 
  CvSize sampleSize=cvGetMatSize(SampleMatrix);
  
  int sampleCount=sampleSize.width;
  int featureCount=sampleSize.height;
  int WCount=sampleCount-MCount;
  CvMat* Dummy=cvCreateMat(featureCount,featureCount,CV_32F);

  cout<< "Calculating Mean!\n";
  //!Calculate Mean
  CvMat* MMean=cvCreateMat(featureCount,1,CV_32F);
  CvMat* WMean=cvCreateMat(featureCount,1,CV_32F);
  
  for(int i=0;i<featureCount;i++)
  {
    cvSetReal1D(MMean,i,cvMean(cvGetCols(cvGetRow(SampleMatrix,Dummy,i),Dummy,0,MCount)));
    cvSetReal1D(WMean,i,cvMean(cvGetCols(cvGetRow(SampleMatrix,Dummy,i),Dummy,MCount,sampleCount)));
    
  }
  
  CvMat* Dummy2=cvCreateMat(1,1,CV_32F);
  cvReleaseMat(&Dummy);
  Dummy=cvCreateMat(featureCount,1,CV_32F);
  int trans=CV_GEMM_B_T;
  
  cout<< "Calculating Scatter Matrix!\n";
  
  //!Calculate Scatter Matrices
  CvMat* ScatterWithin=cvCreateMat(featureCount,featureCount,CV_32F);
  CvMat* Dummy3=cvCreateMat(featureCount,1,CV_32F);
  for (int i=0;i<MCount;i++)
  {
    Dummy=cvGetCol(SampleMatrix,Dummy2,i);
    cvSub(MMean,Dummy,Dummy3);
    cvGEMM(Dummy3,Dummy3,1,ScatterWithin,1,ScatterWithin,trans);
  }
  
  for (int i=MCount;i<sampleCount;i++)
  {
    Dummy=cvGetCol(SampleMatrix,Dummy2,i);
    cvSub(WMean,Dummy,Dummy3);
    cvGEMM(Dummy3,Dummy3,1,ScatterWithin,1,ScatterWithin,trans);
  }
  
  
  //!From now on, ScatterWithin is inverted
  cvInvert(ScatterWithin,ScatterWithin);  
  cout<<"Scattering Complete\n";
  cvSub(MMean,WMean,Dummy3);  
  cvGEMM(ScatterWithin,Dummy3,1,NULL,0,Dummy3,0);

  cvReleaseMat(&MMean);
  cvReleaseMat(&WMean);
  cvReleaseMat(&ScatterWithin);;
  return Dummy3;
}







CvMat* PCA(CvMat* SampleMatrix)
{
 
  CvSize sampleSize=cvGetMatSize(SampleMatrix);
  
  int sampleCount=sampleSize.width;
  int featureCount=sampleSize.height;
  
  
  CvMat* Dummy=cvCreateMat(featureCount,featureCount,CV_32F); 
  CvMat* Scatter=cvCreateMat(featureCount,featureCount,CV_32F);
  
  
  
  cout<< "Calculating Mean!\n";
  //!Calculate Mean
  CvMat* Mean=cvCreateMat(featureCount,1,CV_32F);
  for(int i=0;i<featureCount;i++)
    cvSetReal1D(Mean,i,cvMean(cvGetRow(SampleMatrix,Dummy,i)));
  
  CvMat* Dummy2=cvCreateMat(1,1,CV_32F);;
  cvReleaseMat(&Dummy);
  Dummy=cvCreateMat(featureCount,1,CV_32F);
  int trans=CV_GEMM_B_T;
  
    cout<< "Calculating Scatter Matrix!\n";
  CvMat* Dummy3=cvCreateMat(featureCount,1,CV_32F);
  //!Calculate Scatter Matrix
  for (int i=0;i<sampleCount;i++)
  {
    Dummy=cvGetCol(SampleMatrix,Dummy2,i);
    cvSub(Mean,Dummy,Dummy3);
    cvGEMM(Dummy3,Dummy3,1,Scatter,1,Scatter,trans);
  }
  cvReleaseMat(&Dummy3);
  cout<< "Calculated Scatter Matrix!\n";
  
  
  
  
  MatrixXf SampleMat;
  SampleMat=cvToEigen(Scatter);
  
  
  cout<< "CV To Eigen Conversion Complete!\n";
  
  SelfAdjointEigenSolver<MatrixXf> eigensolver(SampleMat);
    
  VectorXf EigenVals=eigensolver.eigenvalues();
  ofstream ff;
  
 // ff.open("eigens.dat");
  int index=0;
  for (int i=0;i<featureCount;i++)
  {
 //   printf("EigenValue is:%f\n",EigenVals(i));
    
   // ff<< EigenVals(i) << "\n";
    if (EigenVals(i)<1000)
    {
	index=i;
	cout<< "New Dimension is: "<< i << "\n";
	break;
    }      
  }
 // ff.close();
  CvMat* cvVectors;
  
  EigenToCV(eigensolver,cvVectors,index,featureCount);
  
  cout<< "PCA Complete\n";
 //cvReleaseMat(&Dummy);
  cvReleaseMat(&Mean);
 // cvReleaseMat(&Dummy2);
  cvReleaseMat(&Scatter);
  return cvVectors;
  
  
  
}