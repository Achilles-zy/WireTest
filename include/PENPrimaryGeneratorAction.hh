#ifndef PENPrimaryGeneratorAction_h
#define PENPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "PENRunAction.hh"

class G4GeneralParticleSource;
class G4Event;
class PENRunAction;

class PENPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
private:
    G4GeneralParticleSource* PENGPS;
    G4double PrimaryE;
    G4String PrimaryName;
    PENRunAction* RunAction;
public:
    PENPrimaryGeneratorAction();
    ~PENPrimaryGeneratorAction();
    void GeneratePrimaries(G4Event* anEvent);

    G4double GetPrimaryE();
    G4String GetPrimaryName();

};

#endif