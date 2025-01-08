#include "widget.h"
#include "command.h"

namespace s21 {

Widget::Widget(Controller *controller, QWidget *parent)
    : QWidget(parent = nullptr), controller_(controller) {
  setWindowTitle("3D Viewer");
  setAttribute(Qt::WA_DeleteOnClose);
  glview_ = new GlView(controller_);

  QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

  this->setMinimumHeight(800);
  this->setMinimumWidth(1200);
  label_ = new QLabel(this);
  showPathFile();

  screenshotBmpJpg_ = new QPushButton("Screenshot", this);
  gif_ = new QPushButton("GIF", this);
  loadModelButton_ = new QPushButton("Load Model", this);
  quit_ = new QPushButton("Quit", this);

  gif_timer_ = new QTimer(this);
  takeScreenshotCommand_ = new TakeScreenshotCommand(this);
  makeGifCommand_ = new MakeGifCommand(this, gif_timer_, gif_name_);

  scalePlusCommand_ = new ScalePlusCommand(controller_, glview_);
  scaleMinusCommand_ = new ScaleMinusCommand(controller_, glview_);

  loadModelCommand_ =
      new LoadModelCommand(this, glview_, controller_, fileName_);

  QShortcut *takeScreenshotShotrcut =
      new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_S), this);
  QShortcut *makeGifShotcut =
      new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_G), this);
  QShortcut *scalePlusShortcut =
      new QShortcut(QKeySequence(Qt::Key_Equal), this);
  QShortcut *scaleMinusShortcut =
      new QShortcut(QKeySequence(Qt::Key_Minus), this);
  QShortcut *loadModelShortcut =
      new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_O), this);

  initMainGrid();

  connect(quit_, &QPushButton::clicked, this, &Widget::close);
  connect(loadModelButton_, &QPushButton::clicked, this,
          &Widget::onLoadModelClicked);

  connect(screenshotBmpJpg_, &QPushButton::clicked, this,
          &Widget::takeScreenshot);
  connect(gif_, &QPushButton::clicked, this, &Widget::makeGif);
  connect(gif_timer_, &QTimer::timeout, this, &Widget::recordingGif);
  connect(scaleMinusButton_, &QPushButton::clicked, this,
          &Widget::onScaleMinusClicked);
  connect(scalePlusButton_, &QPushButton::clicked, this,
          &Widget::onScalePlusClicked);

  connect(takeScreenshotShotrcut, &QShortcut::activated, this,
          &Widget::takeScreenshot);
  connect(makeGifShotcut, &QShortcut::activated, this, &Widget::makeGif);
  connect(scalePlusShortcut, &QShortcut::activated, this,
          &Widget::onScalePlusClicked);
  connect(scaleMinusShortcut, &QShortcut::activated, this,
          &Widget::onScaleMinusClicked);
  connect(loadModelShortcut, &QShortcut::activated, this,
          &Widget::onLoadModelClicked);

  createConnectForTranslate();
  createConnectForRotate();

  createConnectForRadioButtonColor();
  createConnectForRGB();

  createConnectForLine();
  createConnectForVertex();

  connect(centralProjectionButton_, &QRadioButton::toggled, this,
          [=](bool checked) {
            if (checked) {
              glview_->changeProjection(TypeProjection::CENTRAL);
              glview_->update();
            }
          });

  connect(parallelProjectionButton_, &QRadioButton::toggled, this,
          [=](bool checked) {
            if (checked) {
              glview_->changeProjection(TypeProjection::PARALLEL);
              glview_->update();
            }
          });

  connect(moveX_, &QSlider::valueChanged, this, &Widget::changePositionX);
  connect(moveY_, &QSlider::valueChanged, this, &Widget::changePositionY);
  connect(moveZ_, &QSlider::valueChanged, this, &Widget::changePositionZ);

  connect(rotateX_, &QSlider::valueChanged, this, &Widget::changeRotatationX);
  connect(rotateY_, &QSlider::valueChanged, this, &Widget::changeRotatationY);
  connect(rotateZ_, &QSlider::valueChanged, this, &Widget::changeRotatationZ);

  connect(applyScale_, &QPushButton::clicked, this, &Widget::changeScale);

  connect(thickness_, &QSlider::valueChanged, this,
          &Widget::changeThicknessEdge);
  connect(size_, &QSlider::valueChanged, this, &Widget::changeSizeVertex);
}

