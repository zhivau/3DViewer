#include "glview.h"

namespace s21 {

void GlView::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  updateProjection();
}

void GlView::updateProjection() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if (settings_.type_of_projection == TypeProjection::PARALLEL) {
    float left = -1.0f;
    float right = 2.0f;
    float bottom = -1.0f;
    float top = 2.0f;
    float nearPlane = -10.0f;
    float farPlane = 10.0f;
    glOrtho(left, right, bottom, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat ignoreZMatrix[16] = {
        1, 0, 0, 0, 0, 1,
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 1};
    glMultMatrixf(ignoreZMatrix);

  } else if (settings_.type_of_projection == TypeProjection::CENTRAL) {
    float left = -0.5f;
    float right = 0.5f;
    float bottom = -0.5f;
    float top = 0.5f;
    float nearPlane = 1.0f;
    float farPlane = 10.0f;

    glFrustum(left, right, bottom, top, nearPlane, farPlane);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(-0.47, -0.46, -3.0f);
    glRotatef(4, 1.0f, 0.0f, 0.0f);
    glRotatef(-1, 0.0f, 1.0f, 0.0f);
  }
  settings_.is_updated_type_of_projection = false;
}

void GlView::paintGL() {
  if (settings_.is_updated_type_of_projection) {
    updateProjection();
  }

  controller_->DrawScene();
}

void GlView::initializeGL() {
  initializeOpenGLFunctions();
  controller_->InitQtSceneDrawer();

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

  glEnable(GL_DEPTH_TEST);
}

void GlView::writeSettingsView() {
  QString currentPath = QDir::currentPath();

  QSettings settingsFile(currentPath + "/settings.ini", QSettings::IniFormat);

  settingsFile.beginGroup("SettingsView");
  settingsFile.setValue("type_of_projection",
                        static_cast<int>(settings_.type_of_projection));
  settingsFile.setValue("is_updated_type_of_projection",
                        settings_.is_updated_type_of_projection);
  settingsFile.endGroup();
}

}