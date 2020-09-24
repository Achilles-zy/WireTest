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
	PrimaryGen = new PENPrimaryGeneratorAction();
	fDet = det;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENActionInitialization::~PENActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::BuildForMaster() const
{
	PENRunAction* runAction = new PENRunAction(PrimaryGen, fDet);
	SetUserAction(runAction);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENActionInitialization::Build() const
{
	//PENPrimaryGeneratorAction* PrimaryGen = new PENPrimaryGeneratorAction();
	SetUserAction(PrimaryGen);

	PENRunAction* runAction = new PENRunAction(PrimaryGen, fDet);
	SetUserAction(runAction);

	PENEventAction* eventAction = new PENEventAction(runAction);
	SetUserAction(eventAction);

	SetUserAction(new PENSteppingAction(eventAction, runAction));
	SetUserAction(new PENStackingAction);
}