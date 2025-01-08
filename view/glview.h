#pragma once

#include <QFileDialog>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QVector3D>
#include <QWidget>
#include <QtOpenGL>

#include "controller.h"

namespace s21 {
/**
 * @class GlView
 * @brief Класс для работы с OpenGL в контексте Qt, используется для отображения
 * 3D-сцен.
 */
class GlView : public QOpenGLWidget, protected QOpenGLFunctions {
 public:
  GlView(Controller* controller, QWidget* parent = nullptr)
      : QOpenGLWidget(parent),
        controller_(controller),
        settings_(controller->LoadSettingsView()){};
  ~GlView() {
    writeSettingsView();
  };

  void changeProjection(TypeProjection new_type_of_projection) {
    settings_.type_of_projection = new_type_of_projection;
    settings_.is_updated_type_of_projection = true;
  };
  SettingsView getSettings() { return settings_; };

 protected:
  /**
   * @brief Инициализация OpenGL и установка основных параметров.
   */
  void initializeGL() override;  // Инициализация OpenGL
  /**
   * @brief Метод изменения размера окна отображения OpenGL.
   * @param w Новая ширина окна.
   * @param h Новая высота окна.
   */
  void resizeGL(int w, int h) override;  // Обработка изменения размера окна
  /**
   * @brief Основной метод для отрисовки сцены с использованием OpenGL.
   */
  void paintGL() override;  // Отрисовка сцены

 private:
  /**
   * @brief Обновляет параметры проекции (ортографической или перспективной) в
   * зависимости от настроек.
   */
  void updateProjection();
  /**
   * @brief Сохранение настроек отображения в INI-файл.
   */
  void writeSettingsView();

  Controller* controller_;
  SettingsView settings_;
};

}  // namespace s21
