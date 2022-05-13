import unittest

import numpy as np
import richdem as rd
from richdem import depression_hierarchy as dephier

class py_richdem_tests(unittest.TestCase):
  def test_depression_defaults(self) -> None:
    d = dephier.Depression()
    self.assertEqual(d.out_cell, dephier.NO_VALUE)
    self.assertEqual(d.pit_cell, dephier.NO_VALUE)
    self.assertEqual(d.parent, dephier.NO_PARENT)
    self.assertEqual(d.odep, dephier.NO_VALUE)
    self.assertEqual(d.geolink, dephier.NO_VALUE)
    self.assertEqual(d.lchild, dephier.NO_VALUE)
    self.assertEqual(d.rchild, dephier.NO_VALUE)
    self.assertEqual(d.ocean_parent, False)
    self.assertEqual(d.dep_label, 0)
    self.assertEqual(d.cell_count, 0)
    self.assertEqual(d.dep_vol, 0)
    self.assertEqual(d.water_vol, 0)
    self.assertEqual(d.total_elevation, 0)

  def test_random_terrain(self) -> None:
    dem = rd.GeneratePerlinTerrain(20, 20)
    # A labels array where all the edge cells are in the ocean and all the
    # interior cells are not yet assigned to a depression
    labels = rd.rdarray(
      dephier.OCEAN*np.ones(dem.shape, dtype = np.uint32),
      no_data=-9999,
      geotransform=rd.STANDARD_GEOTRANSFORM
    )
    labels[1:-1,1:-1] = dephier.NO_DEP
    dh, flowdirs = rd.get_depression_hierarchy(dem, labels)