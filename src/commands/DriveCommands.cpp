#include "DriveCommands.h"
// #include "controllers/DriveController.h"

// extern DriveController driveController;

// static void handleAutoDrive(JsonObject &payload) {
//     const char *navigation = payload["navigation"] | "";
//     if (!navigation)
//         return;
//     if (strcmp(navigation, "duration") == 0) {
//         int16_t velocity = payload["velocity"] | 0;
//         int16_t turn = payload["turn"] | 0;
//         uint32_t ms = payload["duration"] | 0;
//         driveController.driveFor(velocity, turn, ms);
//         return;
//     }
//     if (strcmp(navigation, "distance") == 0) {
//         int16_t velocity = payload["velocity"] | 0;
//         double meters = payload["distance"] | 0.00f;
//         driveController.driveDistance(velocity, meters);
//         return;
//     }
// }

// static void handleManualDrive(JsonObject &payload) {
//     int16_t velocity = payload["velocity"] | 0;
//     int16_t turn = payload["turn"] | 0;
//     const char *control = payload["control"] | "";
//     if (!control)
//         return;
//     if (strcmp(control, "joystic") == 0) {
//         driveController.driveDifferential(velocity, turn);
//         return;
//     }
//     if (strcmp(control, "keyboard") == 0) {
//         bool up = payload["up"] | false;
//         bool down = payload["down"] | false;
//         bool right = payload["right"] | false;
//         bool left = payload["left"] | false;
//         driveController.driveDiscreteArcade(velocity, turn, up, down, right, left);
//     }
// }

// static void handleDrive(JsonDocument &doc) {
//     JsonObject payload = doc["payload"];
//     if (!payload) 
//         return;
//     if(driveController.isModeManual()) {
//         handleManualDrive(payload);
//         return;
//     }
//     handleAutoDrive(payload);
// }

CommandEntry driveCommands[] = {
//     { "drive", handleDrive },
//     { "stop", [](JsonDocument &) { driveController.stop(); } },
//     { "set-mode", [](JsonDocument &doc) {
//         const char *mode = doc["payload"] | "";
//         driveController.setMode(mode);
//     }},
};

const size_t driveCommandCount = sizeof(driveCommands) / sizeof(CommandEntry);
