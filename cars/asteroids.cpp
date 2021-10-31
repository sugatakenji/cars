#include "asteroids.hpp"

#include <cppitertools/itertools.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

void Asteroids::initializeGL(GLuint program) {
  terminateGL();

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  // Create asteroids
  m_asteroids.clear();
  m_asteroids.resize(5);

  for (auto &asteroid : m_asteroids) {
    asteroid = createAsteroid();

    // Make sure the asteroid won't collide with the 
    do {
      asteroid.m_translation = {m_randomDist(m_randomEngine),
                                m_randomDist(m_randomEngine)};
    } while (glm::length(asteroid.m_translation) < 0.5f);
  }
}

void Asteroids::paintGL() {
  abcg::glUseProgram(m_program);

  for (const auto &asteroid : m_asteroids) {
    abcg::glBindVertexArray(asteroid.m_vao);

    abcg::glUniform4fv(m_colorLoc, 1, &asteroid.m_color.r);
    abcg::glUniform1f(m_scaleLoc, asteroid.m_scale);
    abcg::glUniform1f(m_rotationLoc, 3.15F);
    
    abcg::glUniform2f(m_translationLoc, asteroid.m_translation.x, 
                          asteroid.m_translation.y ); 
     abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

    abcg::glBindVertexArray(0);
  }

  abcg::glUseProgram(0);
}

void Asteroids::terminateGL() {
  for (auto asteroid : m_asteroids) {
    abcg::glDeleteBuffers(1, &asteroid.m_vbo);
    abcg::glDeleteVertexArrays(1, &asteroid.m_vao);
  }
}

void Asteroids::update(const Cars &car, float deltaTime) {
  auto &re{m_randomEngine};
  std::uniform_real_distribution<float> randompos(-1.0f, 1.0f);

  for (auto &asteroid : m_asteroids) {
    asteroid.m_translation.y -= 0.5f * deltaTime*car.m_velocity.y;
    if (asteroid.m_translation.y < -1.5f)
    {
	
          asteroid.m_translation.y = 1.5f;
              	asteroid.m_translation.x = randompos(re);
     }
  }
}

Asteroids::Asteroid Asteroids::createAsteroid(glm::vec2 translation) {
  Asteroid asteroid;
  asteroid.m_color.a = 1.0f;
  asteroid.m_rotation = 1.0f;
  asteroid.m_scale = 0.125f;
  asteroid.m_translation = translation;

   
     std::array<glm::vec2, 24> positions{
     // car body
      glm::vec2{0.0f, +10.0f}, glm::vec2{-10.00f, -12.5f},
      glm::vec2{+10.0f, -12.5f}, glm::vec2{-10.0f, -45.0f},
      glm::vec2{10.0f, -45.0f},
       
      // Pneus direito
      glm::vec2{10.0f, -7.0f}, glm::vec2{20.0f, -7.0f},
      glm::vec2{10.0f, -22.0f}, glm::vec2{20.0f, -22.0f},
      glm::vec2{+10.0f, -40.0f}, glm::vec2{20.0f, -40.0f},
      glm::vec2{+10.0f, -55.0f}, glm::vec2{20.0f, -55.0f},
      
      // Peneu esquerdo
      glm::vec2{-20.0f, -7.0f}, glm::vec2{-10.0f, -7.0f},
      glm::vec2{-20.0f, -22.0f}, glm::vec2{-10.0f, -22.0f},
      glm::vec2{-20.0f, -40.0f}, glm::vec2{-10.0f, -40.0f},
      glm::vec2{-20.0f, -55.0f}, glm::vec2{-10.0f, -55.0f},
      };
      
        // Normalize
  for (auto &position : positions) {
    position /= glm::vec2{15.5f, 15.5f};
  }

  const std::array indices{0, 1, 2,
                           1, 3, 4,
                           1, 2, 4,
                           //pneu
                           5, 7, 8,
                           5, 6 ,8,
                           9, 11, 12,
                          9, 10, 12,
                           
                          17, 19, 20,
                           17, 18, 20,
                           13, 15, 16,
                           13, 14, 16
                           };
                           //////
                           
  // Generate VBO
  abcg::glGenBuffers(1, &asteroid.m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &asteroid.m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, asteroid.m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &asteroid.m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(asteroid.m_vao);

  abcg::glBindBuffer(GL_ARRAY_BUFFER, asteroid.m_vbo);
  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);
    abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, asteroid.m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
///


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
  return asteroid;
}
