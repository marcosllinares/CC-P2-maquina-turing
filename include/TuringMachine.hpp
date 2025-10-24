#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include "Tape.hpp"
#include "Transition.hpp"
#include "Alphabet.hpp"

class TuringMachine {
public:
  TuringMachine(const std::set<std::string>& states,
                const Alphabet& input_alphabet,
                const Alphabet& tape_alphabet,
                const std::string& initial_state,
                char blank_symbol,
                const std::set<std::string>& final_states,
                size_t num_tapes = 1);
  
  void addTransition(const Transition& transition);
  bool run(const std::string& input, size_t max_steps = 100);
  
  bool isAccepted() const;
  std::string getCurrentState() const;
  std::vector<std::string> getTapesContent() const;
  std::vector<std::string> getTapesContentWithHead() const;
  size_t getStepCount() const;
  
  void reset();
  void printConfiguration() const;
  std::string getResultFromFirstTape() const;

private:
  std::set<std::string> states_;
  Alphabet input_alphabet_;
  Alphabet tape_alphabet_;
  std::string initial_state_;
  char blank_symbol_;
  std::set<std::string> final_states_;
  size_t num_tapes_;
  
  std::vector<Transition> transitions_;
  std::vector<Tape> tapes_;
  std::string current_state_;
  size_t step_count_;
  bool halted_;
  
  const Transition* findTransition(const std::string& state, 
                                   const std::vector<char>& symbols) const;
  bool executeStep();
  void initializeTapes(const std::string& input);
  std::vector<char> readCurrentSymbols() const;
};
