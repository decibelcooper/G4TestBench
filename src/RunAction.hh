#ifndef G4_TEST_BENCH_RUN_ACTION_H
#define G4_TEST_BENCH_RUN_ACTION_H

#include <proio/writer.h>
#include <G4String.hh>
#include <G4UserRunAction.hh>

namespace g4testbench {
class RunAction : public G4UserRunAction {
   public:
    RunAction(const G4String &outputFile);
    virtual ~RunAction();

    void BeginOfRunAction(const G4Run *run);
    void EndOfRunAction(const G4Run *run);

   private:
    G4String outputFile;
};

extern pthread_mutex_t *eventWriteLock;
extern proio::Writer *proioWriter;
}  // namespace g4testbench

#endif  // G4_TEST_BENCH_RUN_ACTION_H
