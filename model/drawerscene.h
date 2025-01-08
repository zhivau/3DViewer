#pragma once

#include <QFileDialog>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QSettings>
#include <QVector3D>

#include "scene.h"

namespace s21 {
/**
 * @class QtSceneDrawer
 * @brief Класс для отрисовки сцены в OpenGL с использованием настроек
 * пользователя.
 */
class SceneDrawerBase {
 public:
  virtual void DrawScene(Scene& scene) = 0;
  virtual ~SceneDrawerBase() = default;
};

class QtSceneDrawer : public SceneDrawerBase, protected QOpenGLFunctions {
 public:
  ~QtSceneDrawer() {
    writeSettingsDrawer();
  };
  /**
   * @brief Отрисовывает сцену с вершинами и рёбрами.
   * @param scene Сцена, содержащая фигуры.
   */
  void DrawScene(Scene& scene) override;
  /**
   * @brief Инициализирует функции OpenGL.
   */
  void initGL();
  /**
   * @brief Изменяет тип рёбер.
   * @param new_type_of_edge Новый тип рёбер (сплошной или пунктирный).
   */
  void changeTypeOfEdge(TypeEdge new_type_of_edge);
  /**
   * @brief Изменяет тип вершин.
   * @param new_type_of_vertex Новый тип вершин (круг, квадрат или без
   * отображения).
   */
  void changeTypeOfVertex(TypeVertex new_type_of_vertex);
  /**
   * @brief Изменяет цвет фона.
   * @param new_background_color Новый цвет фона (массив из 3 элементов).
   */
  void changeBackgoundColor(float new_background_color[3]);
  /**
   * @brief Изменяет толщину рёбер.
   * @param new_thickness_of_edge Новая толщина рёбер.
   */
  void changeThichnessOfEdge(float new_thickness_of_edge);
  /**
   * @brief Изменяет размер вершин.
   * @param new_size_of_vertex Новый размер вершин.
   */
  void changeSizeOfVertex(float new_size_of_vertex);
  /**
   * @brief Изменяет цвет рёбер.
   * @param new_edge_color Новый цвет рёбер (массив из 3 элементов).
   */
  void changeEdgeColor(float new_edge_color[3]);
  /**
   * @brief Изменяет цвет вершин.
   * @param new_vertex_color Новый цвет вершин (массив из 3 элементов).
   */
  void changeVertexColor(float new_vertex_color[3]);

  void setSettings(SettingsDrawer settings) { settings_ = settings; };
  SettingsDrawer getSettings() { return settings_; };

 private:
  /**
   * @brief Устанавливает стиль отображения вершин (точек).
   */
  void setStyleVertex();
  /**
   * @brief Устанавливает стиль отображения рёбер (линий).
   */
  void setStyleEdge();
  /**
   * @brief Сохраняет настройки отрисовщика в файл settings.ini.
   */
  void writeSettingsDrawer();

  SettingsDrawer settings_;
};
}  // namespace s21
