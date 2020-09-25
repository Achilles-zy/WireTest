#ifndef PENPrimaryGeneratorAction_h
#define PENPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "PENRunAction.hh"
#include "PENDetectorConstruction.hh"

class G4GeneralParticleSource;
class G4Event;
class PENRunAction;
class PENDetectorConstruction;

class PENPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
private:
    G4GeneralParticleSource* PENGPS;
    G4double PrimaryE;
    G4String PrimaryName;
    PENRunAction* RunAction;
    PENDetectorConstruction* fDetCons;
public:
    PENPrimaryGeneratorAction(PENDetectorConstruction*);
    ~PENPrimaryGeneratorAction();
    void GeneratePrimaries(G4Event* anEvent);

    G4double GetPrimaryE();
    G4String GetPrimaryName();

};

#endif