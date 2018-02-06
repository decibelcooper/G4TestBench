#ifndef G4_TEST_BENCH_PRIMARY_GENERATOR_ACTION_H
#define G4_TEST_BENCH_PRIMARY_GENERATOR_ACTION_H

#include <G4ParticleGun.hh>
#include <G4VUserPrimaryGeneratorAction.hh>

namespace g4testbench {
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
   public:
    PrimaryGeneratorAction();
    virtual ~PrimaryGeneratorAction();

    void GeneratePrimaries(G4Event *event);

   private:
    G4ParticleGun *gun;
};
}  // namespace g4testbench

#endif  // G4_TEST_BENCH_PRIMARY_GENERATOR_ACTION_H
