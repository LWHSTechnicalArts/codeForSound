/*
  ==============================================================================

    Gain.h
    Created: 1 Apr 2026 6:37:54pm
    Author:  trqkdata

  ==============================================================================
*/

#pragma once

class Gain
{
public:
    
    Gain();
    ~Gain();
    
    void process(float* inAudio,
                 float inGain,
                 float* outAudio,
                 int inNumSamplesToRender);
    
private:    
};
