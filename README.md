# Description 

This is the implementation of the Navier-Slip boundary condition in OpenFOAM, information can be found in [Dirk Gründing's Ph.D. thesis, chapter 6.1.2, page 85](https://tuprints.ulb.tu-darmstadt.de/11442/), and chapters 6.3.2 and 6.3.3 are of interest for the capillary rise case.

* Dirk Gründing (former association: MMA, TU Darmstadt, SFB 1194/B02) 

    * original developer

* Tomislav Maric (MMA, TU Darmstadt, CRC 1194/Z-INF, maric@mma.tu-darmstadt.de) 

    * repository maintenance, documentation, testing

* Suraj Raju (MMA, TU Darmstadt, CRC 1194/B02, raju@mma.tu-darmstadt.de)

    * repository maintenance, documentation, testing

**Funded by the German Research Foundation (DFG - Project-ID 265191195 - SFB 1194** 

# Installation 

## Dependencies 

The BC should work with other OpenFOAM versions/forks, it was tested with

* OpenFOAM-v2112 
* g++ (GCC) 11.2.0

## Building 

Make sure OpenFOAM's environment is set, then run  

```
 navierSlipFvPatchField ?> wmake  
```
this compiles and installs the binary in `$FOAM_USER_LIBBIN`. 

# Usage

Inside an OpenFOAM simulation case, in `system/cotnrolDict` add 

```
libs ("libnavierSlipBc.so")
```

Edit a `0/U` velocity field boundary condition and use  

```
    nameOfYourPatch 
    {
        type            navierSlip;
        value           uniform (0 0 0);
        slipLength      uniform 1e-03;
        wallVelocity    uniform (0 0 0);
    }
```

Run your OpenFOAM solver. 
