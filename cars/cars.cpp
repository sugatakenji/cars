#include "cars.hpp"

#include <glm/gtx/fast_trigonometry.hpp>
#include <glm/gtx/rotate_vector.hpp>

void Cars::initializeGL(GLuint program) {//void Ship::initializeGL(GLuint program) {
  terminateGL();

  m_program = program;
  m_colorLoc = abcg::glGetUniformLocation(m_program, "color");
  m_rotationLoc = abcg::glGetUniformLocation(m_program, "rotation");
  m_scaleLoc = abcg::glGetUniformLocation(m_program, "scale");
  m_translationLoc = abcg::glGetUniformLocation(m_program, "translation");

  //m_rotation = 0.0f;
  m_translation = glm::vec2(0);
  m_velocity = glm::vec2(0);

  // clang-format off
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
    //position -= glm::vec2(0.0f, 60.0f);
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
  // clang-format on                           

  // Generate VBO
  abcg::glGenBuffers(1, &m_vbo);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  // Generate EBO
  abcg::glGenBuffers(1, &m_ebo);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
  abcg::glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(),
                     GL_STATIC_DRAW);
  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // Get location of attributes in the program
  GLint positionAttribute{abcg::glGetAttribLocation(m_program, "inPosition")};

  // Create VAO
  abcg::glGenVertexArrays(1, &m_vao);

  // Bind vertex attributes to current VAO
  abcg::glBindVertexArray(m_vao);

  abcg::glEnableVertexAttribArray(positionAttribute);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  abcg::glVertexAttribPointer(positionAttribute, 2, GL_FLOAT, GL_FALSE, 0,
                              nullptr);
  abcg::glBindBuffer(GL_ARRAY_BUFFER, 0);

  abcg::glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

  // End of binding to current VAO
  abcg::glBindVertexArray(0);
}

void Cars::paintGL(const GameData &gameData) {//void Ship::paintGL(const GameData &gameData) {
  if (gameData.m_state != State::Playing) return;

  abcg::glUseProgram(m_program);

  abcg::glBindVertexArray(m_vao);

  abcg::glUniform1f(m_scaleLoc, 0.125f);
  abcg::glUniform1f(m_rotationLoc, 0.0f);
  abcg::glUniform2fv(m_translationLoc, 1, &m_translation.x);
    abcg::glUniform2f(m_translationLoc,m_translation.x,m_translation.y);

  abcg::glUniform4fv(m_colorLoc, 1, &m_color.r);
  abcg::glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, nullptr);

  abcg::glBindVertexArray(0);

  abcg::glUseProgram(0);
}

void Cars::terminateGL() {//void Ship::terminateGL() {
  abcg::glDeleteBuffers(1, &m_vbo);
  abcg::glDeleteBuffers(1, &m_ebo);
  abcg::glDeleteVertexArrays(1, &m_vao);
}

void Cars::update(const GameData &gameData, float deltaTime) {
  if (gameData.m_input[static_cast<size_t>(Input::Left)])
    m_translation.x -= 1.0f * deltaTime*m_velocity.x;
  if (gameData.m_input[static_cast<size_t>(Input::Right)])
    m_translation.x += +1.0f * deltaTime*m_velocity.x;
 
  if (gameData.m_input[static_cast<size_t>(Input::Up)]){
     m_velocity.x = 2;
     m_velocity.y = 2;
 }
   if (!gameData.m_input[static_cast<size_t>(Input::Up)]){
     m_velocity.x = 1;
     m_velocity.y = 1;
 }
 
    if (m_translation.x < -0.8f) m_translation.x = -0.8f;
    if (m_translation.x > +0.8f) m_translation.x = +0.8f;

}
