//PENRunAction.cc
 
#include "PENRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::PENRunAction():
	EscapedElectronCount(0),
	SignalEventCount(0)
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->CreateH1("Electron Energy", "Electron Energy", 200, 0 * keV, 1 * MeV);
  analysisManager->CreateH1("Total Energy", "Total Energy", 200, 0, 1 * MeV);

  analysisManager->CreateNtuple("Data", "EventCount");
  analysisManager->CreateNtupleIColumn("EscapedEventCount");
  analysisManager->CreateNtupleIColumn("SignalEventCount");
  analysisManager->CreateNtupleDColumn("Electron Energy");
  analysisManager->CreateNtupleDColumn("Total Energy");

  analysisManager->FinishNtuple();
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(EscapedElectronCount);
  accumulableManager->RegisterAccumulable(SignalEventCount);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::~PENRunAction()
{
  

  // delete G4AnalysisManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENRunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4int RunID = aRun->GetRunID();
  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = "Result";
  fileName = std::to_string(RunID);
  analysisManager->SetFileName(fileName);
  analysisManager->OpenFile();

  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  G4cout << "Run " << aRun->GetRunID() << " started." << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();
  G4cout << "Run " << aRun->GetRunID() << " finished." << G4endl;
  G4cout << "Escaped Electron Count = " << EscapedElectronCount.GetValue() << G4endl;
  G4cout << "Signal Event Count =" << SignalEventCount.GetValue() << G4endl;
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleIColumn(0, EscapedElectronCount.GetValue());
  analysisManager->FillNtupleIColumn(1, SignalEventCount.GetValue());

  analysisManager->AddNtupleRow();

  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
