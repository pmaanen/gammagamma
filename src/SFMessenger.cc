/*
 * SFMessenger.cc
 *
 *  Created on: Jan 23, 2014
 *      Author: maanen
 */

#include "SFMessenger.hh"
#include "SFEventGenerator.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
SFMessenger::SFMessenger(SFEventGenerator* gen):myGenerator(gen) {
	gunDir = new G4UIdirectory("/GammaGamma/Generator/");
	gunDir->SetGuidance("Generator Control");

	modeCmd= new G4UIcmdWithAnInteger("/GammaGamma/Generator/SetMode",this);
	modeCmd->SetGuidance("Set generator mode. 0: Use input file. 1: use particle gun 2:Use Internal Generator");
	modeCmd->SetParameterName("mode",false);
	modeCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

SFMessenger::~SFMessenger() {
	delete myGenerator;
}

void SFMessenger::SetNewValue(G4UIcommand *cmd, G4String g4String)
{
	if(cmd==modeCmd){
		myGenerator->setMode(((G4UIcmdWithAnInteger*)cmd)->GetNewIntValue(g4String));
	}
}


