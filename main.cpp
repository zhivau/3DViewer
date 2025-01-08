#include <QApplication>

#include "view/widget.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  s21::Facade* facade = new s21::Facade();
  s21::Controller* controller = new s21::Controller(facade);
  s21::Widget* w = new s21::Widget(controller);
  w->show();

  return a.exec();
}
