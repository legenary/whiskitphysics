
#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#define PI 3.1415927

#define LINK_THETA_MAX PI/4
#define LINK_PHI_MAX PI/4
#define LINK_ZETA_MAX PI/360

#include <string>
#include <vector>
#include "LinearMath/btVector3.h"

struct Parameters{
	

	// integration parameters
	float TIME_STEP;
	int NUM_STEP_INT;
	float TIME_STOP;

	// simulation parameters
	int DEBUG;
	int PRINT;
	int SAVE;
	int SAVE_VIDEO;
	
	// collision object type
	int OBJECT;

	// whisker model parameters
    std::vector<std::string> WHISKER_NAMES;
    float BLOW; // scale whisker diameter for visibility - ATTENTION: will affect dynamics!!!
	int NO_CURVATURE; // remove curvature for debugging
	int NO_MASS; 	  // set mass to zero for debugging
	int NO_WHISKERS;  // remove whiskers for debugging
	int NUM_LINKS;
	float ROH_BASE;
	float ROH_TIP;
	float E;
	float ZETA;
	
	// configuration parameters of rat head
	std::vector<float> POSITION;
	std::vector<float> ORIENTATION;
	float PITCH;
	float YAW;
	float ROLL;

	// whisking parameters
	int ACTIVE;
	float AMP_BWD; // in degrees
	float AMP_FWD; // in degrees
	float WHISK_FREQ; // in hertz
	float SPEED; // in mm/second
	
	// camera configuration
	float DIST;
	float CPITCH;
	float CYAW;

	// directories/paths
	std::string dir_out;
	std::string file_video;
	std::string file_env;
};

void set_default(Parameters* param);
std::vector<float> get_vector(int N, float value);
std::vector<float> stringToFloatVect(std::vector<std::string> vect_string);

#endif