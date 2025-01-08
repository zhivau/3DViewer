#include <gtest/gtest.h>
#include "../model/scene.h"
#include "../model/fileReader.h"

namespace s21 {

class TransformTest : public ::testing::Test {
protected:
    static Scene scene;

    static void SetUpTestSuite() {
        NormalizationParamaters params;
        scene = FileReader::GetInstance()->ReadScene("cube.obj", params);
    }
};

Scene TransformTest::scene;

TEST_F(TransformTest, Move1) {
    Figure fig = scene.GetFigure();
    scene.TransformFigure(TransformMatrixBuilder::CreateMoveMatrix(1, 1, 1));

    EXPECT_EQ(fig.GetCenter().x + 1, scene.GetFigure().GetCenter().x);
    EXPECT_EQ(fig.GetCenter().y + 1, scene.GetFigure().GetCenter().y);
    EXPECT_EQ(fig.GetCenter().z + 1, scene.GetFigure().GetCenter().z);
}

TEST_F(TransformTest, Move2) {

    Figure fig = scene.GetFigure();
    scene.TransformFigure(TransformMatrixBuilder::CreateMoveMatrix(-1, -1, -1));

    EXPECT_EQ(fig.GetCenter().x - 1, scene.GetFigure().GetCenter().x);
    EXPECT_EQ(fig.GetCenter().y - 1, scene.GetFigure().GetCenter().y);
    EXPECT_EQ(fig.GetCenter().z - 1, scene.GetFigure().GetCenter().z);
}

TEST_F(TransformTest, Move3) {

    Figure fig = scene.GetFigure();
    scene.TransformFigure(TransformMatrixBuilder::CreateMoveMatrix(0, 0, 0));

    EXPECT_EQ(fig.GetCenter().x, scene.GetFigure().GetCenter().x);
    EXPECT_EQ(fig.GetCenter().y, scene.GetFigure().GetCenter().y);
    EXPECT_EQ(fig.GetCenter().z, scene.GetFigure().GetCenter().z);
}

TEST_F(TransformTest, Move4) {

    Figure fig = scene.GetFigure();
    scene.TransformFigure(TransformMatrixBuilder::CreateMoveMatrix(0.5, 0.5, 0.5));

    EXPECT_EQ(fig.GetCenter().x + 0.5, scene.GetFigure().GetCenter().x);
    EXPECT_EQ(fig.GetCenter().y + 0.5, scene.GetFigure().GetCenter().y);
    EXPECT_EQ(fig.GetCenter().z + 0.5, scene.GetFigure().GetCenter().z);
}

TEST_F(TransformTest, Move5) {

    Figure fig = scene.GetFigure();
    scene.TransformFigure(TransformMatrixBuilder::CreateMoveMatrix(10, 15, 25));

    EXPECT_EQ(fig.GetCenter().x  + 10, scene.GetFigure().GetCenter().x);
    EXPECT_EQ(fig.GetCenter().y  + 15, scene.GetFigure().GetCenter().y);
    EXPECT_EQ(fig.GetCenter().z  + 25, scene.GetFigure().GetCenter().z);
}

TEST_F(TransformTest, Scale1) {
    NormalizationParamaters params;
    scene = FileReader::GetInstance()->ReadScene("cube.obj", params);

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateScaleMatrix(0.1, 0.1, 0.1, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateScaleMatrix(0.1, 0.1, 0.1, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Scale2) {
    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateScaleMatrix(2, 2, 2, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateScaleMatrix(2, 2, 2, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}


TEST_F(TransformTest, Scale3) {

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateScaleMatrix(5, 5, 5, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateScaleMatrix(5, 5, 5, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Scale4) {

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateScaleMatrix(0.9, 0.9, 0.9, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateScaleMatrix(0.9, 0.9, 0.9, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Scale5) {

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateScaleMatrix(2.5, 2.5, 2.5, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateScaleMatrix(2.5, 2.5, 2.5, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Rotate1) {
    NormalizationParamaters params;
    scene = FileReader::GetInstance()->ReadScene("cube.obj", params);

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateRotationMatrix(2.5, 2.5, 2.5, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateRotationMatrix(2.5, 2.5, 2.5, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Rotate2) {

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateRotationMatrix(0.1, 0.1, 0.1, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateRotationMatrix(0.1, 0.1, 0.1, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Rotate3) {

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateRotationMatrix(-0.1, -0.1, -0.1, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateRotationMatrix(-0.1, -0.1, -0.1, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Rotate4) {

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateRotationMatrix(-2.5, -2.5, -2.5, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateRotationMatrix(-2.5, -2.5, -2.5, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

TEST_F(TransformTest, Rotate5) {

    Figure fig = scene.GetFigure();
    fig.Transform(TransformMatrixBuilder::CreateRotationMatrix(3.14, 3.14, 3.14, fig.GetCenter()));
    scene.TransformFigure(TransformMatrixBuilder::CreateRotationMatrix(3.14, 3.14, 3.14, scene.GetFigure().GetCenter()));

    EXPECT_EQ(fig.GetVertices()[0].GetPosition().x, scene.GetFigure().GetVertices()[0].GetPosition().x);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().y, scene.GetFigure().GetVertices()[0].GetPosition().y);
    EXPECT_EQ(fig.GetVertices()[0].GetPosition().z, scene.GetFigure().GetVertices()[0].GetPosition().z);
}

}