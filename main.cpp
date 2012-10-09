#include "filereader.hpp"
#include "globalHeader.hpp"
#include "LBH-HF.hpp"
//#include "LDA/libface.h"
#include "trainer.hpp"
//#include "trainImage.hpp"
#include "images2Columns.hpp"
#include "PCA.hpp"
#include "faceProcessor.hpp"
trainImage* MImages;
trainImage* WImages;
int totalMCount=0;
int totalWCount=0;
CvMat* SampleMatrix;



void cleanRaw()
{
    for (int i=0;i<totalWCount;i++)
  {
    cvReleaseImage(&(WImages[i].RawImage));
  }
  for (int i=0;i<totalMCount;i++)
  {
    cvReleaseImage(&(MImages[i].RawImage));
  }
}



void cleanUp()
{
    for (int i=0;i<totalWCount;i++)
  {
    delete[] WImages[i].LBPHF;
    
    cvReleaseImage(&(WImages[i].EyeImage));
    cvReleaseImage(&(WImages[i].MouthImage));
    cvReleaseImage(&(WImages[i].NoseImage));
    delete[] WImages[i].components;
  }
  for (int i=0;i<totalMCount;i++)
  {
   delete[] MImages[i].LBPHF;
    
    cvReleaseImage(&(MImages[i].EyeImage));
    cvReleaseImage(&(MImages[i].MouthImage));
     cvReleaseImage(&(MImages[i].NoseImage));
     delete[] MImages[i].components;
  }

  cvReleaseMat(&SampleMatrix);
  delete[] MImages;
  delete[] WImages;
  exit(1);
}


