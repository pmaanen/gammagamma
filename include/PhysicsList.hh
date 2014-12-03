/*
 * PhysicsList.hh
 *
 *  Created on: 28.11.2014
 *      Author: pmaanen
 */

#ifndef INCLUDE_PHYSICSLIST_HH_
#define INCLUDE_PHYSICSLIST_HH_

#include <G4VModularPhysicsList.hh>

class PhysicsList: public G4VModularPhysicsList {
public:
	PhysicsList();
	virtual ~PhysicsList();

	virtual void SetCuts();
};

#endif /* INCLUDE_PHYSICSLIST_HH_ */
