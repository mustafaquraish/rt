/**
 * Based on code by Eliot Eshelman.
 * 
 * Copyright (c) 2007-2012 Eliot Eshelman
 * Copyright (c) 2021 Mustafa Quraish
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SIMPLEX_H__
#define __SIMPLEX_H__

// Put this in the global namespace for convenience
enum PerlinType {Layered, Turbulence, Marble};

namespace Simplex {
// Multi-octave Simplex noise [-1, 1]
double layered(double octaves, double persistence, double x, double y);
double layered(double octaves, double persistence, double x, double y, double z);
double layered(double octaves, double persistence, double x, double y, double z, double w);

// Raw Simplex noise [-1, 1]
double noise(double x);
double noise(double x, double y);
double noise(double x, double y, double z);
double noise(double x, double y, double, double w);

double convertTo(double perlinValue, PerlinType type);

};  // namespace Simplex

#endif // __SIMPLEX_H__
