//#include <G4RunManager.hh>

#include "RunAction.hh"

pthread_mutex_t *g4testbench::eventWriteLock;
proio::Writer *g4testbench::proioWriter;

using namespace g4testbench;

RunAction::RunAction(const G4String &file) {
    outputFile = file;
    eventWriteLock = new pthread_mutex_t;
    pthread_mutex_init(eventWriteLock, NULL);
}

RunAction::~RunAction() {
    pthread_mutex_destroy(eventWriteLock);
    delete eventWriteLock;
}

void RunAction::BeginOfRunAction(const G4Run *) { proioWriter = new proio::Writer(outputFile); }

void RunAction::EndOfRunAction(const G4Run *) { delete proioWriter; }
