#include "command.h"

namespace s21 {

void TakeScreenshotCommand::execute() {
  QString file_name =
      QFileDialog::getSaveFileName(widget_, "Сохранить скриншот", "",
                                   "Images (*.jpeg *.bmp);;All Files (*)");

  if (!file_name.isEmpty()) {
    if (!file_name.endsWith(".jpeg", Qt::CaseInsensitive)) {
      file_name += ".jpeg";
    }

    QImage screen = widget_->getFramebuffer();
    screen.save(file_name);
    QMessageBox success_msg;
    success_msg.setText("Printscreen saved successfully!");
    success_msg.exec();
  }
}

void MakeGifCommand::execute() {
  gif_name_ = QFileDialog::getSaveFileName(widget_, "Сохранить gif", "",
                                           "Gif (*.gif);;All Files (*)");

  if (!gif_name_.isEmpty()) {
    if (!gif_name_.endsWith(".gif", Qt::CaseInsensitive)) {
      gif_name_ += ".gif";
    }
    gif_timer_->setInterval(100);
    gif_timer_->start();
  }
}

void ScalePlusCommand::execute() {
  controller_->ScaleScene(1.1, 1.1, 1.1);
  glview_->update();
}

void ScaleMinusCommand::execute() {
  controller_->ScaleScene(0.9, 0.9, 0.9);
  glview_->update();
}

void LoadModelCommand::execute() {
  fileName_ = QFileDialog::getOpenFileName(widget_, "Выбрать файл", "",
                                           "All Files (*.obj)");
  if (!fileName_.isEmpty()) {
    controller_->LoadScene(fileName_.toStdString());
    glview_->update();
  }
}

}