Widget::~Widget() {
  delete controller_;
}

QImage Widget::getFramebuffer() { return glview_->grabFramebuffer(); }

void Widget::initMainGrid() {
  initGroupTranslate();
  initGroupRotate();
  initGroupScale();
  initGroupColor();
  initGroupProjection();
  initGroupLine();
  initGroupVertex();

  gridLayout_ = new QGridLayout(this);

  countEdges_ = new QLabel("The count of edges: " +
                           QString::number(controller_->GetCountEdges()));
  countVertices_ = new QLabel("The count of vertices: " +
                              QString::number(controller_->GetCountVertices()));

  gridLayout_->addWidget(countEdges_, 1, 0, 1, 3);
  gridLayout_->addWidget(countVertices_, 1, 4, 1, 3);
  gridLayout_->addWidget(glview_, 2, 0, 50,
                         8);  // Окно отрисовки занимает 5 строк и 4 столбца
  gridLayout_->addWidget(loadModelButton_, 0, 8, 2,
                         3);  // Кнопка загрузки модели
  gridLayout_->addWidget(translateGroup_, 2, 8, 5, 3);
  gridLayout_->addWidget(rotateGroup_, 7, 8, 5, 3);
  gridLayout_->addWidget(scaleGroup_, 12, 8, 3, 3);
  gridLayout_->addWidget(colorGroup_, 15, 8, 7, 3);
  gridLayout_->addWidget(projectionGroup_, 22, 8, 2, 3);
  gridLayout_->addWidget(lineGroup_, 24, 8, 5, 3);
  gridLayout_->addWidget(vertexGroup_, 29, 8, 5, 3);
  gridLayout_->addWidget(screenshotBmpJpg_, 35, 8, 2, 3);
  gridLayout_->addWidget(gif_, 37, 8, 2, 3);
  gridLayout_->addWidget(quit_, 50, 10, 2, 1);
  setLayout(gridLayout_);
}

