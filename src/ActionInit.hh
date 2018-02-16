#ifndef G4_TEST_BENCH_ACTION_INIT_H
#define G4_TEST_BENCH_ACTION_INIT_H

#include <pthread.h>

#include <G4String.hh>
#include <G4VUserActionInitialization.hh>

namespace g4testbench {
class ActionInit : public G4VUserActionInitialization {
   public:
    ActionInit(const G4String &outputFile);
    virtual ~ActionInit();

    void Build() const;
    void BuildForMaster() const;

   private:
    G4String outputFile;
};
}  // namespace g4testbench

#endif  // G4_TEST_BENCH_ACTION_INIT_H
