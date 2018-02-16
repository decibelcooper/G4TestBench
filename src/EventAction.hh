#ifndef G4_TEST_BENCH_EVENT_ACTION_H
#define G4_TEST_BENCH_EVENT_ACTION_H

#include <G4UserEventAction.hh>

namespace g4testbench {
class EventAction : public G4UserEventAction {
   public:
    EventAction();
    virtual ~EventAction();

    void BeginOfEventAction(const G4Event *event);
    void EndOfEventAction(const G4Event *event);

   private:
};
}  // namespace g4testbench

#endif  // G4_TEST_BENCH_EVENT_ACTION_H