void Widget::initGroupTranslate() {
  initQScrollBarForTranslate();
  initQLineEditForTranslate();
  translateGroup_ = new QGroupBox("Translate:");
  translateLayout_ = new QGridLayout;

  translateLayout_->addWidget(new QLabel("X:"), 0, 0);
  translateLayout_->addWidget(new QLabel("Y:"), 1, 0);
  translateLayout_->addWidget(new QLabel("Z:"), 2, 0);
  translateLayout_->addWidget(moveX_, 0, 0, 1, 2);
  translateLayout_->addWidget(moveY_, 1, 0, 1, 2);
  translateLayout_->addWidget(moveZ_, 2, 0, 1, 2);
  translateLayout_->addWidget(inputTranslateX_, 0, 3);
  translateLayout_->addWidget(inputTranslateY_, 1, 3);
  translateLayout_->addWidget(inputTranslateZ_, 2, 3);

  translateLayout_->setVerticalSpacing(5);
  translateLayout_->setContentsMargins(5, 5, 5, 5);
  translateGroup_->setLayout(translateLayout_);
}
void Widget::initGroupRotate() {
  initQScrollBarForRotate();
  initQLineEditForRotate();
  rotateGroup_ = new QGroupBox("Rotate:");
  rotateLayout_ = new QGridLayout;
  rotateLayout_->addWidget(new QLabel("X:"), 6, 8);
  rotateLayout_->addWidget(new QLabel("Y:"), 7, 8);
  rotateLayout_->addWidget(new QLabel("Z:"), 8, 8);
  rotateLayout_->addWidget(rotateX_, 6, 8, 1, 2);
  rotateLayout_->addWidget(rotateY_, 7, 8, 1, 2);
  rotateLayout_->addWidget(rotateZ_, 8, 8, 1, 2);
  rotateLayout_->addWidget(inputRotateX_, 6, 10);
  rotateLayout_->addWidget(inputRotateY_, 7, 10);
  rotateLayout_->addWidget(inputRotateZ_, 8, 10);
  rotateLayout_->setVerticalSpacing(5);
  rotateLayout_->setContentsMargins(5, 5, 5, 5);
  rotateGroup_->setLayout(rotateLayout_);
}
void Widget::initGroupScale() {
  initButtonsForScale();
  initQLineEditForScale();
  scaleGroup_ = new QGroupBox("Scale:");
  scaleLayout_ = new QGridLayout;
  scaleLayout_->addWidget(scaleMinusButton_, 10, 8, 1, 1);
  scaleLayout_->addWidget(scalePlusButton_, 10, 9, 1, 1);
  scaleLayout_->addWidget(applyScale_, 10, 11, 1, 1);
  scaleLayout_->addWidget(inputScale_, 10, 10);
  scaleLayout_->setContentsMargins(5, 5, 5, 5);
  scaleGroup_->setLayout(scaleLayout_);
}
void Widget::initGroupColor() {
  initQScrollBarForRGB();
  colorGroup_ = new QGroupBox("Color: ");
  backgroundButton_ = new QRadioButton("background");
  lineButton_ = new QRadioButton("line");
  pointButton_ = new QRadioButton("point");
  backgroundButton_->setChecked(true);
  colorLayout_ = new QGridLayout;

  colorLayout_->addWidget(backgroundButton_, 0, 0);
  colorLayout_->addWidget(lineButton_, 0, 1);
  colorLayout_->addWidget(pointButton_, 0, 2);
  colorLayout_->addWidget(new QLabel("R:"), 1, 0);
  colorLayout_->addWidget(rSlider_, 1, 0, 1, 3);
  colorLayout_->addWidget(new QLabel("G:"), 2, 0);
  colorLayout_->addWidget(gSlider_, 2, 0, 1, 3);
  colorLayout_->addWidget(new QLabel("B:"), 3, 0);
  colorLayout_->addWidget(bSlider_, 3, 0, 1, 3);
  colorLayout_->setVerticalSpacing(5);
  colorLayout_->setContentsMargins(5, 5, 5, 5);
  colorGroup_->setLayout(colorLayout_);
}
void Widget::initGroupProjection() {
  SettingsView settings_viewer = glview_->getSettings();

  projectionGroup_ = new QGroupBox("Projection: ");
  projectionLayout_ = new QGridLayout;
  parallelProjectionButton_ = new QRadioButton("parallel");
  centralProjectionButton_ = new QRadioButton("central");

  if (settings_viewer.type_of_projection == TypeProjection::PARALLEL) {
    parallelProjectionButton_->setChecked(true);
  } else if (settings_viewer.type_of_projection == TypeProjection::CENTRAL) {
    centralProjectionButton_->setChecked(true);
  }

  projectionLayout_->addWidget(parallelProjectionButton_, 0, 0);
  projectionLayout_->addWidget(centralProjectionButton_, 0, 1);
  projectionLayout_->setContentsMargins(5, 5, 5, 5);

  projectionGroup_->setLayout(projectionLayout_);
}
void Widget::initGroupLine() {
  SettingsDrawer settings_drawer = controller_->GetSettingsDrawer();

  lineGroup_ = new QGroupBox("Line");
  lineLayout_ = new QGridLayout;
  solid_ = new QRadioButton("solid");
  dashed_ = new QRadioButton("dashed");

  if (settings_drawer.type_of_edge == TypeEdge::SOLID) {
    solid_->setChecked(true);
  } else if (settings_drawer.type_of_edge == TypeEdge::DASHED) {
    dashed_->setChecked(true);
  }

  thickness_ = new QScrollBar(Qt::Horizontal);
  thickness_->setRange(1, 10);
  thickness_->setValue(settings_drawer.thickness_of_edge);
  lineLayout_->addWidget(solid_, 0, 0);
  lineLayout_->addWidget(dashed_, 0, 1);
  lineLayout_->addWidget(new QLabel("thickness"), 1, 0);
  lineLayout_->addWidget(thickness_, 1, 1, 1, 2);
  lineLayout_->setVerticalSpacing(5);
  lineLayout_->setContentsMargins(5, 5, 5, 5);
  lineGroup_->setLayout(lineLayout_);
}
void Widget::initGroupVertex() {
  SettingsDrawer settings_drawer = controller_->GetSettingsDrawer();
  vertexGroup_ = new QGroupBox("Vertex");
  vertexLayout_ = new QGridLayout;
  nothingVertexRadioButton_ = new QRadioButton("nothing");
  circle_ = new QRadioButton("circle");
  square_ = new QRadioButton("square");

  if (settings_drawer.type_of_vertex == TypeVertex::NONE) {
    nothingVertexRadioButton_->setChecked(true);
  } else if (settings_drawer.type_of_vertex == TypeVertex::CIRCLE) {
    circle_->setChecked(true);
  } else if (settings_drawer.type_of_vertex == TypeVertex::SQUARE) {
    square_->setChecked(true);
  }

  size_ = new QScrollBar(Qt::Horizontal);
  size_->setRange(1, 10);
  size_->setValue(settings_drawer.size_of_vertex);
  vertexLayout_->addWidget(nothingVertexRadioButton_, 0, 0);
  vertexLayout_->addWidget(circle_, 0, 1);
  vertexLayout_->addWidget(square_, 0, 2);
  vertexLayout_->addWidget(new QLabel("size"), 1, 0);
  vertexLayout_->addWidget(size_, 1, 1, 1, 2);
  vertexLayout_->setVerticalSpacing(5);
  vertexLayout_->setContentsMargins(5, 5, 5, 5);
  vertexGroup_->setLayout(vertexLayout_);
}

