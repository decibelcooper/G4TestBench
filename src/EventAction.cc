#include <proio/event.h>
#include <G4Event.hh>
#include <G4HCofThisEvent.hh>
#include <G4VHit.hh>

#include "EventAction.hh"
#include "RunAction.hh"

using namespace g4testbench;

EventAction::EventAction() { ; }

EventAction::~EventAction() { ; }

void EventAction::BeginOfEventAction(const G4Event *) { ; }

void EventAction::EndOfEventAction(const G4Event *event) {
    pthread_mutex_lock(eventWriteLock);
    auto proioEvent = new proio::Event();

    G4HCofThisEvent *hcc = event->GetHCofThisEvent();
    for (int i = 0; i < hcc->GetCapacity(); i++) {
        G4VHitsCollection *hc = hcc->GetHC(i);
        if (!hc) continue;
        for (size_t j = 0; j < hc->GetSize(); j++) {
            auto hit = dynamic_cast<google::protobuf::Message *>(hc->GetHit(j));
            if (hit) {
                auto hitCopy = hit->New();
                hitCopy->CopyFrom(*hit);
                proioEvent->AddEntry(hitCopy, "Simulated");
            }
        }
    }

    proioWriter->Push(proioEvent);
    delete proioEvent;
    pthread_mutex_unlock(eventWriteLock);
}
