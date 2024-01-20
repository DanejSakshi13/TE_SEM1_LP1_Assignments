#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

using namespace std;

int pageHits = 0;
int pageMisses = 0;

void simulateFIFO(vector<int>& pages, int frameCount) {
    queue<int> frameQueue; //queue to represent the frame 
    unordered_set<int> frameSet; //unordered set for fast lookup of pages in the frame

    pageHits = 0;
    pageMisses = 0;

    for (int page : pages) {
        if (frameSet.find(page) != frameSet.end()) {
            pageHits++;
        } else {
            pageMisses++;
            if (frameQueue.size() == frameCount) {
                int oldPage = frameQueue.front();
                frameSet.erase(oldPage);
                frameQueue.pop();
            }

            frameQueue.push(page);
            frameSet.insert(page);
        }
    }

    cout << "FIFO Page Replacement Algorithm - Page Hits: " << pageHits << " Page Misses: " << pageMisses << endl;
}

void simulateLRU(vector<int>& pages, int frameCount) {
    int pageFaults = 0;
    unordered_map<int, int> pageToFrame;
    vector<int> frameQueue;

    pageHits = 0;
    pageMisses = 0;

    for (int page : pages) {
        if (pageToFrame.find(page) != pageToFrame.end()) {
            pageHits++;
            int frameIndex = pageToFrame[page];
            frameQueue.erase(frameQueue.begin() + frameIndex);
            frameQueue.push_back(page);
            for (int i = frameIndex; i < frameQueue.size(); ++i) {
                pageToFrame[frameQueue[i]] = i;
            }
        } else {
            pageMisses++;
            if (frameQueue.size() == frameCount) {
                int leastRecentPage = frameQueue.front();
                pageToFrame.erase(leastRecentPage);
                frameQueue.erase(frameQueue.begin());
            }

            frameQueue.push_back(page);
            pageToFrame[page] = frameQueue.size() - 1;
        }
    }

    cout << "LRU Page Replacement Algorithm - Page Hits: " << pageHits << " Page Misses: " << pageMisses << endl;
}

void simulateMRU(vector<int>& pages, int frameCount) {
    int pageFaults = 0;
    unordered_map<int, int> pageToFrame;
    vector<int> frameQueue;

    pageHits = 0;
    pageMisses = 0;

    for (int page : pages) {
        if (pageToFrame.find(page) != pageToFrame.end()) {
            pageHits++;
            int frameIndex = pageToFrame[page];
            frameQueue.erase(frameQueue.begin() + frameIndex);
            frameQueue.insert(frameQueue.begin(), page);
            for (int i = 0; i < frameQueue.size(); ++i) {
                pageToFrame[frameQueue[i]] = i;
            }
        } else {
            pageMisses++;
            if (frameQueue.size() == frameCount) {
                int mostRecentPage = frameQueue.front();
                pageToFrame.erase(mostRecentPage);
                frameQueue.pop_back();
            }

            frameQueue.insert(frameQueue.begin(), page);
            pageToFrame[page] = 0;
        }
    }

    cout << "MRU Page Replacement Algorithm - Page Hits: " << pageHits << " Page Misses: " << pageMisses << endl;
}

void simulateOptimal(vector<int>& pages, int frameCount) {
    int pageFaults = 0;
    unordered_set<int> frameSet;
    vector<int> futureReference(pages.size(), -1);

    pageHits = 0;
    pageMisses = 0;

    for (int i = 0; i < pages.size(); i++) {
        int page = pages[i];
        if (frameSet.find(page) != frameSet.end()) {
            pageHits++;
        } else {
            pageMisses++;
            if (frameSet.size() == frameCount) {
                int pageToReplace = -1;
                int farthestDistance = -1;
                for (int frame : frameSet) {
                    if (futureReference[frame] == -1) {
                        pageToReplace = frame;
                        break;
                    }
                    if (futureReference[frame] > farthestDistance) {
                        farthestDistance = futureReference[frame];
                        pageToReplace = frame;
                    }
                }
                frameSet.erase(pageToReplace);
            }

            frameSet.insert(page);
        }
        futureReference[page] = i;
    }

    cout << "Optimal Page Replacement Algorithm - Page Hits: " << pageHits << " Page Misses: " << pageMisses << endl;
}

int main() {
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 2, 1};
    int frameCount = 4;

    simulateFIFO(pages, frameCount);
    simulateLRU(pages, frameCount);
    simulateMRU(pages, frameCount);
    simulateOptimal(pages, frameCount);

    return 0;
}
