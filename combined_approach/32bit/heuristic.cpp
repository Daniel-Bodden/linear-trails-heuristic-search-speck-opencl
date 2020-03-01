//  heuristic.cpp
//  LSARXCL
//
//  Created by Db on 15/03/17.
//  Copyright (c) 2017 Daniel Bodden. All rights reserved.
//

//=========================INCLUDES=========================================================================
#include <stdlib.h>         // malloc, calloc, realloc, free */
#include <stdio.h>          //
#include <iostream>         // std::cout
#include <fstream>          // std::ifstream
#include <sstream>          // std::ostringstream
#include <string>           //
#include <vector>           //
#include <iomanip>          // to output cout with hex numbers
#include <sys/time.h>       // to keep time
#include "heuristic.h"    // include header file
#include <cmath>

//Include source files for OPENCL
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


//=========================NAMESPACES=======================================================================
using namespace std;
using namespace heuristic_lib;
//===========================================================================================================



//=========================GENERATE KERNEL FUNCTION==========================================================
void::heuristic_lib::Heuristic::generateKernel()
{
	// TODO

}
//===========================================================================================================


//===============================================================
//FUNCTION RETURNS THE NUMBER OF BITS IN A MASK
cl_ulong countOneBits(cl_ulong k)
{
    int bits = 0;
    int i = __builtin_popcountll(k); // returns number of one bits
    bits = i;
    
    return bits;
}
//===============================================================


//===============================================================
//CALCULATE HAMMINGWEIGHT
cl_ulong hammingWeight(cl_ulong mask)
{
    int value = 0;
    value = (countOneBits(mask)) ;
    
    return value;
};
//===============================================================



//==========================WRITE TO FILE - COMBO FORWARD BACKWARD===============================================
void::heuristic_lib::Heuristic::saveRoundResultsComboToFile(cl_ulong *arrayResults, const char* filename, cl_ulong nrRounds,int typeOfoutput)
{
	/* typeOfoutput =
	 * 1 - best
	 * 2 - rounds
	 * 3 - combo
	 */



	if( remove(filename) != 0 )
	{
		perror( "no file to remove" );
	}
	ofstream outputFile (filename);

	//Check choices of output format
	if(typeOfoutput == 1)
	{
		outputFile  << "BEST ROUND: \t";
		outputFile  << arrayResults[0];
		outputFile  << "\n";
		outputFile  << "BIAS: \t";
		outputFile << arrayResults[5];
		outputFile  << "\n";
		outputFile << "Output MASK X last successful round: \t";
		outputFile  <<  "0x" <<  hex << arrayResults[1];
		outputFile  << "\n";
		outputFile  << "Output MASK Y last successful round: \t";
		outputFile  << "0x" << hex << arrayResults[2];
		outputFile << "\n";
		outputFile  << "Starting mask X: \t";
		outputFile  << "0x" <<  hex << arrayResults[3];
		outputFile  << "\n";
		outputFile  << "Starting mask Y: \t";
		outputFile << "0x" << hex << arrayResults[4];
	}
	else if(typeOfoutput == 2)
	{
		outputFile << "ROUND: \t";
		outputFile << "BIAS: \t";
		outputFile <<  "INPUT MASK Xi: \t";
		outputFile << "INPUT MASK Yi: \t";
		outputFile << "OUTPUT MASK Xi+1: \t";
		outputFile << "OUTPUT MASK Yi+1: \t";
		outputFile << "\n";

		for (cl_ulong round = 1; round <= nrRounds; round++)
		{
			if( arrayResults[((round-1)*6)+5] !=99  && arrayResults[((round-1)*6)] !=0 &&   arrayResults[((round-1)*6)] < nrRounds)
			{
				outputFile << std::dec << arrayResults[((round-1)*6)];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << std::dec << arrayResults[((round-1)*6)+5];
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "0x" << std::hex << arrayResults[((round-1)*6) +1];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "0x" << std::hex << arrayResults[((round-1)*6) +2];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << std::hex <<  arrayResults[((round-1)*6) +3];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << std::hex <<  arrayResults[((round-1)*6) +4];
				outputFile << "\n";
			}
			else
			{
				outputFile << dec << round;
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "NA";
				outputFile << "\n";
			}
		}
	}
	else if (typeOfoutput == 3)
	{
		outputFile << "ROUND: \t";
		outputFile << "BIAS: \t";
		outputFile << "Starting Xi: \t";
		outputFile << "Starting Yi: \t";
		outputFile << "#rounds backward: \t";
		outputFile << "#rounds forward: \t";
		outputFile << "\n";

		for (cl_ulong round = 1; round <= nrRounds; round++)
		{
			if(arrayResults[((round-1)*6)] !=0 &&  arrayResults[((round-1)*6)] <= nrRounds)
			{
				outputFile << std::dec << arrayResults[((round-1)*6)];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << std::dec << arrayResults[((round-1)*6)+5];
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "0x" << std::hex << arrayResults[((round-1)*6) +1];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "0x" << std::hex << arrayResults[((round-1)*6) +2];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << std::dec <<  arrayResults[((round-1)*6) +3];
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << std::dec <<  arrayResults[((round-1)*6) +4];
				outputFile << "\n";
			}
			else
			{
				outputFile << dec << round;
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "NA";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile << "\t";
				outputFile <<  "NA";
				outputFile << "\n";
			}
		}
	}

	outputFile.close();
}
//==============================================================================================================



//======================KEEP TRACK OF COMPUTATION TIME==========================================================

double::heuristic_lib::Heuristic::get_wall_time()
{
	struct timeval time;
	if (gettimeofday(&time,NULL))
	{
		//  Handle error
		return 0;
	}
	return (double)time.tv_sec + (double)time.tv_usec * .000001;
}
//==============================================================================================================


//======================KEEP TRACK OF COMPUTATION CPU TIME=======================================================
double::heuristic_lib::Heuristic::get_cpu_time()
{
	return (double)clock() / CLOCKS_PER_SEC;
}
//==============================================================================================================


//=======================DISPLAY PLATFORM AND DEVICE INFORMATION ===============================================
void::heuristic_lib::Heuristic::displayInfo()
{

	//todo

}
//==============================================================================================================


//=======================CHECK FOR ERRORS IN OPENCL CODE========================================================
void::heuristic_lib::Heuristic::checkErr(cl_int err, const char * name)
{
	if (err != CL_SUCCESS) {
		std::cerr << "ERROR: " <<  name << " (" << err << ")" << std::endl;
		exit(EXIT_FAILURE);
	}
}
//==============================================================================================================


//=========================PRINT INTEGERS AS BINARY=========================================================
void::heuristic_lib::Heuristic::printBinary(cl_long n, cl_long blocksize)
{
	cl_long k;
	
	cout << "**************************** \n";
	for (cl_long c = (blocksize-1); c >= 0; c--)
	{
		k = n >> c;

		if (k & 1)
			printf("1");
		else
			printf("0");
	}
	cout << "\n";
	cout << "**************************** \n";
}
//==============================================================================================================


//==============================================================================================================

///// Look-up table /////

