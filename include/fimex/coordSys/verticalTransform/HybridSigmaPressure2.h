/*
 * Fimex, HybridSigmaPressure2.h
 *
 * (C) Copyright 2013, met.no
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
 *  Created on: Aug 6, 2013
 *      Author: heikok
 */

#ifndef HYBRIDSIGMAPRESSURE2_H_
#define HYBRIDSIGMAPRESSURE2_H_

#include "fimex/coordSys/verticalTransform/VerticalTransformation.h"

namespace MetNoFimex
{
/**
 * @headerfile fimex/coordSys/verticalTransform/HybridSigmaPressure2.h
 */
/// Hybrid sigma pressure vertical coordinate, expressed with a and b
class HybridSigmaPressure2 : public VerticalTransformation
{
public:
    const std::string a;
    const std::string b;
    const std::string ps;
    const std::string p0;

    /**
     * Initialize HybridSigmaPressure with formula
     *   p(k, x,y,t) = a(k)*p0 + b(k)*ps(x,y,t)
     *
     * @param a parameter in pressure unit
     * @param b  dimensionless parameter
     * @param ps surface pressure variable name
     * @param p0 p0 base-pressure
     */
    HybridSigmaPressure2(const std::string& a, const std::string& b, const std::string& ps, const std::string& p0 = "")
        : a(a), b(b), ps(ps), p0(p0) {}

    /**
     * static NAME constant
     * @return atmosphere_hybrid_sigma_pressure_coordinate_2
     */
    static const std::string NAME() {return "atmosphere_hybrid_sigma_pressure_coordinate_2";}
    /**
     * @return same as static NAME()
     */
    virtual std::string getName() const { return NAME(); }
    virtual int getPreferredVerticalType() const { return MIFI_VINT_PRESSURE; }
    virtual std::string getParameterString() const { return "a="+a+",b="+b+",ps="+ps+",p0="+p0; }
    virtual bool isComplete() const {return a != "" && b != "" && ps != "" && p0 != "";}

protected:
    VerticalConverter_p getPressureConverter(CDMReader_p reader, CoordinateSystem_cp cs) const;
};

} /* namespace MetNoFimex */

#endif /* HYBRIDSIGMAPRESSURE2_H_ */
