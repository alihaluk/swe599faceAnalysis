#include <iostream>
#include <cassert>
#include <cmath>
#include "Face Analysis/GameLogic.h"

void test_initial_state() {
    GameLogic game;
    assert(game.getState() == GameStateIdle);
    assert(game.getShaveProgress() == 0.0f);
    std::cout << "test_initial_state passed" << std::endl;
}

void test_win_condition() {
    GameLogic game;
    game.start();
    assert(game.getState() == GameStatePlaying);

    // Simulate 10 seconds (REQUIRED_SHAVE_TIME) with face detected in center
    // dt = 0.1s. 10s / 0.1s = 100 steps.
    for (int i = 0; i < 100; ++i) {
        game.update(0.1, 0.4, 0.4, 0.2, 0.2);
    }

    if (game.getState() != GameStateWon) {
        // Just in case of float precision issues
        game.update(0.1, 0.4, 0.4, 0.2, 0.2);
    }

    assert(game.getState() == GameStateWon);
    assert(game.getShaveProgress() >= 1.0f);
    std::cout << "test_win_condition passed" << std::endl;
}

void test_loss_condition() {
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
    std::cout << "test_loss_condition passed" << std::endl;
}

void test_progress_accumulation() {
    GameLogic game;
    game.start();

    // 1 second of shaving (should be 10% progress)
    for(int i=0; i<10; i++) {
        game.update(0.1, 0.4, 0.4, 0.2, 0.2);
    }

    float p = game.getShaveProgress();
    assert(p > 0.09f && p < 0.11f);

    // 1 second lost (progress shouldn't change)
    for(int i=0; i<10; i++) {
        game.update(0.1, -1.0, -1.0, 0.0, 0.0);
    }

    assert(game.getShaveProgress() == p); // Should remain same

    std::cout << "test_progress_accumulation passed" << std::endl;
}

int main() {
    test_initial_state();
    test_win_condition();
    test_loss_condition();
    test_progress_accumulation();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