const cl_ulong getHexBit64_consecutive[32] = { 
	0x3,      //11 //1 (2bit)
	0xc,      //1100 //2 (4bit)
	0x30,     //110000 //3 (6bit)
	0xC0,     //11000000 //4 (8bit)
	0x300,    //1100000000 //5 (10bit)
	0xC00,    //110000000000 //6  (12bit)
	0x3000,   //11000000000000//7 (14bit)
	0xC000,   //1100000000000000 //8 (16bit)
	0x30000,  //110000000000000000 //9 (18bit)
	0xC0000,  //11000000000000000000 //10 (20bit)
	0x300000, //1100000000000000000000 //11 (22bit)
	0xC00000, //110000000000000000000000 //12 (24bit)
	0x3000000,   //11000000000000000000000000 //13 (26bit)
	0xC000000,   //1100000000000000000000000000 //14 (28bit)
	0x30000000,   //110000000000000000000000000000 //15 (30bit)
	0xC0000000,   //11000000000000000000000000000000 //16 (32bit)
	0x300000000,  //1100000000000000000000000000000000 //17 (34bit)
	0xC00000000,  //110000000000000000000000000000000000 //18 (36bit)
	0x3000000000,  //11000000000000000000000000000000000000 //19 (38bit)
	0xC000000000,  //1100000000000000000000000000000000000000 //20 (40bit)
	0x30000000000,  //110000000000000000000000000000000000000000 //21 (42bit)
	0xC0000000000,  //11000000000000000000000000000000000000000000 //22 (44bit)
	0x300000000000,  //1100000000000000000000000000000000000000000000 //23 (46bit)
	0xC00000000000,  //110000000000000000000000000000000000000000000000 //24 (48bit)
	0x3000000000000,  //11000000000000000000000000000000000000000000000000 //25 (50bit)
	0xC000000000000,  //1100000000000000000000000000000000000000000000000000 //26 (52bit)
	0x30000000000000,  //110000000000000000000000000000000000000000000000000000 //27 (54bit)
	0xC0000000000000,  //11000000000000000000000000000000000000000000000000000000 //28 (56bit)
	0x300000000000000,  //1100000000000000000000000000000000000000000000000000000000 //29 (58bit)
	0xC00000000000000,  //110000000000000000000000000000000000000000000000000000000000 //30 (60bit)
	0x3000000000000000,  //11000000000000000000000000000000000000000000000000000000000000 //31 (62bit)
	0xC000000000000000,  //1100 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 0000 //32 (64bit)

		
    };
//==============================================================================================================


	///// Look-up table /////

	const cl_ulong getHexBit64[65] = { 
		0x1,      //1 //1
		0x2,      //10 //2
		0x4,      //100 //3
		0x8,      //1000 //4
		0x10,     //10000 //5
		0x20,     //100000 //6
		0x40,     //1000000 //7
		0x80,     //10000000 //8
		0x100,    //100000000 //9
		0x200,    //1000000000 //10
		0x400,    //10000000000 //11
		0x800,    //100000000000 //12
		0x1000,   //1000000000000 //13 
		0x2000,   //10000000000000 //14
		0x4000,   //100000000000000 //15
		0x8000,   //1000000000000000 //16
		0x10000,  //10000000000000000 //17
		0x20000,  //100000000000000000 //18
		0x40000,  //1000000000000000000 //19
		0x80000,  //10000000000000000000 //20
		0x100000,  //100000000000000000000 //21
		0x200000,  //1000000000000000000000 //22
		0x400000,  //10000000000000000000000 //23
		0x800000,  //100000000000000000000000 //24
		0x1000000,  //1000000000000000000000000 //25
		0x2000000,  //10000000000000000000000000 //26
		0x4000000,  //100000000000000000000000000 //27
		0x8000000,  //1000000000000000000000000000 //28
		0x10000000,  //10000000000000000000000000000 //29
		0x20000000,  //100000000000000000000000000000 //30
		0x40000000,  //1000000000000000000000000000000 //31
		0x80000000,  //10000000000000000000000000000000 //32
		0x100000000,  //100000000000000000000000000000000 //33
		0x200000000,  //1000000000000000000000000000000000 //34
		0x400000000,  //10000000000000000000000000000000000 //35
		0x800000000,  //100000000000000000000000000000000000 //36
		0x1000000000,  //1000000000000000000000000000000000000 //37
		0x2000000000,  //10000000000000000000000000000000000000 //38
		0x4000000000,  //100000000000000000000000000000000000000 //39
		0x8000000000,  //1000000000000000000000000000000000000000 //40
		0x10000000000,  //10000000000000000000000000000000000000000 //41
		0x20000000000,  //100000000000000000000000000000000000000000 //42
		0x40000000000,        //1000000000000000000000000000000000000000000 //43
		0x80000000000,        //10000000000000000000000000000000000000000000 //44
		0x100000000000,       //100000000000000000000000000000000000000000000 //45
		0x200000000000,       //1000000000000000000000000000000000000000000000 //46
		0x400000000000,       //10000000000000000000000000000000000000000000000 //47
		0x800000000000,       //100000000000000000000000000000000000000000000000 //48
		0x1000000000000,      //1000000000000000000000000000000000000000000000000 //49
		0x2000000000000,      //10000000000000000000000000000000000000000000000000 //50
		0x4000000000000,      //100000000000000000000000000000000000000000000000000 //51
		0x8000000000000,      //1000000000000000000000000000000000000000000000000000 //52
		0x10000000000000,     //10000000000000000000000000000000000000000000000000000 //53
		0x20000000000000,     //100000000000000000000000000000000000000000000000000000 //54
		0x40000000000000,     //1000000000000000000000000000000000000000000000000000000 //55
		0x80000000000000,     //10000000000000000000000000000000000000000000000000000000 //56
		0x100000000000000,    //100000000000000000000000000000000000000000000000000000000 //57
		0x200000000000000,    //1000000000000000000000000000000000000000000000000000000000 //58
		0x400000000000000,    //10000000000000000000000000000000000000000000000000000000000 //59
		0x800000000000000,    //100000000000000000000000000000000000000000000000000000000000 //60
		0x1000000000000000,   //1000000000000000000000000000000000000000000000000000000000000 //61
		0x2000000000000000,   //10000000000000000000000000000000000000000000000000000000000000 //62
		0x4000000000000000,   //100000000000000000000000000000000000000000000000000000000000000 //63
		0x8000000000000000   //1000000000000000000000000000000000000000000000000000000000000000 //64		
	    };
	//==============================================================================================================



//=========================RUN OPENCL FOR ===========================================================================
/* OPENCL STEPS
 * Step 1  : Obtain platform
 * Step 2  : Obtain device id for at least one device (accelerator)
 * Step 3  : Create context for device (getting info about the device)
 * Step 4  : Create accelerator program from source code (read the kernel file(s))
 * Step 5  : Build the program
 * STEP 6  : Create cl_kernel(s) from program functions
 * STEP 7  : Create command queue for kernel dispatch
 * STEP 8  : Allocate device memory / move input data to device
 * Step 9  : Associate arguments to kernel with kernel object
 * Step 10 : Deploy kernel for device execution
 * Step 11 : Write output device data back to host
 * Step 12 : Release context/program/kernels/memory
 */
