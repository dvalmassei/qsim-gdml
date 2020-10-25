#include "qsimDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "qsimDetector.hh"
#include "qsimScintDetector.hh"
#include "G4SDManager.hh"
#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Element.hh"
#include "G4ElementTable.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4GenericTrap.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"
#include "G4PVPlacement.hh"
#include "G4OpticalSurface.hh"
#include "G4SubtractionSolid.hh"
#include "G4VisAttributes.hh"
#include "G4UserLimits.hh"

#include "G4GDMLParser.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void qsimDetectorConstruction::DetModeSet(G4int detMode = 1) {
  
  fDetMode = detMode;
  // 0 is PREX-I design
  // 1 is PREX-II prototype (so-called "design 3")
  // 2 SAM
  // 3 showerMax
  // 4 tandem mount
  
}

void qsimDetectorConstruction::QModeSet(G4int qMode = 0) {
  
  fQMode = qMode;
  // 0 is PREX-I design
  // 0 is PREX-II prototype (so-called "design 3")
  // 1 SAM
  // 2 showerMax
  
}

void qsimDetectorConstruction::StandModeSet(G4int standMode = 0) {
  
  fStandMode = standMode;
  // 1 cosmic setup (detector, lead, scintillators)
  // 0 beam setup (detector only)
  
}


qsimDetectorConstruction::qsimDetectorConstruction(const G4GDMLParser& parser)
  : G4VUserDetectorConstruction(),
    fParser(parser)
{
  
  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  qsimDetector* cathSD = new qsimDetector("PhotoCathode",1);
  SDman->AddNewDetector(cathSD);

  ///////////////////////////////////////////////////////////////////////
  //
  // Example how to retrieve Auxiliary Information for sensitive detector
  //
  const G4GDMLAuxMapType* auxmap = fParser.GetAuxMap();
  G4cout << "Found " << auxmap->size()
	 << " volume(s) with auxiliary information."
	 << G4endl << G4endl;
  for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
      iter!=auxmap->end(); iter++)
    {
      G4cout << "Volume " << ((*iter).first)->GetName()
	     << " has the following list of auxiliary information: "
	     << G4endl << G4endl;
      for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
	   vit!=(*iter).second.end(); vit++)
	{
	  G4cout << "--> Type: " << (*vit).type
		 << " Value: " << (*vit).value << G4endl;
	}
    }
  G4cout << G4endl;

  // The same as above, but now we are looking for
  // sensitive detectors setting them for the volumes
  
  for(G4GDMLAuxMapType::const_iterator iter=auxmap->begin();
      iter!=auxmap->end(); iter++)
    {
      G4cout << "Volume " << ((*iter).first)->GetName()
	     << " has the following list of auxiliary information: "
	     << G4endl << G4endl;
      for (G4GDMLAuxListType::const_iterator vit=(*iter).second.begin();
	   vit!=(*iter).second.end();vit++)
	{
	  if ((*vit).type=="SensDet")
	    {
	      G4cout << "Attaching sensitive detector " << (*vit).value
		     << " to volume " << ((*iter).first)->GetName()
		     <<  G4endl << G4endl;
	      
	      G4VSensitiveDetector* mydet =
		SDman->FindSensitiveDetector((*vit).value);
	      if(mydet)
		{
		  G4LogicalVolume* myvol = (*iter).first;
		  myvol->SetSensitiveDetector(mydet);
		}
	      else
		{
		  G4cout << (*vit).value << " detector not found" << G4endl;
		}
	    }
	}
    }
  
}
                                                                               

qsimDetectorConstruction::~qsimDetectorConstruction(){;}                        
                                                                                                                                                               

G4VPhysicalVolume* qsimDetectorConstruction::Construct() {
  return fParser.GetWorldVolume();
}

