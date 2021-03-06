/* KERNEL FILE
 *
 *   SUPPORT FUNCTIONS :
 *   - inline int CountTwoConsecutiveBitsSets(int16  n, int localnrLoops)
 *   - inline int countOneBits(int k)
 *   - inline bool checkTwoConsecutiveBits(int16  mask, int localnrLoops)
 *   - inline int hammingWeight(int16  mask)
 *   - inline int16 rotate_left16(int16 value, int number)
 *   - inline int16 rotate_right16(int16 value, int number)
 *   - inline bool checkUpperBound(int bias)
 *
 *   KERNEL FUNCTION
 *
 *
 *
 */


 //===============================================================
 //FUNCTION RETURNS NUMBER OF PAIRS OF 2 BITS
 inline int countTwoConsecutiveBitsSets(
 				   __private  ulong n, 
				   __private  ulong wordlength)
 {
     //local variables
     __private  ulong prev = 0;				// =1 if previous bit was =1 ; else = 0
     __private  ulong current = 0;			// =1 if current bit is =0 ; else = 0
     __private  ulong pairs = 0;				// the number of pairs in mask
     __private ulong count = 0;				// ...
     __private  ulong temp = 0;				// ...
    
     for (__private  uint i = 0; i < wordlength; i++) //check
     {
        
         if (current == 1) {
             prev = 1;
         } else {
             //reset value
             prev = 0;
         }
        
         temp = n & 1;
        
         if (temp == 1)
         {
             current = 1;
             count++;
         }
         else
         {
             current = 0;
         }
         /*
          printf(" current = %x", current);
          printf("  prev = %x", prev);
          printf("  remain= %x", count%2);
          printf("  count= %x", count);
          */
        
         if (prev == 1 && current == 1)
         {
             //reset value
             count = 0;
         }
        
         if (prev == 1 && current == 1 && count % 2 == 0)
         {
             pairs++;
             //reset values
             count = 0;
             prev = 0;
             current = 0;
         }
         //printf("   number of sets= %x\n", sets);
        
         n >>= 1;
         /*
          printf(" next round mask in binary ");
          printBinary(n);
          printf("\n");
          */
     }
     return pairs;
 }
 //===============================================================


 //===============================================================
 //FUNCTION RETURNS THE NUMBER OF BITS IN A MASK
 inline uint countOneBits(  
	 	 					__private  ulong k)
 {
     __private  ulong bits = 0;
     __private  ulong i = __builtin_popcountll(k); // returns number of one bits
     bits = i;
    
     return bits;
 }
 //===============================================================

 //===============================================================
 // CHECK FOR IF UPPERBOUND HAS BEEN MET - based on ...
 inline bool checkUpperBound(   
	 							__private  ulong bias,   
	 	 						__private  ulong blocksize)
 {
        __private  bool checkUpperLimit = false;
    
     if (bias > ((blocksize/2))) //fixed => meeting Vincent 24052017
     {
         checkUpperLimit =true;
     }
    
     return checkUpperLimit;
 }
 //===============================================================


 //===============================================================
 //CALCULATE BIAS COUNT BY HAMMINGWEIGHT /2 
 inline uint biascount(   __private  ulong mask)
 {
     __private  ulong value = 0;
	 //bias
 	value = (countOneBits(mask))/2;
    
     return value;
 };
 //===============================================================


 //===============================================================
 //FUNCTION CHECKS IF MASK CONSIST EXCLUSIVELY OF CONSECUTIVE BITS
 inline bool checkTwoConsecutiveBits( 	
	 									__private ulong mask, 
										__private ulong wordlength)
 {
     __private uint n; //keep count number of 1 bits in mask
     __private bool check = false;
    
     n = countOneBits(mask); //count 1 bits
     //printf("number of 1 bits  %d", n);
    
     if (n == 0)
     {
         //all are even.
         check = true;
     }
    
    
     else if (n % 2 == 0)
     {
         //check if consecutive
         if (countTwoConsecutiveBitsSets(mask, wordlength) == (n / 2)) {
             // bits are consecutive
             check = true;
         }
         else
         {
             //not consecutive
             check = false;
         }
     }
    
    
     return check;
 }
 //===============================================================


//===============================================================

//CIRCULAR SHIFT FUNCTION LEFT
inline ulong rotate_left( 
						__private ulong value, 
						__private ulong number, 
						__private ulong wordlength)
{
    return ((((value & 0xffffffffffff) << (number)) | ((value & 0xffffffffffff) >> ((wordlength) - number))) & 0xffffffffffff);
};
//===============================================================



