// File:	SFEventGenerator.hh
// Date:	18-Lan-2008 G.Macharashvili Dubna

#ifndef SFEventGenerator_h
#define SFEventGenerator_h 1

#include <iomanip> 
#include <G4RunManager.hh>
#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <G4ParticleGun.hh>
#include <G4DecayTable.hh>
#include <G4PhaseSpaceDecayChannel.hh>
#include <G4ParticleTable.hh>
#include <G4ParticleDefinition.hh>
#include <G4PrimaryParticle.hh>
#include <G4DynamicParticle.hh>
#include <G4DecayProducts.hh>
#include <G4LorentzVector.hh>
#include <G4PrimaryVertex.hh>
#include <G4ThreeVector.hh>
#include "G4GenericMessenger.hh"
#include <G4Proton.hh>
#include <G4Deuteron.hh>
#include <G4ChargedGeantino.hh>
#include <Randomize.hh>

#include <TH1.h>
#include <TH2.h>
class G4GeneralParticleSource;
class SFEventGenerator : public G4VUserPrimaryGeneratorAction {


public:
	enum GeneratorMode {GUN=1,ION=2};


  ~SFEventGenerator();
  SFEventGenerator() ;
  void GeneratePrimaries(G4Event* E);
  void generateEventFromDecay(G4Event* E);
  void generateEventFromGun(G4Event* E);
  G4int getMode() const { return _mode;}
  void setMode(G4int mode){_mode=static_cast<GeneratorMode>(mode);}

  G4ParticleGun* getPGun() const{return _pGun;};
private:
  void DefineCommands();
  G4ParticleGun			*_pGun ;
  GeneratorMode			_mode;
  G4GenericMessenger*			fMessenger;

      

protected:

} ;

#endif

// eof