void Widget::createConnectForTranslate() {
  connect(moveX_, &QScrollBar::valueChanged, this, [this](int value) {
    inputTranslateX_->setText(QString::number(value));
  });
  connect(inputTranslateX_, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            int value = text.toInt();
            moveX_->setValue(value);
          });
  connect(moveY_, &QScrollBar::valueChanged, this, [this](int value) {
    inputTranslateY_->setText(QString::number(value));
  });
  connect(inputTranslateY_, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            int value = text.toInt();
            moveY_->setValue(value);
          });
  connect(moveZ_, &QScrollBar::valueChanged, this, [this](int value) {
    inputTranslateZ_->setText(QString::number(value));
  });
  connect(inputTranslateZ_, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            int value = text.toInt();
            moveZ_->setValue(value);
          });
}
void Widget::createConnectForRotate() {
  connect(rotateX_, &QScrollBar::valueChanged, this, [this](int value) {
    inputRotateX_->setText(QString::number(value));
  });
  connect(inputRotateX_, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            int value = text.toInt();
            rotateX_->setValue(value);
          });
  connect(rotateY_, &QScrollBar::valueChanged, this, [this](int value) {
    inputRotateY_->setText(QString::number(value));
  });
  connect(inputRotateY_, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            int value = text.toInt();
            rotateY_->setValue(value);
          });
  connect(rotateZ_, &QScrollBar::valueChanged, this, [this](int value) {
    inputRotateZ_->setText(QString::number(value));
  });
  connect(inputRotateZ_, &QLineEdit::textChanged, this,
          [this](const QString &text) {
            int value = text.toInt();
            rotateZ_->setValue(value);
          });
}

