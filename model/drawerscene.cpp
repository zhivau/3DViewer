#include "drawerscene.h"
#include <QThread>

namespace s21 {

void QtSceneDrawer::initGL() {
  initializeOpenGLFunctions();
}

void QtSceneDrawer::setStyleVertex() {
  if (settings_.type_of_vertex == TypeVertex::NONE) {
    glColor3f(settings_.vertex_color[0], settings_.vertex_color[1],
              settings_.vertex_color[2]);
    glPointSize(settings_.size_of_vertex);
  } else if (settings_.type_of_vertex == TypeVertex::CIRCLE) {
    glColor3f(settings_.vertex_color[0], settings_.vertex_color[1],
              settings_.vertex_color[2]);
    glPointSize(settings_.size_of_vertex);
    glEnable(GL_POINT_SMOOTH);
  } else if (settings_.type_of_vertex == TypeVertex::SQUARE) {
    glColor3f(settings_.vertex_color[0], settings_.vertex_color[1],
              settings_.vertex_color[2]);
    if (settings_.size_of_vertex < 10) {
      glPointSize(10);
    } else {
      glPointSize(settings_.size_of_vertex);
    }
  }
}

void QtSceneDrawer::setStyleEdge() {
  if (settings_.type_of_edge == TypeEdge::SOLID) {
    glLineWidth(settings_.thickness_of_edge);
    glColor3f(settings_.edge_color[0], settings_.edge_color[1],
              settings_.edge_color[2]);
  } else if (settings_.type_of_edge == TypeEdge::DASHED) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00FF);

    glLineWidth(settings_.thickness_of_edge);
    glColor3f(settings_.edge_color[0], settings_.edge_color[1],
              settings_.edge_color[2]);
  }
}

void QtSceneDrawer::changeTypeOfEdge(TypeEdge new_type_of_edge) {
  settings_.type_of_edge = new_type_of_edge;
}

void QtSceneDrawer::changeTypeOfVertex(TypeVertex new_type_of_vertex) {
  settings_.type_of_vertex = new_type_of_vertex;
}

void QtSceneDrawer::changeBackgoundColor(float new_background_color[3]) {
  settings_.background_color[0] = new_background_color[0];
  settings_.background_color[1] = new_background_color[1];
  settings_.background_color[2] = new_background_color[2];
}

void QtSceneDrawer::changeThichnessOfEdge(float new_thickness_of_edge) {
  settings_.thickness_of_edge = new_thickness_of_edge;
}

void QtSceneDrawer::changeSizeOfVertex(float new_size_of_vertex) {
  settings_.size_of_vertex = new_size_of_vertex;
}

void QtSceneDrawer::changeEdgeColor(float new_edge_color[3]) {
  settings_.edge_color[0] = new_edge_color[0];
  settings_.edge_color[1] = new_edge_color[1];
  settings_.edge_color[2] = new_edge_color[2];
}

void QtSceneDrawer::changeVertexColor(float new_vertex_color[3]) {
  settings_.vertex_color[0] = new_vertex_color[0];
  settings_.vertex_color[1] = new_vertex_color[1];
  settings_.vertex_color[2] = new_vertex_color[2];
}

void QtSceneDrawer::DrawScene(Scene& scene) {
  glEnableClientState(GL_VERTEX_ARRAY);

  glVertexPointer(
      3, GL_FLOAT, 0,
      nullptr);
  glClearColor(settings_.background_color[0], settings_.background_color[1],
               settings_.background_color[2], 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  setStyleVertex();
  glBegin(GL_POINTS);

  for (const Vertex& vertex : scene.GetFigure().GetVertices()) {
    Point3D pos = vertex.GetPosition();
    glVertex3f(pos.x, pos.y, pos.z);
  }
  glEnd();

  if (settings_.type_of_vertex == TypeVertex::CIRCLE) {
    glDisable(GL_POINT_SMOOTH);
  }

  setStyleEdge();
  glBegin(GL_LINES);
  for (const Edge& edge : scene.GetFigure().GetEdges()) {
    const Vertex& begin = edge.GetBegin();
    const Vertex& end = edge.GetEnd();

    Point3D posBegin = begin.GetPosition();
    Point3D posEnd = end.GetPosition();

    glVertex3f(posBegin.x, posBegin.y, posBegin.z);
    glVertex3f(posEnd.x, posEnd.y, posEnd.z);
  }
  glEnd();

  if (settings_.type_of_edge == TypeEdge::DASHED) {
    glDisable(GL_LINE_STIPPLE);
  }

  glDisableClientState(GL_VERTEX_ARRAY);
}

void QtSceneDrawer::writeSettingsDrawer() {
  QString currentPath = QDir::currentPath();

  QSettings settingsFile(currentPath + "/settings.ini", QSettings::IniFormat);

  settingsFile.beginGroup("SettingsDrawer");
  settingsFile.setValue("type_of_edge",
                        static_cast<int>(settings_.type_of_edge));
  settingsFile.setValue("type_of_vertex",
                        static_cast<int>(settings_.type_of_vertex));
  settingsFile.setValue("background_color_r", settings_.background_color[0]);
  settingsFile.setValue("background_color_g", settings_.background_color[1]);
  settingsFile.setValue("background_color_b", settings_.background_color[2]);
  settingsFile.setValue("thickness_of_edge", settings_.thickness_of_edge);
  settingsFile.setValue("size_of_vertex", settings_.size_of_vertex);
  settingsFile.setValue("edge_color_r", settings_.edge_color[0]);
  settingsFile.setValue("edge_color_g", settings_.edge_color[1]);
  settingsFile.setValue("edge_color_b", settings_.edge_color[2]);
  settingsFile.setValue("vertex_color_r", settings_.vertex_color[0]);
  settingsFile.setValue("vertex_color_g", settings_.vertex_color[1]);
  settingsFile.setValue("vertex_color_b", settings_.vertex_color[2]);
  settingsFile.endGroup();
}

}