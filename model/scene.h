#pragma once

#include "transformMatrix.h"

namespace s21 {

class SceneObject {
 public:
  virtual void Transform(const TransformMatrix& matrix) = 0;
};

class Vertex : public SceneObject {
 public:
  Vertex() = default;
  Vertex(Point3D position) : position_(position){};
  Vertex(double x, double y, double z) : position_(x, y, z){};

  Point3D GetPosition() const { return position_; }
  void SetPosition(const Point3D& position) { position_ = position; }

  void Transform(const TransformMatrix& matrix) override {
    position_ = matrix.TransformPoint(position_);
  }

 private:
  Point3D position_;
};

class Edge {
 public:
  Edge() = delete;
  Edge(Vertex& begin, Vertex& end) : begin_(begin), end_(end){};

  Vertex& GetBegin() const { return begin_; }
  Vertex& GetEnd() const { return end_; }

 private:
  Vertex& begin_;
  Vertex& end_;
};

class Figure : public SceneObject {
 public:
  Figure() = default;
  Figure(const std::vector<Vertex>& vertices, const std::vector<Edge>& edges)
      : vertices_(vertices), edges_(edges) {
    FindCenter();
  };
  Figure(std::vector<Vertex>&& vertices, std::vector<Edge>&& edges)
      : vertices_(std::move(vertices)), edges_(std::move(edges)) {
    FindCenter();
  };

  std::vector<Vertex> GetVertices() { return vertices_; }
  std::vector<Edge> GetEdges() { return edges_; }
  Point3D GetCenter() { return center_; }

  void Transform(const TransformMatrix& matrix) override {
    for (auto& vertex : vertices_) {
      vertex.Transform(matrix);
    }
    center_ = matrix.TransformPoint(center_);
  }

  void FindCenter() {
    double x = 0.0, y = 0.0, z = 0.0;
    for (const auto& vertex : vertices_) {
      x += vertex.GetPosition().x;
      y += vertex.GetPosition().y;
      z += vertex.GetPosition().z;
    }
    int n = vertices_.size();
    center_ = Point3D(x / n, y / n, z / n);
  }

 private:
  std::vector<Vertex> vertices_;
  std::vector<Edge> edges_;
  Point3D center_;
};

class Scene {
 public:
  Scene() = default;
  Scene(const Figure& figure) : figure_(figure){};
  Scene(Figure&& figure) : figure_(std::move(figure)) {}

  Figure GetFigure() { return figure_; }

  void TransformFigure(const TransformMatrix& matrix) {
    figure_.Transform(matrix);
  }

 private:
  Figure figure_;
};

}  // namespace s21