//===============================================================
//CIRECULAR SHIFT FUNCTION RIGHT
inline ulong rotate_right(
							__private ulong value, 
							__private ulong number, 
							__private ulong wordlength)
{
    
    return ((((value & 0xffffffffffff) >> (number)) | ((value & 0xffffffffffff) << ((wordlength) - number))) & 0xffffffffffff); 
};
//===============================================================


//===============================================================
inline void cipherRoundFunction(
                               __private const ulong inMaskX,
				               __private const ulong inMaskY,
                               __private const uint blocksize,
                               __private const ulong nrRounds,
                               __global ulong *outputBestRoundCombo,               //OUTPUT - write the best round combo (forward, backward)
                               __global ulong *outMasksBestRoundBackward,          //OUTPUT - write the best round backward
                               __global ulong *outMasksOverallBackward,            //OUTPUT - write the best overall result backward
                               __global ulong *outMasksBestRoundForward,           //OUTPUT - write the best round forward
                               __global ulong *outMasksOverallForward             //OUTPUT - write the best overall result forward
                                )
{

		 //todo 


        //Create local result tables
        __private ulong roundResultBackward[28][6];
        __private ulong roundResultForward[28][6];
        
        //initalize
        for (__private uint round = 0; round < 26; round++)
        {
            roundResultBackward[round][5] = 99 ;
            roundResultForward[round][5] = 99 ;
        }
        
        //==============================FORWARD================================================
        //Variables for round function
        __private ulong inputMaskX = 0x0;
        __private ulong inputMaskY = 0x0;
        __private ulong iMaskXRound = 0x0;
        __private ulong iMaskYRound = 0x0;
        __private uint lshift = 3;
        __private uint rshift = 8;
        __private bool xorCheck;
        __private ulong count = 0;
        __private ulong wordlength= blocksize/2;  //divided by the number of words in the round function
        __private ulong maskX = inMaskX;
        __private ulong maskY = inMaskY ;
        
        //==============================FORWARD================================================
        //LOOP OVER ALL POSSIBLE ROUNDS
        for(__private uint round = 1; round<=28;round++) // SPECK HAS 22 ROUNDS
        {
            //---------------------------------------------------------------------
            //KEEP INITIAL MASK
            if(round==1)
            {
                iMaskXRound = maskX;
                iMaskYRound = maskY;
            }
            //----------------------------------------------------------------------------------
            //CALL ROUND FUNCTION
            //Keep input mask
            inputMaskX = 0x0;
            inputMaskY = 0x0;
            inputMaskX = maskX;
            inputMaskY = maskY;
            
            //---------------------------------------------------------------------------------
            //CONDITIONS =============================
            xorCheck = checkTwoConsecutiveBits((rotate_right(inputMaskX,rshift,wordlength)),wordlength);
            
            //CALL ROUND FUNCTION ====================
            maskX = ((rotate_right(inputMaskX, rshift,wordlength)) ^ (rotate_left(( rotate_right(inputMaskX, rshift,wordlength)  ^ inputMaskY ), lshift,wordlength)));
            maskY = (rotate_left((rotate_right(inputMaskX, rshift,wordlength)  ^ inputMaskY ), lshift, wordlength));
            
            //------------------------------------------------------------------------------------
            //STOP CONDITIONS
            //CHECK XOR
            if(xorCheck==false)
            {
                //STOP CURRENT ITERATION
                break;
            }
            //Check UpperBoundLimit
	         /**/
            else if (checkUpperBound(count + biascount((rotate_right(inputMaskX,rshift,wordlength))),blocksize)==true)
            {
                //STOP CURRENT ITERATION
                break;
            }
            else
            {
                //Update BIAS COUNT ON ALL W-POSITIONS (transformed from non-linear to approx linear)
                count = count +  biascount((rotate_right(inputMaskX,rshift,wordlength)));
                
                roundResultForward[round-1][0] = round;
                roundResultForward[round-1][1] = maskX;
                roundResultForward[round-1][2] = maskY;
                roundResultForward[round-1][3] = inputMaskX;
                roundResultForward[round-1][4] = inputMaskY;
                roundResultForward[round-1][5] = count;
                
                //GLOBAL CHECK BEST ROUND RESULTS
                /* OpenCL cannot deal with 2d arrays
                 * Round        (i-1)*6
                 * MaskX        ((i-1)*6) +1
                 * MaskY        ((i-1)*6) +2
                 * InputMaskX   ((i-1)*6) +3
                 * inputMaskY   ((i-1)*6) +4
                 * BIAS         ((i-1)*6) +5
                 */
                if(count < outMasksBestRoundForward[( (round-1)*6)+5]) //check against bias of current best for given ROUND
                {
                    outMasksBestRoundForward[(round-1)*6] = round;
                    outMasksBestRoundForward[((round-1)*6)+1] = maskX;
                    outMasksBestRoundForward[((round-1)*6)+2] = maskY;
                    outMasksBestRoundForward[((round-1)*6)+3] = inputMaskX;
                    outMasksBestRoundForward[((round-1)*6)+4] = inputMaskY;
                    outMasksBestRoundForward[((round-1)*6)+5] =  count;
                }
                //save best result
                if(round > outMasksOverallForward[0])
                {
                    outMasksOverallForward[0] = round;             //current round
                    outMasksOverallForward[1] = maskX; 			//inputmask X
                    outMasksOverallForward[2] = maskY;             //inputmask Y
                    outMasksOverallForward[3] = iMaskXRound;       //outputmask X at the start of round 1
                    outMasksOverallForward[4] = iMaskYRound;       //outputmask Y at the start of round 1
                    outMasksOverallForward[5] = count;              //BIAS
                }
                else if(round >= outMasksOverallForward[0] && count < outMasksOverallForward[5])
                {
                    outMasksOverallForward[0] = round;             //current round
                    outMasksOverallForward[1] = maskX; 			//inputmask X
                    outMasksOverallForward[2] = maskY;             //inputmask Y
                    outMasksOverallForward[3] = iMaskXRound;       //outputmask X at the start of round 1
                    outMasksOverallForward[4] = iMaskYRound;       //outputmask Y at the start of round 1
                    outMasksOverallForward[5] = count;              //BIAS
                }
            }// END ELSE
        }// END LOOP ROUND
        
        //==============================BACKWARD===============================================
        //=========Re-initialize
        
        //Variables for round function
        inputMaskX = 0x0;
        inputMaskY = 0x0;
        iMaskXRound = 0x0;
        iMaskYRound = 0x0;
        lshift = 8;
        rshift = 3;
        xorCheck = false;
        count = 0;
        maskX =inMaskX;
        maskY = inMaskY ;
        
        //LOOP OVER ALL POSSIBLE ROUNDS
        for(__private uint round = 1; round <= 28 ;round++) // SPECK HAS 22 ROUNDS
        {
            //KEEP INITIAL MASK  =====================
            if(round==1)
            {
                iMaskXRound = maskX;
                iMaskYRound = maskY;
            }
            
            //Keep input mask   ====================
            inputMaskX = 0x0;
            inputMaskY = 0x0;
            inputMaskX = maskX;
            inputMaskY = maskY;
            
            //CONDITIONS =============================
            xorCheck = checkTwoConsecutiveBits((inputMaskX ^ inputMaskY), wordlength);
            
            //CALL ROUND FUNCTION ====================
            maskX = rotate_left((inputMaskX ^  inputMaskY),lshift,wordlength);
            maskY = (inputMaskX ^ inputMaskY) ^ (rotate_right( maskY , rshift,wordlength));
            
            ///UPDATE ROUND WITH CHECK ON CONDITIONS  ============================================================================
            if(xorCheck==false)
            {
                //STOP CURRENT ITERATION
                break;
            }
            //Check UpperBoundLimit
            /*    */
	 	   else if (checkUpperBound(count + biascount((inputMaskX ^ inputMaskY)), blocksize)==true)
            {
                //STOP CURRENT ITERATION
                break;
            }

            else
            {
                //Update BIAS COUNT ON ALL W-POSITIONS (transformed from non-linear to approx linear)
                count = count + biascount((inputMaskX ^ inputMaskY));
                
                //Save local round results
                
                roundResultBackward[round-1][0] = round;
                roundResultBackward[round-1][1] = maskX;
                roundResultBackward[round-1][2] = maskY;
                roundResultBackward[round-1][3] = inputMaskX;
                roundResultBackward[round-1][4] = inputMaskY;
                roundResultBackward[round-1][5] = count;
                
                //CHECK BEST ROUND RESULTS
                /* OpenCL cannot deal with 2d arrays
                 * Round        (i-1)*6
                 * MaskX        ((i-1)*6) +1
                 * MaskY        ((i-1)*6) +2
                 * InputMaskX   ((i-1)*6) +3
                 * inputMaskY   ((i-1)*6) +4
                 * BIAS         ((i-1)*6) +5
                 */
                
                if(count < outMasksBestRoundBackward[( (round-1)*6)+5]) //check against bias of current best for given ROUND
                {
                    outMasksBestRoundBackward[(round-1)*6] = round;
                    outMasksBestRoundBackward[((round-1)*6)+1] = maskX;
                    outMasksBestRoundBackward[((round-1)*6)+2] = maskY;
                    outMasksBestRoundBackward[((round-1)*6)+3] = inputMaskX;
                    outMasksBestRoundBackward[((round-1)*6)+4] = inputMaskY;
                    outMasksBestRoundBackward[((round-1)*6)+5] =  count;
                }
                
                //save best result
                if(round > outMasksOverallBackward[0])
                {
                    outMasksOverallBackward[0] = round;             //current round
                    outMasksOverallBackward[1] = maskX; 			//inputmask X
                    outMasksOverallBackward[2] = maskY;             //inputmask Y
                    outMasksOverallBackward[3] = iMaskXRound;       //outputmask X at the start of round 1
                    outMasksOverallBackward[4] = iMaskYRound;       //outputmask Y at the start of round 1
                    outMasksOverallBackward[5]= count;              //BIAS
                }
                else if(round >= outMasksOverallBackward[0] && count < outMasksOverallBackward[5])
                {
                    outMasksOverallBackward[0] = round;             //current round
                    outMasksOverallBackward[1] = maskX; 			//inputmask X
                    outMasksOverallBackward[2] = maskY;             //inputmask Y
                    outMasksOverallBackward[3] = iMaskXRound;       //outputmask X at the start of round 1
                    outMasksOverallBackward[4] = iMaskYRound;       //outputmask Y at the start of round 1
                    outMasksOverallBackward[5]= count;              //BIAS
                }
                
            } // END UPDATE BIAS
            
            
        } // End ROUNDFUNCTION BACKWARD
        
        
        
        
        //===============CHECK BEST ROUND RESULT GLOBAL===================================================
        
        // go over all rounds
        for(__private uint round=1; round <= 28; round++)
        {
            
            //chekc all combinations
            for (__private uint k =0 ; k <= round ; k++ )
            {
                /* a + b = i
                 * finding all possible combinations for i rounds
                 */
                __private uint a = round-k;
                __private uint b = k;
                
                if ( (a == 0 & b == 0))
                {
                    //do nothing
                }
                //Check bounds
                /*  
	        if ( (a == 0 & b == 0) | ( checkUpperBound(roundResultForward[b-1][5], blocksize)) |  (checkUpperBound(roundResultBackward[a-1][5], blocksize)))
                {
                    //do nothing
                }
              */
                //Check if Backward is set to round 0
                else if(a==0)
                {
		    /**/
                    // if bias of combination is bigger than the upperbound, then break of combo
                    if (checkUpperBound(roundResultForward[b-1][5], blocksize))
                    {
                        //do nothing
                    }
                    else
                    {
		    	
                        //Check if result has improved
                        if (  0 + roundResultForward[b-1][5] < outputBestRoundCombo[((round-1)*6) +5] )
                        {
                            //CHECK BEST ROUND RESULTS
                            /* OpenCL cannot deal with 2d arrays
                             * Round            (i-1)*6
                             * MaskX            ((i-1)*6) +1
                             * MaskY            ((i-1)*6) +2
                             * #rounds backward ((i-1)*6) +3
                             * #rounds forward  ((i-1)*6) +4
                             * BIAS             ((i-1)*6) +5
                             */
                            
                            outputBestRoundCombo[(round-1)*6] = round ;                                           	//ROUND NUMBER
                            outputBestRoundCombo[((round-1)*6) +1] = roundResultForward[0][3];        	//starting MASK x
                            outputBestRoundCombo[((round-1)*6) +2] = roundResultForward[0][4];        	//starting MASK y
                            outputBestRoundCombo[((round-1)*6) +3] = a;                                        	//Backward rounds
                            outputBestRoundCombo[((round-1)*6) +4] = b;                                        	//Forward  rounds
                            outputBestRoundCombo[((round-1)*6) +5] = 0 	+ roundResultForward[b-1][5];        	//BIAS
                        }
                    }
                }
                
                //Check if Forward is set to round 0
                else if (b==0)
                {
		   /*  */
                    // if bias of combination is bigger than the upperbound, then break of combo
                    if (checkUpperBound(0 + roundResultBackward[a-1][5], blocksize))
                    {
                        //do nothing
                        
                    }
                    else
		  
                    {
                        //check i improved
                        if ( 0 + roundResultBackward[a-1][5] < outputBestRoundCombo[((round-1)*6) +5] )
                            
                        {
                            outputBestRoundCombo[(round-1)*6] = round;                                           	//ROUND NUMBER
                            outputBestRoundCombo[((round-1)*6) +1] = roundResultBackward[0][3];        	//starting MASK x
                            outputBestRoundCombo[((round-1)*6) +2] = roundResultBackward[0][4];        	//starting MASK y
                            outputBestRoundCombo[((round-1)*6) +3] = a;                                        	//Backward rounds
                            outputBestRoundCombo[((round-1)*6) +4] = b;                                        	//Forward  rounds
                            outputBestRoundCombo[((round-1)*6) +5] = 0 	+ roundResultBackward[a-1][5]                        	;        	//BIAS
                        }
                    }
                }
                
                //Both are not round 0
                else
                {
	   	    /**/
                    // if bios of combination is bigger than the upperbound, then break of combo
                    if (checkUpperBound(roundResultBackward[a-1][5] + roundResultForward[b-1][5] , blocksize))
                    {
                        // do  nothing
                    }
                    else
                    {
	  	    
                        //Check if improved
                        if (  (roundResultBackward[a-1][5] + roundResultForward[b-1][5]) < outputBestRoundCombo[((round-1)*6) +5] )
                        {
                            outputBestRoundCombo[(round-1)*6] = round;                                                                  //ROUND NUMBER
                            outputBestRoundCombo[((round-1)*6) +1] = roundResultBackward[0][3];                                         //starting MASK x
                            outputBestRoundCombo[((round-1)*6) +2] = roundResultBackward[0][4];                                         //starting MASK y
                            outputBestRoundCombo[((round-1)*6) +3] = a;                                                                 //Backward rounds
                            outputBestRoundCombo[((round-1)*6) +4] = b;                                                                 //Forward  rounds
                            outputBestRoundCombo[((round-1)*6) +5] = (roundResultBackward[a-1][5] + roundResultForward[b-1][5]);        	//BIAS
                            
                        }
                        
						}//end else
                    
                } // END ELSE BOTH NOT zero
                
                
                
            } // End loop checking combinations
            
        } // end round loop
        
}
//=====================================================================================








