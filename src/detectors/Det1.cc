#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>

#include "G4TestBench.hh"

class Det1 : public G4VUserDetectorConstruction {
   public:
    Det1() { ; }
    ~Det1() { ; }

    G4VPhysicalVolume *Construct() {
        G4NistManager *nist = G4NistManager::Instance();
        G4Material *air = nist->FindOrBuildMaterial("G4_AIR");

        auto worldSolid = new G4Box("worldSolid", 1, 1, 1);
        auto worldLog = new G4LogicalVolume(worldSolid, air, "worldLog");

        G4VPhysicalVolume *worldPhys =
            new G4PVPlacement(0, G4ThreeVector(), worldLog, "worldPhys", 0, false, 0, true);
        return worldPhys;
    }

    void ConstructSDandField() { ; }
};

DETECTOR_TYPE(Det1)
