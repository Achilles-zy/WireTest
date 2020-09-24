#include "PENDetectorConstruction.hh"

#include "G4Element.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4String.hh"

#include "G4OpBoundaryProcess.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4NistManager.hh"
#include "G4Cerenkov.hh"

#include "G4VPhysicalVolume.hh"
#include "G4PhysicalVolumeStore.hh"

#include "PENMaterials.hh"
#include <G4VisAttributes.hh>
#include <iostream>
#include <fstream>
#include <iterator>

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

//#include "TMath.h"

using namespace std;
PENDetectorConstruction::PENDetectorConstruction():
	G4VUserDetectorConstruction(),
    Env(nullptr),
    SiPM_0(nullptr),
    SiPM_1(nullptr),
    SiPM_2(nullptr),
    SiPM_3(nullptr),
    SiPM_4(nullptr),
    SiPM_5(nullptr),
    SiPM_6(nullptr),
    SiPM_7(nullptr),
    SiPM_8(nullptr),
    SiPM_9(nullptr),
	SiPM_10(nullptr),
	SiPM_11(nullptr)
{
	fDetectorMessenger = new PENDetectorMessenger(this);
	matconstructor = new PENMaterials;
	MPT_PEN = new G4MaterialPropertiesTable();
	AbsorptionLength = 1.5;//value at 400 nm
	fRES = 1.0;
	fLY = 3000. / MeV;
	fABSFile = "PEN_ABS";
	fType = "A1";
	fConfine = "Wire";
	pmtReflectivity = 0.50;
	G4cout << "Start Construction" << G4endl;
	DefineMat();
	fTargetMaterial = G4Material::GetMaterial("PVT_structure");
	fGlassMaterialPMT = G4Material::GetMaterial("BorosilicateGlass");
	SetABS(AbsorptionLength);
}

PENDetectorConstruction::~PENDetectorConstruction()
{
	delete fDetectorMessenger;
}