void Widget::createConnectForRadioButtonColor() {
  connect(backgroundButton_, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      rSlider_->blockSignals(true);
      gSlider_->blockSignals(true);
      bSlider_->blockSignals(true);

      selectedObjectToChangeColor_ = 0;
      rSlider_->setValue(controller_->GetSettingsDrawer().background_color[0] *
                         255);
      gSlider_->setValue(controller_->GetSettingsDrawer().background_color[1] *
                         255);
      bSlider_->setValue(controller_->GetSettingsDrawer().background_color[2] *
                         255);

      rSlider_->blockSignals(false);
      gSlider_->blockSignals(false);
      bSlider_->blockSignals(false);
    }
  });

  connect(lineButton_, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      rSlider_->blockSignals(true);
      gSlider_->blockSignals(true);
      bSlider_->blockSignals(true);

      selectedObjectToChangeColor_ = 1;
      rSlider_->setValue(controller_->GetSettingsDrawer().edge_color[0] * 255);
      gSlider_->setValue(controller_->GetSettingsDrawer().edge_color[1] * 255);
      bSlider_->setValue(controller_->GetSettingsDrawer().edge_color[2] * 255);

      rSlider_->blockSignals(false);
      gSlider_->blockSignals(false);
      bSlider_->blockSignals(false);
    }
  });
  connect(pointButton_, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      rSlider_->blockSignals(true);
      gSlider_->blockSignals(true);
      bSlider_->blockSignals(true);

      selectedObjectToChangeColor_ = 2;
      rSlider_->setValue(controller_->GetSettingsDrawer().vertex_color[0] *
                         255);
      gSlider_->setValue(controller_->GetSettingsDrawer().vertex_color[1] *
                         255);
      bSlider_->setValue(controller_->GetSettingsDrawer().vertex_color[2] *
                         255);

      rSlider_->blockSignals(false);
      gSlider_->blockSignals(false);
      bSlider_->blockSignals(false);
    }
  });
}

void Widget::createConnectForRGB() {
  connect(rSlider_, &QSlider::valueChanged, this, &Widget::changeColor);
  connect(gSlider_, &QSlider::valueChanged, this, &Widget::changeColor);
  connect(bSlider_, &QSlider::valueChanged, this, &Widget::changeColor);
}

void Widget::createConnectForLine() {
  connect(solid_, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      controller_->ChangeTypeOfEdge(TypeEdge::SOLID);
      glview_->update();
    }
  });

  connect(dashed_, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      controller_->ChangeTypeOfEdge(TypeEdge::DASHED);
      glview_->update();
    }
  });
}
void Widget::createConnectForVertex() {
  connect(nothingVertexRadioButton_, &QRadioButton::toggled, this,
          [=](bool checked) {
            if (checked) {
              controller_->ChangeTypeOfVertex(TypeVertex::NONE);
              glview_->update();
            }
          });

  connect(circle_, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      controller_->ChangeTypeOfVertex(TypeVertex::CIRCLE);
      glview_->update();
    }
  });
  connect(square_, &QRadioButton::toggled, this, [=](bool checked) {
    if (checked) {
      controller_->ChangeTypeOfVertex(TypeVertex::SQUARE);
      glview_->update();
    }
  });
}

void Widget::initQScrollBarForTranslate() {
  moveX_ = new QScrollBar(Qt::Horizontal);
  moveY_ = new QScrollBar(Qt::Horizontal);
  moveZ_ = new QScrollBar(Qt::Horizontal);
  moveX_->setRange(-100, 100);
  moveX_->setValue(0);
  moveY_->setRange(-100, 100);
  moveY_->setValue(0);
  moveZ_->setRange(-100, 100);
  moveZ_->setValue(0);
}
void Widget::initQScrollBarForRotate() {
  rotateX_ = new QScrollBar(Qt::Horizontal);
  rotateY_ = new QScrollBar(Qt::Horizontal);
  rotateZ_ = new QScrollBar(Qt::Horizontal);
  rotateX_->setRange(-180, 180);
  rotateX_->setValue(0);
  rotateY_->setRange(-180, 180);
  rotateY_->setValue(0);
  rotateZ_->setRange(-180, 180);
  rotateZ_->setValue(0);
}
void Widget::initButtonsForScale() {
  scaleMinusButton_ = new QPushButton("size-", this);
  scalePlusButton_ = new QPushButton("size+", this);
}
void Widget::initQScrollBarForRGB() {
  rSlider_ = new QScrollBar(Qt::Horizontal);
  gSlider_ = new QScrollBar(Qt::Horizontal);
  bSlider_ = new QScrollBar(Qt::Horizontal);
  rSlider_->setRange(0, 255);
  gSlider_->setRange(0, 255);
  bSlider_->setRange(0, 255);

  SettingsDrawer settingsDrawer = controller_->GetSettingsDrawer();

  rSlider_->setValue(settingsDrawer.background_color[0] * 255);
  gSlider_->setValue(settingsDrawer.background_color[1] * 255);
  bSlider_->setValue(settingsDrawer.background_color[2] * 255);
}

