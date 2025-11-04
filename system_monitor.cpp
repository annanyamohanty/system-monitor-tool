#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <iomanip>
#include <cstdlib>
using namespace std;

// 🧠 Function to display memory usage
void getMemoryUsage() {
    ifstream meminfo("/proc/meminfo");
    string key; long value; string unit;
    long memTotal = 0, memAvailable = 0;
    while (meminfo >> key >> value >> unit) {
        if (key == "MemTotal:") memTotal = value;
        if (key == "MemAvailable:") memAvailable = value;
    }
    double usedPercent = (double)(memTotal - memAvailable) / memTotal * 100.0;
    cout << fixed << setprecision(2);
    cout << "🧠 Memory Usage : " << usedPercent << "% ("
         << (memTotal - memAvailable) / 1024 << " MB used / "
         << memTotal / 1024 << " MB total)" << endl;
}

// ⚙️ Function to calculate CPU usage
double getCPUUsage() {
    static unsigned long long prevIdle = 0, prevTotal = 0;
    string line; ifstream statf("/proc/stat");
    getline(statf, line);
    string cpu; unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    stringstream ss(line);
    ss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    unsigned long long idleTime = idle + iowait;
    unsigned long long nonIdle = user + nice + system + irq + softirq + steal;
    unsigned long long total = idleTime + nonIdle;
    unsigned long long totalDiff = total - prevTotal;
    unsigned long long idleDiff = idleTime - prevIdle;
    prevTotal = total; prevIdle = idleTime;
    if (totalDiff == 0) return 0.0;
    return (double)(totalDiff - idleDiff) / totalDiff * 100.0;
}

// 📋 Function to display top processes
void listProcesses() {
    cout << "\n📋 Top 5 Processes (by CPU):" << endl;
    system("ps -eo pid,comm,%cpu,%mem --sort=-%cpu | head -n 6");
}

// 🔪 Optional: Kill a process by PID
void killProcess() {
    int pid;
    cout << "\n🔪 Enter PID to kill (0 to skip): ";
    cin >> pid;
    if (pid > 0) {
        string cmd = "kill -9 " + to_string(pid);
        int result = system(cmd.c_str());
        if (result == 0)
            cout << "✅ Process " << pid << " terminated successfully.\n";
        else
            cout << "❌ Failed to terminate process " << pid << ". Check permissions.\n";
    }
}

// 🖥️ Main function
int main() {
    cout << "=====================================\n";
    cout << "🖥️  Simple System Monitor Tool – Annanya Mohanty\n";
    cout << "=====================================\n";
    while (true) {
        cout << "\n🔄 Refreshing system data...\n\n";
        cout << "⚙️ CPU Usage : " << fixed << setprecision(2) << getCPUUsage() << "%\n";
        getMemoryUsage();
        listProcesses();
        killProcess();  // Optional feature
        cout << "\n(Press Ctrl + C to exit)  Refreshing in 3 seconds...\n";
        sleep(3);
        system("clear");
    }
}
