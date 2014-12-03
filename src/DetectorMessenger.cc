/*
 * DetectorMessenger.cc
 *
 *  Created on: Jul 15, 2013
 *      Author: maanen
 */

#include "DetectorMessenger.hh"
#include "G4UIdirectory.hh"
#include "DetectorConstruction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detc):dc(detc) {
	dcDir=new G4UIdirectory("/GammaGamma/DetectorConstruction/");
	dcDir->SetGuidance("Detector Construction Control");

	OpeningAngleCmd=new G4UIcmdWithADoubleAndUnit("/GammaGamma/DetectorConstruction/SetOpeningAngle",this);
	OpeningAngleCmd->SetGuidance("");
	OpeningAngleCmd->AvailableForStates(G4State_Idle);

	PMTAngleCmd=new G4UIcmdWithADoubleAndUnit("/GammaGamma/DetectorConstruction/SetPMTAngle",this);
	PMTAngleCmd->SetGuidance("");
	PMTAngleCmd->AvailableForStates(G4State_Idle);


	updateCmd = new G4UIcmdWithoutParameter("/GammaGamma/DetectorConstruction/update",this);
	updateCmd->SetGuidance("Update geometry.");
	updateCmd->SetGuidance("This command MUST be applied before \"beamOn\" ");
	updateCmd->SetGuidance("if you changed geometrical value(s).");
	updateCmd->AvailableForStates(G4State_Idle);


}
void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue){
	if(command==OpeningAngleCmd){
		dc->SetOpeningAngle(OpeningAngleCmd->GetNewDoubleValue(newValue));
	}
	else if (command==PMTAngleCmd){
		dc->SetPMTAngle(PMTAngleCmd->GetNewDoubleValue(newValue));
	}
	else if (command == updateCmd){
		dc->UpdateGeometry();
	}
}

DetectorMessenger::~DetectorMessenger() {
	delete dcDir;
	delete OpeningAngleCmd;
	delete PMTAngleCmd;
	delete updateCmd;
}
