#pragma once

#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QKeySequence>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRadioButton>
#include <QScrollBar>
#include <QShortcut>
#include <QSlider>
#include <QString>
#include <QWidget>

#include "QtGifImage/gifimage/qgifimage.h"
#include "controller.h"
#include "glview.h"

namespace s21 {
/**
 * @class Command
 * @brief Абстрактный класс команд
 */

class Command {
 public:
  virtual ~Command() {}
  virtual void execute() = 0;
};

/**
 * @class Widget
 * @brief Главный виджет отображения
 */

class Widget : public QWidget {
  Q_OBJECT

 public:
  Widget(Controller *controller, QWidget *parent = nullptr);
  ~Widget() override;

  QImage getFramebuffer();

 private slots:
  void onLoadModelClicked();
  void onScalePlusClicked();
  void onScaleMinusClicked();

  void changeScale();

  void changePositionX();
  void changePositionY();
  void changePositionZ();

  void changeRotatationX();
  void changeRotatationY();
  void changeRotatationZ();

  void changeThicknessEdge();
  void changeSizeVertex();

  void changeColor();

  void takeScreenshot();
  void makeGif();

  void recordingGif();

 private:
  Controller *controller_;
  GlView *glview_;

  Command *takeScreenshotCommand_;
  Command *makeGifCommand_;

  Command *scalePlusCommand_;
  Command *scaleMinusCommand_;

  Command *loadModelCommand_;

  QString gif_name_;
  QTimer *gif_timer_;

  QString fileName_;
  QLabel *label_;

  QPushButton *loadModelButton_;
  QPushButton *quit_;

  QPushButton *scalePlusButton_;
  QPushButton *scaleMinusButton_;

  QScrollBar *moveX_, *moveY_, *moveZ_;
  int previousPosition[3];

  QScrollBar *rotateX_, *rotateY_, *rotateZ_;
  int previousRotation[3];

  QScrollBar *rSlider_, *gSlider_, *bSlider_;

  QLineEdit *inputTranslateX_, *inputTranslateY_, *inputTranslateZ_;
  QLineEdit *inputRotateX_, *inputRotateY_, *inputRotateZ_;

  QLineEdit *inputScale_;
  QPushButton *applyScale_;

  QGroupBox *projectionGroup_, *colorGroup_;
  QGridLayout *colorLayout_, *projectionLayout_;
  QRadioButton *backgroundButton_, *lineButton_, *pointButton_;
  int selectedObjectToChangeColor_;

  QRadioButton *parallelProjectionButton_, *centralProjectionButton_;

  QGroupBox *translateGroup_, *rotateGroup_, *scaleGroup_;
  QGridLayout *gridLayout_, *translateLayout_, *rotateLayout_, *scaleLayout_;

  QGroupBox *lineGroup_;
  QGridLayout *lineLayout_;
  QRadioButton *solid_, *dashed_;
  QScrollBar *thickness_;

  QGroupBox *vertexGroup_;
  QGridLayout *vertexLayout_;
  QRadioButton *nothingVertexRadioButton_, *circle_, *square_;
  QScrollBar *size_;

  QPushButton *screenshotBmpJpg_, *gif_;

  QLabel *countVertices_, *countEdges_;

  /**
   * @defgroup createConnect
   * группа методов , которая создает connect для бегунков и полей ввода
   * @{
   */
  void createConnectForTranslate();
  void createConnectForRotate();
  void createConnectForScale();
  void createConnectForRadioButtonColor();
  void createConnectForRGB();
  void createConnectForLine();
  void createConnectForVertex();
  /** @} */
  /**
   * @defgroup initQScrollBar
   * группа методов , которая инициализирует бегунки
   * @{
   */
  void initQScrollBarForTranslate();
  void initQScrollBarForRotate();
  void initButtonsForScale();
  void initQScrollBarForRGB();
  /** @} */
  /**
   * @defgroup initQLineEdit
   * группа методов , которая инициализирует поля ввода
   * @{
   */
  void initQLineEditForTranslate();
  void initQLineEditForRotate();
  void initQLineEditForScale();
  /** @} */

  /**
   * @defgroup initGroupGrid
   * группа методов , которая инициализирует сетку для размещения элементов
   * управления моделью
   * @{
   */
  void initGroupTranslate();
  void initGroupRotate();
  void initGroupScale();
  void initGroupColor();
  void initGroupProjection();
  void initGroupLine();
  void initGroupVertex();
  ///
  /// \brief initMainGrid
  /// главная сетка для размещения групп управления моделью
  ///
  void initMainGrid();
  /** @} */

  void showPathFile();
};

}  // namespace s21
