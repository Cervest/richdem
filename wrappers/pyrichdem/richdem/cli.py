#!/usr/bin/env python

import richdem as rd
import sys
import argparse
from argparse import RawTextHelpFormatter

def DepressionFilling():
  parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter, description='RichDEM Depression Filling')

  parser.add_argument('dem',     type=str,                     help='Elevation model')
  parser.add_argument('outname', type=str,                     help='Name of output file')
  parser.add_argument('-g', '--gradient', action='store_true', help='Ensure that all cells are at least an epsilon above their downstream cell. This ensures that each cell has a defined flow direction.')
  parser.add_argument('-v', '--version',  action='version', version=rd._RichDEMVersion())
  args = parser.parse_args()

  dem = rd.LoadGDAL(args.dem)
  rd._AddAnalysis(dem, ' '.join(sys.argv))
  rd.FillDepressions(dem, epsilon=args.gradient, in_place=True)
  rd.SaveGDAL(args.outname, dem)



def BreachDepressions():
  parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter, description="""RichDEM Depression Breaching

Modes:
Complete:    Breach everything.
            Ignore max_path_len, max_path_depth.
            There will be no depressions.
            There will be no mercy.
Selective:   Only breach those depressions that can be breached using the
            above criteria.
Constrained: Dig as long a path as necessary, but don't dig it deeper than
            max_path_depth.
""")

  parser.add_argument('dem',                    type=str,                help='Elevation model')
  parser.add_argument('outname',                type=str,                help='Name of output file')
  parser.add_argument('-m', '--mode',           required=True, type=str, help='Breaching mode to use')
  parser.add_argument('-f', '--fill',           action='store_true',     help="If depressions can't be breached, should they be filled?")
  parser.add_argument('-l', '--max_path_len',   type=int,                help="Maximum length of breaching path in cells")
  parser.add_argument('-d', '--max_path_depth', type=float,              help="Maximum depth of breaching path in z-units")
  parser.add_argument('-v', '--version',  action='version', version=rd._RichDEMVersion())
  args = parser.parse_args()

  dem = rd.LoadGDAL(args.dem)
  rd._AddAnalysis(dem, ' '.join(sys.argv))
  rd.BreachDepressions(dem, mode=args.mode, fill=args.fill, max_path_len=args.max_path_len, max_path_depth=args.max_path_depth, in_place=True)
  rd.SaveGDAL(args.outname, dem)



def FlowAccumulation():
  parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter, description="""RichDEM Flow Accumulation

A variety of methods are available.

Method            Note                           Reference
Tarboton          Alias for Dinf.
Dinf              Alias for Tarboton.
Quinn             Holmgren with exponent=1.
Holmgren(E)       Generalization of Quinn.
Freeman(E)        TODO
FairfieldLeymarie Alias for Rho8.
Rho8              Alias for FairfieldLeymarie.
OCallaghan        Alias for D8.                  10.1016/S0734-189X(84)80011-0
D8                Alias for OCallaghan.          10.1016/S0734-189X(84)80011-0

Methods marked (E) require the exponent argument.
""")
  parser.add_argument('dem',              type=str,                help='Elevation model')
  parser.add_argument('outname',          type=str,                help='Name of output file')
  parser.add_argument('-m', '--method',   type=str, required=True, help='Flow accumulation method to use')
  parser.add_argument('-e', '--exponent', type=float,              help='Some methods require an exponent')
  parser.add_argument('-v', '--version',  action='version', version=rd._RichDEMVersion())
  args = parser.parse_args()

  dem = rd.LoadGDAL(args.dem)
  rd._AddAnalysis(dem, ' '.join(sys.argv))
  accum = rd.FlowAccumulation(dem, method=args.method, exponent=args.exponent)
  rd.SaveGDAL(args.outname, accum)



def TerrainAttribute():
  parser = argparse.ArgumentParser(formatter_class=RawTextHelpFormatter, description="""RichDEM Terrain Attribute

A variety of methods are available.

Parameters:
dem      -- An elevation model
attrib   -- Terrain attribute to calculate. (See below.)
zscale   -- How much to scale the z-axis by prior to calculation

Method:
slope_riserun
slope_percentage
slope_degrees
slope_radians
aspect
curvature
planform_curvature
profile_curvature
""")
  parser.add_argument('dem',              type=str,                help='Elevation model')
  parser.add_argument('outname',          type=str,                help='Name of output file')
  parser.add_argument('-a', '--attrib',   type=str, required=True, help='Terrain attribute to calculate')
  parser.add_argument('-z', '--zscale',   type=float, default=1.0, help='Scale elevations by this factor prior to calculation')
  parser.add_argument('-v', '--version',  action='version', version=rd._RichDEMVersion())
  args = parser.parse_args()

  dem = rd.LoadGDAL(args.dem)
  rd._AddAnalysis(dem, ' '.join(sys.argv))
  tattrib = rd.TerrainAttribute(dem, attrib=args.attrib, zscale=args.zscale)
  rd.SaveGDAL(args.outname, tattrib)
