// Author:
//  Dirk Gruending

#include "volFields.H"
#include "addToRunTimeSelectionTable.H"
#include <exception>
#include <string>
#include "symmTransformField.H"

#include "navierSlipFvPatchField.H"

#include "JobInfo.H"
#include "OStringStream.H"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#include "IDLList.H"
#include "entry.H"
#include "dictionary.H"
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //


navierSlipFvPatchField::navierSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF
)
:
    fvPatchField<vector>(p, iF),
    slipLength_(p.size(), 0.0),
    wallVelocity_(p.size(), vector(0,0,0))
{}


navierSlipFvPatchField::navierSlipFvPatchField
(
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const dictionary& dict
)
:
    fvPatchField<vector>(p, iF, dict, true),
    slipLength_("slipLength", dict, p.size()),
    wallVelocity_("wallVelocity", dict, p.size())
{
    evaluate();
}


navierSlipFvPatchField::navierSlipFvPatchField
(
    const navierSlipFvPatchField& ptf,
    const fvPatch& p,
    const DimensionedField<vector, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    fvPatchField<vector>(ptf, p, iF, mapper),
    slipLength_(ptf.slipLength(), mapper),
    wallVelocity_(ptf.wallVelocity(), mapper)
{}


navierSlipFvPatchField::navierSlipFvPatchField
(
    const navierSlipFvPatchField& ptf
)
:
    fvPatchField<vector>(ptf),
    slipLength_(ptf.slipLength()),
    wallVelocity_(ptf.wallVelocity())
{}


navierSlipFvPatchField::navierSlipFvPatchField
(
    const navierSlipFvPatchField& ptf,
    const DimensionedField<vector, volMesh>& iF
)
:
    fvPatchField<vector>(ptf, iF),
    slipLength_(ptf.slipLength()),
    wallVelocity_(ptf.wallVelocity())
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //


void navierSlipFvPatchField::autoMap
(
    const fvPatchFieldMapper& m
)
{
    fvPatchField<vector>::autoMap(m);
    slipLength_.autoMap(m);
}


void navierSlipFvPatchField::rmap
(
    const fvPatchField<vector>& ptf,
    const labelList& addr
)
{
    fvPatchField<vector>::rmap(ptf, addr);
    const navierSlipFvPatchField& dmptf =
        refCast<const navierSlipFvPatchField >(ptf);
    slipLength_.rmap(dmptf.slipLength_, addr);
}


tmp<Field<vector> > navierSlipFvPatchField::snGrad() const
{
    return
     this->patch().deltaCoeffs()*
        transform
        (
            (slipLength()*this->patch().deltaCoeffs()/(1.0 + slipLength()*this->patch().deltaCoeffs()) - 1.0) * I
            - slipLength()*this->patch().deltaCoeffs()/(1.0 + slipLength()*this->patch().deltaCoeffs()) * sqr(this->patch().nf())
            , this->patchInternalField()
        )
        + this->patch().deltaCoeffs()/(1.0 + slipLength()*this->patch().deltaCoeffs()) 
        * transform(I-sqr(this->patch().nf()),wallVelocity());
}


void navierSlipFvPatchField::evaluate(const Pstream::commsTypes)
{
    if (!this->updated())
    {
        this->updateCoeffs();
    }
    Field<vector>::operator=
    (
        slipLength()*this->patch().deltaCoeffs()
        /(1.0 + slipLength()*this->patch().deltaCoeffs())
            * transform(I - sqr(this->patch().nf()),this->patchInternalField()()) 
        + 1.0/(1.0 + slipLength()*this->patch().deltaCoeffs()) 
        * transform(I - sqr(this->patch().nf()),wallVelocity())
    );

    fvPatchField<vector>::evaluate();
}


tmp<Field<vector> > navierSlipFvPatchField::valueInternalCoeffs
(
    const tmp<scalarField>&
) const
{
    vectorField nnVectorField(this->size(), pTraits<vector>::zero);

    forAll(nnVectorField,faceI)
    {
        nnVectorField[faceI].x() = pow(this->patch().nf()()[faceI].x(),2);
        nnVectorField[faceI].y() = pow(this->patch().nf()()[faceI].y(),2);
        nnVectorField[faceI].z() = pow(this->patch().nf()()[faceI].z(),2);
    }

    return 
    (tmp<vectorField> ( new vectorField(this->size(),pTraits<vector>::one) )-nnVectorField)
    *slipLength()*this->patch().deltaCoeffs()  /(1.0 + slipLength()*this->patch().deltaCoeffs());
}


tmp<Field<vector> > navierSlipFvPatchField::valueBoundaryCoeffs
(
    const tmp<scalarField>&
) const
{
    tensorField diagNN(this->size(),pTraits<tensor>::zero);

    forAll(diagNN,faceI)
    {
        diagNN[faceI].xx() = pow(this->patch().nf()()[faceI].x(),2);
        diagNN[faceI].yy() = pow(this->patch().nf()()[faceI].y(),2);
        diagNN[faceI].zz() = pow(this->patch().nf()()[faceI].z(),2);
    }

    return 
        - slipLength()*this->patch().deltaCoeffs()
           /(1.0 + slipLength()*this->patch().deltaCoeffs())
        *(
            transform(sqr(this->patch().nf()()) - diagNN,this-> patchInternalField()())
         )
        + 1.0/(1.0 + slipLength()*this->patch().deltaCoeffs()) * transform(I - sqr(this->patch().nf()()),wallVelocity());

} 

tmp<Field<vector> > navierSlipFvPatchField::
gradientInternalCoeffs() const
{
    vectorField rightTerm(this->size(), pTraits<vector>::one);
    rightTerm*=slipLength()*this->patch().deltaCoeffs()/(1.0 + slipLength()*this->patch().deltaCoeffs());
    
    forAll(this->patch().nf()(), faceI)
    {
        rightTerm[faceI].x() *= pow(this->patch().nf()()[faceI].x(),2);
        rightTerm[faceI].y() *= pow(this->patch().nf()()[faceI].y(),2);
        rightTerm[faceI].z() *= pow(this->patch().nf()()[faceI].z(),2);
    }

    vectorField leftTerm(this->size(), pTraits<vector>::one);
    leftTerm *= slipLength()*this->patch().deltaCoeffs();
    leftTerm /= 1.0 + slipLength()*this->patch().deltaCoeffs();
    leftTerm -= vectorField(this->size(),pTraits<vector>::one);

    return tmp<vectorField> 
    (
        new Field<vector>
        (
            this->patch().deltaCoeffs()*(leftTerm - rightTerm)
        )
    );
}


tmp<Field<vector> > navierSlipFvPatchField::
gradientBoundaryCoeffs() const
{
    tensorField diagNN(this->size(),pTraits<tensor>::zero);
    forAll(diagNN,faceI)
    {
        diagNN[faceI].xx() = pow(this->patch().nf()()[faceI].x(),2);
        diagNN[faceI].yy() = pow(this->patch().nf()()[faceI].y(),2);
        diagNN[faceI].zz() = pow(this->patch().nf()()[faceI].z(),2);
    }

    return 
        tmp<Field<vector> >
        (
            new Field<vector>
            (
                -this->patch().deltaCoeffs()
                *slipLength()*this->patch().deltaCoeffs()
                /(1.0 + slipLength()*this->patch().deltaCoeffs())
                       * transform(sqr(this->patch().nf()()) - diagNN,this-> patchInternalField()())
                + this->patch().deltaCoeffs()/(1.0 + slipLength()*this->patch().deltaCoeffs()) 
                * transform(I - sqr(this->patch().nf()()),wallVelocity())
            )
        );
}

void navierSlipFvPatchField::setSlipLength(tmp<scalarField> tslipLength)
{
    if(tslipLength().size() != this->size())
    {
        FatalErrorIn("navierSlip::setSlipLength(tmp<scalarField> tslipLength)")
            << "Provided slipLength field does not have the"
            << " same size as the patch."
            << abort(FatalError)
            << endl;
    }

    slipLength_ = tslipLength();
}

void navierSlipFvPatchField::write(Ostream& os) const
{
    fvPatchField<vector>::write(os);
    this->writeEntry("value", os);
    slipLength_.writeEntry("slipLength", os);
    wallVelocity_.writeEntry("wallVelocity", os);
}


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

makePatchTypeField(fvPatchVectorField, navierSlipFvPatchField);

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// ************************************************************************* //
