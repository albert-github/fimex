# Fimex, modules/python/test_pyfimex0.py
#
# Copyright (C) 2018-2019 met.no
#
# Contact information:
# Norwegian Meteorological Institute
# Box 43 Blindern
# 0313 OSLO
# NORWAY
# email: diana@met.no
#
# Project Info:  https://wiki.met.no/fimex/start
#
# This library is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
# License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
# USA.

import pyfimex0
import numpy
import os.path
import unittest

test_srcdir = os.path.join(os.path.dirname(__file__), "..", "..", "test")

class TestReader(unittest.TestCase):

    def test_MifiVersion(self):
        mv = pyfimex0.mifi_version()
        self.assertEqual(4, len(mv))
        major, minor, patch, status = mv
        self.assertTrue(type(major) is int)
        self.assertTrue((major == 0 and minor > 0) or (major > 0 and minor >= 0))

    def test_OpenAndInspect(self):
        test_ncfile = os.path.join(test_srcdir, 'testdata_vertical_ensemble_in.nc')
        r = pyfimex0.createFileReader('netcdf', test_ncfile)

        self.assertTrue(type(r.getDataSlice('hybrid',0).values()[0]) is numpy.float64)

        self.assertEqual(r.getDataSlice('upward_air_velocity_ml',0).values()[0], 305)
        self.assertAlmostEqual(r.getScaledDataSlice('upward_air_velocity_ml',0).values()[0], 0.0305)
        self.assertAlmostEqual(r.getScaledDataSliceInUnit('upward_air_velocity_ml','mm/s',0).values()[0], 30.5)

        self.assertTrue(numpy.isnan(r.getScaledDataSlice('upward_air_velocity_ml',1).values()[-1]))

        r_cdm = r.getCDM()


        d_ens = r_cdm.getDimension('ensemble_member')
        d_ens_len = d_ens.getLength()
        self.assertEqual(3, d_ens_len)
        self.assertFalse(d_ens.isUnlimited())

        d_time = r_cdm.getDimension('time')
        self.assertTrue(d_time.isUnlimited())

        r_dims = r_cdm.getDimensionNames()
        self.assertTrue('time' in r_dims)

        r_vars = r_cdm.getVariableNames()
        self.assertTrue('surface_geopotential' in r_vars)

    def test_DataString(self):
        d = pyfimex0.createData("hello")
        self.assertEqual(pyfimex0.CDMDataType.STRING, d.getDataType())
        self.assertEqual(5, d.size())
        self.assertEqual("hello", d.values())

        d = pyfimex0.createData(pyfimex0.CDMDataType.STRING, "values")
        self.assertEqual(pyfimex0.CDMDataType.STRING, d.getDataType())
        self.assertEqual(6, d.size())
        self.assertEqual("values", d.values())

    def test_DataFloat(self):
        d = pyfimex0.createData(pyfimex0.CDMDataType.FLOAT, [1, 2, 3])
        self.assertEqual(pyfimex0.CDMDataType.FLOAT, d.getDataType())
        self.assertEqual(3, d.size())
        self.assertEqual(numpy.float32, d.values().dtype)

    def test_DataChar(self):
        d = pyfimex0.createData(numpy.arange(5, dtype=numpy.int8))
        self.assertEqual(pyfimex0.CDMDataType.CHAR, d.getDataType())
        self.assertEqual(5, d.size())

    def test_DataUChar(self):
        d = pyfimex0.createData(numpy.arange(5, dtype=numpy.uint8))
        self.assertEqual(pyfimex0.CDMDataType.UCHAR, d.getDataType())

    def test_AttributeFromFile(self):
        test_ncfile = os.path.join(test_srcdir, 'testdata_vertical_ensemble_in.nc')
        r = pyfimex0.createFileReader('netcdf', test_ncfile)
        r_cdm = r.getCDM()

        self.assertTrue('long_name' in r_cdm.getAttributeNames('x_wind_10m'))
        r_att = r_cdm.getAttribute('x_wind_10m', 'long_name')
        self.assertEqual("Zonal 10 metre wind (U10M)", r_att.getStringValue())

        self.assertTrue('title' in r_cdm.getGlobalAttributeNames())
        r_gatt = r_cdm.getGlobalAttribute('title')
        self.assertEqual("MEPS 2.5km", r_gatt.getStringValue())

    def test_AttributeString(self):
        att = pyfimex0.CDMAttribute("name", "value")
        self.assertEqual("name", att.getName())
        self.assertEqual("value", att.getStringValue())
        self.assertEqual(pyfimex0.CDMDataType.STRING, att.getDataType())

        att.setName("navn")
        self.assertEqual("navn", att.getName())

        att.setData(pyfimex0.createData("content"))
        self.assertEqual("content", att.getStringValue())
        self.assertEqual(pyfimex0.CDMDataType.STRING, att.getDataType())

    def test_AttributeFloat(self):
        att = pyfimex0.CDMAttribute("f", pyfimex0.createData(pyfimex0.CDMDataType.FLOAT, [1]))
        self.assertEqual("f", att.getName())
        self.assertEqual("1", att.getStringValue())
        self.assertEqual([1.0], att.getData().values())
        self.assertEqual(pyfimex0.CDMDataType.FLOAT, att.getDataType())

    def test_AttributeUChar(self):
        l = [0, 12, 234]
        att = pyfimex0.CDMAttribute("u", pyfimex0.createData(pyfimex0.CDMDataType.UCHAR, l))
        self.assertEqual(" ".join([str(x) for x in l]), att.getStringValue())
        d = att.getData()
        self.assertEqual(len(l), d.size())
        v = d.values()
        self.assertEqual(numpy.uint8, v.dtype)
        self.assertEqual(l, list(v))

    def test_VariableFromFile(self):
        test_ncfile = os.path.join(test_srcdir, 'testdata_vertical_ensemble_in.nc')
        r = pyfimex0.createFileReader('netcdf', test_ncfile)
        r_cdm = r.getCDM()

        v_xwind10m = r_cdm.getVariable('x_wind_10m')
        v_xwind10m_name = v_xwind10m.getName()
        self.assertEqual('x_wind_10m', v_xwind10m_name)

        self.assertEqual(['x', 'y', 'ensemble_member', 'height7', 'time'], v_xwind10m.getShape())

    def test_VariableFloat(self):
        shp = ['x', 'y', 'time']
        var = pyfimex0.CDMVariable("f", pyfimex0.CDMDataType.FLOAT, shp)
        self.assertEqual("f", var.getName())
        self.assertEqual(shp, var.getShape())

    def test_CDM_variables(self):
        cdm = pyfimex0.CDM()
        self.assertEqual(0, len(cdm.getVariableNames()))

        cdm.addVariable(pyfimex0.CDMVariable("varf", pyfimex0.CDMDataType.FLOAT, ['x', 'y']))
        self.assertEqual(['varf'], cdm.getVariableNames())

        cdm.addAttribute('varf', pyfimex0.CDMAttribute('_FillValue', pyfimex0.createData(pyfimex0.CDMDataType.FLOAT, -1)))
        self.assertEqual(1, len(cdm.getAttributeNames('varf')))

        cdm.removeVariable('varf')
        self.assertEqual(0, len(cdm.getVariableNames()))

if __name__ == '__main__':
    unittest.main()
