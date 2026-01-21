#include <iostream>
#include <iostream>
#include <thread>
#include <chrono>
#include <windows.h>
#include <atomic>

class AutoClicker {
private:
    std::atomic<bool> isRunning;
    int clickInterval;
    POINT clickPosition;

public:
    AutoClicker() : isRunning(false), clickInterval(1000) {
        clickPosition.x = GetSystemMetrics(SM_CXSCREEN) / 2;
        clickPosition.y = GetSystemMetrics(SM_CYSCREEN) / 2;
    }

    void setClickInterval(int interval) {
        clickInterval = interval;
    }

    void setClickPosition(int x, int y) {
        clickPosition.x = x;
        clickPosition.y = y;
    }

    void getCurrentCursorPosition() {
        GetCursorPos(&clickPosition);
        std::cout << "Current cursor position set: (" << clickPosition.x << ", " << clickPosition.y << ")\n";
    }

    void performClick() {
        POINT originalPos;
        GetCursorPos(&originalPos);
        SetCursorPos(clickPosition.x, clickPosition.y);

        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    }

    void start() {
        isRunning = true;
        std::cout << "\n=== AUTO-CLICKER STARTED ===\n";
        std::cout << "Position: (" << clickPosition.x << ", " << clickPosition.y << ")\n";
        std::cout << "Interval: " << clickInterval << "ms\n";
        std::cout << "\n*** PRESS ESC KEY TO STOP ***\n";
        std::cout << "*** OR PRESS CTRL+C TO FORCE QUIT ***\n\n";

        int clickCount = 0;
        while (isRunning) {
            // Check for ESC key (more reliable than _kbhit)
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                stop();
                break;
            }

            // Check for Ctrl+C
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('C') & 0x8000) {
                stop();
                break;
            }

            performClick();
            clickCount++;
            std::cout << "Click #" << clickCount << " - Press ESC to stop\r" << std::flush;

            // Sleep in smaller chunks to check for exit more frequently
            for (int i = 0; i < clickInterval && isRunning; i += 50) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                    stop();
                    break;
                }
            }
        }
    }

    void stop() {
        isRunning = false;
        std::cout << "\n\n*** AUTO-CLICKER STOPPED ***\n";
    }

    bool getRunningStatus() {
        return isRunning;
    }
};

void displayMenu() {
    std::cout << "\n=== SAFE AUTO CLICKER ===\n";
    std::cout << "1. Set click interval (ms)\n";
    std::cout << "2. Set specific click position\n";
    std::cout << "3. Use current cursor position\n";
    std::cout << "4. Start auto-clicker (ESC to stop)\n";
    std::cout << "5. Exit program\n";
    std::cout << "Choice: ";
}

int main() {
    AutoClicker clicker;
    int choice;

    std::cout << "SAFE AUTO CLICKER PROGRAM\n";
    std::cout << "=========================\n";
    std::cout << "Multiple exit methods built-in!\n";

    while (true) {
        displayMenu();
        std::cin >> choice;

        switch (choice) {
        case 1: {
            int interval;
            std::cout << "Enter click interval in milliseconds (minimum 1ms): ";
            std::cin >> interval;
            if (interval >= 1) {
                clicker.setClickInterval(interval);
                std::cout << "Click interval set to " << interval << "ms\n";
            }
            else {
                std::cout << "Minimum interval is 1ms for safety.\n";
            }
            break;
        }
        case 2: {
            int x, y;
            std::cout << "Enter X coordinate: ";
            std::cin >> x;
            std::cout << "Enter Y coordinate: ";
            std::cin >> y;
            clicker.setClickPosition(x, y);
            std::cout << "Click position set to (" << x << ", " << y << ")\n";
            break;
        }
        case 3: {
            std::cout << "Move cursor to desired position and press Enter...";
            std::cin.ignore();
            std::cin.get();
            clicker.getCurrentCursorPosition();
            break;
        }
        case 4: {
            std::cout << "\nStarting in 3 seconds...\n";
            std::cout << "REMEMBER: Press ESC to stop!\n";
            for (int i = 3; i > 0; i--) {
                std::cout << i << "...\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            clicker.start();
            break;
        }
        case 5: {
            std::cout << "Goodbye!\n";
            return 0;
        }
        default: {
            std::cout << "Invalid choice.\n";
            break;
        }
        }
    }

    return 0;
}