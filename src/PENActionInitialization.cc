#include "PENActionInitialization.hh"
#include "PENPrimaryGeneratorAction.hh"
#include "PENRunAction.hh"
#include "PENEventAction.hh"
#include "PENSteppingAction.hh"
#include "PENTrackingAction.hh"
#include "PENStackingAction.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::PENActionInitialization(PENDetectorConstruction* det)
	: G4VUserActionInitialization()
{
	fDet = det;
	fPrimaryGen = new PENPrimaryGeneratorAction(fDet);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::~PENActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::BuildForMaster() const
{
	PENRunAction* runAction = new PENRunAction(fPrimaryGen, fDet);
	SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::Build() const
{
	//PENPrimaryGeneratorAction* PrimaryGen = new PENPrimaryGeneratorAction();
	SetUserAction(fPrimaryGen);

	PENRunAction* runAction = new PENRunAction(fPrimaryGen, fDet);
	SetUserAction(runAction);

	PENEventAction* eventAction = new PENEventAction(runAction);
	SetUserAction(eventAction);

	SetUserAction(new PENSteppingAction(eventAction, runAction));
	SetUserAction(new PENStackingAction);
}