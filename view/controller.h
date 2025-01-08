#pragma once

#include "../model/facade.h"

namespace s21 {
/**
 * @class Controller
 * @brief Контроллер для отображения
 */
class Controller {
 public:
  Controller(Facade* facade) : facade_(facade) {}
  ~Controller() {
    delete facade_;
  };

  /**
   * @brief Основной метод для отрисовки
   */
  void DrawScene() { facade_->DrawScene(); }

  void InitQtSceneDrawer() { facade_->InitQtSceneDrawer(); }

  /** @} */
  /**
   * @defgroup SettingsDrawer
   * группа методов для изменения настроек отрисовщика
   * @{
   */
  void ChangeTypeOfEdge(TypeEdge new_type_of_edge) {
    facade_->ChangeTypeOfEdge(new_type_of_edge);
  }

  void ChangeTypeOfVertex(TypeVertex new_type_of_vertex) {
    facade_->ChangeTypeOfVertex(new_type_of_vertex);
  }

  void ChangeBackgoundColor(float new_background_color[3]) {
    facade_->ChangeBackgoundColor(new_background_color);
  }

  void ChangeThichnessOfEdge(float new_thickness_of_edge) {
    facade_->ChangeThichnessOfEdge(new_thickness_of_edge);
  }

  void ChangeSizeOfVertex(float new_size_of_vertex) {
    facade_->ChangeSizeOfVertex(new_size_of_vertex);
  }

  void ChangeEdgeColor(float new_edge_color[3]) {
    facade_->ChangeEdgeColor(new_edge_color);
  }

  void ChangeVertexColor(float new_vertex_color[3]) {
    facade_->ChangeVertexColor(new_vertex_color);
  }
  /** @} */

  /**
   * @brief Метод для масштабирования фигуры
   * @param x, y, z параметры для создания матрицы преобразований
   */
  void ScaleScene(double x, double y, double z) {
    facade_->ScaleScene(x, y, z);
  }

  /**
   * @brief Метод для поворота фигуры
   * @param x, y, z параметры для создания матрицы преобразований
   */
  void RotateScene(double x, double y, double z) {
    facade_->RotateScene(x, y, z);
  }

  /**
   * @brief Метод для перемещение фигуры
   * @param x, y, z параметры для создания матрицы преобразований
   */
  void MoveScene(double x, double y, double z) { facade_->MoveScene(x, y, z); }

  /**
   * @brief Метод для загрузки фигуры
   * @param path путь до файла
   */
  void LoadScene(std::string path) {
    NormalizationParamaters params = {0, 1, 0.001, 0.001};

    facade_->LoadScene(path, params);
  }

  /**
   * @brief Метод для загрузки фигуры
   */
  SettingsView LoadSettingsView() { return facade_->LoadSettings(); }

  /**
   * @brief Метод для загрузки настроек
   */
  SettingsDrawer GetSettingsDrawer() { return facade_->GetSettingsDrawer(); }

  int GetCountEdges() { return facade_->GetCountEdges(); }

  int GetCountVertices() { return facade_->GetCountVertices(); }

 private:
  Facade* facade_;
};
}  // namespace s21