void::heuristic_lib::Heuristic::runCipher()
{

	//==================================================================================
	//0. Declare OpenCL variables
	cl_int errNum;                             // Storing error codes generated by OpenCL
	cl_uint numPlatforms;                      // Storing the number of avaialable platforms
	cl_uint numDevices;                        // Storing the number of available devices
	cl_platform_id * platformIDs;              // Storing the avaialble platformIDs
	cl_device_id * deviceIDs;                  // Storing the available deviceIDs
	cl_context context;                        // The container for all opencl objecta
	cl_program program;                        // To compile the opencl programe
	int platform = 0;                          // set platform fixed to host computer (when working in clusters this needs to be dynami
	//std::vector<cl_command_queue> queues;
	std::vector<cl_kernel> kernels;
	std::vector<cl_mem> buffersPairs;
	std::vector<cl_mem> buffersCipher;


	//==================================================================================
	// 1. Select a platform (NVIDIA Cuda, AMD APP, Intel SDK...)

	// a. Get number of avaiable platforms
	errNum = clGetPlatformIDs( 0, NULL,&numPlatforms);
	checkErr( (errNum != CL_SUCCESS) ? errNum : (numPlatforms <= 0 ? -1 : CL_SUCCESS),   "clGetPlatformIDs");

	// b. Reserve memory for storing the platform IDs
	platformIDs = (cl_platform_id *)alloca( sizeof(cl_platform_id) * numPlatforms);
	std::cout << "Number of platforms: \t" << numPlatforms << std::endl;

	// c. Ge the platformIDs of the avaiable platforms
	errNum = clGetPlatformIDs(numPlatforms, platformIDs, NULL);
	checkErr( (errNum != CL_SUCCESS) ? errNum : (numPlatforms <= 0 ? -1 : CL_SUCCESS), "clGetPlatformIDs");


	// d. Iterate through the list of platforms displaying associated  information
	for( int i = 0; i < (int)numPlatforms; i++ )
	{
		fprintf( stderr, "Platform ID %d:\n", i );
		size_t size;
		char *str;

		clGetPlatformInfo( platformIDs[i], CL_PLATFORM_NAME, 0, NULL, &size );
		str = new char [ size ];
		clGetPlatformInfo( platformIDs[i], CL_PLATFORM_NAME, size, str, NULL );
		fprintf( stderr, "\tName    = '%s'\n", str );
		delete[ ] str;

		clGetPlatformInfo( platformIDs[i], CL_PLATFORM_VENDOR, 0, NULL, &size );
		str = new char [ size ];
		clGetPlatformInfo( platformIDs[i], CL_PLATFORM_VENDOR, size, str, NULL );
		fprintf( stderr, "\tVendor  = '%s'\n", str );
		delete[ ] str;

		clGetPlatformInfo( platformIDs[i], CL_PLATFORM_VERSION, 0, NULL, &size );
		str = new char [ size ];
		clGetPlatformInfo( platformIDs[i], CL_PLATFORM_VERSION, size, str, NULL );
		fprintf( stderr, "\tVersion = '%s'\n", str );
		delete[ ] str;

		clGetPlatformInfo( platformIDs[i], CL_PLATFORM_PROFILE, 0, NULL, &size );
		str = new char [ size ];
		clGetPlatformInfo(platformIDs[i], CL_PLATFORM_PROFILE, size, str, NULL );
		fprintf( stderr, "\tProfile = '%s'\n", str );
		delete[ ] str;
	}


	//==================================================================================
	//2.  Choose a device for this platform (GPU, CPU)

	// a. set deviceIDs on NULL
	deviceIDs = NULL;

	// b. Get number of available devices for the given platform in B
	errNum = clGetDeviceIDs(platformIDs[platform], CL_DEVICE_TYPE_CPU,0, NULL, &numDevices);
	if (errNum != CL_SUCCESS && errNum != CL_DEVICE_NOT_FOUND)
	{
		checkErr(errNum, "clGetDeviceIDs");
	}

	// c. Allocate memory tp
	deviceIDs = (cl_device_id *)alloca(sizeof(cl_device_id) * numDevices);

	// d. Get the devicesIDs of the avaible devices on the given platform
	errNum = clGetDeviceIDs(platformIDs[platform],CL_DEVICE_TYPE_CPU,numDevices,&deviceIDs[0],NULL);
	checkErr(errNum, "clGetDeviceIDs");


	// e. show information about available devices
	for( int j = 0; j < (int)numDevices; j++ )
	{
		fprintf( stderr, "\tDevice %d:\n", j );
		size_t size;
		cl_device_type type;
		cl_uint ui;
		size_t sizes[3] = { 0, 0, 0 };

		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_TYPE, sizeof(type), &type, NULL );

		fprintf( stderr, "\t\tType = 0x%04llx = ", type );
		switch( type )
		{
		case CL_DEVICE_TYPE_CPU:
			fprintf( stderr, "CL_DEVICE_TYPE_CPU\n" );
			break;
		case CL_DEVICE_TYPE_GPU:
			fprintf( stderr, "CL_DEVICE_TYPE_GPU\n" );
			break;
		case CL_DEVICE_TYPE_ACCELERATOR:
			fprintf( stderr, "CL_DEVICE_TYPE_ACCELERATOR\n" );
			break;
		default:
			fprintf( stderr, "Other...\n" );
			break;
		}


		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(ui), &ui, NULL );
		fprintf( stderr, "\t\tDevice Maximum Compute Units = %d\n", ui );

		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(ui), &ui, NULL );
		fprintf( stderr, "\t\tDevice Maximum Clock Frequency = %d MHz\n", ui );

		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(ui), &ui, NULL );
		fprintf( stderr, "\t\tDevice Maximum Work Item Dimensions = %d\n", ui );

		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(sizes), sizes, NULL );
		fprintf( stderr, "\t\tDevice Maximum Work Item Sizes = %zu x %zu x %zu\n", sizes[0], sizes[1], sizes[2] );

		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), &size, NULL );
		fprintf( stderr, "\t\tDevice Maximum Work Group Size = %zu\n", size );

		//Can be accessed by all workgroups and work items
		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(size), &size, NULL ); // the size for the global memory
		fprintf( stderr, "\t\tDevice Global Memory Size = %zu bytes\n", size );

		//can be read by all workgroups and work items - READ ONLY
		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(size), &size, NULL ); // the size for the Constant memory
		fprintf( stderr, "\t\tDevice Constant Memory Size = %zu bytes\n", size );

		//Can be accessed by all shared work items on a single work group
		clGetDeviceInfo( deviceIDs[j], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(size), &size, NULL ); // the size for the Local memory
		fprintf( stderr, "\t\tDevice Local Memory Size = %zu bytes\n", size );

	}

	//==================================================================================
	// 3. Create a context on this device

	// a. Retreive context properties
	cl_context_properties contextProperties[] ={CL_CONTEXT_PLATFORM,(cl_context_properties)platformIDs[platform], 0 };

	// b. Generate context
	context = clCreateContext(contextProperties,numDevices,deviceIDs,NULL,NULL,&errNum);
	checkErr(errNum, "clCreateContext");

	//==================================================================================
	//4. Create and compile the OpenCL program from sources

	//a. Load the opencl file with the kernels
	std::ifstream srcFile("computation.cl");
	checkErr(srcFile.is_open() ? CL_SUCCESS : -1, "computation.cl");

	std::string srcProg(std::istreambuf_iterator<char>(srcFile),(std::istreambuf_iterator<char>()));
	const char * src = srcProg.c_str();
	size_t length = srcProg.length();

	//b. Create program from source
	program = clCreateProgramWithSource(context,1,&src,&length, &errNum);
	checkErr(errNum, "clCreateProgramWithSource");

	//c.  Build opencl program
	errNum = clBuildProgram( program, numDevices, deviceIDs, "-I.", NULL,NULL);

	if (errNum != CL_SUCCESS)
	{
		// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo( program, deviceIDs[0], CL_PROGRAM_BUILD_LOG,sizeof(buildLog),buildLog,NULL);
		std::cerr << "Error in OpenCL C source: " << std::endl;
		std::cerr << buildLog;
		checkErr(errNum, "clBuildProgram");
	}

	//==========CREATE COMMAND queue===============================================================

	//Create queues per available devices============================
	cl_command_queue queue = clCreateCommandQueue(context,deviceIDs[0],0,&errNum); //execute in order
	checkErr(errNum, "clCreateCommandQueue");
	//queues.push_back(queue);





	cl_uint numberOfThreads;

	cl_ulong elementsLengthIn = 99999999; //75025
	cl_ulong dataSizeInput =sizeof(cl_ulong)*elementsLengthIn;
	cl_ulong* inputData = (cl_ulong*)malloc(dataSizeInput);
    
	//cl_ulong blocksize = 32;
	cl_ulong nrRounds = 22;

	//==========CREATE STATIC DATA===============================================================


	//1. Static allocation of variables
	cl_ulong outputBestRoundCombo[nrRounds*6];
	cl_ulong outputBestRoundsForward[nrRounds*6];
	cl_ulong outputBestRoundsBackward[nrRounds*6]; // 6 fields times the number of rounds
	cl_ulong outputBestOverallForward[6];
	cl_ulong outputBestOverallBackward[6];

	cl_ulong dataSizeOverall = sizeof(cl_ulong)*6;
	cl_ulong dataSizeRounds = sizeof(cl_ulong)*(nrRounds*6);


	//2. initialize BEST OVERALL RESULTS OUTPUT - forward & backward
	/* outMasksOverall[0] = round;          | current round
	 * outMasksOverall[1] = maskX; 			| inputmask X
	 * outMasksOverall[2] = maskY;          | inputmask Y
	 * outMasksOverall[3] = iMaskXRound;    | outputmask X at the start of round 1
	 * outMasksOverall[4] = iMaskYRound;    | outputmask Y at the start of round 1
	 * outMasksOverall[5] = count;          | BIAS
	 */
	for (int k =0; k < 6; k++)
	{
		outputBestOverallForward[k] = 0 ;
		outputBestOverallBackward[k] = 0 ;
	}

	//3. initialize BEST ROUND RESULTS OUTPUT - forward & backward
	/* outputStaticBestRounds[nrRounds][0] = round;          | current round
	 * outputStaticBestRounds[nrRounds][1] = maskX; 		 | inputmask X
	 * outputStaticBestRounds[nrRounds][2] = maskY;          | inputmask Y
	 * outputStaticBestRounds[nrRounds][3] = iMaskXRound;    | outputmask X at the start of round 1
	 * outputStaticBestRounds[nrRounds][4] = iMaskYRound;    | outputmask Y at the start of round 1
	 * outputStaticBestRounds[nrRounds][5] = count;          | BIAS
	 */

	/* OpenCL cannot deal with 2d arrays so we convert 2d to 1d
	 * Round        (i-1)6
	 * MaskX        ((i-1)*6) +1
	 * MaskY        ((i-1)*6) +2
	 * InputMaskX   ((i-1)*6) +3
	 * inputMaskY   ((i-1)*6) +4
	 * BIAS         ((i-1)*6) +5
	 */

	for (int i = 0; i < nrRounds; i++)
	{
		outputBestRoundsBackward[((i-1)*6) +5] = 99;
		outputBestRoundsForward[((i-1)*6) +5] = 99;
		outputBestRoundsBackward[((i-1)*6)] = 99;
		outputBestRoundsForward[((i-1)*6)] = 99;
		outputBestRoundCombo[((i-1)*6) +5] = 99;

	}

	//CREATE EVENTS to synchronize the execution================================================
	cl_event k_eventsComputeRead[5];    // To hold events

	//  Start Timers
	double wall0 = get_wall_time();
	double cpu0  = get_cpu_time();

	/*************************************************************
	 *CREATE DATA
	 **************************************************************/


	
	cout << "Start generating input data " << endl;
	cl_ulong blocksize = 16; 	        //Inner loop - pre-generated for the 28 bits
	
	cl_ulong counter = 0;                         


    inputData[counter] = 0x0;
    counter++;
	
	/*
	
	for (cl_ulong i1=blocksize-1  ;  i1<blocksize;i1++)
	{//i1
		
	for (cl_ulong i2=blocksize-2;i2<blocksize-1;i2++)
	{//i2
			
	for (cl_ulong i3=blocksize-3;i3<blocksize-2;i3++)
	{//i3
				
	for (cl_ulong i4=blocksize-4;i4<blocksize-3;i4++)
	{//i4
					
	for (cl_ulong i5=blocksize-5;i5<blocksize-4;i5++)
	{//i5
		
	for (cl_ulong i6=blocksize-6;i6<blocksize-5;i6++)
	{//i6
		
	for (cl_ulong i7=blocksize-7;i7<blocksize-6;i7++)
	{//i7
	
	for (cl_ulong i8=blocksize-8;i8<blocksize-7;i8++)
	{//i8
	
	for (cl_ulong i9=blocksize-9;i9<blocksize-8;i9++)
	{//i9
	
	for (cl_ulong i10=blocksize-10;i10<blocksize-9;i10++)
	{//i10
	
	for (cl_ulong i11=blocksize-11;i11<blocksize-10;i11++)
	{//i11
					
	for (cl_ulong i12=blocksize-12;i12<blocksize-11;i12++)
	{//i12
	
	for (cl_ulong i13=blocksize-13;i13<blocksize-12;i13++)
	{//i13
	
	for (cl_ulong i14=blocksize-14;i14<blocksize-13;i14++)
	{//i14
	
	for (cl_ulong i15=blocksize-15;i15<blocksize-14;i15++)
	{//i15
	
	for (cl_ulong i16=blocksize-16;i16<blocksize-15;i16++)
	{//i16
		//*******************************************************************
		//HAM1

	

		inputData[counter] =getHexBit64[i1];
		counter++;

		inputData[counter] =getHexBit64[i2];
		counter++;

		inputData[counter] =getHexBit64[i3];
		counter++;

		inputData[counter] =getHexBit64[i4];
		counter++;

		inputData[counter] =getHexBit64[i5];
		counter++;

		inputData[counter] =getHexBit64[i6];
		counter++;

		inputData[counter] =getHexBit64[i7];
		counter++;

		inputData[counter] =getHexBit64[i8];
		counter++;

		inputData[counter] =getHexBit64[i9];
		counter++;

		inputData[counter] =getHexBit64[i10];
		counter++;

		inputData[counter] =getHexBit64[i11];
		counter++;

		inputData[counter] =getHexBit64[i12];
		counter++;

		inputData[counter] =getHexBit64[i13];
		counter++;

		inputData[counter] =getHexBit64[i14];
		counter++;

		inputData[counter] =getHexBit64[i15];
		counter++;

		inputData[counter] =getHexBit64[i16];
		counter++;
		//*******************************************************************
		
		
		//*******************************************************************
		//HAM 2

	
		
		//i1
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i16];
		counter++;
		
		//i2
		inputData[counter] =getHexBit64[i2]^getHexBit64[i3];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i4];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i16];
		counter++;
		
		//i3
		inputData[counter] =getHexBit64[i3]^getHexBit64[i4];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i16];
		counter++;

		//i4
		inputData[counter] =getHexBit64[i4]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i16];
		counter++;
		
		//i5
		inputData[counter] =getHexBit64[i5]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i16];
		counter++;
		
		//i6
		inputData[counter] =getHexBit64[i6]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i16];
		counter++;
		
		//i7
		inputData[counter] =getHexBit64[i7]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i16];
		counter++;
		
		//i8
		inputData[counter] =getHexBit64[i8]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i16];
		counter++;
		
		//i9
		inputData[counter] =getHexBit64[i9]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i16];
		counter++;
		
		//i10
		inputData[counter] =getHexBit64[i10]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i16];
		counter++;
		
		//i11
		inputData[counter] =getHexBit64[i11]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i16];
		counter++;
		
		//i12
		inputData[counter] =getHexBit64[i12]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i16];
		counter++;
		
		//i13
		inputData[counter] =getHexBit64[i13]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i16];
		counter++;
		
		//i14
		inputData[counter] =getHexBit64[i14]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i16];
		counter++;
		
		//i15
		inputData[counter] =getHexBit64[i15]^getHexBit64[i16];
		counter++;
		//*******************************************************************



		//*******************************************************************
		//HAM 3
		
	
		//i1,
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i16];
		counter++;
		
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i6]^getHexBit64[i16];
		counter++;
	
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i7]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i8]^getHexBit64[i16];
		counter++;
		
		
		//ham4
		//i1,
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i4];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i3]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i4]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i5]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i6]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i7]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i8]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i9]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i10]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i10]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i10]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i10]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i10]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i10]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i11]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i11]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i11]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i11]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i11]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i12]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i12]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i12]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i12]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i13]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i13]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i13]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i14]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i14]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i2]^getHexBit64[i15]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i5];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i4]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i5]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i6]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i7]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i8]^getHexBit64[i16];
		counter++;

		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i9]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i10]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i10]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i10]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i10]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i10]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i10]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i11]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i11]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i11]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i11]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i11]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i12]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i12]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i12]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i12]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i13]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i13]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i13]^getHexBit64[i16];
		counter++;

		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i14]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i14]^getHexBit64[i16];
		counter++;

		inputData[counter] =getHexBit64[i1]^getHexBit64[i3]^getHexBit64[i15]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i6];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i5]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i6]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i7]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i8]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i9]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i10]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i10]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i10]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i10]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i10]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i10]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i11]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i11]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i11]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i11]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i11]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i12]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i12]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i12]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i12]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i13]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i13]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i13]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i14]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i14]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i4]^getHexBit64[i15]^getHexBit64[i16];
		counter++;
		
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i7];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i8];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i9];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i10];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i11];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i12];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i13];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i5]^getHexBit64[i6]^getHexBit64[i15];
		counter++;
		
		
		
		//ham5
		
		
		
		//ham6
		
		
		
		//ham7
		
		
		//ham8 



	
		
																	
													
																
																	
	//*******************************************************************
										
	}//i16
	}//i15
	}//i14
	}//i13
	}//i12
	}//i11			
	}//i10
	}//i9
	}//i8
	}//i7
	}//i6
	}//i5
	}//i4
	}//i3
	}//i2
	}//i1
	
	*/
	
	for(cl_long i1=1; i1 < 0xffff; i1++)
	{
		if( hammingWeight(i1)<= 8 )
		{
			inputData[counter] =i1;
			counter++;
		}
	}
	
	
	

	cout << "Number of pairs created: " << dec << counter << endl;
	
	size_t sizes[3] = { 0, 0, 0 };
		clGetDeviceInfo( deviceIDs[0], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(sizes), sizes, NULL );
		cl_ulong optimalmaxworkitemsize = sizes[0];
		cl_ulong nrpartion = round( float(counter) / float(optimalmaxworkitemsize)); 
		cout << "Partitions : " << dec << nrpartion <<  " optimal work item size " << optimalmaxworkitemsize << " total number of masks "  <<  counter <<    endl;
	
		//===================================================================================
		/*************************************************************
	 	*LOOP OVER DATA IN PARTITION
	 	**************************************************************/
		//Run over different partitions
		for( uint indexpartdim1 = 0 ; indexpartdim1 < nrpartion ;    indexpartdim1++   )
		{
			for( uint indexpartdim2 = 0 ; indexpartdim2 < nrpartion ;    indexpartdim2++   )
			{
			
				cout << "******************************************************** \n";
				cl_ulong  currentofssetdim1 = indexpartdim1*optimalmaxworkitemsize;
				cl_ulong  currentofssetdim2 = indexpartdim2*optimalmaxworkitemsize;
			
				cout << "Current ofsset position dim1  : " << dec << currentofssetdim1  <<  " and dim2 " <<   dec << currentofssetdim2 << endl; 
		
		
				/*************************************************************
			 	*SET BUFFER
			 	**************************************************************/
		
				// 0. GLOBAL MEMORY outputBestRoundCombo,
				cl_mem bufferCombo = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeRounds,NULL, &errNum);
				checkErr(errNum, "clCreateBuffer outputBestRoundCombo kernelCipher");
				// 1. GLOBAL MEMORY outMasksBestRoundBackward
				cl_mem bufferRBackward = clCreateBuffer(context,CL_MEM_READ_WRITE, dataSizeRounds, NULL, &errNum);
				checkErr(errNum, "clCreateBuffer  outMasksBestRoundBackward kernelCipher");
				// 2. GLOBAL MEMORY outMasksOverallBackward
				cl_mem bufferOBackward = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeOverall,NULL, &errNum);
				checkErr(errNum, "clCreateBuffer outMasksOverallBackward kernelCipher");
				// 3. GLOBAL MEMORY outMasksBestRoundForward
				cl_mem bufferRForward = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeRounds,NULL, &errNum);
				checkErr(errNum, "clCreateBuffer outMasksBestRoundForward kernelCipher");
				// 4. GLOBAL MEMORY outMasksOverallForward
				cl_mem bufferOForward = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeOverall,NULL, &errNum);
				checkErr(errNum, "clCreateBuffer outMasksOverallForward kernelCipher");
				// 5. GLOBAL MEMORY inputData
				cl_mem bufferInputCipher = clCreateBuffer(context,CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR , dataSizeInput,inputData, &errNum);
				checkErr(errNum, "clCreateBuffer inputData kernelCipher" );
	
		
		
				/*************************************************************
			 	*GENERATE KERNEL AND PASS TROUGH PARAMETERS
			 	**************************************************************/
		
    
			    cl_kernel kernelCipher =  clCreateKernel( program,"ARX_TOOLBOX_Cipher_kernel", &errNum);
			    checkErr(errNum, "clCreateKernel(ARX_TOOLBOX_Cipher_kernel creating)");
			    errNum = clSetKernelArg(kernelCipher, 0,sizeof(cl_mem), (void *)&bufferCombo);
			    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 0 )");
			    errNum |= clSetKernelArg(kernelCipher, 1, sizeof(cl_mem), (void *)&bufferRBackward);
			    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 1)");
			    errNum |= clSetKernelArg(kernelCipher, 2, sizeof(cl_mem), (void *)&bufferOBackward);
			    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 2)");
			    errNum |= clSetKernelArg(kernelCipher, 3, sizeof(cl_mem), (void *)&bufferRForward);
			    checkErr(errNum, "clSetKernelAg(ARX_TOOLBOX_Cipher_kernel 3)");
			    errNum |= clSetKernelArg(kernelCipher, 4, sizeof(cl_mem), (void *)&bufferOForward);
			    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 4)");
			    errNum = clSetKernelArg(kernelCipher, 5, sizeof(cl_mem), (void *)&bufferInputCipher);
			    checkErr(errNum, "clSetKernelAg(ARX_TOOLBOX_Cipher_kernel 5)");
	
		
		
				/*************************************************************
			 	*SCHEDULE UPLOAD OF THE DATA TO THE BUFFERS
			 	**************************************************************/
		
		
			    // 0. GLOBAL MEMORY outputBestRoundCombo,
			    errNum = clEnqueueWriteBuffer(queue,bufferCombo, CL_TRUE, 0,dataSizeRounds, outputBestRoundCombo, 0,NULL,NULL);
			    checkErr(errNum, "upload outputBestRoundCombo KernelCipher)");
			    // 1. GLOBAL MEMORY outMasksBestRoundBackward
			    errNum |= clEnqueueWriteBuffer(queue,bufferRBackward,CL_TRUE,0,dataSizeRounds,outputBestRoundsBackward,0,NULL,NULL);
			    checkErr(errNum, "upload outputBestRoundsBackward Kernelcipher");
			    // 2. GLOBAL MEMORY outMasksOverallBackward
			    errNum |= clEnqueueWriteBuffer(queue,bufferOBackward,CL_TRUE,0,dataSizeOverall,outputBestOverallBackward,0,NULL, NULL);
			    checkErr(errNum, "upload outputBestOverallBackward KernelCipher)");
			    // 3. GLOBAL MEMORY outMasksBestRoundForward
			    errNum = clEnqueueWriteBuffer(queue,bufferRForward,CL_TRUE,0,dataSizeRounds, outputBestRoundsForward,0,NULL, NULL);
			    checkErr(errNum, "upload outputBestRoundsForward KernelCipher)");
			    // 4. GLOBAL MEMORY outMasksOverallForward
			    errNum = clEnqueueWriteBuffer(queue,bufferOForward,CL_TRUE,0,dataSizeOverall, outputBestOverallForward,0,NULL, NULL);
			    checkErr(errNum, "upload outputBestOverallForward KernelCipher)");
			    // 5. GLOBAL MEMORY inputDataFirstCalculation
			    errNum |= clEnqueueWriteBuffer(queue,bufferInputCipher,CL_TRUE,0,dataSizeInput, inputData,0,NULL, NULL); // Wait unit previous kernel has been finished
			    checkErr(errNum, "upload inputDataFirstCalculation KernelCipher)");

    
				/*************************************************************
			 	*SCHEDULE KERNEL EXECUTION 
			 	**************************************************************/
		
		
		       //=======schedule kernel execution in the command queue======================================
		       /* call kernel for each device - kernel for execution on the device using the command-queue
		        * Queuing the kernel for execution does not mean that the kernel executes immediately.
		        * The kernel execution is put into the command-queue for later consumption by the device.
		        */
    
		       //Max number of work items that can assignment to Cipher Kernel -> depends on the memory requirements of the kernel.
		       //size_t  maxWGCipher;
		       //errNum = clGetKernelWorkGroupInfo (kernelCipher, deviceIDs[0],CL_KERNEL_WORK_GROUP_SIZE,sizeof(maxWGCipher), &maxWGCipher,NULL);
		       //cout << "Workgroupsize for Cipher Kernel " <<  maxWGCipher << endl;
    
	
			    //numberOfThreads = optimalmaxworkitemsize; // set to optimal size if device 
			    numberOfThreads = optimalmaxworkitemsize;
				size_t globalWorkSize[2] = {numberOfThreads,numberOfThreads};  
				size_t globalOffset[2] = {currentofssetdim1 ,currentofssetdim2 };
			    size_t localWorkSize[2] = {1,1};
		
			    // Wait with execution until previous kernel is finished
			    errNum = clEnqueueNDRangeKernel(
												queue,
												kernelCipher,
												2, 
												globalOffset, 
												globalWorkSize, 
												localWorkSize,
												0,
												NULL, 
												NULL);
										
			    checkErr(errNum, "Queue KERNAEL");
		
		
				/*************************************************************
			 	*READ RESULT DATA BACK
			 	**************************************************************/
		

			    // A. retreive best round combo results
			    clEnqueueReadBuffer(queue,bufferCombo,CL_TRUE,0,dataSizeRounds,outputBestRoundCombo,0,NULL, &k_eventsComputeRead[0]);
			    // b. retreive best round backward results
			    clEnqueueReadBuffer(queue,bufferRBackward,CL_TRUE,0,dataSizeRounds,outputBestRoundsBackward,0, NULL,&k_eventsComputeRead[1]);
			    // C. retreive best overall backward result
			    clEnqueueReadBuffer(queue,bufferOBackward,CL_TRUE,0,dataSizeOverall,outputBestOverallBackward,0, NULL,&k_eventsComputeRead[2]);
			    // D. retreive best round FORWARD results
			    clEnqueueReadBuffer(queue,bufferRForward,CL_TRUE,0,dataSizeRounds,outputBestRoundsForward,0,NULL, &k_eventsComputeRead[3]);
			    // E. retreive best overall FORWARD result
			    clEnqueueReadBuffer(queue,bufferOForward,CL_TRUE,0,dataSizeOverall,outputBestOverallForward,0,NULL,&k_eventsComputeRead[4]);
		
		
		
				//WAIT UNTIL ALL PREVIOUS WORK HAS FINISHED
		
				errNum - clWaitForEvents(5, k_eventsComputeRead); 
		
		
				//READ RESULT DATA BACK
		
				//std::cout <<  "writing Output for Best overall Result Backward ";
				saveRoundResultsComboToFile( outputBestOverallForward, "~/words32bitBestOverallResultForward.txt", nrRounds, 1);
				//std::cout << "- done" << endl;
				//std::cout <<  "writing Output for Best overall Result Backward ";
				saveRoundResultsComboToFile(outputBestOverallBackward,"~/words32bitBestOverallBackward.txt", nrRounds, 1);
				//std::cout << "- done" << endl;
				//std::cout <<  "writing Output for Best Round Result Forward ";
				saveRoundResultsComboToFile( outputBestRoundsForward,"~/words32bitBestRoundResultForward.txt", nrRounds, 2);
				// std::cout << "- done" << endl;
				//std::cout <<  "writing Output for Best Round Result Backward ";
				saveRoundResultsComboToFile( outputBestRoundsBackward,"~/words32bitBestRoundResultBackward.txt", nrRounds, 2);
				//std::cout << "- done" << endl;
				//std::cout <<  "writing Output for Best Round Result Combo ";
				saveRoundResultsComboToFile(outputBestRoundCombo,"~/words32bitBestRoundResultCombo.txt", nrRounds, 3);
				//std::cout << "- done" << endl;
		
				//WRITE RESULTS TO FILE AND SHOW ON SCREEN
		
				//PRINT BEST RESULTS -
		
				cout << "Advised mask for attack is \n";
				cout << "Rounds :" << outputBestOverallForward[0] << endl;
				cout << "Bias :" << outputBestOverallForward[5] << endl;
				cout << "Input xi : 0x" << std::hex << outputBestOverallForward[3] << endl;
				cout << "Input yi : 0x" << std::hex << outputBestOverallForward[4] << endl;
				cout << "last output xi : 0x" << std::hex << outputBestOverallForward[1] << endl;
				cout << "last output yi : 0x" << std::hex << outputBestOverallForward[2] << endl;
		
		
				cout << "**************************** \n";
		
				cout << "******************************************************** \n";
		
				//check time
				double wall1 = get_wall_time();
				double cpu1  = get_cpu_time();
				std::cout << "Parallized computation has fininished in: " << endl;
				std::cout << "CPU Time = "<< cpu1-cpu0  << endl;
				std::cout << "Wall Time = " << wall1 - wall0 << endl;
			
			}// end loop over partitions dim2
		
		
		} // END LOOP OVER PARTITIONS dim 1
		//===================================================================================
	
    /*
	// 0. GLOBAL MEMORY outputBestRoundCombo,
	cl_mem bufferCombo = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeRounds,NULL, &errNum);
	checkErr(errNum, "clCreateBuffer outputBestRoundCombo kernelCipher");
	// 1. GLOBAL MEMORY outMasksBestRoundBackward
	cl_mem bufferRBackward = clCreateBuffer(context,CL_MEM_READ_WRITE, dataSizeRounds, NULL, &errNum);
	checkErr(errNum, "clCreateBuffer  outMasksBestRoundBackward kernelCipher");
	// 2. GLOBAL MEMORY outMasksOverallBackward
	cl_mem bufferOBackward = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeOverall,NULL, &errNum);
	checkErr(errNum, "clCreateBuffer outMasksOverallBackward kernelCipher");
	// 3. GLOBAL MEMORY outMasksBestRoundForward
	cl_mem bufferRForward = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeRounds,NULL, &errNum);
	checkErr(errNum, "clCreateBuffer outMasksBestRoundForward kernelCipher");
	// 4. GLOBAL MEMORY outMasksOverallForward
	cl_mem bufferOForward = clCreateBuffer(context,CL_MEM_READ_WRITE , dataSizeOverall,NULL, &errNum);
	checkErr(errNum, "clCreateBuffer outMasksOverallForward kernelCipher");
	// 5. GLOBAL MEMORY inputData
	cl_mem bufferInputCipher = clCreateBuffer(context,CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR , dataSizeInput,inputData, &errNum);
	checkErr(errNum, "clCreateBuffer inputData kernelCipher" );

    
    
    
    //Generate KERNEL =================================================
    
    cl_kernel kernelCipher =  clCreateKernel( program,"ARX_TOOLBOX_Cipher_kernel", &errNum);
    checkErr(errNum, "clCreateKernel(ARX_TOOLBOX_Cipher_kernel creating)");
    errNum = clSetKernelArg(kernelCipher, 0,sizeof(cl_mem), (void *)&bufferCombo);
    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 0 )");
    errNum |= clSetKernelArg(kernelCipher, 1, sizeof(cl_mem), (void *)&bufferRBackward);
    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 1)");
    errNum |= clSetKernelArg(kernelCipher, 2, sizeof(cl_mem), (void *)&bufferOBackward);
    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 2)");
    errNum |= clSetKernelArg(kernelCipher, 3, sizeof(cl_mem), (void *)&bufferRForward);
    checkErr(errNum, "clSetKernelAg(ARX_TOOLBOX_Cipher_kernel 3)");
    errNum |= clSetKernelArg(kernelCipher, 4, sizeof(cl_mem), (void *)&bufferOForward);
    checkErr(errNum, "clSetKernelArg(ARX_TOOLBOX_Cipher_kernel 4)");
    errNum = clSetKernelArg(kernelCipher, 5, sizeof(cl_mem), (void *)&bufferInputCipher);
    checkErr(errNum, "clSetKernelAg(ARX_TOOLBOX_Cipher_kernel 5)");
    
    //====Schedule the upload of data to these buffers in the command queue==============
    
    // 0. GLOBAL MEMORY outputBestRoundCombo,
    errNum = clEnqueueWriteBuffer(queue,bufferCombo, CL_TRUE, 0,dataSizeRounds, outputBestRoundCombo, 0,NULL,NULL);
    checkErr(errNum, "upload outputBestRoundCombo KernelCipher)");
    // 1. GLOBAL MEMORY outMasksBestRoundBackward
    errNum |= clEnqueueWriteBuffer(queue,bufferRBackward,CL_TRUE,0,dataSizeRounds,outputBestRoundsBackward,0,NULL,NULL);
    checkErr(errNum, "upload outputBestRoundsBackward Kernelcipher");
    // 2. GLOBAL MEMORY outMasksOverallBackward
    errNum |= clEnqueueWriteBuffer(queue,bufferOBackward,CL_TRUE,0,dataSizeOverall,outputBestOverallBackward,0,NULL, NULL);
    checkErr(errNum, "upload outputBestOverallBackward KernelCipher)");
    // 3. GLOBAL MEMORY outMasksBestRoundForward
    errNum = clEnqueueWriteBuffer(queue,bufferRForward,CL_TRUE,0,dataSizeRounds, outputBestRoundsForward,0,NULL, NULL);
    checkErr(errNum, "upload outputBestRoundsForward KernelCipher)");
    // 4. GLOBAL MEMORY outMasksOverallForward
    errNum = clEnqueueWriteBuffer(queue,bufferOForward,CL_TRUE,0,dataSizeOverall, outputBestOverallForward,0,NULL, NULL);
    checkErr(errNum, "upload outputBestOverallForward KernelCipher)");
    // 5. GLOBAL MEMORY inputDataFirstCalculation
    errNum |= clEnqueueWriteBuffer(queue,bufferInputCipher,CL_TRUE,0,dataSizeInput, inputData,0,NULL, NULL); // Wait unit previous kernel has been finished
    checkErr(errNum, "upload inputDataFirstCalculation KernelCipher)");
    
    //=======schedule kernel execution in the command queue======================================
   
    //Max number of work items that can assignment to Cipher Kernel -> depends on the memory requirements of the kernel.
    //size_t  maxWGCipher;
    //errNum = clGetKernelWorkGroupInfo (kernelCipher, deviceIDs[0],CL_KERNEL_WORK_GROUP_SIZE,sizeof(maxWGCipher), &maxWGCipher,NULL);
    //cout << "Workgroupsize for Cipher Kernel " <<  maxWGCipher << endl;
    
    //Check whether CL_KERNEL_LOCAL_MEM_SIZE is less than or equal to CL_DEVICE_LOCAL_MEM_SIZE.
    
	//  lenghtConMasks 
	//  16b  1597
	//  24b
	//  32b  3524578
	//  48b
	//  64b
    
    numberOfThreads = counter; //fixed size
    size_t globalWorkSizeCalc[2] = {numberOfThreads,numberOfThreads};  // number of workgroups 2ˆ20
    size_t localWorkSizeCal[2] = {1,1};
    // Wait with execution until previous kernel is finished
    errNum = clEnqueueNDRangeKernel(queue,kernelCipher,2, NULL, globalWorkSizeCalc, localWorkSizeCal,0,NULL, NULL);
    checkErr(errNum, "Queue FIRST CALCULATION");
    
    //==================================================================================
    // 13.  Read back the results
    // A. retreive best round combo results
    clEnqueueReadBuffer(queue,bufferCombo,CL_TRUE,0,dataSizeRounds,outputBestRoundCombo,0,NULL, &k_eventsComputeRead[0]);
    // b. retreive best round backward results
    clEnqueueReadBuffer(queue,bufferRBackward,CL_TRUE,0,dataSizeRounds,outputBestRoundsBackward,0, NULL,&k_eventsComputeRead[1]);
    // C. retreive best overall backward result
    clEnqueueReadBuffer(queue,bufferOBackward,CL_TRUE,0,dataSizeOverall,outputBestOverallBackward,0, NULL,&k_eventsComputeRead[2]);
    // D. retreive best round FORWARD results
    clEnqueueReadBuffer(queue,bufferRForward,CL_TRUE,0,dataSizeRounds,outputBestRoundsForward,0,NULL, &k_eventsComputeRead[3]);
    // E. retreive best overall FORWARD result
    clEnqueueReadBuffer(queue,bufferOForward,CL_TRUE,0,dataSizeOverall,outputBestOverallForward,0,NULL,&k_eventsComputeRead[4]);
    
	/**READ RESULT DATA BACK
 
  

	//std::cout <<  "writing Output for Best overall Result Backward ";
	saveRoundResultsComboToFile( outputBestOverallForward, "/users/cosic/dbodden/speckheuristic_m12/32bit_HAM/words32bitBestOverallResultForward.txt", nrRounds, 1);
	//std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best overall Result Backward ";
	saveRoundResultsComboToFile(outputBestOverallBackward,"/users/cosic/dbodden/speckheuristic_m12/32bit_HAM/words32bitBestOverallBackward.txt", nrRounds, 1);
	//std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best Round Result Forward ";
	saveRoundResultsComboToFile( outputBestRoundsForward,"/users/cosic/dbodden/speckheuristic_m12/32bit_HAM/words32bitBestRoundResultForward.txt", nrRounds, 2);
	// std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best Round Result Backward ";
	saveRoundResultsComboToFile( outputBestRoundsBackward,"/users/cosic/dbodden/speckheuristic_m12/32bit_HAM/words32bitBestRoundResultBackward.txt", nrRounds, 2);
	//std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best Round Result Combo ";
	saveRoundResultsComboToFile(outputBestRoundCombo,"/users/cosic/dbodden/speckheuristic_m12/32bit_HAM/words32bitBestRoundResultCombo.txt", nrRounds, 3);
	//std::cout << "- done" << endl;
	
   
	
	/*WRITE RESULTS TO FILE AND SHOW ON SCREEN
		
	//PRINT BEST RESULTS -

	cout << "Advised mask for attack is \n";
	cout << "Rounds :" << outputBestOverallForward[0] << endl;
	cout << "Bias :" << outputBestOverallForward[5] << endl;
	cout << "Input xi : 0x" << std::hex << outputBestOverallForward[3] << endl;
	cout << "Input yi : 0x" << std::hex << outputBestOverallForward[4] << endl;
	cout << "last output xi : 0x" << std::hex << outputBestOverallForward[1] << endl;
	cout << "last output yi : 0x" << std::hex << outputBestOverallForward[2] << endl;


	cout << "**************************** \n";

	cout << "Advised mask for attack is \n";
	cout << "R: \t";
	cout << "B: \t";
	cout << "Xi: \t";
	cout << "Yi: \t";
	cout << "#b: \t";
	cout << "#f: \n";
	for (cl_ulong round = 1; round <= 23; round++)
	{
		if((
				outputBestRoundCombo[((round-1)*6)] !=0 ) &&
				(outputBestRoundCombo[((round-1)*6)] <= nrRounds) &&
				(outputBestRoundCombo[((round-1)*6)+5] !=99 )  )
		{
			cout << std::dec << outputBestRoundCombo[((round-1)*6)] << "\t";
			cout << std::dec << outputBestRoundCombo[((round-1)*6)+5] << "\t" ;
			cout << "0x" << std::hex << outputBestRoundCombo[((round-1)*6) +1] << "\t";
			cout << "0x" << std::hex << outputBestRoundCombo[((round-1)*6) +2] << "\t";
			cout << std::dec <<  outputBestRoundCombo[((round-1)*6) +3] << "\t";
			cout << std::dec <<  outputBestRoundCombo[((round-1)*6) +4] << "\n";
		}
		else
		{
			cout << dec << round << "\t";
			cout << "NA \t" ;
			cout << "NA \t" ;
			cout << "NA \t" ;
			cout << "NA \t" ;
			cout << "NA \n" ;
		}
	}
	cout << "******************************************************** \n";
	
	
	
	/**WRITE RESULTS TO FILE AND SHOW ON SCREEN

	//PRINT BEST RESULTS -

	cout << "Advised mask for attack is \n";
	cout << "Rounds :" << outputBestOverallForward[0] << endl;
	cout << "Bias :" << outputBestOverallForward[5] << endl;
	cout << "Input xi : 0x" << std::hex << outputBestOverallForward[3] << endl;
	cout << "Input yi : 0x" << std::hex << outputBestOverallForward[4] << endl;
	cout << "last output xi : 0x" << std::hex << outputBestOverallForward[1] << endl;
	cout << "last output yi : 0x" << std::hex << outputBestOverallForward[2] << endl;


	cout << "**************************** \n";

	cout << "******************************************************** \n";
	
  */

	
	//===================================================================================
	// 14. CLEAN UP - RETURN MEMORY

	/*
	clReleaseMemObject(bufferCombo);
	clReleaseMemObject(bufferRBackward);
	clReleaseMemObject(bufferOBackward);
	clReleaseMemObject(bufferRForward);
	clReleaseMemObject(bufferOForward);
	clReleaseMemObject(bufferInputCipher);
	clReleaseMemObject(bufferBlockSize);
	clReleaseMemObject(bufferRounds);
	clReleaseMemObject(bufferInDataPairs);
	clReleaseMemObject(bufferOutDataPairs);
	clReleaseMemObject(bufferOForward);
	clReleaseMemObject(bufferLeftPartWord );
	clReleaseKernel(kernelPairs);
	clReleaseKernel(kernelCipher);

  */

	//===========================STOP TIMERS==============================================
		/*
	double wall1 = get_wall_time();
	double cpu1  = get_cpu_time();
	std::cout << "Parallized computation has fininished in: " << endl;
	std::cout << "CPU Time = "<< cpu1-cpu0  << endl;
	std::cout << "Wall Time = " << wall1 - wall0 << endl;

*/
	//===================================================================================
	// 14. CLEAN UP - RETURN MEMORY
	errNum = clReleaseContext(context);

	clReleaseProgram(program);


	//Memory objects
	//Contexts
	//Command Queues
	//Programs
	//Kernels
	//Events



	std::cout << "Program completed successfully" << std::endl;

}
//=====================================================================================================================

//=============================================
//CONSTRUCTOR
heuristic_lib::Heuristic::Heuristic() // Give arguments when running translation program
{
	//1. Initialize values for OpenCL attributes

	runCipher();

}
//=============================================


//=============================================
//DESTRUCTOR
heuristic_lib::Heuristic::~Heuristic()
{





}
//=============================================


//=============================================

int main()
{

	Heuristic();
}
//=============================================
