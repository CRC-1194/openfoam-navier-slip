// Author:
//  Dirk Gruending

#ifndef navierSlipFvPatchField_H
#define navierSlipFvPatchField_H

#include "fvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                      Class navierSlipFvPatch Declaration
\*---------------------------------------------------------------------------*/

class navierSlipFvPatchField
:
    public fvPatchField<vector>
{
        scalarField slipLength_;
        vectorField wallVelocity_;
public:

    TypeName("navierSlip");

        navierSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&
        );

        navierSlipFvPatchField
        (
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const dictionary&
        );

        navierSlipFvPatchField
        (
            const navierSlipFvPatchField&,
            const fvPatch&,
            const DimensionedField<vector, volMesh>&,
            const fvPatchFieldMapper&
        );

        navierSlipFvPatchField
        (
            const navierSlipFvPatchField&
        );

        virtual tmp<fvPatchField<vector> > clone() const
        {
            return tmp<fvPatchField<vector> >
            (
                new navierSlipFvPatchField(*this)
            );
        }

        navierSlipFvPatchField
        (
            const navierSlipFvPatchField&,
            const DimensionedField<vector, volMesh>&
        );

        virtual tmp<fvPatchField<vector> > clone
        (
            const DimensionedField<vector, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<vector> >
            (
                new navierSlipFvPatchField(*this, iF)
            );
        }
            virtual void autoMap
            (
                const fvPatchFieldMapper&
            );

            virtual void rmap
            (
                const fvPatchField<vector>&,
                const labelList&
            );

            virtual bool fixesValue() const
            {
                return true;
            }

            const scalarField& slipLength() const
            {
                return slipLength_;
            }

            void setSlipLength(tmp<scalarField> slipLength);

            const vectorField& wallVelocity() const
            {
                return wallVelocity_;
            }

            virtual tmp<Field<vector> > snGrad() const;

            virtual void evaluate
            (
                const UPstream::commsTypes commsType = UPstream::commsTypes::blocking
            );

            virtual tmp<Field<vector> > valueInternalCoeffs
            (
                const tmp<scalarField>&
            ) const;

            virtual tmp<Field<vector> > valueBoundaryCoeffs
            (
                const tmp<scalarField>&
            ) const;

            virtual tmp<Field<vector> > gradientInternalCoeffs() const;

            virtual tmp<Field<vector> > gradientBoundaryCoeffs() const;

        virtual void write(Ostream&) const;
        virtual void operator=(const UList<vector>&) {}
        virtual void operator=(const fvPatchField<vector>&) {}
        virtual void operator+=(const fvPatchField<vector>&) {}
        virtual void operator-=(const fvPatchField<vector>&) {}
        virtual void operator*=(const fvPatchField<scalar>&) {}
        virtual void operator/=(const fvPatchField<scalar>&) {}
        virtual void operator+=(const Field<vector>&) {}
        virtual void operator-=(const Field<vector>&) {}
        virtual void operator*=(const Field<scalar>&) {}
        virtual void operator/=(const Field<scalar>&) {}
        virtual void operator=(const vector&) {}
        virtual void operator+=(const vector&) {}
        virtual void operator-=(const vector&) {}
        virtual void operator*=(const scalar) {}
        virtual void operator/=(const scalar) {}
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //


#endif

// ************************************************************************* //
