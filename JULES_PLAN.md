# JULES_PLAN

## Mission
Move from "Alpha" to "Beta Version" focusing on the "Core Gameplay Loop" and Mechanics.

## Achievements
- [x] **Core Game Logic**: Implemented state machine (Idle, Playing, Won, Lost) in `GameLogic.h`.
- [x] **Target Zone Mechanic**: Added logic to require face to be in the center of the screen.
- [x] **Danger Level**: Added visualization for "Time without face" (Danger Bar).
- [x] **Visual Feedback**: Implemented visual guides (Target Box, Danger Bar, Status Text) in `ViewController.mm`.
- [x] **Testing**: comprehensive unit tests in `test_game_logic.cpp`.

## Plan (Completed)

1. **Refactor GameLogic**
   - [x] Updated `GameLogic.h` to include all logic (inline) to avoid build system issues.
   - [x] Added `NormalizedRect` struct.

2. **Enhance Game Mechanics**
   - [x] Implemented `update` with coordinate validation.
   - [x] Implemented `getDangerLevel`.
   - [x] Implemented `getTargetZone`.

3. **Integrate into ViewController**
   - [x] Updated `ViewController.mm` to map OpenCV `Rect` to normalized coordinates.
   - [x] Added drawing code for UI elements.

4. **Verify**
   - [x] `test_game_logic.cpp` passes all cases including new mechanics.

## Next Steps (Future)
- [ ] Implement "Image-based shaving" or "Beard Removal" visual effect.
- [ ] Add Sound Effects.
- [ ] Add Start Screen UI.
