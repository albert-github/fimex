/*
 * Fimex, mifi_constants.h
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

#ifndef MIFI_CONSTANTS_H_
#define MIFI_CONSTANTS_H_

// math.h needed for MIFI_NAN
#include <math.h>
// stddef.h needed for size_t
#include <stddef.h>

/**
 * @headerfile fimex/mifi_constants.h
 */

/**
 * M_PI is no longer part of C99, so it needs to be declared for fimex
 */
#define MIFI_PI 3.1415926535897932384626433832795


/**
 * @brief interpolation method
 *
 * Flag for nearest neighbor interpolation. This requires,
 * that the original data comes with a properly defined projection,
 * i.e. implicit as latlon or explicit with projection-string
 */
enum mifi_interpol_method {
   /**
    * Flag for bilinear interpolation. This requires,
    * that the original data comes with a properly defined projection,
    * i.e. implicit as latlon or explicit with projection-string
    */
    MIFI_INTERPOL_NEAREST_NEIGHBOR = 0,
   /**
    * Flag for bicubic interpolation. This requires,
    * that the original data comes with a properly defined projection,
    * i.e. implicit as latlon or explicit with projection-string
    */
    MIFI_INTERPOL_BILINEAR,
   /**
    * Flag for bicubic interpolation. This requires,
    * that the original data comes with a properly defined projection,
    * i.e. implicit as latlon or explicit with projection-string
    */
    MIFI_INTERPOL_BICUBIC,
   /**
    * Flag for nearest neighbor interpolation
    * using lon/lat coordinates rather than the input
    * projection. This is largely a brute force method
    * which may take long time.
    *
    * Vector projection is not implemented (not defined?)
    *
    * @warning this works only from CDMInterpolator
    */
    MIFI_INTERPOL_COORD_NN,
   /**
    * Flag for nearest neighbor interpolation
    * using coordinates with KD-tree. This works
    * as nearest neighbor in the output-projection
    * and has therefore numerical problems in some points,
    * i.e. near southpole when using northpole-polarstereographic.
    *
    * It doesn't work with output projections in degree, i.e.
    * rotated latitude longitude, since distances are calculated as outX^2*outY^2
    *
    * Vector projection is not implemented (not defined?)
    *
    * @warning this works only from CDMInterpolator
    *
    */
    MIFI_INTERPOL_COORD_NN_KD,
   /**
    * forward interpolation, summing over all matching input-cells
    */
    MIFI_INTERPOL_FORWARD_SUM,
   /**
    * forward interpolation, averaging (mean) over all matching defined input-cells
    */
    MIFI_INTERPOL_FORWARD_MEAN,
    /**
     * forward interpolation, median over all matching defined input-cells
     */
    MIFI_INTERPOL_FORWARD_MEDIAN,
    /**
     * forward interpolation, max over all matching defined input-cells
     */
    MIFI_INTERPOL_FORWARD_MAX,
    /**
     * forward interpolation, min over all matching defined input-cells
     */
    MIFI_INTERPOL_FORWARD_MIN
};

/**
 * @brief vector projection flag
 *
 * new size will be like old size
 */
#define MIFI_VECTOR_KEEP_SIZE 0
/**
 * @brief vector projection flag
 *
 * vector might change size with projection
 */
#define MIFI_VECTOR_RESIZE    1


/**
 * @brief vertical interpolation type
 *
 * vertical interpolation to pressure levels in hPa
 */
#define MIFI_VINT_PRESSURE 0

/**
 * @brief vertical interpolation type
 *
 * vertical interpolation to height above ground levels in m
 */
#define MIFI_VINT_HEIGHT 1

/**
 * @brief vertical interpolation type
 *
 * vertical interpolation to depth below sea-level in m
 */
#define MIFI_VINT_DEPTH 2

/**
 * @brief vertical interpolation type
 *
 * vertical interpolation to sigma dimensionless vertical coordinate
 */
#define MIFI_VINT_SIGMA 3


/**
 * @brief vertical interpolation method
 *
 * linear interpolation, e.g. mifi_get_values_lin_f()
 */
#define MIFI_VINT_METHOD_LIN 0

/**
 * @brief vertical interpolation method
 *
 * logarithmic interpolation, e.g. mifi_get_values_log_f()
 */
#define MIFI_VINT_METHOD_LOG 1

/**
 * @brief vertical interpolation method
 *
 * double logarithmic interpolation, e.g. mifi_get_values_log_f()
 */
#define MIFI_VINT_METHOD_LOGLOG 2

/**
 * @brief vertical interpolation method
 *
 * nearest neighbor "interpolation", e.g. mifi_get_values_f()
 */
#define MIFI_VINT_METHOD_NN 3



#ifdef __cplusplus
#include <limits>
/** @brief undefined value for floats */
#define MIFI_UNDEFINED_F (std::numeric_limits<float>::quiet_NaN())
/** @brief undefined value for doubles */
#define MIFI_UNDEFINED_D (std::numeric_limits<double>::quiet_NaN())
#else
/** @brief undefined value for floats */
#define MIFI_UNDEFINED_F (nanf(""))
/** @brief undefined value for doubles */
#define MIFI_UNDEFINED_D (nan(""))
#endif
/** @brief return code, error */
#define MIFI_ERROR -1
/** @brief return code, ok */
#define MIFI_OK 1

/** @brief projection axis in m-equivalent */
#define MIFI_PROJ_AXIS 0
/** @brief longitude projection axis in degrees */
#define MIFI_LONGITUDE 1
/** @brief latitude projection axis in degrees */
#define MIFI_LATITUDE 2

/** @brief debug flag */
#define MIFI_DEBUG 0

#endif /* MIFI_CONSTANTS_H_ */
