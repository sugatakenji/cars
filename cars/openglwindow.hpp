#ifndef OPENGLWINDOW_HPP_
#define OPENGLWINDOW_HPP_

#include <imgui.h>

#include <random>

#include "abcg.hpp"
#include "cars.hpp"
#include "asteroids.hpp"


class OpenGLWindow : public abcg::OpenGLWindow {
 protected:
  void handleEvent(SDL_Event& event) override;
  void initializeGL() override;
  void paintGL() override;
  void paintUI() override;
  void resizeGL(int width, int height) override;
  void terminateGL() override;
  void checkCollisions();//
  void checkWinCondition();//

 private:
  GLuint m_objectsProgram{};
  GLuint m_starsProgram{}; /// opa nenem

  int m_viewportWidth{};
  int m_viewportHeight{};

  GameData m_gameData;

  Cars m_car; //Ship m_ship
  Asteroids m_asteroids; //asteroids

  abcg::ElapsedTimer m_restartWaitTimer;

  ImFont* m_font{};

  std::default_random_engine m_randomEngine;

  void restart();
  void update();
};

#endif
