//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"
#include "G4GenericMessenger.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4SubtractionSolid.hh"
#include "DetectorMessenger.hh"
#include "CaloSensitiveDetector.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
// Visualization attributes
static G4Color
red(1.0,0.0,0.0),
yellow(1.0,1.0,0.0),
green(0.0,1.0,0.0),
blue(0.0,0.0,1.0),
brown(0.4,0.4,0.1),
white(1.0,1.0,1.0),
metal(204/255., 204/255, 255/255.),
lblue(153/255., 153/255., 255/255.),
lgreen(153/255. ,255/255. ,153/255.);

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
DetectorConstruction::DetectorConstruction()
:physiWorld(0),G4VUserDetectorConstruction()
{
	fPMTAngle=180*deg;
	logicScintillator=0;
	fOpeningAngle=2.5*deg;
	source_sizeXY=5*mm;
	source_sizeZ=5*mm;
	myDetector=0;

	DefineCommands();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

void DetectorConstruction::UpdateGeometry()
{
	G4cout <<"Updating Geometry...";
	G4RunManager::GetRunManager()->ReinitializeGeometry();
	G4cout<<"done!"<<G4endl;
	return;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();
	// Option to switch on/off checking of volumes overlaps
	//
	G4bool checkOverlaps = true;



	//
	// World
	//
	G4double world_sizeXY = 50*cm;
	G4double world_sizeZ  = 30*cm;
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
	G4Box* solidWorld =
			new G4Box("World",                       //its name
					0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);     //its size

	G4LogicalVolume* logicWorld =
			new G4LogicalVolume(solidWorld,          //its solid
					world_mat,           //its material
					"World");            //its name

	logicWorld->SetVisAttributes(new G4VisAttributes(G4VisAttributes::Invisible));
	physiWorld =
			new G4PVPlacement(0,                     //no rotation
					G4ThreeVector(),       //at (0,0,0)
					logicWorld,            //its logical volume
					"World",               //its name
					0,                     //its mother  volume
					false,                 //no boolean operation
					0,                     //copy number
					checkOverlaps);        //overlaps checking


	//Source and source mount
	G4Tubs* sourcemount_solid=new G4Tubs("SourceMountSolid",0*mm,6*mm,10*mm, 0*deg,360*deg);
	G4LogicalVolume* sourcemount_logical=new G4LogicalVolume(sourcemount_solid,nist->FindOrBuildMaterial("G4_Al"),"SourceMountLogical");
	new G4PVPlacement(0,G4ThreeVector(0,0,-10*mm),sourcemount_logical,"Source Mount",logicWorld,false,0,checkOverlaps);

	G4Box* sourcehull_solid=new G4Box("SourceHull",source_sizeXY/2+.5*mm,source_sizeXY/2+.5*mm,source_sizeZ/2+.5*mm);
	G4LogicalVolume* sourcehull_logical=new G4LogicalVolume(sourcehull_solid,nist->FindOrBuildMaterial("G4_Al"),"Source");
	new G4PVPlacement(0,G4ThreeVector(0,0,source_sizeZ/2+0.5*mm),sourcehull_logical,"Source",logicWorld,false,0,checkOverlaps);


	G4Box* source_solid=new G4Box("Source",source_sizeXY/2,source_sizeXY/2,source_sizeZ/2);
	G4LogicalVolume* source_logical=new G4LogicalVolume(source_solid,nist->FindOrBuildMaterial("G4_Na"),"Source");
	new G4PVPlacement(0,G4ThreeVector(0,0,0),source_logical,"Source",sourcehull_logical,false,0,checkOverlaps);


	// Collimator parameters
	//
	G4Material* coll_mat = nist->FindOrBuildMaterial("G4_Pb");
	G4double coll_sizeX=50*mm;
	G4double coll_sizeY=40*mm;
	G4double coll_sizeZ=80*mm;

	G4RotationMatrix* subtrRot=new G4RotationMatrix();
	G4Box* coll_box=new G4Box("coll",coll_sizeX/2,coll_sizeY/2,coll_sizeZ/2);
	G4Box* coll_box2=new G4Box("coll2",coll_sizeX/2,coll_sizeY/2,coll_sizeZ/2+1*mm);
	subtrRot->rotateZ(-45*deg);
	G4SubtractionSolid* coll_solid=new G4SubtractionSolid("Collimator",coll_box,coll_box2,subtrRot,G4ThreeVector(-coll_sizeX/2,coll_sizeY/2,0));
	G4LogicalVolume* coll_logical=new G4LogicalVolume(coll_solid,coll_mat,"Collimator");

	G4RotationMatrix* rotation1=new G4RotationMatrix();
	G4RotationMatrix* rotation2=new G4RotationMatrix();

	rotation1->rotateZ(-fOpeningAngle);
	rotation2->rotateZ(fOpeningAngle);
	rotation2->rotateX(180*deg);
	G4RotationMatrix* rotation3=new G4RotationMatrix(*rotation1);
	G4RotationMatrix* rotation4=new G4RotationMatrix(*rotation2);
	rotation3->rotateZ(-fPMTAngle);//+180
	rotation4->rotateZ(fPMTAngle);
	G4ThreeVector placement1(coll_sizeX/2+16*mm,coll_sizeY/2,0);
	G4ThreeVector placement2=placement1;
	placement2.rotateX(180*deg);



	new G4PVPlacement(rotation1,placement1.rotateZ(fOpeningAngle),coll_logical,"Collimator",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(rotation2,placement2.rotateZ(-fOpeningAngle),coll_logical,"Collimator",logicWorld,false,1,checkOverlaps);
	new G4PVPlacement(rotation3,placement1.rotateZ(+fPMTAngle),coll_logical,"Collimator",logicWorld,false,2,checkOverlaps);
	new G4PVPlacement(rotation4,placement2.rotateZ(+fPMTAngle),coll_logical,"Collimator",logicWorld,false,3,checkOverlaps);

	G4RotationMatrix* scint_rotation1=new G4RotationMatrix();
	G4RotationMatrix* scint_rotation2=new G4RotationMatrix();
	scint_rotation1->rotateY(90*deg);
	scint_rotation2->rotateZ(-fPMTAngle);
	scint_rotation2->rotateY(90*deg);
	G4double scint_radius=40*mm;
	G4double scint_length=100*mm;

	G4double shield_thick=1*mm;
	G4Tubs* shield_solid=new G4Tubs("Scintillator",0*mm,scint_radius,scint_length/2,0*deg,360*deg);
	G4LogicalVolume* shield_logical=new G4LogicalVolume(shield_solid,nist->FindOrBuildMaterial("G4_Al"),"ScintillatorShielding");

	new G4PVPlacement(scint_rotation1,G4ThreeVector(scint_length/2+75*mm,0,0),shield_logical,"ScintillatorShielding",logicWorld,false,0,checkOverlaps);
	new G4PVPlacement(scint_rotation2,G4ThreeVector(scint_length/2+75*mm,0,0).rotateZ(fPMTAngle),shield_logical,"ScintillatorShielding",logicWorld,false,1,checkOverlaps);


	G4Tubs* scint_solid=new G4Tubs("Scintillator",0*mm,scint_radius-shield_thick,scint_length/2-shield_thick,0*deg,360*deg);
	logicScintillator=new G4LogicalVolume(scint_solid,nist->FindOrBuildMaterial("G4_POTASSIUM_IODIDE"),"Scintillator");
	new G4PVPlacement(0,G4ThreeVector(0,0,0),logicScintillator,"Scintillator",shield_logical,false,1,checkOverlaps);

	G4VisAttributes* ShieldVisAttr=new G4VisAttributes(lblue);
	shield_logical->SetVisAttributes(ShieldVisAttr);

	G4VisAttributes* ScintVisAttr=new G4VisAttributes(lgreen);
	logicScintillator->SetVisAttributes(ScintVisAttr);

	G4VisAttributes* SourceVisAttr=new G4VisAttributes(red);
	source_logical->SetVisAttributes(SourceVisAttr);



	G4VisAttributes* CollVisAttr=new G4VisAttributes(metal);
	coll_logical->SetVisAttributes(CollVisAttr);

	return physiWorld;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::ConstructSDandField() {
	//------------------------------------------------
	// Sensitive detectors
	//------------------------------------------------
	myDetector = new CaloSensitiveDetector("PMT");
	SetSensitiveDetector(logicScintillator,myDetector);


}

void DetectorConstruction::DefineCommands() {

	// Define /GammaGamma/detector command directory using generic messenger class
	fMessenger = new G4GenericMessenger(this,
			"/GammaGamma/detector/",
			"Detector control");

	G4GenericMessenger::Command& PMTAngleCmd
	= fMessenger->DeclareMethodWithUnit("PMTAngle","deg",
			&DetectorConstruction::SetPMTAngle,
			"Set rotation angle of the second PMT.");
	PMTAngleCmd.SetParameterName("alpha", true);
	PMTAngleCmd.SetRange("alpha>=0. && alpha<=180.");
	PMTAngleCmd.SetDefaultValue("180.");

	G4GenericMessenger::Command& OpeningAngleCmd
	= fMessenger->DeclareMethodWithUnit("CollimatorAngle","deg",
			&DetectorConstruction::SetOpeningAngle,
			"Set opening angle of collimators");
	OpeningAngleCmd.SetParameterName("beta", true);
	OpeningAngleCmd.SetRange("beta>=0. && beta<=90.");
	OpeningAngleCmd.SetDefaultValue("5.");

	G4GenericMessenger::Command& updateCmd
	= fMessenger->DeclareMethod("update",
			&DetectorConstruction::UpdateGeometry,
			"Update geometry");
	updateCmd;

	return;
}
