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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
<<<<<<< HEAD


=======
#include <map>
#include <string>
#include "G4ThreeVector.hh"
class TrackerSensitiveDetector;
class CaloSensitiveDetector;
class G4Box;
class G4LogicalVolume;
class G4VPhysicalVolume;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();

	virtual ~DetectorConstruction();
	virtual G4VPhysicalVolume* Construct();
	virtual void ConstructSDandField();


	void SetPMTAngle(G4double xAngle){fPMTAngle=xAngle;};
	void SetOpeningAngle(G4double xAngle){fOpeningAngle=xAngle;};


	G4double GetSource_SizeXY(){return source_sizeXY;};
	G4double GetSource_SizeZ(){return source_sizeZ;};

	void UpdateGeometry();

private:

	
	G4VPhysicalVolume* physiWorld;
	DetectorMessenger* dcMessenger;

	G4double fPMTAngle;
	G4double fOpeningAngle;
	G4double source_sizeXY;
	G4double source_sizeZ;
	CaloSensitiveDetector* myDetector;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

