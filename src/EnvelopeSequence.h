#pragma once

#include "juce_core/juce_core.h"
#include <vector>

enum EnvelopeStage {
  Attack = 0,
  Hold,
  Release,
};

class AHR {
public:

  float getStage(EnvelopeStage); 
  float getTotalSeconds() noexcept;
  
  void setTotalSeconds (float totalSeconds);
  void setAllStagesTime (float attack, float hold, float release); // set proportion of all stages, if sum of values does not equal to 1, set to normalized values
  void setStageTime (EnvelopeStage stage, float value); // set proportion to 1 stage, compnsating by taking away from the rest of the stages
  
  void trigger () noexcept; // corresponds to juce::ADSR::NoteOn() -> Since this is a triggered LFO, no NoteOff is needed
  void reset() noexcept;

private:
  float totalSeconds = 1.0;
  float attackTime = 0.33;
  float holdTime = 0.34;
  float releaseTime = 0.33;
  float holdVolume = 1.0;
};

class EnvelopeSequence {
public:
  EnvelopeSequence(float tempo, juce::uint32 signatureDenominator);
  ~EnvelopeSequence();

  AHR getEnvelope(juce::uint32 index);
  AHR getActive();
  float getTotalSeconds() noexcept;
  juce::uint32 getNoteSubdivision() noexcept;

  void setTempoSync(float tempo, juce::uint32 signatureDenominator);
  void setNoteSubdivision(juce::uint32 sub);
  
    
private:
  juce::uint32 activeIdx = 0;
  juce::uint32 noteSubdivision = 4;
  float totalSeconds = 1.0;
  std::vector<AHR> sequence;
};
