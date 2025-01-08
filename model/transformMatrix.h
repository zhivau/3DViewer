#pragma once

#include <cmath>
#include <vector>

#include "baseObjects.h"

namespace s21 {

class TransformMatrix {
 public:
  TransformMatrix();
  TransformMatrix operator*(const TransformMatrix &other) const;
  Point3D TransformPoint(const Point3D &point) const;

  std::vector<std::vector<double>> matrix_;
};

class TransformMatrixBuilder {
 public:
  static TransformMatrix CreateRotationMatrix(double x, double y, double z,
                                              Point3D center);
  static TransformMatrix CreateMoveMatrix(double dx, double dy, double dz);
  static TransformMatrix CreateScaleMatrix(double sx, double sy, double sz,
                                           Point3D center);
};

}  // namespace s21
