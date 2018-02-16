#include <G4Box.hh>
#include <G4IntersectionSolid.hh>
#include <G4LogicalVolume.hh>
#include <G4NistManager.hh>
#include <G4PVPlacement.hh>
#include <G4PeriodicBoundaryBuilder.hh>
#include <G4Step.hh>
#include <G4THitsCollection.hh>
#include <G4TouchableHistory.hh>
#include <G4Tubs.hh>
#include <G4VSensitiveDetector.hh>

#include "G4TestBench.hh"

const double porePitch = 0.01085 * CLHEP::mm;
const double poreRadius = 0.005 * CLHEP::mm;
const double biasAngle = 0.1745;
const double cellHalfWidth = porePitch / (2 * cos(biasAngle));
const double cellHalfHeight = porePitch * sqrt(3.) / 2;
const double cellHalfDepth = porePitch / (2 * sin(biasAngle));
const double blockHalfDepth = int(2.5 * CLHEP::cm / (2 * cellHalfDepth)) * 2 * cellHalfDepth;
const double poreXProj = blockHalfDepth * tan(biasAngle);

class MicroPore : public G4VUserDetectorConstruction {
   public:
    G4VPhysicalVolume *Construct() {
        G4NistManager *nist = G4NistManager::Instance();
        G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
        G4Material *lead = nist->FindOrBuildMaterial("G4_Pb");

        auto blockSolid = new G4Box("blockSolid", cellHalfWidth, cellHalfHeight, blockHalfDepth);
        auto worldLog = new G4LogicalVolume(blockSolid, air, "worldLog");
        auto pbb = new G4PeriodicBoundaryBuilder();
        auto periodicLog = pbb->Construct(worldLog);
        periodicLog->SetMaterial(lead);
        auto blockPoreSolid = new G4Tubs("blockPoreSolid", 0, poreRadius, 2 * blockHalfDepth / cos(biasAngle),
                                         0, 2 * CLHEP::pi);
        int nPores = 0;
        for (int i = 0; i < 3; i++) {
            double y = (i - 1) * cellHalfHeight;
            for (double x = (i % 2 - 1) * cellHalfWidth; x <= cellHalfWidth; x += 2 * cellHalfWidth) {
                for (double z = -blockHalfDepth; z <= blockHalfDepth;
                     z += 2 * (2 - (i % 2)) * cellHalfDepth) {
                    std::stringstream nameSS;

                    nameSS << "blockPoreSolid(" << nPores << ")";
                    auto thisBlockPoreSolid = new G4IntersectionSolid(
                        nameSS.str(), periodicLog->GetSolid(), blockPoreSolid,
                        new G4RotationMatrix(CLHEP::pi / 2., biasAngle, 0), G4ThreeVector(x, y, z));
                    nameSS.str("");

                    nameSS << "blockPoreLog(" << nPores << ")";
                    auto blockPoreLog = new G4LogicalVolume(thisBlockPoreSolid, air, nameSS.str());
                    nameSS.str("");

                    nameSS << "blockPorePhys(" << nPores << ")";
                    new G4PVPlacement(0, G4ThreeVector(0, 0, 0), blockPoreLog, nameSS.str(), periodicLog,
                                      false, 0, false);

                    nPores++;
                }
            }
        }
        std::cout << "NUM PORES: " << nPores << std::endl;

        G4VPhysicalVolume *worldPhys =
            new G4PVPlacement(0, G4ThreeVector(), worldLog, "worldPhys", 0, false, 0, false);
        return worldPhys;
    }

    class SensitiveDetector : public G4VSensitiveDetector {
       public:
        SensitiveDetector(G4String name) : G4VSensitiveDetector(name) { collectionName.insert(name); }
        ~SensitiveDetector() { ; }

        void Initialize(G4HCofThisEvent *) { ; }
        void EndOfEvent(G4HCofThisEvent *) { ; }

        G4bool ProcessHits(G4Step *step, G4TouchableHistory *) {
            G4StepPoint *postPoint = step->GetPostStepPoint();
            if (postPoint->GetStepStatus() != fGeomBoundary) return false;
            if (!postPoint->GetPhysicalVolume()->GetName().contains("blockPorePhys")) return false;
            G4Track *track = step->GetTrack();
            G4ParticleDefinition *def = track->GetDefinition();
            if (def->GetPDGCharge() == 0) return false;

            G4ThreeVector pos = postPoint->GetPosition();
            std::cout << "HIT: " << pos.x() << ", " << pos.y() << ", " << pos.z() << std::endl;

            return true;
        }
    };

    void ConstructSDandField() { SetSensitiveDetector("logical_periodic", new SensitiveDetector("poreSD")); }
};

DETECTOR_TYPE(MicroPore)
