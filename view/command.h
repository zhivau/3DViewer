#pragma once

#include "widget.h"

namespace s21 {
/**
 * @class TakeScreenshotCommand
 * @brief Команда для скриншота
 */
class TakeScreenshotCommand : public s21::Command {
 public:
  TakeScreenshotCommand(s21::Widget* widget) : widget_(widget){};
  void execute() override;

 private:
  s21::Widget* widget_;
};

/**
 * @class MakeGifCommand
 * @brief Команда для гифки
 */
class MakeGifCommand : public s21::Command {
 public:
  MakeGifCommand(s21::Widget* widget, QTimer* gif_timer, QString& gif_name)
      : widget_(widget), gif_timer_(gif_timer), gif_name_(gif_name){};
  void execute() override;

 private:
  s21::Widget* widget_;
  QTimer* gif_timer_;
  QString& gif_name_;
};

/**
 * @class ScalePlusCommand
 * @brief Команда для увеличения масштаба
 */
class ScalePlusCommand : public s21::Command {
 public:
  ScalePlusCommand(Controller* controller, s21::GlView* glview)
      : controller_(controller), glview_(glview){};
  void execute() override;

 private:
  Controller* controller_;
  s21::GlView* glview_;
};

/**
 * @class ScaleMinusCommand
 * @brief Команда для уменьшения масштаба
 */
class ScaleMinusCommand : public s21::Command {
 public:
  ScaleMinusCommand(Controller* controller, s21::GlView* glview)
      : controller_(controller), glview_(glview){};
  void execute() override;

 private:
  Controller* controller_;
  s21::GlView* glview_;
};

/**
 * @class LoadModelCommand
 * @brief Команда для загрузки фигуры
 */
class LoadModelCommand : public s21::Command {
 public:
  LoadModelCommand(s21::Widget* widget, s21::GlView* glview,
                   Controller* controller, QString& fileName)
      : widget_(widget),
        glview_(glview),
        controller_(controller),
        fileName_(fileName){};
  void execute() override;

 private:
  s21::Widget* widget_;
  s21::GlView* glview_;
  Controller* controller_;
  QString& fileName_;
};

}  // namespace s21
