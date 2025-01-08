#include "transformMatrix.h"

namespace s21 {

TransformMatrix::TransformMatrix() : matrix_(4, std::vector<double>(4, 0)) {
  // Инициализация единичной матрицы
  for (int i = 0; i < 4; ++i) {
    matrix_[i][i] = 1.0;
  }
}

TransformMatrix TransformMatrix::operator*(const TransformMatrix &other) const {
  TransformMatrix result;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result.matrix_[i][j] = 0;
      for (int k = 0; k < 4; ++k) {
        result.matrix_[i][j] += matrix_[i][k] * other.matrix_[k][j];
      }
    }
  }
  return result;
}

Point3D TransformMatrix::TransformPoint(const Point3D &point) const {
  std::vector<double> p = {point.x, point.y, point.z, 1};
  std::vector<double> result(4, 0);

  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      result[i] += matrix_[i][j] * p[j];
    }
  }

  return Point3D(result[0], result[1], result[2]);
}

TransformMatrix TransformMatrixBuilder::CreateRotationMatrix(double angleX,
                                                             double angleY,
                                                             double angleZ,
                                                             Point3D center) {
  TransformMatrix translationToOrigin;
  translationToOrigin.matrix_[0][3] = -center.x;
  translationToOrigin.matrix_[1][3] = -center.y;
  translationToOrigin.matrix_[2][3] = -center.z;

  TransformMatrix rotationX, rotationY, rotationZ;

  if (angleX != 0) {
    rotationX.matrix_[1][1] = cos(angleX);
    rotationX.matrix_[1][2] = -sin(angleX);
    rotationX.matrix_[2][1] = sin(angleX);
    rotationX.matrix_[2][2] = cos(angleX);
  }

  if (angleY != 0) {
    rotationY.matrix_[0][0] = cos(angleY);
    rotationY.matrix_[0][2] = sin(angleY);
    rotationY.matrix_[2][0] = -sin(angleY);
    rotationY.matrix_[2][2] = cos(angleY);
  }

  if (angleZ != 0) {
    rotationZ.matrix_[0][0] = cos(angleZ);
    rotationZ.matrix_[0][1] = -sin(angleZ);
    rotationZ.matrix_[1][0] = sin(angleZ);
    rotationZ.matrix_[1][1] = cos(angleZ);
  }

  TransformMatrix rotation = rotationZ * rotationY * rotationX;

  TransformMatrix translationBack;
  translationBack.matrix_[0][3] = center.x;
  translationBack.matrix_[1][3] = center.y;
  translationBack.matrix_[2][3] = center.z;

  return translationBack * rotation * translationToOrigin;
}

TransformMatrix TransformMatrixBuilder::CreateMoveMatrix(double dx, double dy,
                                                         double dz) {
  TransformMatrix move;
  move.matrix_[0][3] = dx;
  move.matrix_[1][3] = dy;
  move.matrix_[2][3] = dz;
  return move;
}

TransformMatrix TransformMatrixBuilder::CreateScaleMatrix(double sx, double sy,
                                                          double sz,
                                                          Point3D center) {
  TransformMatrix translationToOrigin;
  translationToOrigin.matrix_[0][3] = -center.x;
  translationToOrigin.matrix_[1][3] = -center.y;
  translationToOrigin.matrix_[2][3] = -center.z;

  TransformMatrix scale;
  scale.matrix_[0][0] = sx;
  scale.matrix_[1][1] = sy;
  scale.matrix_[2][2] = sz;

  TransformMatrix translationBack;
  translationBack.matrix_[0][3] = center.x;
  translationBack.matrix_[1][3] = center.y;
  translationBack.matrix_[2][3] = center.z;

  return translationBack * scale * translationToOrigin;
}

}