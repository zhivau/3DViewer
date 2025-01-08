#pragma once

#include <string>

namespace s21 {
  
struct Point3D {
  double x;
  double y;
  double z;

  Point3D() : x(0), y(0), z(0){};
  Point3D(double coorX, double coorY, double coorZ)
      : x(coorX), y(coorY), z(coorZ){};

  bool operator==(const Point3D& other) const {
    return (x == other.x && y == other.y && z == other.z);
  }
};

struct NormalizationParamaters {
  double min = 0;
  double max = 1;
  double dxStep = 0.001;
  double dyStep = 0.001;
};

class OperationResult {
 public:
  OperationResult() : isSuccess_(true){};

  bool isSuccess() { return isSuccess_; };
  std::string GetErrorMessage() { return errorMessage_; };

  void SetSuccess(bool isSuccess) { isSuccess_ = isSuccess; };
  void SetErrorMessage(std::string errorMessage) {
    errorMessage_ = errorMessage;
  };

 private:
  bool isSuccess_;
  std::string errorMessage_;
};

enum class TypeEdge { SOLID = 0, DASHED };
enum class TypeVertex { NONE = 0, CIRCLE, SQUARE };

struct SettingsDrawer {
  TypeEdge type_of_edge = TypeEdge::SOLID;
  TypeVertex type_of_vertex = TypeVertex::NONE;

  float background_color[3] = {0.0f, 0.0f, 0.0f};
  float thickness_of_edge = 1.0f;
  float size_of_vertex = 5.0f;
  float edge_color[3] = {1.0f, 1.0f, 1.0f};
  float vertex_color[3] = {1.0f, 0.0f, 0.0f};
};

enum class TypeProjection { PARALLEL, CENTRAL };

struct SettingsView {
  TypeProjection type_of_projection = TypeProjection::PARALLEL;
  bool is_updated_type_of_projection = false;
};

}  // namespace s21
