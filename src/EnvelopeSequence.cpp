#include "EnvelopeSequence.h"
#include "juce_core/juce_core.h"
#include <stdexcept>
#include <vector>

EnvelopeSequence::EnvelopeSequence(float tempo, juce::uint32 signatureDenominator) {
  sequence = std::vector<AHR> {};
  for (int i = 0; i < 16; ++i)
    sequence.push_back(AHR {});

  totalSeconds = (tempo / 60.0f) * (static_cast<float>(noteSubdivision) / signatureDenominator);
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
  totalSeconds = (tempo / 60.0f) * (static_cast<float>(noteSubdivision) / signatureDenominator);
}

void EnvelopeSequence::setNoteSubdivision(juce::uint32 sub) {
  if (sub == 0) {
    throw std::invalid_argument("Signature Numerator is 0");
  }
  noteSubdivision = sub;
}
