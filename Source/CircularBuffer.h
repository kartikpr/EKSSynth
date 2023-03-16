/*
  ==============================================================================

    CircularBuffer.h
    Created: 9 Nov 2022 7:46:45pm
    Author:  user

  ==============================================================================
*/

#pragma once
#include <vector>

class CircularBuffer {

private: 
   // int32_t writeIndex;
    int ptr;
    float delayedSample;
    int delayLength;

public:
    std::vector<float> delayLine;
    void initialseDelay(int length) {
        delayLine.resize(length);
        delayLength = length;
        std::fill(delayLine.begin(), delayLine.end(), 0);
        ptr = 0;
        
    }

   float processSample(float sample) {
 

       float y = delayLine[ptr];          // read operation 
       delayLine[ptr++] = sample;               // write operation
       if (ptr >= delayLength)               // wrap ptr if needed
       { 
           ptr -= delayLength; 
       } 
       //    ptr %= M;                   // modulo-operator syntax
       return y;


    }
};