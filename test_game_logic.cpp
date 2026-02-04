#include <iostream>
#include <cassert>
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

    // Simulate 30 seconds with face detected
    for (int i = 0; i < 300; ++i) { // 300 steps of 0.1s
        game.update(0.1, true);
    }

    // It might be exactly 0 or slightly less, check state
    if (game.getState() != GameStateWon) {
        // Update one more time to be sure
        game.update(0.1, true);
    }

    assert(game.getState() == GameStateWon);
    std::cout << "test_win_condition passed" << std::endl;
}

void test_loss_condition() {
    GameLogic game;
    game.start();

    // Simulate 3 seconds without face
    for (int i = 0; i < 30; ++i) { // 30 steps of 0.1s
        game.update(0.1, false);
    }

    // Should be lost or about to be
     if (game.getState() != GameStateLost) {
        game.update(0.1, false);
    }

    assert(game.getState() == GameStateLost);
    std::cout << "test_loss_condition passed" << std::endl;
}

void test_recovery() {
    GameLogic game;
    game.start();

    // 2 seconds without face
    game.update(2.0, false);
    assert(game.getState() == GameStatePlaying);

    // Face comes back
    game.update(0.1, true);
    assert(game.getState() == GameStatePlaying);

    // 2 more seconds without face (should reset counter)
    game.update(2.0, false);
    assert(game.getState() == GameStatePlaying);

    std::cout << "test_recovery passed" << std::endl;
}

int main() {
    test_initial_state();
    test_win_condition();
    test_loss_condition();
    test_recovery();
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
