#include <gtest/gtest.h>
#include "../model/fileReader.h"
#include <string>
#include <sstream>

namespace s21 {

class FileReaderTest : public ::testing::Test {
protected:
    static BaseFileReader* fileReader;
    NormalizationParamaters params;

    static void SetUpTestSuite() {
        fileReader = FileReader::GetInstance();
    }
};

BaseFileReader* FileReaderTest::fileReader = nullptr;

TEST_F(FileReaderTest, ReadScene_ValidFile) {
    std::string testFilePath = "valid_scene.obj";
    
    std::ofstream testFile(testFilePath);
    testFile << "v 1.0 2.0 3.0\n";
    testFile << "v 4.0 5.0 6.0\n";
    testFile << "f 1 2\n";
    testFile.close();

    Scene scene = fileReader->ReadScene(testFilePath, params);

    fileReader->GetOperationResult().isSuccess();
    EXPECT_TRUE(fileReader->GetOperationResult().isSuccess());
    EXPECT_EQ(scene.GetFigure().GetVertices().size(), 2);
    EXPECT_EQ(scene.GetFigure().GetEdges().size(), 1);
}

TEST_F(FileReaderTest, ReadScene_FileNotFound) {
    std::string invalidFilePath = "non_existent.obj";

    Scene scene = fileReader->ReadScene(invalidFilePath, params);

    EXPECT_FALSE(fileReader->GetOperationResult().isSuccess());
    EXPECT_EQ(fileReader->GetOperationResult().GetErrorMessage(), "Ошибка: Не удалось открыть obj файл");
}

TEST_F(FileReaderTest, ReadScene_InvalidVertexFormat) {
    std::string testFilePath = "invalid_vertex.obj";

    std::ofstream testFile(testFilePath);
    testFile << "v 1.0 2.0\n";
    testFile.close();

    Scene scene = fileReader->ReadScene(testFilePath, params);

    EXPECT_TRUE(fileReader->GetOperationResult().isSuccess());
    EXPECT_EQ(fileReader->GetOperationResult().GetErrorMessage(), "Ошибка: Неверный формат строки вершины: ");
}

TEST_F(FileReaderTest, ReadScene_InvalidFaceFormat) {
    std::string testFilePath = "invalid_face.obj";
    
    std::ofstream testFile(testFilePath);
    testFile << "v 1.0 2.0 3.0\n";
    testFile << "f 1\n";
    testFile.close();

    Scene scene = fileReader->ReadScene(testFilePath, params);

    EXPECT_TRUE(fileReader->GetOperationResult().isSuccess());
    EXPECT_EQ(fileReader->GetOperationResult().GetErrorMessage(), "Ошибка: В грань должно входить как минимум 2 вершины:");
}

TEST_F(FileReaderTest, ReadScene_InvalidVertexIndex) {
    std::string testFilePath = "invalid_vertex_index.obj";

    std::ofstream testFile(testFilePath);
    testFile << "v 1.0 2.0 3.0\n";
    testFile << "f 1 3\n";
    testFile.close();

    Scene scene = fileReader->ReadScene(testFilePath, params);

    EXPECT_TRUE(fileReader->GetOperationResult().isSuccess());
    EXPECT_EQ(fileReader->GetOperationResult().GetErrorMessage(), "Ошибка: Индекс вершины за пределами допустимого диапазона:");
}

TEST_F(FileReaderTest, ReadScene_NormalizationCheck) {
    std::string testFilePath = "normalization_test.obj";
    
    std::ofstream testFile(testFilePath);
    testFile << "v 10.0 20.0 30.0\n";
    testFile << "v 40.0 50.0 60.0\n";
    testFile << "f 1 2\n";
    testFile.close();

    Scene scene = fileReader->ReadScene(testFilePath, params);

    EXPECT_TRUE(fileReader->GetOperationResult().isSuccess());
    const auto &vertices = scene.GetFigure().GetVertices();
    EXPECT_NEAR(vertices[0].GetPosition().x, 0.0, 0.01);
    EXPECT_NEAR(vertices[0].GetPosition().y, 0.0, 0.01);
    EXPECT_NEAR(vertices[1].GetPosition().x, 1.0, 0.01);
    EXPECT_NEAR(vertices[1].GetPosition().y, 1.0, 0.01);
}

}