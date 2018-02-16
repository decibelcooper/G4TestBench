#include <G4PeriodicBoundaryPhysics.hh>
#include <G4VModularPhysicsList.hh>
#include <QGSP_BERT_HP.hh>

#include "G4TestBench.hh"

class PeriodicXY : public QGSP_BERT_HP {
   public:
    PeriodicXY() {
        auto constructor = new G4PeriodicBoundaryPhysics("PeriodicXY", true, true, false, false);
        constructor->SetVerboseLevel(0);
        RegisterPhysics(constructor);
    }
};

PHYSICS_LIST(PeriodicXY)
