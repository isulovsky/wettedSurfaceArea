// wettedSurfaceArea.H
#ifndef wettedSurfaceArea_H
#define wettedSurfaceArea_H

#include "fvCFD.H"
#include "functionObject.H"
#include "dictionary.H"
#include "volFields.H"
#include "OFstream.H"

namespace Foam
{
namespace functionObjects
{

class wettedSurfaceArea
:
    public functionObject
{
    // Private data
    
    //- Reference to mesh
    const fvMesh& mesh_;
    
    //- Name of the phase fraction field
    word alphaName_;
    
    //- Name of the patch to analyze
    word patchName_;
    
    //- Threshold for wetted surface
    scalar threshold_;

    //- Output file pointer
    autoPtr<OFstream> outputFilePtr_;

public:

    //- Runtime type information
    TypeName("wettedSurfaceArea");

    // Constructors
    wettedSurfaceArea
    (
        const word& name,
        const Time& runTime,
        const dictionary& dict
    );

    //- Destructor
    virtual ~wettedSurfaceArea();

    // Member Functions
    virtual bool start();
    
    virtual bool read(const dictionary& dict);

    virtual bool execute();

    virtual bool write();
};

} // End namespace functionObjects
} // End namespace Foam

#endif