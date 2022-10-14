import numpy as np

# analytic reference solution for the Couette and 
# Poiseuille flow between two plates

def poiseuille(x, mu, H, G, L=0):
    '''Analytic Poiseuille-flow velocity field between two plates

    The coordinate system is located in the center of the channel
    x - coordinate in the channel (cross section)
    mu - dynamic viscosity
    H - height of the channel
    G - pressure different in flow direction
    L - slip length (default L=0)
    '''

    return G*H/(2*mu) * ( (4*L + H)/4 - np.power(x,2)/H)
