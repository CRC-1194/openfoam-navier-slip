# Description 

This is the implementation of the Navier-Slip boundary condition in OpenFOAM, information can be found in [Dirk Gründing's Ph.D. thesis, chapter 6.1.2, page 85](https://tuprints.ulb.tu-darmstadt.de/11442/), and chapters 6.3.2 and 6.3.3 are of interest for the capillary rise case.

* Dirk Gründing (former association: MMA, TU Darmstadt, SFB 1194/B02) 

    * original developer

* Tomislav Maric (MMA, TU Darmstadt, CRC 1194/Z-INF, maric@mma.tu-darmstadt.de) 

    * development, repository maintenance, documentation, testing

* Suraj Raju (MMA, TU Darmstadt, CRC 1194/B02, raju@mma.tu-darmstadt.de)

    * repository maintenance, documentation, testing

**Funded by the German Research Foundation (DFG - Project-ID 265191195 - SFB 1194)** 

# Installation 

## Dependencies 

The BC should work with other OpenFOAM versions/forks, it was tested with

* OpenFOAM-v2206
* g++ (GCC) 11.2.0

## Building 

Make sure OpenFOAM's environment is set, then run  

```
 src/navierSlipFvPatchField ?> wmake  
```

this compiles and installs the binary in `$FOAM_USER_LIBBIN`. 

# Usage

Inside an OpenFOAM simulation case, in `system/controlDict` add 

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

# Test case: capillary rise

The capillary rise in 2D is present as a test case for the BC. The case is for oscillatory capillary rise with a parameter omega = 1, and fluid properties given in [Table 1](https://www.sciencedirect.com/science/article/pii/S0307904X20302134#tbl0001) published in [Gruending2020](https://www.sciencedirect.com/science/article/pii/S0307904X20302134).

The capillary interface shape should be first initialised and so first run,

```

cd ~/navierSlipFvPatchField/tutorials/capillaryRise2D/init/
./Allrun

```

After the simulation in init is completed, do

```
cd ..
bash Allrise

```
The above command will setup the case for rise of capillary and do the post processing. **Note:Select option to run or only setup `rise` case, input the end time of interface initialisation and number of processes selected for parallel run in the Allrise script**

## Post-processing the capillary rise apex height evolution

To get the evolution of apex height of capillary rise at each time step the `extractCapRiseApex` utility is used. To use the utility first source the OpenFOAM environment and then,

```
cd ~/navierSlipFvPatchField/extractCapRiseApex
wmake libso

```

The utility can be simply called from the command line also in the case folder for rise


```
extractCapRiseApex

```

# Effect of surface roughness on the slip boundary condition

TODO: short description.

For more information, read [studies/effective-slip-length/README.md](studies/effective-slip-length/README.md)
