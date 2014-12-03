/*
 * DetectorMessenger.hh
 *
 *  Created on: Jul 15, 2013
 *      Author: maanen
 */

#ifndef DETECTORMESSENGER_HH_
#define DETECTORMESSENGER_HH_

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

class DetectorMessenger:public G4UImessenger {
public:
	DetectorMessenger(DetectorConstruction*);
	virtual ~DetectorMessenger();
	void SetNewValue(G4UIcommand*, G4String);

private:

	DetectorConstruction* dc;
	G4UIdirectory* dcDir;
	G4UIcmdWithADoubleAndUnit* OpeningAngleCmd;
	G4UIcmdWithADoubleAndUnit* PMTAngleCmd;
	G4UIcmdWithoutParameter* updateCmd;
};

#endif /* DETECTORMESSENGER_HH_ */
