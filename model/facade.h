#pragma once

#include "baseObjects.h"
#include "drawerscene.h"
#include "fileReader.h"

namespace s21 {

class Facade {
 public:
  Facade();
  ~Facade() {
    delete sceneDrawer_;
  };

  OperationResult DrawScene();
  void InitQtSceneDrawer();
  void ChangeTypeOfEdge(TypeEdge new_type_of_edge);
  void ChangeTypeOfVertex(TypeVertex new_type_of_vertex);
  void ChangeBackgoundColor(float new_background_color[3]);
  void ChangeThichnessOfEdge(float new_thickness_of_edge);
  void ChangeSizeOfVertex(float new_size_of_vertex);
  void ChangeEdgeColor(float new_edge_color[3]);
  void ChangeVertexColor(float new_vertex_color[3]);

  OperationResult LoadScene(std::string path, NormalizationParamaters &params);
  SettingsView LoadSettings();

  OperationResult MoveScene(double x, double y, double z);
  OperationResult RotateScene(double x, double y, double z);
  OperationResult ScaleScene(double x, double y, double z);

  SettingsDrawer GetSettingsDrawer();

  int GetCountVertices();
  int GetCountEdges();

 private:
  BaseFileReader *fileReader_;
  SceneDrawerBase *sceneDrawer_;
  Scene scene_;
};

}  // namespace s21
