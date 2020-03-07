#ifndef __LSARXCL__heuristic__
#define __LSARXCL__heuristic__

#include <stdio.h>


//Include source files for OPENCL
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

namespace heuristic_lib
{
    class Heuristic
    {
    public:
        Heuristic();                                    //contructor
        void generateKernel();
        void displayInfo();
        void runCipher();
        void checkErr(cl_int err, const char * name);
        void printBinary(cl_long n, cl_long blocksize);
        void saveRoundResultsComboToFile(cl_ulong *arrayResults, const char* filename, cl_ulong nrRounds, int typeOfoutput);
        double get_wall_time();
        double get_cpu_time();
        virtual ~Heuristic();                                                 // Destructor
        
        private:
        
        
    };// END CLASS
    
};//END NAMESPACE

#endif /* defined(__LSARXCL__computation__) */
