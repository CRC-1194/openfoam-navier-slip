/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | foam-extend: Open Source CFD
   \\    /   O peration     |
    \\  /    A nd           | For copyright notice see file Copyright
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of foam-extend.

    foam-extend is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your
    option) any later version.

    foam-extend is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with foam-extend.  If not, see <http://www.gnu.org/licenses/>.

Original Author: 
    Dirk Gründing

Description:
    Utility to extract the surface points (free surface or interface) 
    as a post processing utility.

\*---------------------------------------------------------------------------*/

#include "OFstream.H"
#include "Ostream.H"
#include "fvCFD.H"
#include "fvMesh.H"
#include "Time.H"
#include "interfaceTrackingFvMesh.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
 
int main(int argc, char *argv[])
{
#   include "setRootCase.H"
#   include "createTime.H"
#   include "createMesh.H"

    const label fsPatchIndex = mesh.boundaryMesh().findPatchID("freeSurface");

    scalar curApexHeight(0); // current apexHeight
    //scalar preApexHeight(0); // previous apexHeight

    //scalar curTime(0); // current time
    //scalar preTime(runTime.time().value()); // previous time

    const std::string fileName("capRiseApexHeight.csv");
    std::ofstream heightFile(fileName, std::ios::out);
    heightFile << "t" << tab << "height" << std::endl;

    // iterate through all available time folders
    instantList timeDirs = timeSelector::select0(runTime, args);
    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);

        // data of the mesh needs to be read from files
        mesh.readUpdate();

        const polyBoundaryMesh& bMesh(mesh.boundaryMesh());
        const polyPatch& patch(bMesh[fsPatchIndex]);
        const Field<point>& patchPoints = patch.localPoints();

        boundBox bBox(patchPoints);
        curApexHeight = bBox.min().y();

        // curTime = runTime.timeOutputValue();

        // const scalar dt = curTime - preTime;
        // const scalar dotApexHeight = (curApexHeight-preApexHeight)/dt;

        // write to file
        // preApexHeight = curApexHeight;

        // preTime = curTime;

	heightFile << runTime.timeName() << tab
		   << curApexHeight << std::endl;
    }
    heightFile.close();
    Info << "End\n" << endl;

    return(0);
}

// ************************************************************************* //
