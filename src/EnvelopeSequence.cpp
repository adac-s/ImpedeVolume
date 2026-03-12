#include "EnvelopeSequence.h"
#include "juce_core/juce_core.h"
#include <limits>
#include <stdexcept>
#include <vector>

float AHR::getStage(EnvelopeStage stage) {
  switch (stage) {
    case EnvelopeStage::Attack:
      return attackTime;
    case EnvelopeStage::Hold:
      return holdTime;
    case EnvelopeStage::Release:
      return releaseTime;
    default:
      throw std::invalid_argument("Invalid Envelope Stage queried");
  }
}

float AHR::getTotalSeconds() noexcept {
  return totalSeconds;
}

float AHR::getHoldVolume() noexcept {
  return holdVolume;
}

void AHR::setTotalSeconds(float seconds) {
  totalSeconds = seconds;
}

void AHR::setHoldVolume(float volume) {
  holdVolume = volume;
}

void AHR::setAllStagesTime(float attack, float hold, float release) {
  float compositeTime = attack + hold +release;
  if (std::abs(compositeTime - 1.0) >= std::numeric_limits<float>().epsilon()) {
    attack = attack / compositeTime;
    hold = hold / compositeTime;
    release = release / compositeTime;
  }

  attackTime = attack;
  holdTime = hold;
  releaseTime = release;
}

void AHR::setStageTime(EnvelopeStage stage, float newTime) {
  if (stage == EnvelopeStage::Attack) {
    // If attack, take away from hold, then release
    float newHold = holdTime - (attackTime - newTime);
    attackTime = newTime;
    if (newHold <= 0.0) {
      holdTime = 0.0;
      releaseTime = -newHold;
    }
    else {
      holdTime = newHold;
    }
  }
  else if (stage == EnvelopeStage::Release) {
  // If release, take away from hold, then attack
    float newHold = holdTime - (releaseTime - newTime);
    releaseTime = newTime;
    if (newHold <= 0.0) {
      holdTime = 0.0;
      attackTime = -newHold;
    }
    else {
      holdTime = newHold;
    }
  }
}

EnvelopeSequence::EnvelopeSequence(float tempo, juce::uint32 signatureDenominator) {
  sequence = std::vector<AHR> {};
  for (int i = 0; i < 16; ++i)
    sequence.push_back(AHR {});

  totalSeconds = (60.0f/ tempo) * (static_cast<float>(noteSubdivision) / signatureDenominator);
}

EnvelopeSequence::~EnvelopeSequence() {
  
}

AHR EnvelopeSequence::getEnvelope(juce::uint32 index) {
  if (index >= sequence.size()) {
    throw std::invalid_argument("Attempted to access a index out-of-bounds in sequence");
  }
  return sequence[index];
}

AHR EnvelopeSequence::getActive() {
  return getEnvelope(activeIdx);
}

juce::uint32 EnvelopeSequence::getNoteSubdivision() noexcept {
  return noteSubdivision;
}

float EnvelopeSequence::getTotalSeconds() noexcept {
  return totalSeconds;
}

void EnvelopeSequence::setTempoSync(float tempo, juce::uint32 signatureDenominator) {
  if (tempo > 0 && signatureDenominator == 0){
    throw std::invalid_argument("Signature Denominator is 0 or tempo is not greater than 0");
  }
  totalSeconds = (60.0f / tempo) * (static_cast<float>(noteSubdivision) / signatureDenominator);
}

void EnvelopeSequence::setNoteSubdivision(juce::uint32 sub) {
  if (sub == 0) {
    throw std::invalid_argument("Signature Numerator is 0");
  }
  noteSubdivision = sub;
}

