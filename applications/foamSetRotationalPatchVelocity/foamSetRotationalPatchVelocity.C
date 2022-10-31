/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2020 2020 Tomislav Maric, MMA, TU Darmstadt
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    foamSetRotationalPatchVelocity

Description
    Set the velocity field of a patch identified by a user-provided patch name
    to a rotational velocity, where

        x = point where the rotational velocity is calculated 

        a = a point that lies on the rotation axis (user-provided option)

        w = angular velocity (user-provided option)

    The rotational velocity is then given as 

    \f$ v = w \times r  = w \times (x - a - (x - a) \cdot w_u w_u) \f$

    where 

    \f$ w_u = \frac{w}{\|w\|} \f$


    Usage example: 

    foamSetRotationalPatchVelocity -axisPoint '(0 0 0)' -angularVelocity '(0 0 1)' -patchName top

Autor
    Tomislav Maric, maric@mma.tu-darmstadt.de
    Mathematical Modeling and Analysis (Head: Prof. Dieter Bothe)
    Mathematics Department
    TU Darmstadt

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addOption
    (
        "patchName",
        "patchName",
        "Name of the boundary patch whose values will be overwritten."
    );

    argList::addOption
    (
        "axisPoint",
        "(a_x a_y a_z)",
        "A point that lies on the rotation axis."
    );

    argList::addOption
    (
        "angularVelocity",
        "(w_x w_y w_z)",
        "Angular velocity omega."
    );

    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"

    const word patchName = args.get<word>("patchName"); 
    const vector axisPoint = args.get<vector>("axisPoint"); 
    const vector omega = args.get<vector>("angularVelocity"); 
    const vector omegaUnit = omega / mag(omega);

    volVectorField U
    (
        IOobject
        (
            "U", 
            runTime.timeName(), 
            mesh, 
            IOobject::MUST_READ, 
            IOobject::AUTO_WRITE
        ),
        mesh
    );

    auto& UboundaryField = U.boundaryFieldRef(); 
    const auto& Cf = mesh.Cf(); // Face centers.
    const auto& CfBoundaryField = Cf.boundaryField(); 
    forAll(UboundaryField, patchI)
    {
        auto& UpatchField = UboundaryField[patchI]; 
        const auto& patch = UpatchField.patch(); 
        const auto& CfPatchField = CfBoundaryField[patchI]; 

        // Set rotational velocity for this patch
        if (patch.name() == patchName) 
        {
            forAll(UpatchField, faceI)
            {
                vector axisPointCf = CfPatchField[faceI] - axisPoint;
                UpatchField[faceI] = 
                    omega ^ (axisPointCf - dot(axisPointCf,omegaUnit) * omegaUnit);
            }
        }
    }

    U.write();

    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info<< nl;
    runTime.printExecutionTime(Info);

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
