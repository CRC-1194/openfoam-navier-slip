import numpy as np
import scipy.special as spe

# In the paper Pao et al, 1972
# R : dimensional radius (in meter)
# s : distance between the top and bottom disks
# b : radius of the disk
# r : dimensionless radius r=R/b
# Omega : angular speed of the rotating disk
# nu : kinematic viscosity of the liquid
# Re : Reynolds number Re=Omega b^2/ nu
# Z : dimensional height (im meter), measured from the rotating disk
# z : dimensionless height z=Z/b
# beta : length scale ratios beta=s/b

def v_pao1972_lim_unscaled(R, Z, b, s, Omega):
    '''
    Analytic function for the limit Re < 10, beta << 1

    Parameters:
    R : radius coordinate in meter
    Z : height coordinate in meter, measured from the moving surface
    b : radius of the wedge
    s : height of the wedge
    Omega : angular velocity of the rotating disk
    '''
    return Omega * b * v_pao1972_lim_scaled(R/b, Z/s)

def v_pao1972_lim_scaled(r, z_bar):
    '''
    Scaled analytic function for the limit Re < 10, beta << 1

    Parameters:
    r : dimensionless radius coordinate, r=R/b
    z_b : dimensionless height coordinate, z_bar=Z/s measured from the moving
    surface
    '''
    return (1.0 - z_bar)*r

def v_pao1972_unscaled(R, Z, b, s, Omega, N=400):
    '''
    Analytic solution for the limit Re < 10
    Parameters:

    R : radius coordinate in meter
    Z : height coordinate in meter, measured from the moving surface
    b : radius of the wedge
    s : height of the wedge
    Omega : angular velocity
    N : number of series terms
    '''

    return Omega * b * v_pao1972_scaled(R/b, Z/b, s/b, N) 

def v_pao1972_scaled(r, z, beta, N):
    '''Analytic flow velocity field between two rotating plates

    Parameters:

    r : dimensionless radius coordinate, r=R/b
    z : dimensionless height coordinate, z=Z/b measured from the moving surface
    beta : dimensionless length ratio, s/b
    N : number of series terms
    '''

    #the positive zeros of the bessel function 1st degree
    zero_J1 = spe.jn_zeros(1,400)    
    v_phi_r = []
    
    # compute velocity at each point individually
    for rj in r:
        v_phi_rj = 0
        # sum up all summands at point rj
        for alpha_n in zero_J1:
            denom = alpha_n * np.sinh(alpha_n * beta) * spe.jv(0, alpha_n)
            fraction =  2.0 / denom
            factor = spe.jv(1, alpha_n * rj) * np.sinh( alpha_n * (z - beta) )

            summand =  fraction * factor
            v_phi_rj += summand

        v_phi_r.append(v_phi_rj)

    return np.array(v_phi_r)