void Widget::initQLineEditForTranslate() {
  inputTranslateX_ = new QLineEdit(this);
  inputTranslateY_ = new QLineEdit(this);
  inputTranslateZ_ = new QLineEdit(this);
  inputTranslateX_->setMaximumWidth(30);
  inputTranslateY_->setMaximumWidth(30);
  inputTranslateZ_->setMaximumWidth(30);
  inputTranslateX_->setText(QString::number(moveX_->value()));
  inputTranslateY_->setText(QString::number(moveY_->value()));
  inputTranslateZ_->setText(QString::number(moveZ_->value()));
  inputTranslateX_->setValidator(new QIntValidator(-100, 100, this));
  inputTranslateY_->setValidator(new QIntValidator(-100, 100, this));
  inputTranslateZ_->setValidator(new QIntValidator(-100, 100, this));
}
void Widget::initQLineEditForRotate() {
  inputRotateX_ = new QLineEdit(this);
  inputRotateY_ = new QLineEdit(this);
  inputRotateZ_ = new QLineEdit(this);
  inputRotateX_->setMaximumWidth(30);
  inputRotateY_->setMaximumWidth(30);
  inputRotateZ_->setMaximumWidth(30);
  inputRotateX_->setText(QString::number(rotateX_->value()));
  inputRotateY_->setText(QString::number(rotateY_->value()));
  inputRotateZ_->setText(QString::number(rotateZ_->value()));
  inputRotateX_->setValidator(new QIntValidator(-180, 180, this));
  inputRotateY_->setValidator(new QIntValidator(-180, 180, this));
  inputRotateZ_->setValidator(new QIntValidator(-180, 180, this));
}
void Widget::initQLineEditForScale() {
  inputScale_ = new QLineEdit(this);
  applyScale_ = new QPushButton("apply", this);
  inputScale_->setMaximumWidth(30);
  inputScale_->setValidator(new QDoubleValidator(0.01, 2.00, 2, this));
}

// void Widget::loadIconForButton() {
//   iconsForButtons_.append(QIcon(":/images/arrow-down-to-line-svgrepo-com.svg"));
//   iconsForButtons_.append(QIcon(":/images/arrow-rotate-right-svgrepo-com.svg"));
//   iconsForButtons_.append(QIcon(":/images/arrow-rotate-left-svgrepo-com.svg"));
//   iconsForButtons_.append(QIcon(":/images/plus-lens-svgrepo-com.svg"));
//   iconsForButtons_.append(QIcon(":/images/minus-lens-svgrepo-com.svg"));
// }

void Widget::onLoadModelClicked() {
  previousPosition[0] = 0;
  previousPosition[1] = 0;
  previousPosition[2] = 0;

  previousRotation[0] = 0;
  previousRotation[1] = 0;
  previousRotation[2] = 0;

  if (loadModelCommand_) {
    loadModelCommand_->execute();
  }

  countEdges_->setText("The count of edges: " +
                       QString::number(controller_->GetCountEdges()));
  countVertices_->setText("The count of vertices: " +
                          QString::number(controller_->GetCountVertices()));

  showPathFile();
}

void Widget::onScalePlusClicked() {
  if (scalePlusCommand_) {
    scalePlusCommand_->execute();
  }
}

void Widget::onScaleMinusClicked() {
  if (scaleMinusCommand_) {
    scaleMinusCommand_->execute();
  }
}

