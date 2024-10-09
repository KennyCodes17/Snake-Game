#include "SpeedController.h"
#include <iostream>
#include <windows.h> 

int GetSpeed(int score, int& currentSpeed, int minSpeed, int increasedSpeed, int& speedBeforeKeyPress, int& previousScore)
{
    currentSpeed = 50;
    return currentSpeed;
}

/*
// Step 1: Adjust speed based on score (only when the score has changed)
if (score % 10 == 0 && score != previousScore && score != 0 && currentSpeed > minSpeed) {
    currentSpeed -= 50; // Reduce delay by 50ms
    std::cout << "Speed adjusted based on score! New speed: " << currentSpeed << "ms" << std::endl;
    previousScore = score;  // Update previousScore to track the change

    // Store the score-based speed so we can return to it after key hold
    speedBeforeKeyPress = currentSpeed;
}

// Step 2: Check if a directional key is being held down (WASD keys)
if ((GetKeyState('W') & 0x8000) || (GetKeyState('A') & 0x8000) ||
    (GetKeyState('S') & 0x8000) || (GetKeyState('D') & 0x8000)) {

    // If a key is being held, temporarily increase the speed
    std::cout << "Speed increased based on key hold! Increased speed: " << increasedSpeed << "ms" << std::endl;
    return increasedSpeed; // Return the increased speed while the key is held
}

// Step 3: If no key is being held, revert to the score-based speed
return speedBeforeKeyPress; */


