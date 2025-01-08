#include "facade.h"
#include <QThread>

namespace s21 {

Facade::Facade() {
  fileReader_ = FileReader::GetInstance();
  sceneDrawer_ = new QtSceneDrawer();
};

OperationResult Facade::DrawScene() {
  sceneDrawer_->DrawScene(scene_);
  return OperationResult();
}

void Facade::InitQtSceneDrawer() {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->initGL();
}

void Facade::ChangeTypeOfEdge(TypeEdge new_type_of_edge) {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->changeTypeOfEdge(new_type_of_edge);
}

void Facade::ChangeTypeOfVertex(TypeVertex new_type_of_vertex) {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->changeTypeOfVertex(new_type_of_vertex);
}

void Facade::ChangeBackgoundColor(float new_background_color[3]) {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->changeBackgoundColor(new_background_color);
}

void Facade::ChangeThichnessOfEdge(float new_thickness_of_edge) {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->changeThichnessOfEdge(new_thickness_of_edge);
}

void Facade::ChangeSizeOfVertex(float new_size_of_vertex) {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->changeSizeOfVertex(new_size_of_vertex);
}

void Facade::ChangeEdgeColor(float new_edge_color[3]) {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->changeEdgeColor(new_edge_color);
}

void Facade::ChangeVertexColor(float new_vertex_color[3]) {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  qtDrawer->changeVertexColor(new_vertex_color);
}

OperationResult Facade::LoadScene(std::string path,
                                  NormalizationParamaters& params) {
  scene_ = fileReader_->ReadScene(path, params);

  return fileReader_->GetOperationResult();
}

SettingsView Facade::LoadSettings() {
  FileReader* fileReader = dynamic_cast<FileReader*>(fileReader_);
  Settings settings = fileReader->ReadSettings();

  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);

  qtDrawer->setSettings(settings.settings_drawer);

  return settings.settings_view;
}

OperationResult Facade::MoveScene(double x, double y, double z) {
  scene_.TransformFigure(TransformMatrixBuilder::CreateMoveMatrix(x, y, z));
  return OperationResult();
}

OperationResult Facade::RotateScene(double x, double y, double z) {
  scene_.TransformFigure(TransformMatrixBuilder::CreateRotationMatrix(
      x, y, z, scene_.GetFigure().GetCenter()));
  return OperationResult();
}

OperationResult Facade::ScaleScene(double x, double y, double z) {
  scene_.TransformFigure(TransformMatrixBuilder::CreateScaleMatrix(
      x, y, z, scene_.GetFigure().GetCenter()));

  return OperationResult();
}

SettingsDrawer Facade::GetSettingsDrawer() {
  QtSceneDrawer* qtDrawer = dynamic_cast<QtSceneDrawer*>(sceneDrawer_);
  return qtDrawer->getSettings();
}

int Facade::GetCountVertices() {
  return scene_.GetFigure().GetVertices().size();
}

int Facade::GetCountEdges() { return scene_.GetFigure().GetEdges().size(); }

}