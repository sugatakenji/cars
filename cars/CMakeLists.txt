project(cars)

add_executable(${PROJECT_NAME} main.cpp openglwindow.cpp
                                cars.cpp asteroids.cpp)

enable_abcg(${PROJECT_NAME})
