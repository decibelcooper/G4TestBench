#include <iostream>

#include <G4GDMLParser.hh>
#include <G4MTRunManager.hh>
#include <G4NistManager.hh>
#include <G4RunManager.hh>
#include <G4UIExecutive.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <QGSP_BERT_HP.hh>
#include <Randomize.hh>

#include "ActionInit.hh"
#include "G4TestBench.hh"

using namespace g4testbench;

std::map<G4String, DetFactory *> Detector::factories;

void printUsage() {
    std::cerr << "Usage: g4testbench [options] <detector type> <macro file>\n";
    std::cerr << "options:\n";
    std::cerr << "  -n  number of threads (default 1)\n";
    std::cerr << "  -v  verbose\n";
    std::cerr << std::endl;
}

int main(int argc, char **argv) {
    int nThreads = 1;
    bool verbose = false;
    int opt;
    while ((opt = getopt(argc, argv, "n:vh")) != -1) {
        switch (opt) {
            case 'n':
                nThreads = atoi(optarg);
                break;
            case 'v':
                verbose = true;
                break;
            default:
                printUsage();
                exit(EXIT_FAILURE);
        }
    }

    G4String detectorType;
    G4String macroFilename;
    if (optind + 1 < argc) {
        detectorType = argv[optind];
        macroFilename = argv[optind + 1];
    } else {
        printUsage();
        exit(EXIT_FAILURE);
    }

    G4VUserDetectorConstruction *detConst = Detector::Create(detectorType);
    if (detConst == NULL) {
        std::cerr << "Detector type " << detectorType << " not available\n";
        std::cerr << "Available detector types:\n";
        for (auto type : Detector::AvailableTypes()) {
            std::cerr << "\t" << type << "\n";
        }
        exit(EXIT_FAILURE);
    }

#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(nThreads);
#else
    G4RunManager *runManager = new G4RunManager;
#endif

    runManager->SetUserInitialization(detConst);
    runManager->SetUserInitialization(new QGSP_BERT_HP());
    runManager->SetUserInitialization(new ActionInit("asdf"));

    runManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + macroFilename);

    delete runManager;

    exit(EXIT_SUCCESS);
}
