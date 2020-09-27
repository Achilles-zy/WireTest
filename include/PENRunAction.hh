//PENRunAction.hh

#ifndef PENRunAction_h
#define PENRunAction_h

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4AccumulableManager.hh"
#include "G4Accumulable.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "PENPrimaryGeneratorAction.hh"
#include "PENDetectorConstruction.hh"

class G4Run;
class PENPrimaryGeneratorAction;

class PENRunAction : public G4UserRunAction
{
public:
	PENRunAction(PENPrimaryGeneratorAction*, PENDetectorConstruction*);
	~PENRunAction();

	G4double ParticleE;
	void BeginOfRunAction(const G4Run*);
	void EndOfRunAction(const G4Run*);

	void CountEscapedElectron() {
		EscapedElectronCount += 1;
	};

	void CountSignalEvent() {
		SignalEventCount += 1;
	};

private:
	//number of events that generate signals in bulk
	//G4int BulkEventCount;
	//number of events that generate signals in SiPMs
	//G4int SiPMEventCount;
	//G4int VetoEventCount;
	PENPrimaryGeneratorAction* fPrimaryGenerator;
	PENDetectorConstruction* fDetCons;
	G4Accumulable<G4int> EscapedElectronCount;
	G4Accumulable<G4int> SignalEventCount;
	G4String filename;
	G4String txtname;

};

#endif