int main(int argc, char **argv) {
  
  
  
  
  //!Read Images, Store in Matrices-DONE
  //!Find Faces and Locate Parts
  //!Record the parts and their sizes
  
  char** MImageNames;			//Names with paths of Male Images
  char** WImageNames;			//Names with paths of Female Images
  char** MCImageNames;			//Names with paths of Male Images
  char** WCImageNames;			//Names with paths of Female Images  
  int MCount=0;				//Number of Male Images
  int WCount=0;				//Number of female Images
  int MCCount=0;				//Number of Male Images
  int WCCount=0;				//Number of female Imag
  
  
  
  
  
  char* MenDirectory=argv[1];		//Directory Where Male Images Stored
  char* WomenDirectory=argv[2];		//Directory Where Male Images Stored
  bool MCrop=0;
  if (NULL!=argv[4])
  {
    MCrop=1;
  char* MenCroppedDirectory=argv[4];		//Directory Where Female Images Stored
  okans_file_reader_mod( MCImageNames, MCCount,  MenCroppedDirectory);	//Read the Images
  }
  bool WCrop=0;
  if (NULL!=argv[3])
  {
  WCrop=1;
  char* WomenCroppedDirectory=argv[3];		//Directory Where Female Images Stored
  okans_file_reader_mod( WCImageNames, WCCount,  WomenCroppedDirectory);
  }
  okans_file_reader_mod( MImageNames, MCount,  MenDirectory);	//Read the Images
  okans_file_reader_mod( WImageNames, WCount,  WomenDirectory);
  
  
  
  initializeFaceProcessor();
  
 MImages=new trainImage[MCount+MCCount];
  WImages=new trainImage[WCount+WCCount];
  

  
  for (int i=0;i<MCount;i++)
  {
    MImages[totalMCount].RawImage=cvLoadImage( MImageNames[i]);//Fill the Image Array
    if (processFace( MImages[totalMCount].RawImage,  MImages[totalMCount].FaceImage, MImages[totalMCount].MouthImage, MImages[totalMCount].NoseImage, MImages[totalMCount].EyeImage, 0))
    {
      cvReleaseImage(&(MImages[totalMCount].RawImage));
      totalMCount++;
    }
    else
      cvReleaseImage(&(MImages[totalMCount].RawImage));
    cout<<"Men Image " << i+1 << " Complete\n";
   // printf("% %f of Uncropped Men Complete\n",(float)i/(float)MCount);
  }
  if(MCrop)
  {
   for (int i=0;i<MCCount;i++)
    {
      MImages[totalMCount].RawImage=cvLoadImage( MCImageNames[i]);//Fill the Image Array
      if (processFace( MImages[totalMCount].RawImage,  MImages[totalMCount].FaceImage, MImages[totalMCount].MouthImage, MImages[totalMCount].NoseImage, MImages[totalMCount].EyeImage, 1))
      {
	cvReleaseImage(&(MImages[totalMCount].RawImage));
	totalMCount++;
      }
      else
	cvReleaseImage(&(MImages[totalMCount].RawImage));
      cout<<"Cropped Men Image " << i+1 << " Complete\n";
    //  printf("% %d of Cropped Men Complete\n",i/MCCount);
    } 
  }
  cout<<"First Stage Men Complete\n";
  
  int totalWCount=0;
  for (int i=0;i<WCount;i++)
  {
    WImages[totalWCount].RawImage=cvLoadImage( MImageNames[i]);//Fill the Image Array
    if (processFace( WImages[totalWCount].RawImage,  WImages[totalWCount].FaceImage, WImages[totalWCount].MouthImage, WImages[totalWCount].NoseImage, WImages[totalWCount].EyeImage, 0))
    {
      cvReleaseImage(&(WImages[totalWCount].RawImage));
      totalWCount++;
    }
    else
      cvReleaseImage(&(WImages[totalWCount].RawImage));
    cout<<"Women Image " << i+1 << " Complete\n";
  //  printf("% %d of Unropped Women Complete\n",i/WCount);
  }
//cout<<"Uncropped Women Complete\n";
  if(WCrop)
  {
   for (int i=0;i<WCCount;i++)
    {
      //cout<< MImageNames[i]<<"\n";
      WImages[totalWCount].RawImage=cvLoadImage( WCImageNames[i]);//Fill the Image Array
      if (processFace( WImages[totalWCount].RawImage,  WImages[totalWCount].FaceImage, WImages[totalWCount].MouthImage, WImages[totalWCount].NoseImage, WImages[totalWCount].EyeImage, 1))
      {
	cvReleaseImage(&(WImages[totalWCount].RawImage));
	totalWCount++;
      }
      else
	cvReleaseImage(&(WImages[totalWCount].RawImage));
       //   printf("% %d of Cropped Women Complete\n",i/WCCount);
      cout<<"Cropped Women Image " << i+1 << " Complete\n";
    } 

  }


  cout << "Images are Read, Stored in Matrices\n";

 
    cleanRaw();
  
  //!Pass through Gabor
  //!Pass through LBP-HF
  
    
    
  for (int i=0;i<totalMCount;i++)
  {
    
    MImages[i].LBPHF=LBP_HF(MImages[i].FaceImage,MImages[i].nonUniform,MImages[i].complete); //Pass through the LBPHF
//     MImages[i].EyeImage=filterGabor(MImages[i].EyeImage);
//     MImages[i].NoseImage=filterGabor(MImages[i].NoseImage);
//     MImages[i].MouthImage=filterGabor(MImages[i].MouthImage);
    cvReleaseImage(&(MImages[i].FaceImage)); 
    cout<<"Men Image " << i+1 << " Complete\n";
  }
  
  for (int i=0;i<totalWCount;i++)
  {
    
    WImages[i].LBPHF=LBP_HF(WImages[i].FaceImage,WImages[i].nonUniform,WImages[i].complete);
//     WImages[i].EyeImage=filterGabor(WImages[i].EyeImage);
//     WImages[i].NoseImage=filterGabor(WImages[i].NoseImage);
//     WImages[i].MouthImage=filterGabor(WImages[i].MouthImage);
    cvReleaseImage(&(WImages[i].FaceImage)); 
    cout<<"Women Image " << i+1 << " Complete\n";
  }

  cout << "Feature Extraction Complete\n";
  
  //!Turn the Gabor Images to column vectors, create a filled matrix!
  CvMat* SampleMatrix;
 
  mat2Col(MImages,WImages,totalMCount,totalWCount,SampleMatrix);
  cout << "Sample Matrix Acquired\n";
  
 
  //!Input the matrix with labeled vector to the LDA, acquire the projection Matrix 
  
 CvMat* ProjectionMatrix;
 ProjectionMatrix=PCA(SampleMatrix);
 //ProjectionMatrix=LDA(SampleMatrix,totalMCount);
 //ProjectionMatrix=(CvMat*)cvLoad("ProjectionMatrix.xml");
  
 cout << "Projection Matrix Acquired\n";
   
  //!Save Projection Matrix
  cvSave("ProjectionMatrix.xml",ProjectionMatrix);
  cvSave("SampleMatrix.xml",SampleMatrix);
  
  
  //!Acquire a complete Feature Vector
  
  
  CvMat* Features;
  int transp,newDimension;
  if (ProjectionMatrix->cols == SampleMatrix->rows)
  {
    transp=0;
    newDimension=ProjectionMatrix->rows;
  }
  else if(ProjectionMatrix->rows==SampleMatrix->rows)
  {
    transp=CV_GEMM_A_T;
    newDimension=ProjectionMatrix->cols;
  }
  else
  {
   cout<< "Matrix Sizes do not Match";
   return -1;
  }//Check the Multiplication
 
 cout<< "Getting Projection\n";
 Features=cvCreateMat(newDimension,totalWCount+totalMCount,CV_32F);
  cvGEMM(ProjectionMatrix,SampleMatrix,1,NULL,0,Features,transp);
 
 
 //int newDimension=SampleMatrix->rows;
 //CvMat* Features=SampleMatrix;
 
   for (int i=0;i<totalMCount;i++)
  {
    MImages[i].components=new double[newDimension];
    for (int j=0;j<newDimension;j++)
      MImages[i].components[j]=cvGetReal2D(Features,j,i);
  }//Fill in the new Feature Vectors
  
  for (int i=0;i<totalWCount;i++)
  {
    WImages[i].components=new double[newDimension];
    for (int j=0;j<newDimension;j++)
      WImages[i].components[j]=cvGetReal2D(Features,j,i+totalMCount);
  }//Fill in the new Feature Vectors
  
  
  
  
   
    
    //!Create the Overall feature Matrices
    
    CvMat* allFeatures=cvCreateMat(totalWCount+totalMCount,35+2+newDimension,CV_32F);
    CvMat* labels=cvCreateMat(totalWCount+totalMCount,1,CV_32S);
    for (int i=0;i<totalMCount;i++)
    {
      cvSetReal2D(allFeatures,i,0,MImages[i].complete);
      cvSetReal2D(allFeatures,i,1,MImages[i].nonUniform);      
      for (int j=0;j<35;j++)
	cvSetReal2D(allFeatures,i,2+j,MImages[i].LBPHF[j]);
      for (int j=0;j<newDimension;j++)
	cvSetReal2D(allFeatures,i,37+j,MImages[i].components[j]);
      cvSetReal1D(labels,i,0);
    }
    
    
     for (int i=0;i<totalWCount;i++)
    {
      cvSetReal2D(allFeatures,i+totalMCount,0,WImages[i].complete);
      cvSetReal2D(allFeatures,i+totalMCount,1,WImages[i].nonUniform);
      for (int j=0;j<35;j++)
	cvSetReal2D(allFeatures,i+totalMCount,2+j,WImages[i].LBPHF[j]);
      for (int j=0;j<newDimension;j++)
	cvSetReal2D(allFeatures,i+totalMCount,37+j,WImages[i].components[j]);
      cvSetReal1D(labels,i+totalMCount,1);
    }
    
   //!Train AdaBoost and Save the File
    
    
    
    
    CvBoost boost;
    CvMat* var_type;
    var_type = cvCreateMat( 1,35+2+newDimension+1 , CV_8U );
        cvSet( var_type, cvScalarAll(CV_VAR_NUMERICAL) );
	cvSetReal1D( var_type, 35+2+newDimension, CV_VAR_CATEGORICAL );
    
	
	cvSave("Features.xml",allFeatures);
    cvSave("Labels.xml",labels);
    cvSave("VarType.xml",var_type);
	
/*	
    boost.train( allFeatures, CV_ROW_SAMPLE, labels, 0, 0, var_type, 0,
            CvBoostParams(CvBoost::REAL, 50, 0.95, 5, false, 0 ));
    
    boost.save( "boost_classifier.dat" ); 
    
    
  cout<< "Classifier Trained!\n";
  */
  cvReleaseMat(&var_type);
  cvReleaseMat(&allFeatures);
  cvReleaseMat(&labels);
//  cvReleaseMat(&ProjectionMatrix);
  cvReleaseMat(&Features);
  //!Test the classifier
  
  
  
  
  //!Clean Up
  cleanUp();
  

 
}
