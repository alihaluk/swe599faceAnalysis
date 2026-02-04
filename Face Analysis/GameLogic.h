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

struct NormalizedRect {
    float x, y, width, height;
};

class GameLogic {
private:
    GameState currentState;
    double shaveProgress; // 0.0 to 1.0
    double timeWithoutFace;

    const double REQUIRED_SHAVE_TIME = 10.0; // Shaving takes 10 seconds of valid contact
    const double MAX_LOST_TIME = 3.0;

    // Target zone definition (normalized coordinates)
    const NormalizedRect TARGET_ZONE = {0.3f, 0.2f, 0.4f, 0.6f}; // x:[0.3, 0.7], y:[0.2, 0.8]

public:
    GameLogic() {
        currentState = GameStateIdle;
        shaveProgress = 0.0;
        timeWithoutFace = 0;
    }

    void start() {
        currentState = GameStatePlaying;
        shaveProgress = 0.0;
        timeWithoutFace = 0;
    }

    // Update with face properties (normalized 0.0-1.0)
    // Pass -1.0 for all if no face detected
    void update(double dt, float x, float y, float width, float height) {
        if (currentState != GameStatePlaying) return;

        bool faceValid = false;

        // Check if face is detected (values >= 0)
        if (x >= 0 && y >= 0) {
            // Calculate center of the face
            float cx = x + width / 2.0f;
            float cy = y + height / 2.0f;

            // Check if center is within target zone
            bool insideX = (cx >= TARGET_ZONE.x) && (cx <= (TARGET_ZONE.x + TARGET_ZONE.width));
            bool insideY = (cy >= TARGET_ZONE.y) && (cy <= (TARGET_ZONE.y + TARGET_ZONE.height));

            if (insideX && insideY) {
                faceValid = true;
            }
        }

        if (faceValid) {
            timeWithoutFace = 0;
            // Increment progress
            shaveProgress += dt / REQUIRED_SHAVE_TIME;
            if (shaveProgress >= 1.0) {
                shaveProgress = 1.0;
                currentState = GameStateWon;
            }
        } else {
            timeWithoutFace += dt;
            if (timeWithoutFace >= MAX_LOST_TIME) {
                currentState = GameStateLost;
            }
        }
    }

    GameState getState() const {
        return currentState;
    }

    // Returns progress 0.0 to 1.0
    float getShaveProgress() const {
        return (float)shaveProgress;
    }

    std::string getStatusString() const {
        switch (currentState) {
            case GameStateIdle: return "Press Start";
            case GameStatePlaying:
                if (timeWithoutFace > 0) return "Move Face to Center!";
                return "Shaving...";
            case GameStateWon: return "CLEAN SHAVE!";
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

    // Returns danger level from 0.0 (safe) to 1.0 (lost)
    float getDangerLevel() const {
        if (currentState != GameStatePlaying) return 0.0f;
        float level = (float)(timeWithoutFace / MAX_LOST_TIME);
        if (level > 1.0f) level = 1.0f;
        if (level < 0.0f) level = 0.0f;
        return level;
    }

    // Returns the target zone definition
    NormalizedRect getTargetZone() const {
        return TARGET_ZONE;
    }
};

#endif /* GameLogic_h */
