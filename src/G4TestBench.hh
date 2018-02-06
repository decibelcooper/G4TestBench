#ifndef G4_TEST_BENCH_GLOBALS_HH
#define G4_TEST_BENCH_GLOBALS_HH

#include <unistd.h>
#include <map>

#include <G4String.hh>
#include <G4VUserDetectorConstruction.hh>

namespace g4testbench {
class DetFactory {
   public:
    virtual G4VUserDetectorConstruction *Create() = 0;
};

class Detector {
   public:
    static void RegisterType(const G4String &name, DetFactory *factory) { factories[name] = factory; }
    static G4VUserDetectorConstruction *Create(G4String name) {
        if (factories.count(name))
            return factories[name]->Create();
        else
            return NULL;
    }
    static std::vector<G4String> AvailableTypes() {
        std::vector<G4String> types;
        for (auto nameFactPair : factories) {
            types.push_back(nameFactPair.first);
        }
        return types;
    }

   private:
    static std::map<G4String, DetFactory *> factories;
};
}  // namespace g4testbench

#define DETECTOR_TYPE(klass)                                          \
    namespace g4testbench {                                           \
    class klass##Factory : public DetFactory {                        \
       public:                                                        \
        klass##Factory() { Detector::RegisterType(#klass, this); }    \
        G4VUserDetectorConstruction *Create() { return new klass(); } \
    };                                                                \
    static klass##Factory global_##klass##Factory;                    \
    }

#endif  // G4_TEST_BENCH_GLOBALS_HH
