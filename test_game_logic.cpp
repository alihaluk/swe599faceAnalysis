#include <iostream>
#include <cassert>
#include <cmath>
#include "Face Analysis/GameLogic.h"

void test_initial_state() {
    GameLogic game;
    assert(game.getState() == GameStateIdle);
    std::cout << "test_initial_state passed" << std::endl;
}

void test_win_condition() {
    GameLogic game;
    game.start();
    assert(game.getState() == GameStatePlaying);

    // Simulate 30 seconds with face detected in center (x=0.4, y=0.4, w=0.2, h=0.2 -> cx=0.5, cy=0.5)
    // Target zone is cx:[0.3, 0.7], cy:[0.2, 0.8]
    for (int i = 0; i < 300; ++i) { // 300 steps of 0.1s
        game.update(0.1, 0.4, 0.4, 0.2, 0.2);
    }

    if (game.getState() != GameStateWon) {
        game.update(0.1, 0.4, 0.4, 0.2, 0.2);
    }

    assert(game.getState() == GameStateWon);
    std::cout << "test_win_condition passed" << std::endl;
}

void test_loss_condition_no_face() {
    GameLogic game;
    game.start();

    // Simulate 3 seconds without face (pass -1)
    for (int i = 0; i < 30; ++i) { // 30 steps of 0.1s
        game.update(0.1, -1.0, -1.0, 0.0, 0.0);
    }

    if (game.getState() != GameStateLost) {
        game.update(0.1, -1.0, -1.0, 0.0, 0.0);
    }

    assert(game.getState() == GameStateLost);
    std::cout << "test_loss_condition_no_face passed" << std::endl;
}

void test_loss_condition_face_out_of_bounds() {
    GameLogic game;
    game.start();

    // Face detected but outside target zone (cx=0.1, cy=0.1)
    // Target zone starts at x=0.3, y=0.2
    for (int i = 0; i < 30; ++i) { // 30 steps of 0.1s
        game.update(0.1, 0.0, 0.0, 0.2, 0.2); // cx=0.1, cy=0.1
    }

    if (game.getState() != GameStateLost) {
        game.update(0.1, 0.0, 0.0, 0.2, 0.2);
    }

    assert(game.getState() == GameStateLost);
    std::cout << "test_loss_condition_face_out_of_bounds passed" << std::endl;
}

void test_recovery() {
    GameLogic game;
    game.start();

    // 2 seconds without face
    game.update(2.0, -1.0, -1.0, 0.0, 0.0);
    assert(game.getState() == GameStatePlaying);
    assert(game.getDangerLevel() > 0.0f);

    // Face comes back in center
    game.update(0.1, 0.4, 0.4, 0.2, 0.2);
    assert(game.getState() == GameStatePlaying);
    assert(game.getDangerLevel() == 0.0f);

    // 2 more seconds out of bounds
    game.update(2.0, 0.0, 0.0, 0.2, 0.2);
    assert(game.getState() == GameStatePlaying);

    std::cout << "test_recovery passed" << std::endl;
}

void test_danger_level() {
    GameLogic game;
    game.start();
    assert(game.getDangerLevel() == 0.0f);

    game.update(1.5, -1.0, -1.0, 0.0, 0.0); // 1.5s lost / 3.0s max = 0.5
    float danger = game.getDangerLevel();
    assert(std::abs(danger - 0.5f) < 0.001f);

    std::cout << "test_danger_level passed" << std::endl;
}

int main() {
    test_initial_state();
    test_win_condition();
    test_loss_condition_no_face();
    test_loss_condition_face_out_of_bounds();
    test_recovery();
    test_danger_level();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
