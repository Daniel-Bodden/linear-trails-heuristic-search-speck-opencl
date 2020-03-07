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
inline int countTwoConsecutiveBitsSets( __private ulong n, 
										__private ulong wordlength)
{
    //local variables
    __private ulong prev = 0;				// =1 if previous bit was =1 ; else = 0
    __private ulong current = 0;			// =1 if current bit is =0 ; else = 0
    __private ulong pairs = 0;				// the number of pairs in mask
    __private ulong count = 0;				// ...
    __private ulong temp = 0;				// ...
    
    for (__private ulong i = 0; i < wordlength; i++) //check
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
inline ulong countOneBits(
						__private ulong k)
{
    __private ulong bits = 0;
    int i = __builtin_popcountll(k); // returns number of one bits
    bits = i;
    
    return bits;
}
//===============================================================

//===============================================================
// CHECK FOR IF UPPERBOUND HAS BEEN MET - based on ...
inline bool checkUpperBound(
						__private ulong bias, 
						__private ulong blocksize)
{
    bool checkUpperLimit = false;
    
    if (bias > ((blocksize/2))) //fixed - meeting vincent 25052017
    {
        checkUpperLimit =true;
    }
    
    return checkUpperLimit;
}
//===============================================================


//===============================================================
//CALCULATE HAMMINGWEIGHT
inline ulong biascount(
					__private ulong mask)
{
    int value = 0;
    value = (countOneBits(mask)) / 2;
    
    return value;
};
//===============================================================


//===============================================================
//FUNCTION CHECKS IF MASK CONSIST EXCLUSIVELY OF CONSECUTIVE BITS
inline bool checkTwoConsecutiveBits(
							__private ulong mask, 
							__private ulong wordlength)
{
    __private ulong n; //keep count number of 1 bits in mask
    bool check = false;
    
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
    return ((((value & 0xffffffffffffffff) << (number)) | ((value & 0xffffffffffffffff) >> ((wordlength) - number))) & 0xffffffffffffffff);
};
//===============================================================



//===============================================================
//CIRECULAR SHIFT FUNCTION RIGHT
inline ulong rotate_right(
						__private ulong value, 
						__private ulong number, 
						__private ulong wordlength)
{
    
    return ((((value&0xffffffffffffffff) >> (number)) | ((value&0xffffffffffffffff) << ((wordlength) - number))) & 0xffffffffffffffff);
};
//===============================================================



//---------------------------------------------------------------------------------------------------------------------------------
//REVERSE BITS
inline ulong reverse_bits(
							__private ulong v, 
							__private ulong s )
{
    //unsigned int v;     // input bits to be reversed
    __private ulong r = v; // r will be reversed bits of v; first get LSB of v
    //ulong64_t s = sizeof(v) * CHAR_BIT - 1; // extra shift needed at end
    s--;
    for (v >>= 1; v; v >>= 1)
    {
        r <<= 1;
        r |= v & 1;
        s--;
    }
    return r <<= s; // shift when v's highest bits are zero
}
//---------------------------------------------------------------------------------------------------------------------------------



//===============================================================
//HEURISTIC AUTOMATON FORWARD
//
// can be used alone but also in combination with other heuristics
inline void heuristic_automaton_forward(
                                        __private ulong V,
                                        __private bool * status,
                                        __private ulong* Uout,
                                        __private ulong* Wout,
                                        __private ulong* bias,
										__private ulong wordlength )
{
    
   
    
   
    __private ulong W = 0x0;
    __private ulong U = 0x0;
    
    __private ulong reversedV = reverse_bits(V, wordlength); //V;

    __private ulong bitOfV =0;

    __private ulong temp_u=0;
    __private ulong temp_w=0;
    
    __private ulong state =0;

    
    
    for(__private ulong bits =0; bits < wordlength; bits++ )
    {
        bitOfV = reversedV % 2;
    
        //if state 0 AND bit 0   => W=0, V=0
        if(state ==0 && bitOfV==0)
        {
  
            temp_u=0;
            temp_w=0;
            
            if (bits ==0)
            {
                
                U = temp_u;
                W = temp_w;
            }
            else
            {
                U = (U<<1)^temp_u;
                W = (W<<1)^temp_w;
            }
            
            state =0;
    
            
        }
        
        //if state 0 AND bit 1   => W=1, V=1
        else if(state ==0 && bitOfV==1)
        {

            
            temp_u=1;
            temp_w=1;
            
            if (bits ==0)
            {
                
                U = temp_u;
                W = temp_w;
            }
            else
            {
                U = (U<<1)^temp_u;
                W = (W<<1)^temp_w;
            }
            
            
            
            state =1;
            
      
            
        }
        
        
        //if state 1 AND bit 0   => W=0, V=1 +1
        else if(state ==1 && bitOfV==0)
        {

		//can be change
		// 001
		// or 010
            
            temp_u=0;
            temp_w=1;
            
            if (bits ==0)
            {
                
                U = temp_u;
                W = temp_w;
            }
            else
            {
                U = (U<<1)^temp_u;
                W = (W<<1)^temp_w;
            }
            
            
            state =0;

            
        }
        
        //if state 1 AND bit 1   => W=1, V=1 +1
        else if(state ==1 && bitOfV==1)
        {
		
		//can be change
		// 111 
		// or 100
            
            
            temp_u=1;
            temp_w=1;
            
            if (bits ==0)
            {
                
                U = temp_u;
                W = temp_w;
            }
            else
            {
                U = (U<<1)^temp_u;
                W = (W<<1)^temp_w;
            }
            
            
            state =0;
            
            //cout << strU << endl;
            //cout << strW << endl;
            
        }
        
        reversedV  = reversedV  >> 1;
    }
    
    
    
    __private ulong tempW =0x0;
    __private ulong tempV =0x0;
    __private ulong tempU =0x0;
    
    __private ulong biasAutomaton = 0;
    __private ulong flagAutomaton = 0;
    
    __private bool automatonCheck = true;
    __private ulong bitW =0;
    __private ulong bitV =0;
    __private ulong bitU =0;
    
    //check automaton for all bits
    tempV = reverse_bits(V,wordlength);
    tempW = reverse_bits(W,wordlength);
    tempU = reverse_bits(U,wordlength);
    
   ;
    
    for(__private ulong bits =0; bits < wordlength; bits++ )
    {
        
        bitW = tempW % 2;
        bitV = tempV % 2;
        bitU = tempU % 2;
        //current bits

        
        if (flagAutomaton == 0)
        {
            //state 0
            if ( bitU== 0x0 && bitV== 0x0 && bitW== 0x0)
            {
                //set to state 0
                flagAutomaton = 0;
                //cout << "OK, keep flag = 0"  << endl;
            }
            else if(bitU== 0x1 && bitV== 0x1 && bitW== 0x1)
            {
                //set to state 1
                flagAutomaton = 1;
                //cout << "OK, change flag = 1"  << endl;
            }
            else
            {
                //break computation - invalid bits (001, 010, 011, 100, 101, 110)
                automatonCheck=false;
                
       
                //cout << "BREAK AUTOMATON"  << endl;
                break;
            }
        }//end if
        else if (flagAutomaton == 1)
        {
            //state 1
            if( (bitU== 0x0 && bitV== 0x0 && bitW== 0x0) || (bitU== 0x0 && bitV== 0x1 && bitW== 0x1) || (bitU== 0x1 && bitV== 0x0 && bitW== 0x1) || (bitU== 0x1 && bitV== 0x1 && bitW== 0x0))
            {
                biasAutomaton++; //add 1 bias count
                //cout << "OK, keep flag = 1, cost + 1 "  << endl;
            }
            else
            {
                //set to state 0
                biasAutomaton++; //add 1 bias count
                flagAutomaton = 0;
                // cout << "OK, change flag = 0, cost + 1 "  << endl;
            }//end else
        }//end else
        
        //update temp values
        tempW = tempW >> 1;
        tempV = tempV >> 1;
        tempU = tempU >> 1;
        
    }//END AUTOMATON
    
    
    
    //10010100110000000001010101000001 NORMAL
    //10000010101010000000001100101001 REVERSED
    
    if (automatonCheck)
    {
    
        *status = true;
        *Uout = U;
        *Wout = W;
        *bias += biasAutomaton;
    }
	//fixed -> error before 02-06-2017
	else
	{ 
		*status = false;
 	}
    
}
//===============================================================


//===============================================================
//HEURISTIC AUTOMATON BACKWARD
//
// can be used alone but also in combination with other heuristics
inline void heuristic_automaton_backward(
                                         __private ulong U,
                                         __private bool * status,
                                         __private ulong* Vout,
                                         __private ulong* Wout,
                                         __private ulong* bias, 
										 __private ulong wordlength )
{
    
    __private ulong W = 0x0;
    __private ulong V = 0x0;
    

    __private ulong reversedU = reverse_bits(U, wordlength); //V;
    //ulong16_t itemp = temp;
    
    __private ulong bitOfU  =0;
    
    __private ulong temp_v=0;
    
    __private ulong temp_w=0;
    
    
    __private ulong state =0;
    
    
    
    
    for(__private ulong bits =0; bits <wordlength; bits++ )
    {
        bitOfU = reversedU % 2;
        
     
        
        //if state 0 AND bit 0   => W=0, V=0
        if(state ==0 && bitOfU==0)
        {

            
            temp_v=0;
            temp_w=0;
            
            if (bits ==0)
            {
                
                V = temp_v;
                W = temp_w;
            }
            else
            {
                V = (V<<1)^temp_v;
                W = (W<<1)^temp_w;
            }
            
            
            state =0;
            
 
            
        }
        
        //if state 0 AND bit 1   => W=1, V=1
        else if(state ==0 && bitOfU==1)
        {
         
            
            temp_v=1;
            temp_w=1;
            
            if (bits ==0)
            {
                
                V = temp_v;
                W = temp_w;
            }
            else
            {
                V = (V<<1)^temp_v;
                W = (W<<1)^temp_w;
            }
            
            
            
            state =1;
            
       
            
        }
        
        
        
        //if state 1 AND bit 0   => W=0, V=1 +1
        else if(state ==1 && bitOfU==0)
        {
           
		//can be change
		// 001
		// or 010
            
            temp_v=0;
            temp_w=1;
            
            if (bits ==0)
            {
                
                V = temp_v;
                W = temp_w;
            }
            else
            {
                V = (V<<1)^temp_v;
                W = (W<<1)^temp_w;
            }
            
            
            
            state =0;
            
       
            
        }
        
        //if state 1 AND bit 1   => W=1, V=1 +1
        else if(state ==1 && bitOfU==1)
        {
            
		//can be change
		// 111 
		// or 100
		
            temp_v=1;
            temp_w=1;
            
            if (bits ==0)
            {
                
                V = temp_v;
                W = temp_w;
            }
            else
            {
                V = (V<<1)^temp_v;
                W = (W<<1)^temp_w;
            }
            
            
            
            state =0;
            
           
            
        }
        
        reversedU  = reversedU  >> 1;
    }
    
    
    __private ulong tempW =0x0;
    __private ulong tempV =0x0;
    __private ulong tempU =0x0;
    
    __private ulong biasAutomaton = 0;
    __private ulong flagAutomaton = 0;
    
    __private bool automatonCheck = true;
    __private ulong bitW =0;
    __private ulong bitV =0;
    __private ulong bitU =0;
    
    //check automaton for all bits
    tempW = reverse_bits(W, wordlength);
    tempV = reverse_bits(V, wordlength);
    tempU = reverse_bits(U, wordlength);
    
    
    
    for(__private ulong bits =0; bits < wordlength; bits++ )
    {
        
        bitW = tempW % 2;
        bitV = tempV % 2;
        bitU = tempU % 2;
        //current bits
       
        
        if (flagAutomaton == 0)
        {
            //state 0
            if ( bitU== 0x0 && bitV== 0x0 && bitW== 0x0)
            {
                //set to state 0
                flagAutomaton = 0;
                //cout << "OK, keep flag = 0"  << endl;
            }
            else if(bitU== 0x1 && bitV== 0x1 && bitW== 0x1)
            {
                //set to state 1
                flagAutomaton = 1;
                //cout << "OK, change flag = 1"  << endl;
            }
            else
            {
                //break computation - invalid bits (001, 010, 011, 100, 101, 110)
                automatonCheck=false;
           
                break;
            }
        }//end if
        else if (flagAutomaton == 1)
        {
            //state 1
            if( (bitU== 0x0 && bitV== 0x0 && bitW== 0x0) || (bitU== 0x0 && bitV== 0x1 && bitW== 0x1) || (bitU== 0x1 && bitV== 0x0 && bitW== 0x1) || (bitU== 0x1 && bitV== 0x1 && bitW== 0x0))
            {
                biasAutomaton++; //add 1 bias count
                //cout << "OK, keep flag = 1, cost + 1 "  << endl;
            }
            else
            {
                //set to state 0
                biasAutomaton++; //add 1 bias count
                flagAutomaton = 0;
                // cout << "OK, change flag = 0, cost + 1 "  << endl;
            }//end else
        }//end else
        
        //update temp values
        tempW = tempW >> 1;
        tempV = tempV >> 1;
        tempU = tempU >> 1;
        
    }//END AUTOMATON
    
    
    
    //10010100110000000001010101000001 NORMAL
    //10000010101010000000001100101001 REVERSED
    
    if (automatonCheck)
    {
      
        
        *status = true;
        *Vout = V;
        *Wout = W;
        *bias += biasAutomaton;
    }
	//fixed -> error before 02-06-2017
	else
	{ 
		*status = false;
 	}
    
}
//===============================================================







//===============================================================
inline void cipherRoundFunction(
                                __private const ulong inMaskX,
                                __private const ulong inMaskY,
                                __private const ulong blocksize,
                                __private const ulong nrRounds,
                                __global ulong *outputBestRoundCombo,               //OUTPUT - write the best round combo (forward, backward)
                                __global ulong *outMasksBestRoundBackward,          //OUTPUT - write the best round backward
                                __global ulong *outMasksOverallBackward,            //OUTPUT - write the best overall result backward
                                __global ulong *outMasksBestRoundForward,           //OUTPUT - write the best round forward
                                __global ulong *outMasksOverallForward             //OUTPUT - write the best overall result forward
                                )
{

    //Create local result tables
    __private ulong roundResultBackward[32][6];
    __private ulong roundResultForward[32][6];

    //initalize
    for (__private int round = 0; round < 32; round++)
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
    __private ulong lshift = 3;
    __private ulong rshift = 8;
    __private bool xorCheck;
    __private ulong count = 0;
    __private ulong wordlength= blocksize/2;  //divided by the number of words in the round function
    __private ulong maskX = inMaskX;
    __private ulong maskY = inMaskY ;

    //==============================FORWARD================================================
    //LOOP OVER ALL POSSIBLE ROUNDS
    for(__private ulong round = 1; round<=nrRounds;round++) // SPECK HAS 22 ROUNDS
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
        
        
        __private ulong addedThisRound = biascount((rotate_right(inputMaskX,rshift,wordlength)));
        
        //CHECK XOR
        if(xorCheck==false)
        {
            //STOP CURRENT ITERATION
             __private ulong out_UoutF= 0x0;
             __private ulong out_WoutF= 0x0;
             __private ulong out_biasF = 0;
            
             __private ulong VF = rotate_right(inputMaskX,rshift,wordlength);
            
            //cout << "IN " << xorCheck << endl;
            heuristic_automaton_forward(VF, &xorCheck, &out_UoutF,  &out_WoutF, &out_biasF, wordlength );
            
            
            if(xorCheck==false)
            {
              
                break;
            }
            else
            {
             
                
                //CORRECT OUTPUT VALUES
                
                //Xi+1 = U XOR <<< (W XOR Yi)
                maskX = out_UoutF ^ rotate_left(  out_WoutF ^ inputMaskY   ,lshift,wordlength) ;
                
                //Yi+1 = <<< ( W XOR Y)
                maskY = rotate_left(  out_WoutF ^ inputMaskY  ,lshift,wordlength);
                
                //CORRECT BIAS
                addedThisRound =0; 
                addedThisRound  = out_biasF;
                
            }
            
   
        }
        
       
        //Check UpperBoundLimit
         if (checkUpperBound(count + +addedThisRound,blocksize))
        {
            //STOP CURRENT ITERATION
            break;
        }
        else
        {
            //Update BIAS COUNT ON ALL W-POSITIONS (transformed from non-linear to approx linear)
            count = count + addedThisRound;
        
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
    
barrier(CLK_LOCAL_MEM_FENCE);

//Variables for round function
inputMaskX = 0x0;
inputMaskY = 0x0;
iMaskXRound = 0x0;
iMaskYRound = 0x0;
lshift = 8;
rshift = 3;
xorCheck = false;
count = 0;
// blocksize = 32;
maskX = inMaskX;
maskY = inMaskY ;

//LOOP OVER ALL POSSIBLE ROUNDS
for(__private ulong round = 1; round <= nrRounds ;round++) // SPECK HAS 22 ROUNDS
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
    
    
     __private ulong  addedThisRound =  biascount((inputMaskX ^ inputMaskY));
    
    ///UPDATE ROUND WITH CHECK ON CONDITIONS  ============================================================================
    if(xorCheck==false)
    {
        /*
        //TRY TO FIX
        __private ulong  out_VoutB= 0x0;
        __private ulong  out_WoutB= 0x0;
        __private ulong  out_biasB = 0;
        
        __private ulong  UB= inputMaskX ^ inputMaskY;
        
        //cout << "IN " << xorCheck << endl;
        heuristic_automaton_backward(UB, &xorCheck, &out_VoutB,  &out_WoutB, &out_biasB, wordlength );
        
        if(xorCheck==false)
        {
            //STOP CURRENT ITERATION
            break;
        }
        else
        {
            
            //CORRECT OUTPUT VALUES
            
            //Xi+1 = V <<<
            maskX =  rotate_left ( out_VoutB ,lshift, wordlength);;
            
            //Yi+1 = W XOR ( >>>> Yi+1)
            maskY = out_WoutB ^ (rotate_right( inputMaskY , rshift, wordlength));
            
            //CORRECT BIAS
            addedThisRound =0;
            addedThisRound  = out_biasB;
        }
         */
        
        break;
        
    }

    //Check UpperBoundLimit
    else if (checkUpperBound( count + addedThisRound , blocksize)==true)
    {
        //STOP CURRENT ITERATION
        break;
    }
    else
    {
        //Update BIAS COUNT ON ALL W-POSITIONS (transformed from non-linear to approx linear)
        count = count + addedThisRound;
        
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



barrier(CLK_LOCAL_MEM_FENCE);
    
//===============CHECK BEST ROUND RESULT GLOBAL===================================================

// go over all rounds
for(__private ulong round=1; round <= nrRounds; round++)
{
    
    //chekc all combinations
    for (__private ulong k =0 ; k <= round ; k++ )
    {
        /* a + b = i
         * finding all possible combinations for i rounds
         */
        __private int a = round-k;
        __private int b = k;
        
        
        //Check bounds
		//| ( checkUpperBound(roundResultForward[b-1][5], blocksize)) |  (checkUpperBound(roundResultBackward[a-1][5], blocksize))
        if ( (a == 0 & b == 0) )
        {
            //do nothing
        }
        
        //Check if Backward is set to round 0
        else if(a==0)
        {
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
                    
                    outputBestRoundCombo[(round-1)*6] = round;                                           	//ROUND NUMBER
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
                
            }
            
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
    
		//nr of of mask created -> upper bound
    if ( (gidDim1 > 8192) || (gidDim2 > 8192))
    {
        return; //don't execute round function if thread number is higher then the available pairs
    }

    else if( (inWords[(gidDim1)] == 0x0) && (inWords[(gidDim2)] == 0x0))
    {
        return;
    }
    else
    {
        
        __private ulong rounds = 32;
        __private ulong blocksize = 128;
        
        cipherRoundFunction(
                            inWords[(gidDim1)],
                            inWords[(gidDim2)],
                            //0x6,
                            // 0x0,
                            blocksize,
                            rounds,
                            outputBestRoundCombo,               //OUTPUT - write the best round combo (forward, backward)
                            outMasksBestRoundBackward,          //OUTPUT - write the best round backward
                            outMasksOverallBackward,            //OUTPUT - write the best overall result backward
                            outMasksBestRoundForward,           //OUTPUT - write the best round forward
                            outMasksOverallForward             //OUTPUT - write the best overall result forward
                            );
        
        
    }// END OF ELSE CONDITION THAT OUTSIDE MEMORY IS NOT USED


} // END OF KERNEL ARX_TOOLBOX
//==============================================================




