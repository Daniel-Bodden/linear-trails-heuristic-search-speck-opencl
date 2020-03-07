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


//=========================PRINT INTEGERS AS BINARY=========================================================
void::heuristic_lib::Heuristic::printBinary(cl_long n, cl_long blocksize)
{
	cl_long k;
	for (cl_long c = (blocksize-1); c >= 0; c--)
	{
		k = n >> c;

		if (k & 1)
			printf("1");
		else
			printf("0");
	}
}
//==============================================================================================================


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

///// Look-up table /////

const cl_ulong getHexBit64[65] = { 
	//0x0,      //0 //0
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
	//0x10000000000000000   //10000000000000000000000000000000000000000000000000000000000000000 //65
		
    };
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


	//========================================================================================================================================
	//==========CREATE MEMORY=======FIRST ROUND GENERATED PAIRs ==============================================================================

	//=====================CREATE DYNAMIC DATA======================================



	cl_uint numberOfThreads;

	cl_ulong elementsLengthIn = 0xffffffff; //75025
	cl_ulong dataSizeInput =sizeof(cl_ulong)*elementsLengthIn;
	cl_ulong* inputData = (cl_ulong*)malloc(dataSizeInput);
    
	cl_ulong nrRounds = 28;

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
	 *CALCULATION
	 **************************************************************/
	


	
	cout << "Start generating input data " << endl;
	cl_ulong blocksize = 48; 	      
	cl_ulong counter = 0;                         
	//inital
    inputData[counter] = 0x0;
    counter++;


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

	for (cl_ulong i17=blocksize-17;i17<blocksize-16;i17++)
	{//i17
	
	for (cl_ulong i18=blocksize-18;i18<blocksize-17;i18++)
	{//i18
	
	for (cl_ulong i19=blocksize-19;i19<blocksize-18;i19++)
	{//i19
	
	for (cl_ulong i20=blocksize-20;i20<blocksize-19;i20++)
	{//i20
	
	for (cl_ulong i21=blocksize-21;i21<blocksize-20;i21++)
	{//i21
	
	for (cl_ulong i22=blocksize-22;i22<blocksize-21;i22++)
	{//i22
	
	for (cl_ulong i23=blocksize-23;i23<blocksize-22;i23++)
	{//i23
	
	for (cl_ulong i24=blocksize-24;i24<blocksize-23;i24++)
	{//i24
	
	for (cl_ulong i25=blocksize-25;i25<blocksize-24;i25++)
	{//i25
	
	for (cl_ulong i26=blocksize-26;i26<blocksize-25;i26++)
	{//i26
	
	for (cl_ulong i27=blocksize-27;i27<blocksize-26;i27++)
	{//i27
	
	for (cl_ulong i28=blocksize-28;i28<blocksize-27;i28++)
	{//i28
	
	for (cl_ulong i29=blocksize-29;i29<blocksize-28;i29++)
	{//i29
	
	for (cl_ulong i30=blocksize-30;i30<blocksize-29;i30++)
	{//i30
	
	for (cl_ulong i31=blocksize-31;i31<blocksize-30;i31++)
	{//i31
	
	for (cl_ulong i32=blocksize-32;i32<blocksize-31;i32++)
	{//i32
	
	for (cl_ulong i33=blocksize-33;i33<blocksize-32;i33++)
	{//i33
	
	for (cl_ulong i34=blocksize-34;i34<blocksize-33;i34++)
	{//i34
	
	for (cl_ulong i35=blocksize-35;i35<blocksize-34;i35++)
	{//i35
	
	for (cl_ulong i36=blocksize-36;i36<blocksize-35;i36++)
	{//i36
	
	for (cl_ulong i37=blocksize-37;i37<blocksize-36;i37++)
	{//i37
	
	for (cl_ulong i38=blocksize-38;i38<blocksize-37;i38++)
	{//i38
	
	for (cl_ulong i39=blocksize-39;i39<blocksize-38;i39++)
	{//i39
	
	for (cl_ulong i40=blocksize-40;i40<blocksize-39;i40++)
	{//i40
	
	for (cl_ulong i41=blocksize-41;i41<blocksize-40;i41++)
	{//i41
	
	for (cl_ulong i42=blocksize-42;i42<blocksize-41;i42++)
	{//i42
	
	for (cl_ulong i43=blocksize-43;i43<blocksize-42;i43++)
	{//i43
	
	for (cl_ulong i44=blocksize-44;i44<blocksize-43;i44++)
	{//i44
	
	for (cl_ulong i45=blocksize-45;i45<blocksize-44;i45++)
	{//i45
	
	for (cl_ulong i46=blocksize-46;i46<blocksize-45;i46++)
	{//i46
	
	for (cl_ulong i47=blocksize-47;i47<blocksize-46;i47++)
	{//i47
	
	for (cl_ulong i48=blocksize-48;i48<blocksize-47;i48++)
	{//i48
			
			
		//*******************************************************************
		//HAM1

		/*
		printBinary(getHexBit64[i1], blocksize); cout << endl;
		printBinary(getHexBit64[i2], blocksize); cout << endl;
		printBinary(getHexBit64[i3], blocksize); cout << endl;
		printBinary(getHexBit64[i15], blocksize); cout << endl;
		
		*/

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
		
		inputData[counter] =getHexBit64[i17];
		counter++;

		inputData[counter] =getHexBit64[i18];
		counter++;

		inputData[counter] =getHexBit64[i19];
		counter++;

		inputData[counter] =getHexBit64[i20];
		counter++;
		
		inputData[counter] =getHexBit64[i21];
		counter++;

		inputData[counter] =getHexBit64[i22];
		counter++;

		inputData[counter] =getHexBit64[i23];
		counter++;

		inputData[counter] =getHexBit64[i24];
		counter++;
		
		inputData[counter] =getHexBit64[i25];
		counter++;

		inputData[counter] =getHexBit64[i26];
		counter++;

		inputData[counter] =getHexBit64[i27];
		counter++;

		inputData[counter] =getHexBit64[i28];
		counter++;
		
		inputData[counter] =getHexBit64[i29];
		counter++;

		inputData[counter] =getHexBit64[i30];
		counter++;

		inputData[counter] =getHexBit64[i31];
		counter++;

		inputData[counter] =getHexBit64[i32];
		counter++;
		
		inputData[counter] =getHexBit64[i33];
		counter++;

		inputData[counter] =getHexBit64[i34];
		counter++;

		inputData[counter] =getHexBit64[i35];
		counter++;

		inputData[counter] =getHexBit64[i36];
		counter++;
		
		inputData[counter] =getHexBit64[i37];
		counter++;

		inputData[counter] =getHexBit64[i38];
		counter++;

		inputData[counter] =getHexBit64[i39];
		counter++;

		inputData[counter] =getHexBit64[i40];
		counter++;
		
		inputData[counter] =getHexBit64[i41];
		counter++;

		inputData[counter] =getHexBit64[i42];
		counter++;

		inputData[counter] =getHexBit64[i43];
		counter++;

		inputData[counter] =getHexBit64[i44];
		counter++;
		
		inputData[counter] =getHexBit64[i45];
		counter++;

		inputData[counter] =getHexBit64[i46];
		counter++;

		inputData[counter] =getHexBit64[i47];
		counter++;

		inputData[counter] =getHexBit64[i48];
		counter++;
		
		
		//*******************************************************************
		
		
		//*******************************************************************
		//HAM 2

		/*
		printBinary(getHexBit64[i1] ^ getHexBit64[i2], blocksize); cout << endl;
		printBinary(getHexBit64[i1] ^ getHexBit64[i3], blocksize); cout << endl;
		printBinary(getHexBit64[i2] ^ getHexBit64[i3], blocksize); cout << endl;

		*/
		
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
		inputData[counter] =getHexBit64[i1]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i1]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i2]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i2]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i3]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i3]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i4]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i4]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i5]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i5]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i6]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i6]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i7]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i7]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i8]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i8]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i9]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i9]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i10]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i10]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i11]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i11]^getHexBit64[i48];
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
		inputData[counter] =getHexBit64[i12]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i12]^getHexBit64[i48];
		counter++;
		
		//i13
		inputData[counter] =getHexBit64[i13]^getHexBit64[i14];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i16];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i13]^getHexBit64[i48];
		counter++;
		
		//i14
		inputData[counter] =getHexBit64[i14]^getHexBit64[i15];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i16];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i14]^getHexBit64[i48];
		counter++;
		
		//i15
		inputData[counter] =getHexBit64[i15]^getHexBit64[i16];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i15]^getHexBit64[i48];
		counter++;
		
		//i16
		inputData[counter] =getHexBit64[i16]^getHexBit64[i17];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i16]^getHexBit64[i48];
		counter++;
		
		//i17
		inputData[counter] =getHexBit64[i17]^getHexBit64[i18];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i24];
		counter++;	
		inputData[counter] =getHexBit64[i17]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i17]^getHexBit64[i48];
		counter++;
		
		//i18
		inputData[counter] =getHexBit64[i18]^getHexBit64[i19];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i18]^getHexBit64[i48];
		counter++;
		
		//i19
		inputData[counter] =getHexBit64[i19]^getHexBit64[i20];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i19]^getHexBit64[i48];
		counter++;
		
		//i20
		inputData[counter] =getHexBit64[i20]^getHexBit64[i21];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i20]^getHexBit64[i48];
		counter++;
		
		//i21
		inputData[counter] =getHexBit64[i21]^getHexBit64[i22];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i21]^getHexBit64[i48];
		counter++;
		
		//i22
		inputData[counter] =getHexBit64[i22]^getHexBit64[i23];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i22]^getHexBit64[i48];
		counter++;
		
		
		//i23
		inputData[counter] =getHexBit64[i23]^getHexBit64[i24];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i23]^getHexBit64[i48];
		counter++;
		
		//i24
		inputData[counter] =getHexBit64[i24]^getHexBit64[i25];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i24]^getHexBit64[i48];
		counter++;
		
		//i25
		inputData[counter] =getHexBit64[i25]^getHexBit64[i26];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i25]^getHexBit64[i48];
		counter++;
		
		//i26
		inputData[counter] =getHexBit64[i26]^getHexBit64[i27];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i26]^getHexBit64[i48];
		counter++;	
		
		//i27
		inputData[counter] =getHexBit64[i27]^getHexBit64[i28];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i27]^getHexBit64[i48];
		counter++;
		
		//i28
		inputData[counter] =getHexBit64[i28]^getHexBit64[i29];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i28]^getHexBit64[i48];
		counter++;
		
		//i29
		inputData[counter] =getHexBit64[i29]^getHexBit64[i30];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i29]^getHexBit64[i48];
		counter++;
		
		//i30
		inputData[counter] =getHexBit64[i30]^getHexBit64[i31];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i30]^getHexBit64[i48];
		counter++;
		
		
		//i31
		inputData[counter] =getHexBit64[i31]^getHexBit64[i32];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i31]^getHexBit64[i48];
		counter++;
		
		//i32
		inputData[counter] =getHexBit64[i32]^getHexBit64[i33];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i32]^getHexBit64[i48];
		counter++;
		
		//i33
		inputData[counter] =getHexBit64[i33]^getHexBit64[i34];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i33]^getHexBit64[i48];
		counter++;
		
		//i34
		inputData[counter] =getHexBit64[i34]^getHexBit64[i35];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i34]^getHexBit64[i48];
		counter++;
		
		//i35
		inputData[counter] =getHexBit64[i35]^getHexBit64[i36];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i35]^getHexBit64[i48];
		counter++;
		
		//i36
		inputData[counter] =getHexBit64[i36]^getHexBit64[i37];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i36]^getHexBit64[i48];
		counter++;
		
		//i37
		inputData[counter] =getHexBit64[i37]^getHexBit64[i38];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i37]^getHexBit64[i48];
		counter++;
		
		//i38
		inputData[counter] =getHexBit64[i38]^getHexBit64[i39];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i38]^getHexBit64[i48];
		counter++;
		
		//i39
		inputData[counter] =getHexBit64[i39]^getHexBit64[i40];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i39]^getHexBit64[i48];
		counter++;
		
		//i40
		inputData[counter] =getHexBit64[i40]^getHexBit64[i41];
		counter++;
		inputData[counter] =getHexBit64[i40]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i40]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i40]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i40]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i40]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i40]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i40]^getHexBit64[i48];
		counter++;
		
		//i41
		inputData[counter] =getHexBit64[i41]^getHexBit64[i42];
		counter++;
		inputData[counter] =getHexBit64[i41]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i41]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i41]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i41]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i41]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i41]^getHexBit64[i48];
		counter++;
		
		//i42
		inputData[counter] =getHexBit64[i42]^getHexBit64[i43];
		counter++;
		inputData[counter] =getHexBit64[i42]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i42]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i42]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i42]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i42]^getHexBit64[i48];
		counter++;
		
		//i43
		inputData[counter] =getHexBit64[i43]^getHexBit64[i44];
		counter++;
		inputData[counter] =getHexBit64[i43]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i43]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i43]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i43]^getHexBit64[i48];
		counter++;
		
		//i44
		inputData[counter] =getHexBit64[i44]^getHexBit64[i45];
		counter++;
		inputData[counter] =getHexBit64[i44]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i44]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i44]^getHexBit64[i48];
		counter++;
		
		//i45
		inputData[counter] =getHexBit64[i45]^getHexBit64[i46];
		counter++;
		inputData[counter] =getHexBit64[i45]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i45]^getHexBit64[i48];
		counter++;
		
		//i46
		inputData[counter] =getHexBit64[i46]^getHexBit64[i47];
		counter++;
		inputData[counter] =getHexBit64[i46]^getHexBit64[i48];
		counter++;
		
		//i47
		inputData[counter] =getHexBit64[i47]^getHexBit64[i48];
		counter++;
		
		//*******************************************************************



		//*******************************************************************
		//HAM 2
		
		/*
		printBinary(getHexBit64[i1] ^ getHexBit64[i2]  ^ getHexBit64[i3], blocksize); cout << endl;

	
		
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



		*/	



	
		
		//*******************************************************************
	}//i48
	}//i47
	}//i46
	}//i45
	}//i44			
	}//i43
	}//i42
	}//i41																
	}//i40
	}//i39
	}//i38
	}//i37
	}//i36			
	}//i35
	}//i34
	}//i33						
	}//i32
	}//i31
	}//i30
	}//i29
	}//i28			
	}//i27
	}//i26
	}//i25																
	}//i24
	}//i23
	}//i22
	}//i21
	}//i20			
	}//i19
	}//i18
	}//i17																
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
	
	
	
	
	
   
	cout << "Number of pairs created: " << dec << counter << endl;

	
	
	
	
	
	// Create memory
    
    
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
    /* call kernel for each device - kernel for execution on the device using the command-queue
     * Queuing the kernel for execution does not mean that the kernel executes immediately.
     * The kernel execution is put into the command-queue for later consumption by the device.
     */
    
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
	
	/*************************************************************
 	*READ RESULT DATA BACK
 	**************************************************************/

	//std::cout <<  "writing Output for Best overall Result Backward ";
	saveRoundResultsComboToFile( outputBestOverallForward, "~/words96bitBestOverallResultForward.txt", nrRounds, 1);
	//std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best overall Result Backward ";
	saveRoundResultsComboToFile(outputBestOverallBackward,"~/words96bitBestOverallBackward.txt", nrRounds, 1);
	//std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best Round Result Forward ";
	saveRoundResultsComboToFile( outputBestRoundsForward,"~/words96bitBestRoundResultForward.txt", nrRounds, 2);
	// std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best Round Result Backward ";
	saveRoundResultsComboToFile( outputBestRoundsBackward,"~/words96bitBestRoundResultBackward.txt", nrRounds, 2);
	//std::cout << "- done" << endl;
	//std::cout <<  "writing Output for Best Round Result Combo ";
	saveRoundResultsComboToFile(outputBestRoundCombo,"~/words96bitBestRoundResultCombo.txt", nrRounds, 3);
	//std::cout << "- done" << endl;



	/*************************************************************
 	*WRITE RESULTS TO FILE AND SHOW ON SCREEN
 	**************************************************************/

	//PRINT BEST RESULTS -

	cout << "Advised mask for attack is \n";
	cout << "Rounds :" << outputBestOverallForward[0] << endl;
	cout << "Bias :" << outputBestOverallForward[5] << endl;
	cout << "Input xi : 0x" << std::hex << outputBestOverallForward[3] << endl;
	cout << "Input yi : 0x" << std::hex << outputBestOverallForward[4] << endl;
	cout << "last output xi : 0x" << std::hex << outputBestOverallForward[1] << endl;
	cout << "last output yi : 0x" << std::hex << outputBestOverallForward[2] << endl;


	cout << "**************************** \n";
	
	
	//PRINT BEST RESULTS -
	cout << "Advised mask for attack is \n";
	cout << "R: \t";
	cout << "B: \t";
	cout << "Xi: \t";
	cout << "Yi: \t";
	cout << "#b: \t";
	cout << "#f: \n";
	for (cl_ulong round = 1; round <= 28; round++)
	{
		if((
				outputBestRoundCombo[((round-1)*6)] !=0 ) &&
				(outputBestRoundCombo[((round-1)*6)] <= nrRounds) &&
				(outputBestRoundCombo[((round-1)*6)+5] !=99 )  )
		{
			cout << std::dec << outputBestRoundCombo[((round-1)*6)] << "\t";
			cout << "2ˆ-" << std::dec << outputBestRoundCombo[((round-1)*6)+5]+1 << "\t" ;
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
    
    

	// WAIT FOR PREVIOUS WORK TO HAVE FINSISHED, BEFORE GOING FURTHER============================
	errNum= clWaitForEvents(5, k_eventsComputeRead); // before going to next pair


	//===================================================================================
	// 14. CLEAN UP - RETURN MEMORY

	clReleaseMemObject(bufferCombo);
	clReleaseMemObject(bufferRBackward);
	clReleaseMemObject(bufferOBackward);
	clReleaseMemObject(bufferRForward);
	clReleaseMemObject(bufferOForward);
	clReleaseMemObject(bufferInputCipher);
    //clReleaseMemObject(bufferBlockSize);
    //clReleaseMemObject(bufferRounds);
    //clReleaseMemObject(bufferInDataPairs);
    //clReleaseMemObject(bufferOutDataPairs);
	clReleaseMemObject(bufferOForward);
	//clReleaseMemObject(bufferLeftPartWord );
    //clReleaseKernel(kernelPairs);
	clReleaseKernel(kernelCipher);
	

	//===========================STOP TIMERS==============================================

	double wall1 = get_wall_time();
	double cpu1  = get_cpu_time();
	std::cout << "Parallized computation has fininished in: " << endl;
	std::cout << "CPU Time = "<< cpu1-cpu0  << endl;
	std::cout << "Wall Time = " << wall1 - wall0 << endl;


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
