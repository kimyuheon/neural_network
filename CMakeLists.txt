cmake_minimum_required(VERSION 3.14)
project(NeuralNetwork CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ─────────────────────────────────────────
# Executables
# ─────────────────────────────────────────

add_executable(nn 0nn.cpp)
add_executable(makeTrainingSamples makeTrainingSamples.cpp)

# 컴파일 경고 설정 (크로스 플랫폼)
if(MSVC)
    target_compile_options(nn PRIVATE /W3)
    target_compile_options(makeTrainingSamples PRIVATE /W3)
else()
    target_compile_options(nn PRIVATE -Wall -Wextra)
    target_compile_options(makeTrainingSamples PRIVATE -Wall -Wextra)
endif()

# ─────────────────────────────────────────
# out.txt 를 빌드 디렉터리로 복사
# (nn 이 상대 경로 "out.txt" 로 파일을 읽기 때문)
# ─────────────────────────────────────────

configure_file(
    ${CMAKE_SOURCE_DIR}/out.txt
    ${CMAKE_BINARY_DIR}/out.txt
    COPYONLY
)

# ─────────────────────────────────────────
# CTest
# ─────────────────────────────────────────

enable_testing()

# 테스트 1: makeTrainingSamples 가 오류 없이 실행되는지 확인
add_test(
    NAME makeTrainingSamples_runs
    COMMAND makeTrainingSamples
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 테스트 2: nn 이 토폴로지 "3 4 1" 로 정상 실행되는지 확인
#   out.txt 가 빌드 디렉터리에 복사되어 있으므로 WORKING_DIRECTORY 를 빌드 디렉터리로 지정
add_test(
    NAME nn_train_3_4_1
    COMMAND nn 3 4 1
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 테스트 3: nn 이 더 깊은 토폴로지 "3 5 5 1" 로도 실행되는지 확인
add_test(
    NAME nn_train_3_5_5_1
    COMMAND nn 3 5 5 1
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# 각 테스트의 타임아웃 (학습이 오래 걸릴 수 있으므로 넉넉하게)
set_tests_properties(nn_train_3_4_1   PROPERTIES TIMEOUT 120)
set_tests_properties(nn_train_3_5_5_1 PROPERTIES TIMEOUT 120)
