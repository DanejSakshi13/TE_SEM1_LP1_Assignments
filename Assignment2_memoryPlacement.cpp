#include <iostream>
#include <climits>

using namespace std;

// Structure to represent a memory block
struct MemoryBlock {
    int id; // Process ID allocated to the block
    int size; // Size of the memory block
    bool allocated; // Indicates whether the memory block is allocated or not
};

// Function to find the best-fit block for a process
int bestFit(MemoryBlock memory[], int numMemoryBlocks, int processSize) {
    int bestFitBlock = -1;
    int minFragmentation = INT_MAX;

    for (int i = 0; i < numMemoryBlocks; ++i) {
        if (!memory[i].allocated && memory[i].size >= processSize) {
            int fragmentation = memory[i].size - processSize;
            if (fragmentation < minFragmentation) {
                bestFitBlock = i;
                minFragmentation = fragmentation;
            }
        }
    }

    return bestFitBlock;
}

// Function to find the worst-fit block for a process
int worstFit(MemoryBlock memory[], int numMemoryBlocks, int processSize) {
    int worstFitBlock = -1;
    int maxFragmentation = -1;

    for (int i = 0; i < numMemoryBlocks; ++i) {
        if (!memory[i].allocated && memory[i].size >= processSize) {
            int fragmentation = memory[i].size - processSize;
            if (fragmentation > maxFragmentation) {
                worstFitBlock = i;
                maxFragmentation = fragmentation;
            }
        }
    }

    return worstFitBlock;
}

// Function to find the next fit block for a process
int nextFit(MemoryBlock memory[], int numMemoryBlocks, int startIndex, int processSize) {
    for (int i = startIndex; i < numMemoryBlocks; ++i) {
        if (!memory[i].allocated && memory[i].size >= processSize) {
            return i;
        }
    }

    for (int i = 0; i < startIndex; ++i) {
        if (!memory[i].allocated && memory[i].size >= processSize) {
            return i;
        }
    }

    return -1; // No suitable block found
}

int main() {
    int numMemoryBlocks, numProcesses;

    cout << "Enter the number of memory blocks: ";
    cin >> numMemoryBlocks;

    MemoryBlock memory[100]; // Assuming a maximum of 100 memory blocks

    // Initialize memory blocks
    cout << "Enter sizes for memory blocks (separated by spaces): ";
    for (int i = 0; i < numMemoryBlocks; ++i) {
        cin >> memory[i].size;
        memory[i].id = -1; // -1 indicates that the block is free
        memory[i].allocated = false;
    }

    cout << "Enter the number of processes: ";
    cin >> numProcesses;

    // Take input for process sizes
    int processSizes[100]; // Assuming a maximum of 100 processes
    for (int i = 0; i < numProcesses; ++i) {
        cout << "Enter size of Process P" << i + 1 << ": ";
        cin >> processSizes[i];
    }

    int startIndex = 0; // Start searching for available blocks from the beginning
    cout << "PID\tPSIZE\tBLOCKNO\tFRAGMENTATION" << endl;

    // Allocate memory to processes using different strategies based on user choice
    for (int i = 0; i < numProcesses; ++i) {
        int processSize = processSizes[i];
        int blockIndex = -1;

        int strategy;
        cout << "Choose memory placement strategy for Process P" << i + 1 << ": " << endl;
        cout << "1. Best Fit" << endl;
        cout << "2. First Fit" << endl;
        cout << "3. Next Fit" << endl;
        cout << "4. Worst Fit" << endl;
        cout << "Enter your choice (1-4): ";
        cin >> strategy;

        switch (strategy) {
            case 1: // Best Fit
                blockIndex = bestFit(memory, numMemoryBlocks, processSize);
                break;
            case 2: // First Fit
                for (int j = 0; j < numMemoryBlocks; ++j) {
                    if (!memory[j].allocated && memory[j].size >= processSize) {
                        memory[j].allocated = true;
                        memory[j].id = i + 1; // Process ID
                        blockIndex = j + 1; // Block number
                        break;
                    }
                }
                break;
            case 3: // Next Fit
                blockIndex = nextFit(memory, numMemoryBlocks, startIndex, processSize);
                startIndex = (blockIndex + 1) % numMemoryBlocks;
                break;
            case 4: // Worst Fit
                blockIndex = worstFit(memory, numMemoryBlocks, processSize);
                break;
            default:
                cout << "Invalid choice. Using Best Fit as the default strategy." << endl;
                blockIndex = bestFit(memory, numMemoryBlocks, processSize);
                break;
        }

        if (blockIndex != -1) {
            memory[blockIndex - 1].allocated = true;
            memory[blockIndex - 1].id = i + 1; // Process ID
            int fragmentation = memory[blockIndex - 1].size - processSize;
            cout << i + 1 << "\t" << processSize << "\t" << "b" << blockIndex << "\t" << fragmentation << endl;
        } else {
            cout << i + 1 << "\t" << processSize << "\t-\t-" << endl;
        }
    }

    // Print unallocated memory blocks
    cout << "Unallocated memory blocks:" << endl;
    for (int i = 0; i < numMemoryBlocks; ++i) {
        if (!memory[i].allocated) {
            cout << "b" << i + 1 << "\t-";
            cout << endl;
        }
    }

    return 0;
}
