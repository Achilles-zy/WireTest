// PENSteppingAction.cc

#include "PENSteppingAction.hh"

#include "PENDetectorConstruction.hh"
#include "PENEventAction.hh"

#include "G4Track.hh"
#include "G4SteppingManager.hh"

#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "G4RunManager.hh"

#include "G4HadronicProcessType.hh"
#include "G4EmProcessSubType.hh"
#include "G4AnalysisMessenger.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PENSteppingAction::PENSteppingAction(
				     PENEventAction* evt, PENRunAction* run)
:EventAction(evt),
RunAction(run)
{
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENSteppingAction::UserSteppingAction(const G4Step* aStep)
{
	const PENDetectorConstruction* detectorConstruction
		= static_cast<const PENDetectorConstruction*>
		(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	auto volume = aStep->GetPostStepPoint()->GetTouchableHandle()->GetVolume();
	auto particle_name = aStep->GetTrack()->GetParticleDefinition()->GetParticleName();
	G4double ElectronEnergy;
	if (volume == detectorConstruction->GetEnv() && particle_name == "e-") {
		auto analysisManager = G4AnalysisManager::Instance();
		ElectronEnergy = aStep->GetPostStepPoint()->GetKineticEnergy();
		analysisManager->FillH1(0, ElectronEnergy);
		analysisManager->FillNtupleDColumn(2, ElectronEnergy);
		EventAction->AddEscapedEnergy(ElectronEnergy);
		EventAction->EscapedTrue();
		RunAction->CountEscapedElectron();
		aStep->GetTrack()->SetTrackStatus(fStopAndKill);
	}
	//G4cout<<particle_name<<G4endl;
	//G4cout<<volume<<G4endl;
	
	auto edep = aStep->GetTotalEnergyDeposit();

	G4int processtype = aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessSubType();
	aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
	G4int creatorprocess = aStep->GetTrack()->GetCreatorProcess()->GetProcessSubType();
	G4int parentID = aStep->GetTrack()->GetParentID();
	if (parentID == 1) {
	//if (processtype == fRadioactiveDecay) {
		if (processtype == fRadioactiveDecay || processtype == fScintillation) {
			//aStep->GetTrack()->SetTrackStatus(fKillTrackAndSecondaries);
		}
	}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

