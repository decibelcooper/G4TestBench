#include <G4ParticleDefinition.hh>
#include <G4ParticleTable.hh>
#include <Randomize.hh>

#include "PrimaryGeneratorAction.hh"

using namespace g4testbench;

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    G4ParticleTable *table = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *particle = table->FindParticle("e-");
    table->DumpTable();

    gun = new G4ParticleGun(1);
    gun->SetParticleDefinition(particle);
    gun->SetParticleMomentumDirection(G4ThreeVector(0, 0, 1));
    gun->SetParticlePolarization(G4ThreeVector(0, 1, 0));
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete gun; }

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event) { gun->GeneratePrimaryVertex(event); }
