#include <iostream>
#include <queue>
#include <vector>
#include <climits> 
using namespace std;

// Structure to represent a process
struct Process {
    int id; // Process ID
    int arrivalTime; // Arrival time
    int burstTime; // Burst time
    int priority; // Priority (lower value indicates higher priority)
};

// Function to simulate FCFS scheduling
void FCFS(vector<Process>& processes) {
    int currentTime = 0;
    for (Process& process : processes) {
        if (process.arrivalTime > currentTime) {
            currentTime = process.arrivalTime;
        }
        cout << "Process " << process.id << " starts at time " << currentTime << endl;
        currentTime += process.burstTime;
        cout << "Process " << process.id << " finishes at time " << currentTime << endl;
    }
}

// Function to simulate SJF (Preemptive) scheduling
void SJF_Preemptive(vector<Process>& processes) {
    vector<int> remainingTime(processes.size(), 0);

    int currentTime = 0;
    int completed = 0;
    while (completed < processes.size()) {
        int minBurst = INT_MAX;
        int minIndex = -1;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrivalTime <= currentTime && remainingTime[i] < minBurst) {
                minBurst = remainingTime[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            currentTime++;
        } else {
            remainingTime[minIndex]++;
            if (remainingTime[minIndex] == processes[minIndex].burstTime) {
                completed++;
                cout << "Process " << processes[minIndex].id << " starts at time " << currentTime << endl;
                cout << "Process " << processes[minIndex].id << " finishes at time " << currentTime + 1 << endl;
                currentTime++;
            }
        }
    }
}

// Function to simulate Priority (Non-Preemptive) scheduling
void Priority_NonPreemptive(vector<Process>& processes) {
    int currentTime = 0;
    while (!processes.empty()) {
        int minPriority = INT_MAX;
        int minIndex = -1;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].priority < minPriority) {
                minPriority = processes[i].priority;
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            currentTime++;
        } else {
            Process selectedProcess = processes[minIndex];
            cout << "Process " << selectedProcess.id << " starts at time " << currentTime << endl;
            currentTime += selectedProcess.burstTime;
            cout << "Process " << selectedProcess.id << " finishes at time " << currentTime << endl;
            processes.erase(processes.begin() + minIndex);
        }
    }
}

// Function to simulate Round Robin (Preemptive) scheduling
void RoundRobin_Preemptive(vector<Process>& processes, int quantum) {
    queue<int> processQueue;
    vector<int> remainingTime(processes.size(), 0);

    int currentTime = 0;
    int completed = 0;
    processQueue.push(0);

    while (completed < processes.size()) {
        int currentProcess = processQueue.front();
        processQueue.pop();

        if (remainingTime[currentProcess] <= quantum) {
            currentTime += remainingTime[currentProcess];
            cout << "Process " << processes[currentProcess].id << " finishes at time " << currentTime << endl;
            completed++;
            remainingTime[currentProcess] = 0;
        } else {
            currentTime += quantum;
            remainingTime[currentProcess] -= quantum;
        }

        while (completed < processes.size() && processes[completed].arrivalTime <= currentTime) {
            processQueue.push(completed);
            completed++;
        }

        if (remainingTime[currentProcess] > 0) {
            processQueue.push(currentProcess);
        }
    }
}

int main() {
    int numProcesses, quantum;
    cout << "Enter the number of processes: ";
    cin >> numProcesses;
    cout << "Enter time quantum for Round Robin: ";
    cin >> quantum;

    vector<Process> processes(numProcesses);

    for (int i = 0; i < numProcesses; i++) {
        processes[i].id = i + 1;
        cout << "Enter arrival time for Process " << i + 1 << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter burst time for Process " << i + 1 << ": ";
        cin >> processes[i].burstTime;
        cout << "Enter priority for Process " << i + 1 << ": ";
        cin >> processes[i].priority;
    }

    int choice;
    cout << "Choose a scheduling algorithm:" << endl;
    cout << "1. FCFS" << endl;
    cout << "2. SJF (Preemptive)" << endl;
    cout << "3. Priority (Non-Preemptive)" << endl;
    cout << "4. Round Robin (Preemptive)" << endl;
    cout << "Enter your choice (1-4): ";
    cin >> choice;

    switch (choice) {
        case 1: // FCFS
            FCFS(processes);
            break;
        case 2: // SJF (Preemptive)
            SJF_Preemptive(processes);
            break;
        case 3: // Priority (Non-Preemptive)
            Priority_NonPreemptive(processes);
            break;
        case 4: // Round Robin (Preemptive)
            RoundRobin_Preemptive(processes, quantum);
            break;
        default:
            cout << "Invalid choice." << endl;
            break;
    }

    return 0;
}