void PENDetectorConstruction::SetWireType(G4String type) {
	fType = type;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PENDetectorConstruction::SetConfine(G4String confine) {
	fConfine = confine;
	G4RunManager::GetRunManager()->ReinitializeGeometry();
}

void PENDetectorConstruction::DefineMat() 
{
	matconstructor->Construct();
	// ============================================================= Materials =============================================================
  //materialConstruction = new PenMaterials;
	matAir = G4Material::GetMaterial("Air");
	matBialkali = G4Material::GetMaterial("Bialkali");
	fGlass = G4Material::GetMaterial("BorosilicateGlass");
	fPOM = G4Material::GetMaterial("POM");
	fABS = G4Material::GetMaterial("ABS");
	matPEN = G4Material::GetMaterial("PEN");
	matSi = G4Material::GetMaterial("G4_Si");
	matCu = G4Material::GetMaterial("G4_Cu");
	matTriggerFoilEJ212 = G4Material::GetMaterial("EJ212");
	Pstyrene = G4Material::GetMaterial("Polystyrene");
	matPMMA = G4Material::GetMaterial("PMMA");
	fVacuum = G4Material::GetMaterial("Vacuum");
	matGreaseEJ550 = G4Material::GetMaterial("Grease");
	matTeflon = G4Material::GetMaterial("G4_TEFLON");
	matVikuiti = G4Material::GetMaterial("Vikuiti");
	matTitanium = G4Material::GetMaterial("titanium");
	matPolyethylene = G4Material::GetMaterial("G4_POLYETHYLENE");
	matEnGe = G4Material::GetMaterial("EnGe");
	matNaGe = G4Material::GetMaterial("NaGe");
	matLN2 = G4Material::GetMaterial("G4_lN2");
	matLAr = G4Material::GetMaterial("G4_lAr");
	matGAGG = G4Material::GetMaterial("GAGG");
	matPTFE = G4Material::GetMaterial("PTFE");

	G4cout << " materials ok " << G4endl;

	G4double wavelength;
	char filler;
	G4double varAbsorLength;
	G4double emission;
	G4double rindex;

	G4double wlPhotonEnergy[102] = { 0 };
	G4double ABSORPTION_PEN[102] = { 0 };
	G4double RINDEX_PEN[102] = { 0 };

	G4int absEntries = 0;
	ifstream ReadAbs;

	G4String absFile = "../input_files/" + fABSFile + ".csv";
	ReadAbs.open(absFile);
	if (ReadAbs.is_open())
	{
		while (!ReadAbs.eof())
		{
			ReadAbs >> wavelength >> filler >> varAbsorLength >> filler >> emission >> filler >> rindex;
			if (ReadAbs.eof()) {
				break;
			}
			wlPhotonEnergy[absEntries] = (1240. / wavelength) * eV;
			ABSORPTION_PEN[absEntries] = (varAbsorLength)*mm;
			RINDEX_PEN[absEntries] = rindex;
			absEntries++;
		}
	}

	else G4cout << "Error opening file: " << absFile << G4endl;
	ReadAbs.close();
	absEntries--;

	const G4int nEntries1 = sizeof(wlPhotonEnergy) / sizeof(G4double);
	assert(sizeof(RINDEX_PEN) == sizeof(wlPhotonEnergy));
	assert(sizeof(ABSORPTION_PEN) == sizeof(wlPhotonEnergy));
	//assert(sizeof(EMISSION_PEN) == sizeof(wlPhotonEnergy));

	MPT_PEN = new G4MaterialPropertiesTable();

	MPT_PEN->AddProperty("RINDEX", wlPhotonEnergy, RINDEX_PEN, nEntries1)->SetSpline(true);
	MPT_PEN->AddProperty("ABSLENGTH", wlPhotonEnergy, ABSORPTION_PEN, nEntries1)->SetSpline(true); // *

	// Read primary emission spectrum from PEN
	// Measurements from MPP Munich
	G4double pWavelength;
	G4String  Scint_file = "../properties/PEN_EM_SPECTRUM.dat";
	std::ifstream ReadScint2(Scint_file), ReadScintPEN;
	//count number of entries
	ReadScint2.unsetf(std::ios_base::skipws);
	//unsigned line_count = std::count(
	int line_count = std::count(
		std::istream_iterator<char>(ReadScint2),
		std::istream_iterator<char>(),
		'\n');
	std::cout << "Lines: " << line_count << "\n";
	ReadScint2.close();
	G4double PEN_EMISSION[500];
	G4double PEN_WL_ENERGY[500];
	G4int nEntriesPEN = 0;
	ReadScintPEN.open(Scint_file);
	if (ReadScintPEN.is_open()) {
		while (!ReadScintPEN.eof()) {

			ReadScintPEN >> pWavelength >> PEN_EMISSION[nEntriesPEN];
			if (ReadScintPEN.eof()) {
				break;
			}
			PEN_WL_ENERGY[nEntriesPEN] = (1240. / pWavelength) * eV;//convert wavelength to eV
		//G4cout<<nEntriesPEN<<" wl "<<PEN_WL_ENERGY[nEntriesPEN]<<" "<<PEN_EMISSION[nEntriesPEN]<<G4endl;
			nEntriesPEN++;
			if (nEntriesPEN > (line_count - 1)) { G4cout << " entries completed " << G4endl; break; }
		}
	}
	else
		G4cout << "Error opening file: " << Scint_file << G4endl;
	ReadScintPEN.close();
	G4cout << " nEntriesPEN " << nEntriesPEN << G4endl;

	MPT_PEN->AddProperty("FASTCOMPONENT", PEN_WL_ENERGY, PEN_EMISSION, line_count)->SetSpline(true);
	MPT_PEN->AddProperty("SLOWCOMPONENT", PEN_WL_ENERGY, PEN_EMISSION, line_count)->SetSpline(true);

	MPT_PEN->AddConstProperty("SCINTILLATIONYIELD", fLY / MeV); // * 2.5 * PEN = PS, 10*PEN=PS
	MPT_PEN->AddConstProperty("RESOLUTIONSCALE", fRES); // * 1, 4, 8
	MPT_PEN->AddConstProperty("FASTTIMECONSTANT", 5.198 * ns);
	MPT_PEN->AddConstProperty("SLOWTIMECONSTANT", 24.336 * ns);
	MPT_PEN->AddConstProperty("YIELDRATIO", 1.);

	matPEN->SetMaterialPropertiesTable(MPT_PEN);
	//pvt_structure->SetMaterialPropertiesTable(MPT_PEN);


	G4cout << " pen ok " << G4endl;


	G4double rindexEnergy[500] = { 0 };
	G4double scintIndex[500] = { 0 };

	G4int rindexEntries = 0;
	ifstream ReadRindex;

	G4String rindex_file = "../input_files/rindexScint.txt";
	ReadRindex.open(rindex_file);

	if (ReadRindex.is_open())
	{
		while (!ReadRindex.eof())
		{

			ReadRindex >> wavelength >> filler >> scintIndex[rindexEntries];
			if (ReadRindex.eof()) {
				break;
			}
			rindexEnergy[rindexEntries] = (1240. / wavelength) * eV;
			rindexEntries++;
		}
	}
	else G4cout << "Error opening file: " << rindex_file << G4endl;
	ReadRindex.close();
	rindexEntries--;

	G4double scintEnergy[501] = { 0 };
	G4double scintEmit[501] = { 0 };
	G4double scintEmitSlow[501] = { 0 };

	G4int scintEntries = 0;
	ifstream ReadScint;

	Scint_file = "../input_files/pTP_emission.txt";
	ReadScint.open(Scint_file);

	if (ReadScint.is_open())
	{
		while (!ReadScint.eof())
		{

			ReadScint >> wavelength >> filler >> scintEmit[scintEntries];
			if (ReadScint.eof()) {
				break;
			}
			//convert wavelength to eV:
			scintEnergy[scintEntries] = (1240. / wavelength) * eV;
			scintEmitSlow[scintEntries] = scintEmit[scintEntries];
			scintEntries++;
		}
	}
	else G4cout << "Error opening file: " << Scint_file << G4endl;
	ReadScint.close();
	scintEntries--;

	G4int absorbEntries = 0;
	G4double varAbsorbLength;
	G4double absorbEnergy[501] = { 0 };
	G4double Absorb[501] = { 0 };

	ifstream ReadAbsorb;
	G4String ReadAbsorbLength = "../input_files/PlasticBulkAbsorb2.cfg";

	ReadAbsorb.open(ReadAbsorbLength);
	if (ReadAbsorb.is_open())
	{
		while (!ReadAbsorb.eof())
		{

			ReadAbsorb >> wavelength >> filler >> varAbsorbLength;
			if (ReadAbsorb.eof()) {
				break;
			}
			absorbEnergy[absorbEntries] = (1240 / wavelength) * eV;
			Absorb[absorbEntries] = (varAbsorbLength)*m;
			absorbEntries++;
		}
	}
	else G4cout << "Error opening file: " << ReadAbsorbLength << G4endl;
	ReadAbsorb.close();
	absorbEntries--;

	G4double wlsEnergy[501] = { 0 };
	G4double wlsEmit[501] = { 0 };

	G4int wlsScintEntries = 0;
	ifstream ReadWLSScint;

	G4String wls_Scint_file = "../input_files/full_popop_emission.cfg";
	ReadWLSScint.open(wls_Scint_file);

	if (ReadWLSScint.is_open())
	{
		while (!ReadWLSScint.eof())
		{

			ReadWLSScint >> wavelength >> filler >> wlsEmit[500 - wlsScintEntries];
			if (ReadWLSScint.eof()) {
				break;
			}
			//convert wavelength to eV:
			wlsEnergy[500 - wlsScintEntries] = (1240 / wavelength) * eV;
			wlsScintEntries++;
		}
	}
	else G4cout << "Error opening file: " << wls_Scint_file << G4endl;
	ReadWLSScint.close();
	wlsScintEntries--;

	G4int wlsAbsorbEntries = 0;
	G4double wlsAbsorbEnergy[501] = { 0 };
	G4double wlsAbsorb[501] = { 0 };

	ifstream ReadWLSAbsorb;
	G4String ReadWLSAbsorbLength = "../input_files/scintAbsLen.txt";

	ReadWLSAbsorb.open(ReadWLSAbsorbLength);
	if (ReadWLSAbsorb.is_open())
	{
		while (!ReadWLSAbsorb.eof())
		{
			ReadWLSAbsorb >> wavelength >> filler >> varAbsorbLength;
			if (ReadWLSAbsorb.eof()) {
				break;
			}
			wlsAbsorbEnergy[wlsAbsorbEntries] = (1240. / wavelength) * eV;
			wlsAbsorb[wlsAbsorbEntries] = varAbsorbLength * mm;
			wlsAbsorbEntries++;
		}
	}
	else G4cout << "Error opening file: " << ReadWLSAbsorbLength << G4endl;
	ReadWLSAbsorb.close();
	wlsAbsorbEntries--;

	G4MaterialPropertiesTable* MPT_FoilEJ212 = new G4MaterialPropertiesTable();

	MPT_FoilEJ212->AddProperty("WLSABSLENGTH", wlsAbsorbEnergy, wlsAbsorb, wlsAbsorbEntries);
	MPT_FoilEJ212->AddProperty("WLSCOMPONENT", wlsEnergy, wlsEmit, wlsScintEntries);
	MPT_FoilEJ212->AddConstProperty("WLSTIMECONSTANT", 12 * ns);

	MPT_FoilEJ212->AddProperty("RINDEX", rindexEnergy, scintIndex, rindexEntries);
	MPT_FoilEJ212->AddProperty("ABSLENGTH", absorbEnergy, Absorb, absorbEntries);
	MPT_FoilEJ212->AddProperty("FASTCOMPONENT", scintEnergy, scintEmit, scintEntries);
	MPT_FoilEJ212->AddProperty("SLOWCOMPONENT", scintEnergy, scintEmitSlow, scintEntries);

	//MPT_FoilEJ212->AddConstProperty("SCINTILLATIONYIELD",11520./MeV);
	MPT_FoilEJ212->AddConstProperty("SCINTILLATIONYIELD", 10. / MeV);//set low LY to make it faster, intead use Edep for coincidences
	MPT_FoilEJ212->AddConstProperty("RESOLUTIONSCALE", 4.0);
	MPT_FoilEJ212->AddConstProperty("FASTTIMECONSTANT", 2.1 * ns);
	MPT_FoilEJ212->AddConstProperty("SLOWTIMECONSTANT", 14.2 * ns);
	MPT_FoilEJ212->AddConstProperty("YIELDRATIO", 1.0);

	matTriggerFoilEJ212->SetMaterialPropertiesTable(MPT_FoilEJ212);

	G4cout << " EJ212 ok " << G4endl;

	G4double refractive_index[] = { 1.49, 1.49, 1.49, 1.49, 1.49, 1.49 };
	G4double absPMMA[] = { 5 * m, 5 * m, 5 * m, 5 * m, 5 * m, 5 * m };
	G4double reflPMMA[] = { 0.1, 0.1, 0.1, 0.1, 0.1, 0.1 };
	G4double energyPMMA[] = { 2.18 * eV, 2.48 * eV, 2.58 * eV, 2.68 * eV, 2.78 * eV, 4.1 * eV };
	const G4int nEntries3 = sizeof(energyPMMA) / sizeof(G4double);

	G4MaterialPropertiesTable* MPT_PMMA = new G4MaterialPropertiesTable();
	MPT_PMMA->AddProperty("RINDEX", energyPMMA, refractive_index, nEntries3);
	MPT_PMMA->AddProperty("ABSLENGTH", energyPMMA, absPMMA, nEntries3)->SetSpline(true);
	MPT_PMMA->AddProperty("REFLECTIVITY", energyPMMA, reflPMMA, nEntries3)->SetSpline(true);
	matPMMA->SetMaterialPropertiesTable(MPT_PMMA);

}

void PENDetectorConstruction::SetABS(G4double value) {
	AbsorptionLength = value;
	//read file and add the value given by the user
	G4double wavelength;
	char filler;
	G4double varAbsorLength;
	G4double emission;
	G4double rindex;

	G4double wlPhotonEnergy[102] = { 0 };
	G4double ABSORPTION_PEN[102] = { 0 };

	G4int absEntries = 0;
	ifstream ReadAbs;

	G4String absFile = "../input_files/" + fABSFile + ".csv";
	ReadAbs.open(absFile);
	if (ReadAbs.is_open())
	{
		while (!ReadAbs.eof())
		{
			ReadAbs >> wavelength >> filler >> varAbsorLength >> filler >> emission >> filler >> rindex;
			if (ReadAbs.eof()) {
				break;
			}
			wlPhotonEnergy[absEntries] = (1240. / wavelength) * eV;
			ABSORPTION_PEN[absEntries] = (varAbsorLength * AbsorptionLength) * mm; //use measured value of attenuation to constrain curve and then change values multiplying the curve for a given factor
			absEntries++;
		}
	}

	else G4cout << "Error opening file: " << absFile << G4endl;
	ReadAbs.close();
	absEntries--;

	const G4int nEntries1 = sizeof(wlPhotonEnergy) / sizeof(G4double);
	assert(sizeof(ABSORPTION_PEN) == sizeof(wlPhotonEnergy));
	MPT_PEN->AddProperty("ABSLENGTH", wlPhotonEnergy, ABSORPTION_PEN, nEntries1)->SetSpline(true); // *
	//G4RunManager::GetRunManager()->PhysicsHasBeenModified();
#ifdef G4MULTITHREADED
	G4MTRunManager::GetRunManager()->PhysicsHasBeenModified();
#else
	G4RunManager::GetRunManager()->PhysicsHasBeenModified();
#endif
}



G4VPhysicalVolume* PENDetectorConstruction::Construct()
{
	G4NistManager* nist = G4NistManager::Instance();

	/*
	//Material Properties
	//PEN
	auto matPEN = new G4Material("PEN", density = 1.33 * g / cm3, nComponents = 3);
	matPEN->AddElement(elC, nAtoms = 14);
	matPEN->AddElement(elH, nAtoms = 10);
	matPEN->AddElement(elH, nAtoms = 4);

	const G4int NUMENTRIES_PEN = 11;
	G4double Scnt_PP_PEN[NUMENTRIES_PEN] = { hc / 545. * eV, hc / 520. * eV, hc / 495. * eV, hc / 480. * eV, hc / 467. * eV, hc / 450. * eV, hc / 431. * eV, hc / 425. * eV, hc / 419. * eV,hc / 412. * eV,hc / 407. * eV };
	G4double Scnt_FAST_PEN[NUMENTRIES_PEN] = { 0.1, 0.2, 0.4, 0.6, 0.8, 1, 0.8, 0.6, 0.4, 0.2, 0.1 };
	G4double Scnt_SLOW_PEN[NUMENTRIES_PEN] = { 0.1, 0.2, 0.4, 0.6, 0.8, 1, 0.8, 0.6, 0.4, 0.2, 0.1 };
	G4double Scnt_RefractiveIndex_PEN[NUMENTRIES_PEN] = { 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63, 1.63 };
	G4double Scnt_AbsorptionLength_PEN[NUMENTRIES_PEN] = { 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm, 20 * mm };
	G4double Scnt_Electron_Energy_PEN[NUMENTRIES_PEN] = { 0.0001 * keV, 0.3 * keV, 15. * keV, 22. * keV, 40. * keV, 60. * keV, 100. * keV,300. * keV, 662. * keV, 1. * MeV, 10. * GeV };
	G4double Scnt_Proton_Energy_PEN[NUMENTRIES_PEN] = { 0.0001 * keV, 10. * keV, 100. * keV, 1. * MeV, 2. * MeV,3.3 * MeV, 5.5 * MeV,8.8 * MeV, 10. * MeV, 2700. * MeV, 10. * GeV };
	G4double Scnt_Electron_Yield_PEN[NUMENTRIES_PEN] = { 0,6.254,927.64,1391.,2640.,3919.,6671.,20637.5,46000.,69486.,6.9486E+08 };
	G4double Scnt_Proton_Yield_PEN[NUMENTRIES_PEN] = { 0,137.58,1500.,15634.,31964,55033,95544,183444,222356,1.8761E+08,6.9486E+08 };
	G4double Scnt_Alpha_Yield_PEN[NUMENTRIES_PEN] = { 0,137.58,1500.,15634.,31964,55033,95544,183444,222356,1.8761E+08,6.9486E+08 };
	G4double Scnt_Ion_Yield_PEN[NUMENTRIES_PEN] = { 0,137.58,1500.,15634.,31964,55033,95544,183444,222356,1.8761E+08,6.9486E+08 };
	G4MaterialPropertiesTable* Scnt_MPT_PEN = new G4MaterialPropertiesTable();
	Scnt_MPT_PEN->AddProperty("FASTCOMPONENT", Scnt_PP_PEN, Scnt_FAST_PEN, NUMENTRIES_PEN);
	//Scnt_MPT_PEN->AddProperty("SLOWCOMPONENT", Scnt_PP_PEN, Scnt_SLOW_PEN, NUMENTRIES_PEN);
	Scnt_MPT_PEN->AddProperty("RINDEX", Scnt_PP_PEN, Scnt_RefractiveIndex_PEN, NUMENTRIES_PEN);
	Scnt_MPT_PEN->AddProperty("ABSLENGTH", Scnt_PP_PEN, Scnt_AbsorptionLength_PEN, NUMENTRIES_PEN);
	//Scnt_MPT_PEN->AddProperty("ELECTRONSCINTILLATIONYIELD", Scnt_Electron_Energy_PEN, Scnt_Electron_Yield_PEN, NUMENTRIES_PEN);
	Scnt_MPT_PEN->AddConstProperty("SCINTILLATIONYIELD", 4000. / MeV);
	Scnt_MPT_PEN->AddConstProperty("RESOLUTIONSCALE", 1.0);
	Scnt_MPT_PEN->AddConstProperty("FASTTIMECONSTANT", 27. * ns);
	//Scnt_MPT_PEN->AddConstProperty("SLOWTIMECONSTANT", 100. * ns);
	//Scnt_MPT_PEN->AddConstProperty("YIELDRATIO", 1.0);
	matPEN->SetMaterialPropertiesTable(Scnt_MPT_PEN);
	*/
	
	// Option to switch on/off checking of volumes overlaps
    //
	G4bool checkOverlaps = true;

  //Vaccum for world
  //G4Material* vacuum=new G4Material("Galactic",z=1.,a=1.01*g/mole,density=universe_mean_density,kStateGas,2.73*kelvin,3.e-18*pascal);

//------------------------------------------------------ volumes
//
  G4Material* world_mat = fVacuum;
  G4Material* env_mat = matLN2;
  G4Material* det_mat = matEnGe;

  //     
  // World&Envelope
  //
  G4double world_size = 50 * cm;
  G4double env_size = 30 * cm;
  G4Box* solidWorld = new G4Box("World", 0.5 * world_size, 0.5 * world_size, 0.5 * world_size);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, world_mat, "World");
  G4VPhysicalVolume* physWorld =
	  new G4PVPlacement(0,                     //no rotation
		  G4ThreeVector(),       //at (0,0,0)
		  logicWorld,            //its logical volume
		  "World",               //its name
		  0,                     //its mother  volume
		  false,                 //no boolean operation
		  0,                     //copy number
		  checkOverlaps);        //overlaps checking

  G4Box* solidEnv = new G4Box("Envelope",  0.5 * env_size, 0.5 * env_size, 0.5 * env_size);
  G4LogicalVolume* logicEnv = new G4LogicalVolume(solidEnv, env_mat, "Envelope");
  auto physEnv = new G4PVPlacement(0, G4ThreeVector(), logicEnv, "Envelope", logicWorld, false, 0, checkOverlaps);

  G4String WireType = fType;
  G4double WireLength = 20 * cm;

  if (WireType == "A2") {

	  //=======================================
	  // A2 rho = 2.0531 g/m, 2 g/m from ref.
	  //=======================================
	  
	  G4double ConductorRadius = 0.0762 * mm / 2;
	  G4double BraidThickness = 0.04 * mm;
	  G4double JacketRadius = 0.68 * mm / 2;
	  G4double BraidRadius = JacketRadius + BraidThickness;
	  G4double WireRadius = 1 * mm / 2;
	  G4Material* JacketMat = matPTFE;

	  G4Tubs* solidWire = new G4Tubs("solidJacket", 0 * mm, WireRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicWire = new G4LogicalVolume(solidWire, JacketMat, "logicWire");
	  G4PVPlacement* physWire = new G4PVPlacement(0, G4ThreeVector(), logicWire, "Wire", logicEnv, false, 0, checkOverlaps);

	  G4Tubs* solidConductor = new G4Tubs("solidConductor", 0 * mm, ConductorRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicConductor = new G4LogicalVolume(solidConductor, matCu, "logicConductor");
	  G4PVPlacement* physConductor = new G4PVPlacement(0, G4ThreeVector(), logicConductor, "Conductor", logicWire, false, 0, checkOverlaps);

	  G4Tubs* solidJacket = new G4Tubs("solidJacket", ConductorRadius, JacketRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicJacket = new G4LogicalVolume(solidJacket, JacketMat, "logicJacket");
	  G4PVPlacement* physJacket = new G4PVPlacement(0, G4ThreeVector(), logicJacket, "Jacket", logicWire, false, 0, checkOverlaps);

	  G4Tubs* solidBraid = new G4Tubs("solidBraid", JacketRadius, BraidRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicBraid = new G4LogicalVolume(solidBraid, matCu, "logicBraid");
	  G4PVPlacement* physBraid = new G4PVPlacement(0, G4ThreeVector(), logicBraid, "Braid", logicWire, false, 0, checkOverlaps);

	  G4Tubs* solidOuterJacket = new G4Tubs("solidOuterJacket", BraidRadius, WireRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicOuterJacket = new G4LogicalVolume(solidOuterJacket, JacketMat, "logicOuterJacket");
	  G4PVPlacement* physOuterJacket = new G4PVPlacement(0, G4ThreeVector(), logicOuterJacket, "OuterJacket", logicWire, false, 0, checkOverlaps);
  }

  else if (WireType == "A1") {

	  //=======================================
	  // A1 rho = 2.8457 g/m, 2.80 g/m from ref.
	  //=======================================

	  G4double ConductorRadius = 0.0762 * mm / 2;
	  G4double JacketRadius = 0.254 * mm / 2;
	  G4double BraidThickness = 0.02 * mm;
	  G4double BraidRadius = JacketRadius + BraidThickness;
	  G4double WireRadius = 0.5 * mm / 2;
	  G4double HarnessRadius = 1.25 * mm / 2;
	  G4double HJacketThickness = HarnessRadius - (1 + sqrt(2)) * WireRadius;
	  G4Material* JacketMat = matPTFE;

	  G4Tubs* solidHarness = new G4Tubs("solidHarness", 0 * mm, HarnessRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicHarness = new G4LogicalVolume(solidHarness, JacketMat, "logicHarness");
	  G4PVPlacement* physHarness = new G4PVPlacement(0, G4ThreeVector(), logicHarness, "Harness", logicEnv, false, 0, checkOverlaps);

	  G4Tubs* solidHJacket = new G4Tubs("solidHJacket", HarnessRadius - HJacketThickness, HarnessRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicHJacket = new G4LogicalVolume(solidHJacket, JacketMat, "logicHJacket");
	  G4PVPlacement* physHJacket = new G4PVPlacement(0, G4ThreeVector(), logicHJacket, "HarnessJacket", logicHarness, false, 0, checkOverlaps);

	  G4Tubs* solidWire = new G4Tubs("solidWire", 0 * mm, WireRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicWire = new G4LogicalVolume(solidWire, JacketMat, "logicWire");
	  G4PVPlacement* physWire_0 = new G4PVPlacement(0, G4ThreeVector(sqrt(2) * WireRadius, 0, 0), logicWire, "Wire", logicHarness, false, 0, checkOverlaps);
	  G4PVPlacement* physWire_1 = new G4PVPlacement(0, G4ThreeVector(0, sqrt(2) * WireRadius, 0), logicWire, "Wire", logicHarness, false, 1, checkOverlaps);
	  G4PVPlacement* physWire_2 = new G4PVPlacement(0, G4ThreeVector(-sqrt(2) * WireRadius, 0, 0), logicWire, "Wire", logicHarness, false, 2, checkOverlaps);
	  G4PVPlacement* physWire_3 = new G4PVPlacement(0, G4ThreeVector(0, -sqrt(2) * WireRadius, 0), logicWire, "Wire", logicHarness, false, 3, checkOverlaps);

	  G4Tubs* solidConductor = new G4Tubs("solidConductor", 0 * mm, ConductorRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicConductor = new G4LogicalVolume(solidConductor, matCu, "logicConductor");
	  G4PVPlacement* physConductor = new G4PVPlacement(0, G4ThreeVector(), logicConductor, "Conductor", logicWire, false, 0, checkOverlaps);

	  G4Tubs* solidJacket = new G4Tubs("solidJacket", ConductorRadius, JacketRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicJacket = new G4LogicalVolume(solidJacket, JacketMat, "logicJacket");
	  G4PVPlacement* physJacket = new G4PVPlacement(0, G4ThreeVector(), logicJacket, "Jacket", logicWire, false, 0, checkOverlaps);

	  G4Tubs* solidBraid = new G4Tubs("solidBraid", JacketRadius, BraidRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicBraid = new G4LogicalVolume(solidBraid, matCu, "logicBraid");
	  G4PVPlacement* physBraid = new G4PVPlacement(0, G4ThreeVector(), logicBraid, "Braid", logicWire, false, 0, checkOverlaps);

	  G4Tubs* solidOuterJacket = new G4Tubs("solidOuterJacket", BraidRadius, WireRadius, WireLength / 2, 0, twopi);
	  G4LogicalVolume* logicOuterJacket = new G4LogicalVolume(solidOuterJacket, JacketMat, "logicOuterJacket");
	  G4PVPlacement* physOuterJacket = new G4PVPlacement(0, G4ThreeVector(), logicOuterJacket, "OuterJacket", logicWire, false, 0, checkOverlaps);
  }
  else {
	  G4cout << "Type does not exist!" << G4endl;
  }

  Env = physEnv;

  return physWorld;
}

