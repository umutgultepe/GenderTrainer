#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;
int getdir (string dir, vector<string> &files);
void getName(char * &dir);
void  seperateFolder(vector<string> &files, vector<string> &folder, int &isize, int &fsize);
void fileSearch(string dir,vector<string> &images, vector<string> &folder , int &FoldreNum,int &ImgaeNum );
void findMatch(vector<string> &files, vector<string> &folder);
char* str2char(string dir);
string char2str(char* dir);
char ** strv2charp( vector<string> dir);
void convertFullDirectory(vector<string> &dir,char* dirx);
void okans_file_reader( char** &directories, char** &jpegs,int &dirCount,int &jpCount,  char* OVERVIEW_DIRECTORY);
void okans_file_reader_mod( char** &images,int &imgCount,  char* OVERVIEW_DIRECTORY);