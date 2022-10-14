#!/usr/bin/env python 

from optparse import OptionParser
import sys
from subprocess import call

usage = """A wrapper for pyFoamRunParameterVariation.py that generates the
directory structure for a parameter study. 

Meshes are not generated and preprocessing is not done. 
Used to prepare for execution on a cluster for larger cases.

Usage: lentParameterInit -c templateCase -p paramFile -s studyName"""

parser = OptionParser(usage=usage)

parser.add_option("-c", "--case", dest="casedir",
                  help="Template case directory.", 
                  metavar="CASEDIR")

parser.add_option("-p", "--parameter-file", dest="paramfile", 
                  help="PyFoam parameter file used by pyFoamRunParameterVariation.py.", 
                  metavar="PARAMFILE")

parser.add_option("-s", "--study-name", dest="studyname", 
                  help="Name of the parameter study.", 
                  metavar="STUDYNAME")

(options, args) = parser.parse_args()

if ((options.casedir == None) or  
    (options.paramfile == None) or 
    (options.studyname == None)): 
    print ("Error: case, parameter or study option not used. Use --help option for more information.") 
    sys.exit(1)

(options, args) = parser.parse_args()

call(["pyFoamRunParameterVariation.py", "--no-execute-solver", "--no-server-process", 
      "--no-mesh-create", "--no-case-setup", "--cloned-case-prefix=%s" % options.studyname, 
      "--every-variant-one-case-execution",
      "--create-database", options.casedir, options.paramfile])
