/*
 * Fimex, vertical_coordinate_transformations.h
 *
 * (C) Copyright 2011, met.no
 *
 * Project Info:  https://wiki.met.no/fimex/start
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 *  Created on: Jul 28, 2011
 *      Author: Heiko Klein
 */

#ifndef VERTICAL_COORDINATE_TRANSFORMATIONS_H_
#define VERTICAL_COORDINATE_TRANSFORMATIONS_H_
/**
 * @headerfile "fimex/vertical_coordinate_transformations.h"
 */

#include "fimex/mifi_constants.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * convert a standard_name="atmosphere_ln_pressure_coordinate" to pressure using the
 * formular  p(k) = p0 * exp(-lev(k))
 *
 * @param n size of arrays lev and pressure
 * @param p0 base pressure
 * @param lev level values
 * @param pressure output values in the same unit as p0
 * @return MIFI_OK on success or MIFI_ERROR on failure
 */
extern int mifi_atmosphere_ln_pressure(size_t n, double p0, const double* lev, double* pressure);

/**
 * convert a standard_name="atmosphere_sigma_coordinate" to pressure using the
 * formular  p(k) = ptop + sigma(k)*(ps-ptop)
 *
 *
 * @param n size of arrays sigma and pressure
 * @param ptop pressure on model top layer (constant for a model)
 * @param ps surface pressure - usually varying in time,x,y
 * @param sigma level values
 * @param pressure output values in the same unit as ptop and ps and at the same place as ps
 * @return MIFI_OK on success or MIFI_ERROR on failure
 */
extern int mifi_atmosphere_sigma_pressure(size_t n, double ptop, double ps, const double* lev, double* pressure);

/**
 * convert a standard_name="atmosphere_hybrid_sigma_pressure_coordinate" to pressure using the
 * formular  p(k) = a(k)*p0 + b(k)*ps
 *
 *
 * @param n size of arrays a, b and pressure
 * @param p0 reference pressure
 * @param ps surface pressure - usually varying in time,x,y
 * @param a dimensionless level values
 * @param b dimensionless level values
 * @param pressure output values in the same unit as p0 and ps and at the same place as ps
 * @return MIFI_OK on success or MIFI_ERROR on failure
 */
extern int mifi_atmosphere_hybrid_sigma_pressure(size_t n, double p0, double ps, const double* a, const double* b, double* pressure);

/**
 * convert a standard_name="atmosphere_hybrid_sigma_pressure_coordinate" to pressure using the
 * formular  p(k) = ap(k) + b(k)*ps
 *
 * This is the same as mifi_atmosphere_hybrid_sigma_pressure(), but with the reference pressure
 * and a joined already. Choice depends on the model, i.e. available input values.
 *
 * @param n size of arrays ap, b and pressure
 * @param ps surface pressure - usually varying in time,x,y
 * @param ap pressure level values
 * @param b dimensionless level values
 * @param pressure output values in the same unit as p0 and ps and at the same place as ps
 * @return MIFI_OK on success or MIFI_ERROR on failure
 */
extern int mifi_atmosphere_hybrid_sigma_ap_pressure(size_t n, double ps, const double* ap, const double* b, double* pressure);

/**
 * convert height to pressure using the formular http://en.wikipedia.org/wiki/Barometric_formula
 *
 * P(h) = P_b exp[ -gM/R * h/T_b  ]
 *
 * with P_b and T_b pressure and temperature at the layer b (i.e. surface) and h_b
 * the height above the layer b
 *
 * g = 9.80665 m/s2
 * M = Molar mass of Earth's air (0.0289644 kg/mol)
 * R = Universal gas constant (8.31432 N·m /(mol·K) )
 *
 * @param n size of array h and pressure
 * @param P_b pressure at base-layer (i.e. surface, or means-sea-level) - usually varying in time,x,y
 * @param h height in m above base-layer
 * @param T_b temperature at base layer in K - usually varying in time,x,y
 * @param pressure output values in the same unit as p_b and at the same place as ps
 *
 * @warning This function has not been tested against possibly existing implementations
 */
extern int mifi_barometric_pressure(size_t n, double P_b, const double* h, double T_b, double* pressure);

/**
 * convert height to pressure using the formular http://en.wikipedia.org/wiki/Barometric_formula
 * and using the international standard atmosphere http://en.wikimedia.org/wiki/International_Standard_Atmosphere
 */
extern int mifi_barometric_standard_pressure(size_t n, const double* h, double* pressure);


/**
 * convert pressure to height using the inverse formular http://en.wikipedia.org/wiki/Barometric_formula
 *
 * h(k) = -R*T_b/g*M * log(p(k)/P_b);
 *
 * with P_b and T_b pressure and temperature at the layer b (i.e. surface)
 *
 * g = 9.80665 m/s2
 * M = Molar mass of Earth's air (0.0289644 kg/mol)
 * R = Universal gas constant (8.31432 N·m /(mol·K) )
 *
 * @param n size of array h and pressure
 * @param P_b pressure at base-layer (i.e. surface, or means-sea-level) - usually varying in time,x,y
 * @param p pressure at level
 * @param T_b temperature at base layer in K - usually varying in time,x,y
 * @param height output values, height above base_layer in m
 *
 * @warning This function has not been tested against possibly existing implementations
 *
 */
extern int mifi_barometric_height(size_t n, double P_b, const double* p, double T_b, double* height);

#ifdef __cplusplus
}
#endif

#endif /* VERTICAL_COORDINATE_TRANSFORMATIONS_H_ */
