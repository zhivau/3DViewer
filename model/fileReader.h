#pragma once

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <QSettings>
#include <QString>
#include <limits>

#include "baseObjects.h"
#include "scene.h"

namespace s21 {

struct Settings {
  SettingsDrawer settings_drawer;
  SettingsView settings_view;
};

class BaseFileReader {
 public:
  virtual Scene ReadScene(std::string path,
                          const NormalizationParamaters &params) = 0;
  virtual ~BaseFileReader() = default;

  OperationResult GetOperationResult() { return operationResult_; }

 protected:
  OperationResult operationResult_;
};

/**
 * @class FileReader
 * @brief Класс для чтения данных из файлов
 */
class FileReader : public BaseFileReader {
 public:
  static FileReader *GetInstance() {
    static FileReader instance;
    return &instance;
  }
  /**
   * @brief Метод для чтения фигуры
   * @param path, params путь до файла и параметры нормализации вершин
   */
  Scene ReadScene(std::string path,
                  const NormalizationParamaters &params) override;

  /**
   * @brief Метод для настроек отрисовщика и отображения
   */
  Settings ReadSettings();

 private:
  FileReader() = default;
  ~FileReader() = default;

  FileReader(const FileReader &) = delete;
  FileReader(FileReader &&) = delete;

  FileReader &operator=(const FileReader &) = delete;
  FileReader &operator=(FileReader &&) = delete;

  void ParseIndices(std::vector<int> &indices, std::istringstream &iss);
  void AddEdges(std::vector<int> &indices, std::vector<Vertex> &vertices,
                std::vector<Edge> &edges);
  void NormalizeVertices(std::vector<Vertex> &vertices,
                         const NormalizationParamaters &params);
};

}  // namespace s21
