// ********************************************************************
// *                                                                  *
// *                          EDM Polarimetry                         *
// *                                                                  *
// * Detector physics                                                 *
// * RWTH Aachen 24.10.2012                                           *
// * Fabian Hinder                                                    *
// ********************************************************************
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef polarimeterStudiesDetectorConstruction_h
#define polarimeterStudiesDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include <map>
#include <string>
#include "G4ThreeVector.hh"
class TrackerSensitiveDetector;
class CaloSensitiveDetector;
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4MaterialPropertiesTable;
class DetectorMessenger;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();

	virtual ~DetectorConstruction();
	virtual G4VPhysicalVolume* Construct();
	void ConstructSDandField();


	void SetPMTAngle(G4double xAngle){fPMTAngle=xAngle;};
	void SetOpeningAngle(G4double xAngle){fOpeningAngle=xAngle;};


	G4double GetSource_SizeXY(){return source_sizeXY;};
	G4double GetSource_SizeZ(){return source_sizeZ;};

	void UpdateGeometry();

private:

	void DefineScorers();

	G4VPhysicalVolume* physiWorld;
	G4LogicalVolume* logicScintillator;
	DetectorMessenger* dcMessenger;

	G4double fPMTAngle;
	G4double fOpeningAngle;
	G4double source_sizeXY;
	G4double source_sizeZ;
	CaloSensitiveDetector* myDetector;




};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
