# Description 

This is the implementation of the Navier-Slip boundary condition in OpenFOAM. 

* Dirk Gründing (former association: MMA, TU Darmstadt, SFB 1194/B02) 

* Tomislav Maric (MMA, TU Darmstadt, SFB 1194/Z-INF, maric@mma.tu-darmstadt.de) 
    * repository maintenance, documentation

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
