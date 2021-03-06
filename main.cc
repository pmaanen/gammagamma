

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
//#include "G4UIterminal.hh"

#include "DetectorConstruction.hh"
#include "UserActionInitialization.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "FTFP_BERT.hh"
#include "G4RadioactiveDecayPhysics.hh"

#define MAIN
#include "global.hh"
namespace CLHEP {}
using namespace CLHEP; 


int main(int argc,char** argv) {

	namespace po = boost::program_options;
	po::options_description description("Usage");
	description.add_options()
												("help,h", "Display this help message")
												("general.config_file,c", po::value<std::string>(), "config file")
												("general.num_threads,n", po::value<int>()->default_value(1), "number of threads.")
												("general.macro_file,m", po::value<std::string>()->default_value("scripts/vis_T0.mac"), "macro file")
												("general.batch_mode,b", po::bool_switch()->default_value(false), "batch mode")
												("detector.geometry,g", po::value<std::string>()->default_value(""), "geometry file")
												("general.random_seed,r", po::value<int>(), "Seed of random generator")
												("generator.beam_particle,p", po::value<int>()->default_value(0), "PDG id of beam")
												("generator.target_particle,t", po::value<int>()->default_value(0), "PDG id of target")
												("generator.beam_energy,e", po::value<double>()->default_value(1),"energy of beam in MeV");

	std::ifstream cfg;
	po::store(po::parse_command_line(argc, argv, description), vm);
	notify(vm);
	if(vm.count("general.config_file")){
		cfg.open(vm["general.config_file"].as<std::string>().c_str(),std::ifstream::in);
		po::store(po::parse_config_file(cfg, description), vm);
		notify(vm);
	}
	// choose the Random engine
	RanecuEngine* theEngine=new RanecuEngine;
	if(!vm.count("general.random_seed")){
		theEngine->setSeed(time(0));
	}
	else{
		theEngine->setSeed(vm["general.random_seed"].as<int>());
	}
	HepRandom::setTheEngine(theEngine);
#ifdef G4MULTITHREADED
	G4MTRunManager* runManager = new G4MTRunManager;
	runManager->SetNumberOfThreads(vm["general.num_threads"].as<int>());
#else
	G4RunManager* runManager = new G4RunManager;
#endif

	// set mandatory initialization classes
	DetectorConstruction* detector = new DetectorConstruction;  
	runManager->SetUserInitialization(detector);

	// set physics list
	G4VModularPhysicsList* physicsList=new FTFP_BERT();
	physicsList->RegisterPhysics(new G4RadioactiveDecayPhysics());
	physicsList->SetVerboseLevel(0);
	runManager->SetUserInitialization(physicsList);

	//User action initialization
	runManager->SetUserInitialization(new UserActionInitialization);

	// Initialize G4 kernel
	//
	runManager->Initialize();

#ifdef G4VIS_USE
	// Initialize visualization
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();
#endif

	// Get the pointer to the User Interface manager
	//
	G4UImanager* UImanager = G4UImanager::GetUIpointer();



	if (!vm["general.batch_mode"].as<bool>())   // Define UI session for interactive mode
	{

#ifdef G4UI_USE
		G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
		std::stringstream o;
		o<<"/control/execute "<<vm["general.macro_file"].as<std::string>().c_str();
		UImanager->ApplyCommand(o.str().c_str());
#endif
		ui->SessionStart();
		delete ui;
#endif
	}
	else           // Batch mode
	{
		std::stringstream o;
		o<<"/control/execute "<<vm["general.macro_file"].as<std::string>().c_str();
		UImanager->ApplyCommand(o.str().c_str());
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	//                 owned and deleted by the run manager, so they should not
	//                 be deleted in the main() program !

#ifdef G4VIS_USE
	delete visManager;
#endif
	delete runManager;

	return 0;
}

