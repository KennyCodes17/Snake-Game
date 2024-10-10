#include "SpeedController.h"
#include <iostream>
#include <windows.h> 

int GetSpeed(int score, int& currentSpeed, int minSpeed, int increasedSpeed, int& speedBeforeKeyPress, int& previousScore)
{
    // Step 1: Adjust speed based on score, even if a key is held
    if (score % 10 == 0 && score != previousScore && score != 0 && currentSpeed > minSpeed) {
        currentSpeed -= 50; // Reduce delay by 50ms
        // Store the score-based speed so we can return to it after key hold
        speedBeforeKeyPress = currentSpeed;
        // Update previousScore so that the speed doesn't keep adjusting for the same score
        previousScore = score;
    }

    // Step 2: Check if a directional key is being held down (WASD keys)
    if ((GetKeyState('W') & 0x8000) || (GetKeyState('A') & 0x8000) ||
        (GetKeyState('S') & 0x8000) || (GetKeyState('D') & 0x8000)) {

        // Return the increased speed while the key is held, but keep adjusting the score-based speed in the background
        return increasedSpeed;
    }

    // Step 3: If no key is being held, return to the updated score-based speed
    return speedBeforeKeyPress;
}



