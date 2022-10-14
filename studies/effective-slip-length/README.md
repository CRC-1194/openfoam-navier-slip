# Effective slip length 

TODO: short description

## Literature 

Kuhr, Maximilian, Tobias Corneli, and Peter F. Pelz. "Reduction of Bearing Load Capacity and Increase in Volume Flow Due to Wall Slip." (2018): 569-579.

Pao, Hsien‐Ping. "Numerical Solution of the Navier‐Stokes Equations for Flows in the Disk‐Cylinder System." The physics of fluids 15.1 (1972): 4-11.

# Authors 

Tomislav Maric - maric@mma.tu-darmstadt.de  
Mathis Fricke - fricke@mma.tu-darmstadt.de
Houssem Ben Gozlen - 
Dirk Gründing 

## Dependencies

* OpenFOAM-v2206
* ParaView (apt-get install) 
* python-pandas, python-matplotlib, python-numpy, PyFoam 

Python prerequisites can be installed with `pip install`.

## Running a single simulation case  

The template case for the parameter study is `wedgeFstPartialSlip`

```
wedgeFstPartialSlip> ./Allrun 
```

This will create the mesh by calling `blockMesh` and runn the `pimpleFoam` solver in serial, to do this manually in the case, 

```
wedgeFstPartialSlip> blockMesh # generate the mesh 
wedgeFstPartialSlip> pimpleFoam # start the solver
```

The torque is measured at the `bottom` boundary, as defined in `system/controlDict`


```
    forces
    {
        type                  forces;
        libs                  ("libforces.so");
        patches               ("bottom");
        p                     p;
        u                     U;
        CofR                  (0 0 0);
        rho                   rhoInf;
        rhoInf                828;
        writeControl          timeStep;
        writeInterval         100;

    }
```

The validation is done by sampling the axial component of the velocity field as a function of radius, along the centerline of the rheometer wedge mesh, the function object 

```
    forces
    {
        type                  forces;
        libs                  ("libforces.so");
        patches               ("bottom");
        p                     p;
        u                     U;
        CofR                  (0 0 0);
        rho                   rhoInf;
        rhoInf                828;
        writeControl          timeStep;
        writeInterval         100;

    }
```

The function objects store output in `postProcessing` folder of the simulation folder, in tab-separated CSV files with OpenFOAM-specific headers. 

## Validation

### Running the parameter variation  

We use PyFoam applicatoin pyFoamRunParameterVariation.py wrapped into `create-study.py` for the specific needs of this study, 

```
  partialSlipStudy>  ./create-study.py -s meshConvergence -p wedgeFstPartialSlip.parameter -c wedgeFstPartialSlip
```

* `-s meshConvergence`: names the study
* `-p wedgeFstPartialSlip.parameter`: defines the Cartesian product of study parameters
* `-c wedgeFstPartialSlip`: defines the OpenFOAM template case that is copied into specific parameter variation cases. 

**CAREFUL**: existing parameter setup in `wedgeFstPartialSlip.parameter` generates >300 highly-resolved simulations and should be adapted for trial runs by removing a sub-set 

### Post-processing 

Start and run the Jupyter notebook

```
    partialSlipStudy> jupyter ../effective-slip-length/notebooks/compare_analytic_solution_rotatingDiscs.ipynb
```

