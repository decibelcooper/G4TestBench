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
std::map<G4String, PhysFactory *> PhysicsList::factories;

void printUsage() {
    std::cout << "Usage: g4testbench [options] <detector type> [macro file]\n";
    std::cout << "options:\n";
    std::cout << "  -l  list all factories (e.g. physics lists and detector types)\n";
    std::cout << "  -n  number of threads (default 1)\n";
    std::cout << "  -o  output file name (default output.proio)\n";
    std::cout << "  -p  physics list (default QGSP_BERT_HP)\n";
    std::cout << std::endl;
}

void printDetectorTypes() {
    std::cout << "Available detector types:\n";
    for (auto type : Detector::AvailableTypes()) {
        std::cout << "\t" << type << "\n";
    }
}

void printPhysicsLists() {
    std::cout << "Available physics lists:\n";
    for (auto type : PhysicsList::AvailableTypes()) {
        std::cout << "\t" << type << "\n";
    }
}

int main(int argc, char **argv) {
    int nThreads = 1;
    (void)nThreads;
    G4String outputFile = "output.proio";
    G4String physListType = "QGSP_BERT_HP";
    int opt;
    while ((opt = getopt(argc, argv, "ln:o:p:vh")) != -1) {
        switch (opt) {
            case 'l':
                printDetectorTypes();
                printPhysicsLists();
                exit(EXIT_SUCCESS);
            case 'n':
                nThreads = atoi(optarg);
                break;
            case 'o':
                outputFile = optarg;
                break;
            case 'p':
                physListType = optarg;
                break;
            default:
                printUsage();
                exit(EXIT_FAILURE);
        }
    }

    G4String detectorType;
    G4String macroFilename;
    if (optind < argc) {
        detectorType = argv[optind];
    } else {
        printUsage();
        exit(EXIT_FAILURE);
    }

    G4VUserDetectorConstruction *detConst = Detector::Create(detectorType);
    if (detConst == NULL) {
        std::cout << "Detector type " << detectorType << " not available\n";
        printDetectorTypes();
        exit(EXIT_FAILURE);
    }

    G4VUserPhysicsList *physList = PhysicsList::Create(physListType);
    if (physList == NULL) {
        std::cout << "Physics list " << physListType << " not available\n";
        printPhysicsLists();
        exit(EXIT_FAILURE);
    }

#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(nThreads);
#else
    G4RunManager *runManager = new G4RunManager;
#endif

    runManager->SetUserInitialization(detConst);
    runManager->SetUserInitialization(physList);
    runManager->SetUserInitialization(new ActionInit(outputFile));

    runManager->Initialize();

    if (optind + 1 < argc) {
        G4String command = "/control/execute ";
        G4UImanager *UImanager = G4UImanager::GetUIpointer();
        UImanager->ApplyCommand(command + argv[optind + 1]);
    } else {
        G4UIExecutive *ui = new G4UIExecutive(argc, argv);
        ui->SessionStart();
        delete ui;
    }

    delete runManager;

    exit(EXIT_SUCCESS);
}
