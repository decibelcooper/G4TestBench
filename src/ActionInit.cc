#include <G4RunManager.hh>

#include "ActionInit.hh"
#include "PrimaryGeneratorAction.hh"

using namespace g4testbench;

ActionInit::ActionInit(const G4String &file) { outputFile = file; }

ActionInit::~ActionInit() { ; }

void ActionInit::Build() const {
    G4RunManager::GetRunManager()->SetPrintProgress(1000);

    SetUserAction(new PrimaryGeneratorAction);
    // SetUserAction(new SteppingAction);
    // SetUserAction(new TrackingAction);
    // SetUserAction(new EventAction);
}

void ActionInit::BuildForMaster() const {
    // SetUserAction(new RunAction(outputFile));
}
