#include <G4Box.hh>
#include <G4IntersectionSolid.hh>
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

        auto leadBlockSolid = new G4Box("leadBlockSolid", 5 * CLHEP::m, 5 * CLHEP::m, 5 / 2. * CLHEP::cm);
        auto leadBlockLog = new G4LogicalVolume(leadBlockSolid, lead, "leadBlockLog");
        G4VSolid *blockPoreSolid =
            new G4Tubs("blockPoreSolid0", 0, 0.005 * CLHEP::mm, 5 * CLHEP::cm, 0, 2 * CLHEP::pi);
        blockPoreSolid = new G4IntersectionSolid("blockPoreSolid1", leadBlockSolid, blockPoreSolid,
                                                 new G4RotationMatrix(0, 0.175, 0), G4ThreeVector(0, 0, 0));
        auto blockPoreLog = new G4LogicalVolume(blockPoreSolid, air, "blockPoreLog");
        for (int i = -5000; i <= 5000; i++) {
            for (int j = -5000; j <= 5000; j++) {
                std::stringstream physNameSS;
                physNameSS << "blockPorePhys(" << i << "," << j << ")";
                new G4PVPlacement(0,
                                  G4ThreeVector((i + 0.5 * j) * 0.25 * CLHEP::mm,
                                                (0.5 * sqrt(3.) * j) * 0.25 * CLHEP::mm, 0),
                                  blockPoreLog, physNameSS.str(), leadBlockLog, true, i * 1000 + j, false);
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
