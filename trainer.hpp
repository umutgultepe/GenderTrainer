#include "globalHeader.hpp"
#include "opencv2/core/core_c.h"
#include "opencv2/ml/ml.hpp"

static
int build_svm_classifier( char* data_filename );
static
int build_nbayes_classifier( char* data_filename );
static
int build_knearest_classifier( char* data_filename, int K );
static
int build_mlp_classifier( char* data_filename,
    char* filename_to_save, char* filename_to_load );

int build_boost_classifier( char* data_filename,
    char* filename_to_save, char* filename_to_load, double train_ratio ,int class_count);
static
int build_rtrees_classifier( char* data_filename,
    char* filename_to_save, char* filename_to_load );
static int
read_num_class_data( const char* filename, int var_count,
                     CvMat** data, CvMat** responses );

