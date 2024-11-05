// wettedSurfaceArea.C
#include "wettedSurfaceArea.H"
#include "addToRunTimeSelectionTable.H"
#include "Time.H"

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(wettedSurfaceArea, 0);
    addToRunTimeSelectionTable
    (
        functionObject,
        wettedSurfaceArea,
        dictionary
    );
}
}

Foam::functionObjects::wettedSurfaceArea::wettedSurfaceArea
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    functionObject(name),
    mesh_(refCast<const fvMesh>(runTime.lookupObject<objectRegistry>("region0"))),
    alphaName_(dict.lookupOrDefault<word>("alphaName", "alpha.water")),
    patchName_(dict.get<word>("patchName")),
    threshold_(dict.lookupOrDefault<scalar>("threshold", 0.5))
{
    Info<< "Constructed wettedSurfaceArea function object for patch: " 
        << patchName_ << endl;
    read(dict);
}

Foam::functionObjects::wettedSurfaceArea::~wettedSurfaceArea()
{}

bool Foam::functionObjects::wettedSurfaceArea::start()
{
    Info<< "Starting wettedSurfaceArea function object" << endl;
    
    // Create output file
    if (!outputFilePtr_.valid())
    {
        outputFilePtr_.reset(new OFstream
        (
            mesh_.time().path()/"wettedSurfaceArea.dat"
        ));
        
        // Write header
        outputFilePtr_()
            << "# Time" << tab 
            << "WettedArea" << tab 
            << "TotalPatchArea" << endl;
    }
    
    return true;
}

bool Foam::functionObjects::wettedSurfaceArea::read(const dictionary& dict)
{
    Info<< "Reading wettedSurfaceArea configuration" << endl;
    
    alphaName_ = dict.lookupOrDefault<word>("alphaName", "alpha.water");
    patchName_ = dict.get<word>("patchName");
    threshold_ = dict.lookupOrDefault<scalar>("threshold", 0.5);
    
    Info<< "Configuration:" << nl
        << "  - alphaName: " << alphaName_ << nl
        << "  - patchName: " << patchName_ << nl
        << "  - threshold: " << threshold_ << endl;
    
    return true;
}

bool Foam::functionObjects::wettedSurfaceArea::execute()
{
    Info<< "Executing wettedSurfaceArea for time: " 
        << mesh_.time().timeName() << endl;

    const volScalarField& alpha =
        mesh_.lookupObject<volScalarField>(alphaName_);

    const label patchID = mesh_.boundaryMesh().findPatchID(patchName_);

    if (patchID < 0)
    {
        FatalErrorIn("wettedSurfaceArea::execute()")
            << "Cannot find patch " << patchName_ << endl
            << exit(FatalError);
    }

    const fvPatchScalarField& alphaPatch = alpha.boundaryField()[patchID];
    const fvPatch& patch = mesh_.boundary()[patchID];
    
    scalar wettedArea = 0.0;
    scalar totalArea = 0.0;

    forAll(alphaPatch, faceI)
    {
        scalar faceArea = patch.magSf()[faceI];
        totalArea += faceArea;
        
        if (alphaPatch[faceI] > threshold_)
        {
            wettedArea += faceArea;
        }
    }

    reduce(wettedArea, sumOp<scalar>());
    reduce(totalArea, sumOp<scalar>());

    Info<< "Wetted surface area of patch " << patchName_
        << ":" << nl
        << "  - Wetted area: " << wettedArea << " m^2" << nl
        << "  - Total area: " << totalArea << " m^2" << nl
        << "  - Wetted percentage: " << (wettedArea/totalArea)*100.0 << "%" 
        << endl;

    if (outputFilePtr_.valid())
    {
        outputFilePtr_()
            << mesh_.time().value() << tab
            << wettedArea << tab
            << totalArea << endl;
    }

    return true;
}

bool Foam::functionObjects::wettedSurfaceArea::write()
{
    return true;
}