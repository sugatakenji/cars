#ifndef ASTEROIDS_HPP_
#define ASTEROIDS_HPP_

#include <list>
#include <random>

#include "abcg.hpp"
#include "gamedata.hpp"
#include "cars.hpp"

class OpenGLWindow;

class Asteroids {
 public:
  void initializeGL(GLuint program);
  void paintGL();
  void terminateGL();

  void update(const Cars &car, float deltaTime);

 private:
  friend OpenGLWindow;

  GLuint m_program{};
  GLint m_colorLoc{};
  GLint m_rotationLoc{};
  GLint m_translationLoc{};
  GLint m_scaleLoc{};

  struct Asteroid {
    GLuint m_vao{};
    GLuint m_vbo{};
     GLuint m_ebo{};//criei um ebo

    float m_angularVelocity{};
    glm::vec4 m_color{1};
    bool m_hit{false};
    int m_polygonSides{};
    float m_rotation{};
    float m_scale{};
    glm::vec2 m_translation{glm::vec2(0)};
    glm::vec2 m_velocity{glm::vec2(0)};
  };

  std::list<Asteroid> m_asteroids;

  std::default_random_engine m_randomEngine;
  std::uniform_real_distribution<float> m_randomDist{-1.0f, 1.0f};

  Asteroids::Asteroid createAsteroid(glm::vec2 translation = glm::vec2(0));
};

#endif
