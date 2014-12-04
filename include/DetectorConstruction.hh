//
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

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
class G4GenericMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
	DetectorConstruction();
	virtual ~DetectorConstruction();
	virtual G4VPhysicalVolume* Construct();

	virtual void ConstructSDandField();

	void SetPMTAngle(G4double xAngle){G4cout<<xAngle; fPMTAngle=xAngle;};
	void SetOpeningAngle(G4double xAngle){G4cout<<xAngle; fOpeningAngle=xAngle;};

	G4double GetSource_SizeXY(){return source_sizeXY;};
	G4double GetSource_SizeZ(){return source_sizeZ;};

	void UpdateGeometry();

private:

	void DefineCommands();
	G4VPhysicalVolume* physiWorld;
	G4LogicalVolume* logicScintillator;
	G4GenericMessenger* fMessenger;

	G4double fPMTAngle;
	G4double fOpeningAngle;
	G4double source_sizeXY;
	G4double source_sizeZ;
	CaloSensitiveDetector* myDetector;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

