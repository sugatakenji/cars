#include "openglwindow.hpp"

#include <imgui.h>

#include "abcg.hpp"

void OpenGLWindow::handleEvent(SDL_Event &event) {
  // Keyboard events
  if (event.type == SDL_KEYDOWN) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.set(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.set(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.set(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.set(static_cast<size_t>(Input::Right));
  }
  if (event.type == SDL_KEYUP) {
    if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Up));
    if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Down));
    if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Left));
    if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
      m_gameData.m_input.reset(static_cast<size_t>(Input::Right));
  }

}

void OpenGLWindow::initializeGL() {
  // Load a new font
  ImGuiIO &io{ImGui::GetIO()};
  auto filename{getAssetsPath() + "Inconsolata-Medium.ttf"};
  m_font = io.Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::Exception{abcg::Exception::Runtime("Cannot load font file")};
  }

  // Create program to render the other objects
  m_objectsProgram = createProgramFromFile(getAssetsPath() + "objects.vert",
                                           getAssetsPath() + "objects.frag");
m_starsProgram = createProgramFromFile(getAssetsPath() + "stars.vert",
                                       getAssetsPath() + "stars.frag");
  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void OpenGLWindow::restart() {
  m_gameData.m_state = State::Playing;


  m_car.initializeGL(m_objectsProgram);//m_ship.initializeGL(m_objectsProgram);
  m_asteroids.initializeGL(m_objectsProgram);

}

void OpenGLWindow::update() {
  float deltaTime{static_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }

  m_car.update(m_gameData, deltaTime);//  m_ship.initializeGL(m_objectsProgram);
  m_asteroids.update(m_car, deltaTime);
  if (m_gameData.m_state == State::Playing) {
  	checkCollisions();
 	 checkWinCondition();
	}//
}
void OpenGLWindow::paintGL() {
  update();

  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportWidth, m_viewportHeight);
  m_car.paintGL(m_gameData);// m_ship.paintGL(m_gameData);
  m_asteroids.paintGL(); //
} 

void OpenGLWindow::paintUI() {
  float deltaTime{static_cast<float>(getDeltaTime())};
  abcg::OpenGLWindow::paintUI();

  {
    const auto size{ImVec2(300, 85)};
    const auto position{ImVec2((m_viewportWidth - size.x) / 2.0f,
                               (m_viewportHeight - size.y) / 2.0f)};
    ImGui::SetNextWindowPos(position);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags flags{ImGuiWindowFlags_NoBackground |
                           ImGuiWindowFlags_NoTitleBar |
                           ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flags);
    ImGui::PushFont(m_font);
    ImGui::SetCursorScreenPos(-1,1)
    ImGui::Text("%f", 1.0f * m_car.m_velocity.y);
    
    if (m_gameData.m_state == State::GameOver) {
      ImGui::Text("Game Over!");
          
    } else if (m_gameData.m_state == State::Win) {
      ImGui::Text("*You Win!*");
    }

    ImGui::PopFont();
    ImGui::End();
  }
}

void OpenGLWindow::resizeGL(int width, int height) {
  m_viewportWidth = width;
  m_viewportHeight = height;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLWindow::terminateGL() {
  abcg::glDeleteProgram(m_objectsProgram);

  m_car.terminateGL();//ms_ship.terminateGL();
  m_asteroids.terminateGL();
}

void OpenGLWindow::checkCollisions() {
  // Check collision between ship and asteroids
  for (auto &asteroid : m_asteroids.m_asteroids) {
    auto asteroidTranslation{asteroid.m_translation};
    asteroidTranslation.y += 0.3f;
    auto distance{glm::distance(m_car.m_translation, asteroidTranslation)};

    if (distance < m_car.m_scale * 0.8f + asteroid.m_scale * 0.85f) {
      m_gameData.m_state = State::GameOver;
      m_restartWaitTimer.restart();
    }
  }
}

void OpenGLWindow::checkWinCondition() {
  if (m_asteroids.m_asteroids.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }
}