//===================== Kernel ARX_TOOLBOXOL__kernel=========================================

__kernel void ARX_TOOLBOX_Cipher_kernel(
                                        __global ulong *outputBestRoundCombo,               //OUTPUT - write the best round combo (forward, backward)
                                        __global ulong *outMasksBestRoundBackward,          //OUTPUT - write the best round backward
                                        __global ulong *outMasksOverallBackward,            //OUTPUT - write the best overall result backward
                                        __global ulong *outMasksBestRoundForward,           //OUTPUT - write the best round forward
                                        __global ulong *outMasksOverallForward,             //OUTPUT - write the best overall result forward
                                        __global ulong *inWords
                                        )
{
    //==============================PRE-WORK & CHECKS================================================
    size_t lid = get_local_id(0);//get workgroup number
    size_t gidDim1 = get_global_id(0); // work item number
    size_t gidDim2 = get_global_id(1); // work item number
    
    
    //uper bound
	if ( (gidDim1 > 5000) || (gidDim2 > 5000))
	{
	   return; //don't execute round function if thread number is higher then the available pairs
	}
	else if( (gidDim1 == 0x0) && (gidDim2 == 0x0))
	{
	   return;
	}
	else
	{
	
    __private uint rounds = 28;
    __private uint blocksize = 96;
    
		cipherRoundFunction(  		
							inWords[(gidDim1)],
                            inWords[(gidDim2)],
								//0x3,
								//0x0,
                            blocksize,
                            rounds,
                            outputBestRoundCombo,               //OUTPUT - write the best round combo (forward, backward)
                            outMasksBestRoundBackward,          //OUTPUT - write the best round backward
                            outMasksOverallBackward,            //OUTPUT - write the best overall result backward
                            outMasksBestRoundForward,           //OUTPUT - write the best round forward
                            outMasksOverallForward             //OUTPUT - write the best overall result forward
                            );


    

    } // END OF ELSE CONDITION THAT OUTSIDE MEMORY IS NOT USED
    
    
    
} // END OF KERNEL ARX_TOOLBOX
//==============================================================



