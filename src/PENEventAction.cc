//PENEventAction.cc

#include "PENEventAction.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4SystemOfUnits.hh"
#include "G4VVisManager.hh"
#include "G4ios.hh"
#include "g4root.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENEventAction::PENEventAction(PENRunAction* runaction)
	: EscapedEnergy(0.),
    EnergyThreshold(0.),
	SiPM_0(0),
	SiPM_1(0),
	SiPM_2(0),
	SiPM_3(0),
	SiPM_4(0),
	SiPM_5(0),
	SiPM_6(0),
	SiPM_7(0),
	SiPM_8(0),
	SiPM_9(0),
	SiPM_10(0),
	SiPM_11(0),
	SiPM_12(0),
	SiPM_13(0),
	SiPM_14(0),
	SiPM_15(0),
	Total(0),
	ID(0),
	PhotonCount(0),
	ifEscaped(false),
	run(runaction)
    //ResultFile("Distribution_Results_NTuple.root","RECREATE"),
    //Distribution_Results("Distribution_Results","Distribution_Results")
{
    EnergyThreshold = 0 * keV;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
PENEventAction::~PENEventAction()
{
  //Distribution_Results.Write();
  //ResultFile.Close();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENEventAction::BeginOfEventAction(const G4Event* evt)
{
  EscapedEnergy = 0;
  SiPM_0 = 0;
  SiPM_1 = 0;
  SiPM_2 = 0;
  SiPM_3 = 0;
  SiPM_4 = 0;
  SiPM_5 = 0;
  SiPM_6 = 0;
  SiPM_7 = 0;
  SiPM_8 = 0;
  SiPM_9 = 0;
  SiPM_10 = 0;
  SiPM_11 = 0;
  SiPM_12 = 0;
  SiPM_13 = 0;
  SiPM_14 = 0;
  SiPM_15 = 0;
  Total = 0;
  PhotonCount = 0;
  ifEscaped = false;
  // G4cout<<ID<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void PENEventAction::EndOfEventAction(const G4Event* evt)
{
  auto analysisManager = G4AnalysisManager::Instance();
  
  if (ifEscaped == true && EscapedEnergy > EnergyThreshold) {
      analysisManager->FillH1(1, EscapedEnergy);
      analysisManager->FillNtupleDColumn(3, EscapedEnergy);
      run->CountSignalEvent();
  }

  ID++;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PENEventAction::AddEscapedEnergy(G4double de)
{
    EscapedEnergy += de;
}

void PENEventAction::AddToSiPM(G4int i){
  Total++;
  switch (i){
    case 0:
    SiPM_0 ++;
    break;
    case 1:
    SiPM_1 ++;
    break;
    case 2:
    SiPM_2 ++;
    break;
    case 3:
    SiPM_3 ++;
    break;
    case 4:
    SiPM_4 ++;
    break;
    case 5:
    SiPM_5 ++;
    break;
    case 6:
    SiPM_6 ++;
    break;
    case 7:
    SiPM_7 ++;
    break;
    case 8:
    SiPM_8 ++;
    break;
    case 9:
    SiPM_9 ++;
    break;
    case 10:
    SiPM_10 ++;
    break;
    case 11:
    SiPM_11 ++;
    break;
    case 12:
    SiPM_12 ++;
    break;
    case 13:
    SiPM_13 ++;
    break;
    case 14:
    SiPM_14 ++;
    break;
    case 15:
    SiPM_15 ++;
    break;
    default:
    break;
  }
}