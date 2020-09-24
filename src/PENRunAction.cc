//PENRunAction.cc
 
#include "PENRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

#include "g4root.hh"
#include <iomanip>
#include <fstream>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENRunAction::PENRunAction(PENPrimaryGeneratorAction* gen,PENDetectorConstruction* det):
	EscapedElectronCount(0),
	SignalEventCount(0),
	ParticleE(0.),
	PrimaryGenerator(gen),
	FileName("File")
{
	fDetCons = det;
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->CreateH1("ElectronEnergy", "Electron Energy", 200, 0 * keV, 1 * MeV);
	analysisManager->CreateH1("TotalEnergy", "Total Energy", 200, 0, 1 * MeV);

	analysisManager->CreateNtuple("Data", "EventCount");
	analysisManager->CreateNtupleIColumn("EscapedEventCount");
	analysisManager->CreateNtupleIColumn("SignalEventCount");
	analysisManager->CreateNtupleDColumn("ElectronEnergy");
	analysisManager->CreateNtupleDColumn("TotalEnergy");

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
  //G4String FileName = "Result";
  G4String filename = fDetCons->GetWireType() + "_" + fDetCons->GetConfine() + "_" + std::to_string(RunID);
  //analysisManager->SetFileName(FileName);
  analysisManager->OpenFile(filename);

  analysisManager->SetVerboseLevel(1);


  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();
  if (G4RunManager::GetRunManager()->GetRunManagerType() == 1) {
	  G4cout << "Run " << aRun->GetRunID() << " started." << G4endl;
  }
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENRunAction::EndOfRunAction(const G4Run* aRun)
{
  G4RunManager::GetRunManager()->GetRunManagerType();
	
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  auto analysisManager = G4AnalysisManager::Instance();
  G4String fileName = PrimaryGenerator->GetPrimaryName() + "_" + std::to_string(PrimaryGenerator->GetPrimaryE()).substr(0, 4) + "MeV";	  G4cout << fileName << G4endl;
 // analysisManager->SetFileName(fileName);

  if (G4RunManager::GetRunManager()->GetRunManagerType() == 1) {
	  G4cout << "Run " << aRun->GetRunID() << " finished." << G4endl;
	  G4cout << "Particle Source is " << PrimaryGenerator->GetPrimaryName() << G4endl;
	  G4cout << "Primary Energy = " << PrimaryGenerator->GetPrimaryE() << G4endl;
	  G4cout << "Escaped Electron Count = " << EscapedElectronCount.GetValue() << G4endl;
	  G4cout << "Signal Event Count = " << SignalEventCount.GetValue() << G4endl;

	  std::ofstream output;
	  if (aRun->GetRunID() == 0) {
		  output.open("Simulation Result.txt", std::ios::ate);
	  }
	  else
	  {
		  output.open("Simulation Result.txt", std::ios::app);
	  }
	  output
		  << "Run ID:\t" << std::setw(5) << aRun->GetRunID() << '\t'
		  << "Primary Particle is\t" << std::setw(5) << PrimaryGenerator->GetPrimaryName() << '\t'
		  << "Primary Energy(MeV) =\t" << std::setw(5) << std::setiosflags(std::ios::fixed) << std::setprecision(2) << PrimaryGenerator->GetPrimaryE() << '\t'
		  << "Escaped Electron Count =\t" << std::left << std::setw(10) << EscapedElectronCount.GetValue() << '\t'
		  << "Signal Event Count =\t" << std::setw(10) << SignalEventCount.GetValue() << G4endl;
	  output.close();
	  //std::DecimalFormat df1 = new DecimalFormat("0.0");
  }

  analysisManager->FillNtupleIColumn(0, EscapedElectronCount.GetValue());
  analysisManager->FillNtupleIColumn(1, SignalEventCount.GetValue());

  analysisManager->AddNtupleRow();
  analysisManager->Write();

  analysisManager->CloseFile();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
