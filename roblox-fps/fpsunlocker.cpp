#include <iostream>
#include <fstream>
#include <string>
#include <sys/stat.h>

// Colors
const std::string RED = "\033[1;31m";
const std::string YELLOW = "\033[1;33m";
const std::string GREEN = "\033[1;32m";
const std::string NC = "\033[0m";

// Roblox application path
const std::string path = "/Applications/Roblox.app";

// Function to check if a directory exists
bool directoryExists(const std::string &dirPath) {
    struct stat info;
    return (stat(dirPath.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
}

// Function to create a directory if it does not exist
void createDirectory(const std::string &dirPath) {
    mkdir(dirPath.c_str(), 0777);
}

int main() {
    if (!directoryExists(path)) {
        std::cout << "\n" << RED << "Roblox not found in the Applications folder.\n" << NC;
        return 0;
    }

    // Settings path
    const std::string settings_path = path + "/Contents/MacOS/ClientSettings";
    if (!directoryExists(settings_path)) {
        createDirectory(settings_path);
    }

    // Ask for FPS cap
    int fps;
    std::cout << "Please choose an FPS limit: " << YELLOW << "(anything above 120 FPS switches to Vulkan Renderer, which is experimental)" << NC << "\n";
    std::cin >> fps;

    // Validate FPS input
    if (std::cin.fail() || fps <= 0) {
        std::cout << "\n" << RED << "Not a valid FPS cap." << NC << "\n";
        return 0;
    }

    // Ask for ESP flag
    std::string esp_choice;
    std::cout << "Do you want to enable ESP? (Enable Interpolation Visualizer) [y/n]\n";
    std::cin >> esp_choice;

    // Ask for Super Jump flag
    std::string superjump_choice;
    std::cout << "Do you want to enable Super Jump? [y/n]\n";
    std::cin >> superjump_choice;

    // Initialize settings as JSON string
    std::string settings = "{ \"DFIntTaskSchedulerTargetFps\": " + std::to_string(fps) + " }";

    // If FPS is above 120, switch to Vulkan Renderer
    if (fps > 120) {
        settings = "{ "
                   "\"DFIntTaskSchedulerTargetFps\": " + std::to_string(fps) + ", "
                   "\"FFlagDebugGraphicsDisableMetal\": \"true\", "
                   "\"FFlagDebugGraphicsPreferVulkan\": \"true\" "
                   "}";
        std::cout << "\n" << YELLOW << "Switched to Vulkan Renderer." << NC << "\n";
    }

    // Enable ESP if chosen
    if (esp_choice == "y" || esp_choice == "Y") {
        settings = "{ "
                   "\"DFIntTaskSchedulerTargetFps\": " + std::to_string(fps) + ", "
                   "\"FFlagDebugGraphicsDisableMetal\": \"true\", "
                   "\"FFlagDebugGraphicsPreferVulkan\": \"true\", "
                   "\"DFFlagDebugEnableInterpolationVisualizer\": \"true\" "
                   "}";
        std::cout << "\n" << YELLOW << "ESP (Interpolation Visualizer) enabled." << NC << "\n";
    }

    // Enable Super Jump if chosen
    if (superjump_choice == "y" || superjump_choice == "Y") {
        settings = "{ "
                   "\"DFIntTaskSchedulerTargetFps\": " + std::to_string(fps) + ", "
                   "\"gtg br\": \"1500\", "
                   "\"FFlagDebugGraphicsDisableMetal\": \"true\", "
                   "\"FFlagDebugGraphicsPreferVulkan\": \"true\", "
                   "\"DFFlagDebugEnableInterpolationVisualizer\": \"true\" "
                   "}";
        std::cout << "\n" << YELLOW << "Super Jump enabled." << NC << "\n";
    }

    // Write settings to ClientAppSettings.json
    std::ofstream settingsFile(settings_path + "/ClientAppSettings.json");
    if (settingsFile.is_open()) {
        settingsFile << settings;
        settingsFile.close();
        std::cout << "\n" << GREEN << "FPS limit updated! Settings saved." << NC << "\n";
    } else {
        std::cout << "\n" << RED << "Error saving settings." << NC << "\n";
    }

    return 0;
}
