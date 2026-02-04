#ifndef GameLogic_h
#define GameLogic_h

#include <string>
#include <sstream>
#include <iomanip>

enum GameState {
    GameStateIdle,
    GameStatePlaying,
    GameStateWon,
    GameStateLost
};

class GameLogic {
private:
    GameState currentState;
    double remainingTime;
    double timeWithoutFace;

    const double TOTAL_GAME_TIME = 30.0;
    const double MAX_LOST_TIME = 3.0;

public:
    GameLogic() {
        currentState = GameStateIdle;
        remainingTime = TOTAL_GAME_TIME;
        timeWithoutFace = 0;
    }

    void start() {
        currentState = GameStatePlaying;
        remainingTime = TOTAL_GAME_TIME;
        timeWithoutFace = 0;
    }

    void update(double dt, bool faceDetected) {
        if (currentState != GameStatePlaying) return;

        remainingTime -= dt;

        if (faceDetected) {
            timeWithoutFace = 0;
        } else {
            timeWithoutFace += dt;
        }

        if (remainingTime <= 0) {
            currentState = GameStateWon;
            remainingTime = 0;
        } else if (timeWithoutFace >= MAX_LOST_TIME) {
            currentState = GameStateLost;
        }
    }

    GameState getState() const {
        return currentState;
    }

    std::string getTimeString() const {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << remainingTime;
        return ss.str();
    }

    std::string getStatusString() const {
        switch (currentState) {
            case GameStateIdle: return "Press Start";
            case GameStatePlaying: return "Keep Face in Box!";
            case GameStateWon: return "YOU WIN!";
            case GameStateLost: return "GAME OVER";
        }
        return "";
    }

    // Helper for color feedback (R, G, B)
    void getStatusColor(int &r, int &g, int &b) const {
        switch (currentState) {
            case GameStateIdle: r=255; g=255; b=255; break;
            case GameStatePlaying:
                if (timeWithoutFace > 0) { r=255; g=255; b=0; } // Warning
                else { r=0; g=255; b=0; } // Good
                break;
            case GameStateWon: r=0; g=255; b=0; break;
            case GameStateLost: r=255; g=0; b=0; break;
        }
    }
};

#endif /* GameLogic_h */
