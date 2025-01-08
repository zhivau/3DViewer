#include "fileReader.h"

#include <QDebug>
#include <QDir>

namespace s21 {

Scene FileReader::ReadScene(std::string path,
                            const NormalizationParamaters &params) {
  operationResult_.SetSuccess(true);
  std::ifstream fileObj(path);

  if (!fileObj) {
    operationResult_.SetSuccess(false);
    operationResult_.SetErrorMessage("Ошибка: Не удалось открыть obj файл");
  }

  std::string line;
  std::vector<Vertex> vertices;
  std::vector<Edge> edges;

  std::vector<int> indices;

  while (std::getline(fileObj, line) && operationResult_.isSuccess()) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "v") {
      double x, y, z;

      if (!(iss >> x >> y >> z)) {
        operationResult_.SetErrorMessage(
            "Ошибка: Неверный формат строки вершины: ");
        continue;
      }
      vertices.emplace_back(x, y, z);
    } else if (type == "f") {
      ParseIndices(indices, iss);
    }
  }
  NormalizeVertices(vertices, params);
  AddEdges(indices, vertices, edges);

  fileObj.close();

  return Scene(Figure(std::move(vertices), std::move(edges)));
}

Settings FileReader::ReadSettings() {
  QString currentPath = QDir::currentPath();
  QSettings settingsFile(currentPath + "/settings.ini", QSettings::IniFormat);

  Settings settings;

  settingsFile.beginGroup("SettingsDrawer");
  settings.settings_drawer.type_of_edge = static_cast<TypeEdge>(
      settingsFile.value("type_of_edge", static_cast<int>(TypeEdge::DASHED))
          .toInt());
  settings.settings_drawer.type_of_vertex = static_cast<TypeVertex>(
      settingsFile.value("type_of_vertex", static_cast<int>(TypeVertex::SQUARE))
          .toInt());
  settings.settings_drawer.background_color[0] =
      settingsFile.value("background_color_r", 0.0f).toFloat();
  settings.settings_drawer.background_color[1] =
      settingsFile.value("background_color_g", 0.0f).toFloat();
  settings.settings_drawer.background_color[2] =
      settingsFile.value("background_color_b", 0.0f).toFloat();
  settings.settings_drawer.thickness_of_edge =
      settingsFile.value("thickness_of_edge", 1.0f).toFloat();
  settings.settings_drawer.size_of_vertex =
      settingsFile.value("size_of_vertex", 5.0f).toFloat();
  settings.settings_drawer.edge_color[0] =
      settingsFile.value("edge_color_r", 1.0f).toFloat();
  settings.settings_drawer.edge_color[1] =
      settingsFile.value("edge_color_g", 1.0f).toFloat();
  settings.settings_drawer.edge_color[2] =
      settingsFile.value("edge_color_b", 1.0f).toFloat();
  settings.settings_drawer.vertex_color[0] =
      settingsFile.value("vertex_color_r", 1.0f).toFloat();
  settings.settings_drawer.vertex_color[1] =
      settingsFile.value("vertex_color_g", 0.0f).toFloat();
  settings.settings_drawer.vertex_color[2] =
      settingsFile.value("vertex_color_b", 0.0f).toFloat();
  settingsFile.endGroup();

  settingsFile.beginGroup("SettingsView");
  settings.settings_view.type_of_projection = static_cast<TypeProjection>(
      settingsFile
          .value("type_of_projection",
                 static_cast<int>(TypeProjection::PARALLEL))
          .toInt());
  settings.settings_view.is_updated_type_of_projection =
      settingsFile.value("is_updated_type_of_projection", false).toBool();
  settingsFile.endGroup();

  if (settingsFile.contains("background_color_r")) {
    settings.settings_drawer.background_color[0] =
        settingsFile.value("background_color_r").toFloat();
  }
  return settings;
}

void FileReader::ParseIndices(std::vector<int> &indices,
                              std::istringstream &iss) {
  std::string vertexInfo;
  std::vector<int> new_indices;
  while (iss >> vertexInfo) {
    std::istringstream vertexStream(vertexInfo);
    int vertexIndex, textureIndex = -1, normalIndex = -1;

    if (!(vertexStream >> vertexIndex)) {
      operationResult_.SetErrorMessage(
          "Ошибка: Неверный индекс вершины в грани");
      continue;
    }
    if (vertexIndex <= 0) {
      operationResult_.SetErrorMessage(
          "Ошибка: Индекс вершины не может быть нулевым или отрицательным:");
      continue;
    }
    new_indices.push_back(
        vertexIndex - 1);
    if (vertexStream.peek() == '/') {
      vertexStream.ignore(1, '/');
      if (vertexStream.peek() != '/') {
        if (vertexStream >> textureIndex) {
        }
      }

      if (vertexStream.peek() == '/') {
        vertexStream.ignore(1, '/');
        if (vertexStream >> normalIndex) {
        }
      }
    }
  }
  if (new_indices.size() >= 2) {
    indices.insert(indices.end(), new_indices.begin(), new_indices.end());
  } else {
    operationResult_.SetErrorMessage(
        "Ошибка: В грань должно входить как минимум 2 вершины:");
  }
}

void FileReader::AddEdges(std::vector<int> &indices,
                          std::vector<Vertex> &vertices,
                          std::vector<Edge> &edges) {
  for (size_t i = 1; i < indices.size(); ++i) {
    if (indices[i] >= (int)vertices.size() ||
        indices[i - 1] >= (int)vertices.size()) {
      operationResult_.SetErrorMessage(
          "Ошибка: Индекс вершины за пределами допустимого диапазона:");
      continue;
    }
    edges.emplace_back(vertices[indices[i - 1]], vertices[indices[i]]);
  }
}

void FileReader::NormalizeVertices(std::vector<Vertex> &vertices,
                                   const NormalizationParamaters &params) {
  double x_min = std::numeric_limits<double>::max();
  double x_max = std::numeric_limits<double>::min();
  double y_min = std::numeric_limits<double>::max();
  double y_max = std::numeric_limits<double>::min();
  double z_min = std::numeric_limits<double>::max();
  double z_max = std::numeric_limits<double>::min();

  for (auto &vertex : vertices) {
    Point3D pos = vertex.GetPosition();
    x_min = std::min(x_min, pos.x);
    x_max = std::max(x_max, pos.x);
    y_min = std::min(y_min, pos.y);
    y_max = std::max(y_max, pos.y);
    z_min = std::min(z_min, pos.z);
    z_max = std::max(z_max, pos.z);
  }

  std::vector<Vertex> normalized_vertices;
  for (auto &vertex : vertices) {
    Point3D pos = vertex.GetPosition();

    pos.x = ((pos.x - x_min) / (x_max - x_min)) * (params.max - params.min) +
            params.min;
    pos.y = ((pos.y - y_min) / (y_max - y_min)) * (params.max - params.min) +
            params.min;
    pos.z = ((pos.z - z_min) / (z_max - z_min)) * (params.max - params.min) +
            params.min;

    pos.x = std::round(pos.x / params.dxStep) * params.dxStep;
    pos.y = std::round(pos.y / params.dyStep) * params.dyStep;

    vertex.SetPosition(pos);
  }
}

}
