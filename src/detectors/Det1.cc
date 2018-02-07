#include <G4Box.hh>
#include <G4LogicalVolume.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4Tubs.hh>

#include "G4TestBench.hh"

class Det1 : public G4VUserDetectorConstruction {
   public:
    Det1() { ; }
    ~Det1() { ; }

    G4VPhysicalVolume *Construct() {
        G4NistManager *nist = G4NistManager::Instance();
        G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
        G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");

        auto worldSolid = new G4Box("worldSolid", 10 * CLHEP::m, 10 * CLHEP::m, 10 * CLHEP::m);
        auto worldLog = new G4LogicalVolume(worldSolid, air, "worldLog");

        auto leadBlockSolid = new G4Box("leadBlockSolid", 100 * CLHEP::cm, 100 * CLHEP::cm, 1 * CLHEP::cm);
        auto leadBlockLog = new G4LogicalVolume(leadBlockSolid, lead, "leadBlockLog");
        auto blockHoleSolid = new G4Tubs("blockHoleSolid", 0, 0.1 * CLHEP::mm, CLHEP::m, 0, 2 * CLHEP::pi);
        auto blockHoleLog = new G4LogicalVolume(blockHoleSolid, air, "blockHoleLog");
        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 1000; j++) {
                std::stringstream physNameSS;
                physNameSS << "blockHolePhys(" << i << "," << j << ")";
                new G4PVPlacement(0, G4ThreeVector(i * 0.25 * CLHEP::mm, (j + 0.5) * 0.25 * CLHEP::mm, 0),
                                  blockHoleLog, physNameSS.str(), leadBlockLog, true, i * 1000 + j, true);
            }
        }
        new G4PVPlacement(0, G4ThreeVector(), leadBlockLog, "detPhys", worldLog, false, 0, true);

        G4VPhysicalVolume *worldPhys =
            new G4PVPlacement(0, G4ThreeVector(), worldLog, "worldPhys", 0, false, 0, true);
        return worldPhys;
    }

    void ConstructSDandField() { ; }
};

DETECTOR_TYPE(Det1)
