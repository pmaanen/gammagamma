// File:	SFEventGenerator.cc
// Date:	18-Lan-2008 G.Macharashvili Dubna

#include "SFEventGenerator.hh"
#include "Analysis.hh"
#include "TNtuple.h"
#include <TMath.h>
#include "TGenPhaseSpace.h"
#include <G4ParticleTable.hh>
#include "CLHEP/Units/SystemOfUnits.h"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4GeneralParticleSource.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Box.hh"
#include "G4IonTable.hh"
#include "global.hh"
#include "G4Geantino.hh"
using namespace CLHEP;
SFEventGenerator::SFEventGenerator(){
	_mode=GUN;
	G4int Nparticle = 1 ;
	_pGun = new G4ParticleGun(1) ;
	DefineCommands();
}

SFEventGenerator::~SFEventGenerator() {
	delete _pGun ;
}

void SFEventGenerator::generateEventFromGun(G4Event *E)
{
	_pGun->GeneratePrimaryVertex(E) ;
}

void SFEventGenerator::generateEventFromDecay(G4Event* E) {

	//Na22
	/*
	G4int Z = 11, A = 22;
	G4double ionCharge   = 0.*eplus;
	G4double excitEnergy = 0.*keV;

	G4ParticleDefinition* ion
	= G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
	fParticleGun->SetParticleDefinition(ion);
	fParticleGun->SetParticleCharge(ionCharge);
	 */
	// randomized position
	//
	// In order to avoid dependence of PrimaryGeneratorAction
	// on DetectorConstruction class we get Envelope volume
	// from G4LogicalVolumeStore.

	G4double envSizeXY = 0;
	G4double envSizeZ = 0;
	/*	
G4LogicalVolume* envLV
	= G4LogicalVolumeStore::GetInstance()->GetVolume("Source");
	G4Box* envBox = NULL;
	if ( envLV ) envBox = dynamic_cast<G4Box*>(envLV->GetSolid());
	if ( envBox ) {
		envSizeXY = envBox->GetXHalfLength()*2.;
		envSizeZ = envBox->GetZHalfLength()*2.;
	}
	else  {
		G4cerr << "Source volume of box shape not found." << G4endl;
		G4cerr << "Perhaps you have changed geometry." << G4endl;
		G4cerr << "The gun will be placed in the center." << G4endl;
	}
	*/
	G4double size = 1;
	G4double x0 = size * envSizeXY * (G4UniformRand()-0.5);
	G4double y0 = size * envSizeXY * (G4UniformRand()-0.5);
	G4double z0 = G4UniformRand() * envSizeZ+0.5*mm;


	_pGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
	_pGun->GeneratePrimaryVertex(E);

	return;
}

void SFEventGenerator::GeneratePrimaries(G4Event* E) {

	switch(_mode){
	case GUN:
		generateEventFromGun(E);
		break;
	case ION:
		generateEventFromDecay(E);
		break;
	default:
		std::stringstream o;
		o<<"Mode not recognized. Mode: "<<_mode<<G4endl;
		G4Exception("SFEventGenerator::SetMode()", "ModeError", FatalException,
				o.str().c_str());
	}
}


void SFEventGenerator::DefineCommands()
{
	fMessenger = new G4GenericMessenger(this,
				"/GammaGamma/generator/",
				"Generator control");

	G4GenericMessenger::Command& modeCmd
	= fMessenger->DeclareMethod("Mode",
			&SFEventGenerator::setMode,
			"Set mode of generator.");
	modeCmd.SetParameterName("mode", true);
//	modeCmd.SetRange("mode>=1. && mode<=2.");
	modeCmd.SetDefaultValue("1");


}


// eof
