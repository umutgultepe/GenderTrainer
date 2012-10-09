#include "filereader.hpp"

/*function... might want it in some class?*/
int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}


void getName(char * &dir){
  string dirx = string(dir);
  int found;
  found=dirx.find_last_of("/\\");
  dirx=dirx.substr(found+1);
 
  found=dirx.find_last_of(".\\");
  dirx=dirx.substr(0,found);
  
 
  dir = new char [dirx.size()+1];
  strcpy (dir, dirx.c_str());
  dirx.clear();

}
// seperate jpgs from folders or files if there is anything else it is accepted as folders
void  seperateFolder(vector<string> &files, vector<string> &folder, int &isize, int &fsize){
    string jpg = ".jpg";
    string dot = ".";
    string dott = "..";
    vector<string> newimages; // buffer
    for (unsigned int i = 0;i < files.size();i++) {
      //if(files[i].find(dot) != string::npos){
	if(files[i].find(dot) != 0){
	if(files[i].find(dott) == string::npos){
	  
	// cout<<files[i]<<"\n";
	  newimages.push_back(files[i]);
	}
      }

	   
	} 
	
    // for not producing . and ..

    newimages.swap(files);
    newimages.clear(); // clear buffer
    isize = files.size();
    fsize = folder.size();
}
void fileSearch(string dir,vector<string> &images, vector<string> &folder , int &FoldreNum,int &ImgaeNum ){
    
    getdir(dir,images);
    seperateFolder(images, folder, ImgaeNum, FoldreNum);
  
}
void findMatch(vector<string> &files, vector<string> &folder){
    vector<string> jpegwithfolder = vector<string>();
    vector<string> jpegwithoutfolder = vector<string>();
    vector<string> folderwithjpeg = vector<string>();
    vector<int> toBeDeleted =vector<int>(); // list  the items that are in jpegfolder    
    
    if(folder.size()!= 0){
      
	for(unsigned int k = 0; k < files.size(); k++)
      {for (unsigned int i = 0; i < folder.size(); i++)
	{	
	  if((files[k].find(folder[i]) != string::npos)){
	   //cout<< "create menu named: " << folder[i] << " with image: "<< files[k]<< endl;
	    jpegwithfolder.push_back( files[k]);
	    folderwithjpeg.push_back(folder[i]);
	    toBeDeleted.push_back(k);
	  }else{
	      
	  }
	}
	
      }
    }
     for(int i = 0 ; i< files.size() ; i++)  
  //  cout << files[i]<<endl;
    for(int i = 0 ; i< toBeDeleted.size() ; i++)
    {files.erase(files.begin()+toBeDeleted[i]-i);
     // cout<< toBeDeleted[i]<<endl;
      
    }
    for(int i = 0 ; i< files.size() ; i++)  
  //  cout << files[i]<<endl;
    
    jpegwithoutfolder = files;
	    
    folder.clear();
    folder = folderwithjpeg;
    files.clear();
    files = jpegwithfolder;
    for (int i = 0 ; i < jpegwithoutfolder.size() ; i++ ){
      files.push_back(jpegwithoutfolder[i]);
    }
    jpegwithfolder.clear();
    jpegwithoutfolder.clear();
    folderwithjpeg.clear();
    toBeDeleted.clear();
}



char* str2char(string dir){
  
    char* dirx = new char[dir.size()+1];
    strcpy (dirx, dir.c_str());
    
    return dirx;
}

string char2str(char* dir){
    string dirx = string(dir); 
    /*for (int i= 0 ; i< dir.size() ; i++){
      dirx[i]=dir[i];
    }*/
    return dirx;
  
  
}

char ** strv2charp( vector<string> dir){
  char ** dirx = new char*[dir.size()];
  for(int i = 0 ; i<dir.size(); i++ ){
    dirx[i] = str2char( dir[i]);
    
  }
  return dirx;
}
void convertFullDirectory(vector<string> &dir,char* dirx){
  string dirxx = string(dirx);
  for(int i = 0 ; i<dir.size(); i++ ){
    dir[i] = dirxx+'/'+dir[i];
    
  }
  //dirxx.clear();
  
}
void okans_file_reader( char** &directories, char** &jpegs,int &dirCount,int &jpCount,  char* OVERVIEW_DIRECTORY){
    vector<string> files = vector<string>();
    vector<string> folder = vector<string>();
    int FoldreNum,ImgaeNum;
    cout<<OVERVIEW_DIRECTORY<<endl;
    string dir= string(OVERVIEW_DIRECTORY);
    
    fileSearch(dir,files,folder,FoldreNum, ImgaeNum);
    
    findMatch(files,folder);
    
    
    
    convertFullDirectory(files,OVERVIEW_DIRECTORY);
    jpegs = strv2charp(files);
    convertFullDirectory(folder,OVERVIEW_DIRECTORY);
    directories = strv2charp(folder);
    
    dirCount = FoldreNum;
    jpCount = ImgaeNum;
//     for(int i = 0 ; i<files.size(); i++ ){
//       cout<<files[i]<<endl;
//     }
//     for(int i = 0 ; i<folder.size(); i++ ){
//       cout<<folder[i]<<endl;
//     }
    files.clear();
    folder.clear();
}



void okans_file_reader_mod( char** &images,int &imgCount,  char* OVERVIEW_DIRECTORY)
{
    vector<string> files = vector<string>();
    vector<string> folder = vector<string>();
    int FoldreNum,ImgaeNum;
    cout<<OVERVIEW_DIRECTORY<<endl;
    string dir= string(OVERVIEW_DIRECTORY);
    
    fileSearch(dir,files,folder,FoldreNum, ImgaeNum);
    
 //   findMatch(files,folder);
    
    
    
    convertFullDirectory(files,OVERVIEW_DIRECTORY);
    images = strv2charp(files);
    imgCount = ImgaeNum;
//     for(int i = 0 ; i<files.size(); i++ ){
//       cout<<files[i]<<endl;
//     }
//     for(int i = 0 ; i<folder.size(); i++ ){
//       cout<<folder[i]<<endl;
//     }
    files.clear();
    folder.clear();
}