void Widget::changeRotatationX() {
  int shiftX = rotateX_->value() - previousRotation[0];
  double angleRotation = M_PI / 180 * shiftX;
  controller_->RotateScene(angleRotation, 0, 0);
  previousRotation[0] = rotateX_->value();
  glview_->update();
}

void Widget::changeRotatationY() {
  int shiftY = rotateY_->value() - previousRotation[1];
  double angleRotation = M_PI / 180 * shiftY;
  controller_->RotateScene(0, angleRotation, 0);
  previousRotation[1] = rotateY_->value();
  glview_->update();
}

void Widget::changeRotatationZ() {
  int shiftZ = rotateZ_->value() - previousRotation[2];
  double angleRotation = M_PI / 180 * shiftZ;
  controller_->RotateScene(0, 0, angleRotation);
  previousRotation[2] = rotateZ_->value();
  glview_->update();
}

void Widget::changeThicknessEdge() {
  controller_->ChangeThichnessOfEdge(thickness_->value());
  glview_->update();
}

void Widget::changeSizeVertex() {
  controller_->ChangeSizeOfVertex(size_->value());
  glview_->update();
}

void Widget::changeScale() {
  float scaleFactor = inputScale_->text().toFloat();

  qDebug() << scaleFactor;

  if (scaleFactor > 0) {
    controller_->ScaleScene(scaleFactor, scaleFactor, scaleFactor);
  }
  glview_->update();
}

void Widget::changePositionX() {
  int shiftX = moveX_->value() - previousPosition[0];

  controller_->MoveScene(shiftX * 0.01, 0.0, 0.0);
  previousPosition[0] = moveX_->value();

  glview_->update();
}

void Widget::changePositionY() {
  int shiftY = moveY_->value() - previousPosition[1];

  controller_->MoveScene(0.0, shiftY * 0.01, 0.0);
  previousPosition[1] = moveY_->value();

  glview_->update();
}

void Widget::changePositionZ() {
  int shiftZ = moveZ_->value() - previousPosition[2];

  controller_->MoveScene(0.0, 0.0, shiftZ * 0.01);
  previousPosition[2] = moveZ_->value();

  glview_->update();
}

void Widget::changeColor() {
  float new_background_color[3];

  new_background_color[0] = (float)rSlider_->value() / 255;
  new_background_color[1] = (float)gSlider_->value() / 255;
  new_background_color[2] = (float)bSlider_->value() / 255;

  if (selectedObjectToChangeColor_ == 0) {
    controller_->ChangeBackgoundColor(new_background_color);
  } else if (selectedObjectToChangeColor_ == 1) {
    controller_->ChangeEdgeColor(new_background_color);
  } else if (selectedObjectToChangeColor_ == 2) {
    controller_->ChangeVertexColor(new_background_color);
  }

  glview_->update();
}

void Widget::takeScreenshot() {
  if (takeScreenshotCommand_) {
    takeScreenshotCommand_->execute();
  }
}

void Widget::makeGif() {
  if (makeGifCommand_) {
    makeGifCommand_->execute();
  }
}

void Widget::recordingGif() {
  static int screen_counter = 0;
  static QGifImage *gif_frame;

  if (screen_counter == 0) {
    gif_frame = new QGifImage();
    gif_frame->setDefaultDelay(10);
    gif_->setEnabled(false);
  }

  if (screen_counter < 50) {
    QImage screen = glview_->grabFramebuffer();
    gif_frame->addFrame(screen);
    screen_counter++;
    gif_->setText(QString::number(screen_counter / 10, 10));
  } else {
    QMessageBox success_msg;
    success_msg.setText("Gif saved successfully!");
    success_msg.exec();
    gif_timer_->stop();
    gif_frame->save(gif_name_);
    screen_counter = 0;
    delete gif_frame;
    gif_->setText("Create gif");
    gif_->setEnabled(true);
  }
}

void Widget::showPathFile() {  // todo
  if (fileName_.isEmpty()) fileName_ = "path";
  label_->setMinimumWidth(600);
  label_->clear();
  label_->setText(fileName_);
  label_->update();
  label_->show();
}

}
