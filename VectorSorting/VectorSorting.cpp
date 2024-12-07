//============================================================================
// Name        : VectorSorting.cpp
// Author      : William Rosales
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <time.h>
#include "CSVparser.hpp"

using namespace std;


//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);


// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string department;
    string closeDate;
    string fund;
    double amount;
    string inventoryID;
    string vehicleID;
    string receiptNumber;

    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    std::cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
    Bid bid;

    std::cout << "Enter Id: ";
    cin.ignore();
    getline(cin, bid.bidId);

    std::cout << "Enter title: ";
    getline(cin, bid.title);

    std::cout << "Enter fund: ";
    cin >> bid.fund;

    std::cout << "Enter amount: ";
    cin.ignore();
    string strAmount;
    getline(cin, strAmount);
    bid.amount = strToDouble(strAmount, '$');

    return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
    std::cout << "Loading CSV file " << csvPath << endl;

    // Define a vector data structure to hold a collection of bids.
    vector<Bid> bids;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    try {
        // loop to read rows of a CSV file
        for (int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.department = file[i][2];
            bid.closeDate = file[i][3];
            bid.amount = strToDouble(file[i][4], '$');
            bid.inventoryID = file[i][5];
            bid.vehicleID = file[i][6];
            bid.receiptNumber = file[i][7];
            bid.fund = file[i][8];


            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // Debug statment to check department field
            //std::cout << "Loaded Bid: " << bid.bidId << " | VehicleID: " << bid.vehicleID << std::endl;

            // push this bid to the end
            bids.push_back(bid);
        }
    } catch (csv::Error &e) {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return bids;
}

// FIXME (2a): Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid>& bids, int begin, int end) {
    //set low and high equal to begin and end
    int low = begin;
    int high = end;
    // pick the middle element as pivot point
    int mid = begin + (end - begin) / 2;
    string pivot = bids[mid].title;
    // while not done 
    while (true) {
        // keep incrementing low index while bids[low] < bids[pivot]
        while (bids[low].title < pivot) {
            low++;
        }
        // keep decrementing high index while bids[high] > bids[pivot]
        while (bids[high].title > pivot) {
            high--;
        }
        /* If there are zero or one elements remaining,
            all bids are partitioned. Return high */
        if (low >= high) {
            break;
        }
        // else swap the low and high bids (built in vector method)
        else {
            swap(bids[low], bids[high]);
        }
        // move low and high closer ++low, --high
        ++low;
        --high;
    }
    //return high;
    return high;
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid>& bids, int begin, int end) {
    //set mid equal to 0
    /* Base case: If there are 1 or zero bids to sort,
     partition is already sorted otherwise if begin is greater
     than or equal to end then return*/
    if (begin >= end) {
        return;
    }
    /* Partition bids into low and high such that
     midpoint is location of last element in low */
    int pivotIndex = (begin + end) / 2;
    string pivot = bids[pivotIndex].title;

    int i = begin;
    int j = end;
    
    while (i <= j) {
        // recursively sort low partition (begin to mid)
        while (bids[i].title > pivot) {
            i++;
        }
        // recursively sort high partition (mid+1 to end)
        while (bids[j].title > pivot) {
            j--;
        }
        if (i <= j) {
            swap(bids[i], bids[j]);
            i++;
            j--;
        }
    }
    quickSort(bids, begin, j);
    quickSort(bids, i, end);
}

// FIXME (1a): Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid>& bids) {
    //define min as int (index of the current minimum bid)
    unsigned int min;
    // check size of bids vector
    unsigned int size = bids.size();
    // set size_t platform-neutral result equal to bid.size()
    // pos is the position within bids that divides sorted/unsorted
    // for size_t pos = 0 and less than size -1 
    for (size_t pos = 0; pos < size - 1; ++pos) {
        // set min = pos
        size_t min = pos;
        // loop over remaining elements to the right of position
        for (size_t j = pos + 1; j < size; ++j) {
            // if this element's title is less than minimum title
            if (bids[j].title.compare(bids[min].title) < 0) {
                // this element becomes the minimum
                min = j;
            }
        }
        // swap the current minimum with smaller one found
        if (min != pos) {
            // swap is a built in vector method
            swap(bids[pos], bids[min]);
        }
    }
}

void displayFilterMenu(vector<Bid>& bids); 
void filterByFund(vector<Bid>& bids);

void displayFilteredBids(vector<Bid>& bids, int filterChoice);


// Bid Filtering
void displayFilterMenu(vector<Bid>& bids) {
    // Debug statement
    // std::cout << "Entering displayFilterMenu" << std::endl;
    int filterChoice = 0;
    while (true) {
        std::cout << "Filter by:" << endl;
        std::cout << " 1. ArticleTitle" << endl;
        std::cout << " 2. ArticleID" << endl;
        std::cout << " 3. Department" << endl;
        std::cout << " 4. CloseDate" << endl;
        std::cout << " 5. WinningBid" << endl;
        std::cout << " 6. InventoryID" << endl;
        std::cout << " 7. VehicleID" << endl;
        std::cout << " 8. ReceiptNumber" << endl;
        std::cout << " 9. Fund" << endl;
        std::cout << " 0. Exit" << endl;
        std::cout << "Enter choice: ";

        // Clear input buffer of unwanted characters
        std::cin >> filterChoice;

        if (!std::cin || filterChoice < 0 || filterChoice > 9) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number between 0 and 9." << std::endl;
            continue;
        }

        std::cout << "User selected: " << filterChoice << std::endl;

        if (filterChoice == 0) {
            std::cout << "Exiting displayFilterMenu" << std::endl;
            break;
        }

        switch (filterChoice) {
        case 1:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) { return a.title < b.title; });
            std::cout << "Sorted by ArticleTitle" << endl;
            break;

        case 2:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) { return stoi(a.bidId) < stoi(b.bidId); });
            std::cout << "Sorted by ArticleID" << endl;
            break;

        case 3:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) { return a.department < b.department; });
            std::cout << "Sorted by Department" << endl;
            break;

        case 4:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) {
                std::tm dateA = csv::parseDate(a.closeDate);
                std::tm dateB = csv::parseDate(b.closeDate);
                return std::mktime(&dateA) > std::mktime(&dateB);
                });
            std::cout << "Sorted by CloseDate" << endl;
            break;

        case 5:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) { return a.amount < b.amount; });
            std::cout << "Sorted by WinningBid" << endl;
            break;

        case 6:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) {
                try {
                    return stoi(a.inventoryID) < stoi(b.inventoryID);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid inventoryID: " << a.inventoryID << " or " << b.inventoryID << std::endl;
                    return false;
                }
                });
            std::cout << "Sorted by InventoryID" << std::endl;
            break;

        case 7:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) {
                if (a.vehicleID.empty() || b.vehicleID.empty()) {
                    return false; // Handles empty fields
                }
                try {
                    return stoi(a.vehicleID) < stoi(b.vehicleID);
                }
                catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid vehicleID: " << a.vehicleID << " or " << b.vehicleID << std::endl;
                    return false;
                }
                });
            std::cout << "Sorted by VehicleID" << std::endl;
            break;

        case 8:
            sort(bids.begin(), bids.end(), [](Bid a, Bid b) {
                if (!std::all_of(a.receiptNumber.begin(), a.receiptNumber.end(), ::isdigit) ||
                    !std::all_of(b.receiptNumber.begin(), b.receiptNumber.end(), ::isdigit)) {
                    std::cerr << "Invalid receiptNumber: " << a.receiptNumber << " or " << b.receiptNumber << std::endl;
                    return false;
                }
                try {
                    return stoi(a.receiptNumber) < stoi(b.receiptNumber);
                } catch (const std::invalid_argument& e) {
                    std::cerr << "Invalid receiptNumber: " << a.receiptNumber << " or " << b.receiptNumber << std::endl;
                    return false;
                }
                catch (const std::out_of_range& e) {
                    std::cerr << "Out of range receiptNumber: " << a.receiptNumber << " or " << b.receiptNumber << std::endl;
                    return false;
                }
            });
            std::cout << "Sorted by receiptNumber" << std::endl;
            break;

        case 9:
            filterByFund(bids);
            std::cout << "Filtering by Fund" << endl;
            break;

        default:
            std::cout << "Invalid choice. Please enter another option." << endl;
        }
        displayFilteredBids(bids, filterChoice);
        std::cout << "Displayed filtered bids" << std::endl;
    }
    //std::cout << "Exited the while loop in displayFilterMenu" << std::endl;
}

void filterByFund(vector<Bid>& bids) {
    int fundChoice = 0;
    while (fundChoice != 3) {
        std::cout << "Select Fund:" << endl;
        std::cout << " 1. Enterprise" << endl;
        std::cout << " 2. General Fund" << endl;
        std::cout << " 3. Exit" << endl;
        std::cout << "Enter Choice: ";
        cin >> fundChoice;

        vector<Bid> filteredBids;

        if (fundChoice == 1) { 
            copy_if(bids.begin(), bids.end(), back_inserter(filteredBids), [](Bid bid) { return bid.fund == "Enterprise"; }); 
        } else if (fundChoice == 2) { 
            copy_if(bids.begin(), bids.end(), back_inserter(filteredBids), [](Bid bid) { return bid.fund == "General Fund"; }); 
        } else if (fundChoice == 3) { 
            return; 
        } else { 
            std::cout << "Invalid choice. Please enter 1, 2, or 3." << endl; 
        } 
        displayFilteredBids(filteredBids, 9);
    }
}

void displayFilteredBids(vector<Bid>& bids, int filterChoice) {
    std::cout << "Displaying filtered bids with additional column" << std::endl;

    for (size_t i = 0; i < bids.size(); ++i) {
        displayBid(bids[i]);

        // Display additional column based on filterChoice
        switch (filterChoice) {
        case 1: 
            std::cout << " | ArticleTitle: " << bids[i].title;
            break;
        case 2:
            std::cout << " | ArticleID: " << bids[i].bidId;
            break;
        case 3:
            std::cout << " | Department: " << bids[i].department;
            //std::cout << " (Debug: Department field value: " << bids[i].department << ")"; // Debug statement
            break;
        case 4:
            std::cout << " | CloseDate: " << bids[i].closeDate;
            break;
        case 5:
            std::cout << " | WinningBid: " << bids[i].amount;
            //std::cout << " (Debug: WinningBid field value: " << bids[i].amount << ")"; // Debug statement
        case 6:
            std::cout << " | InventoryID: " << bids[i].inventoryID;
            break;
        case 7:
            std::cout << " | VehicleID: " << bids[i].vehicleID;
            break;
        case 8:
            std::cout << " | ReceiptNumber: " << bids[i].receiptNumber;
            break;
        }
        std::cout << std::endl;
    }
    std::cout << endl;
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath1 = "eBid_Monthly_Sales_Dec_2016.csv";
    string csvPath2 = "eBid_Monthly_Sales.csv";
    string csvPath;

    // Define a vector to hold all the bids
    vector<Bid> bids;

    // Define a timer variable
    clock_t ticks;

    // Initial menu for file selection
    int fileChoice = 0;
    while (fileChoice != 1 && fileChoice != 2) {
        std::cout << "Select file to load:" << endl;
        std::cout << " 1. eBid_Monthly_Sales_Dec_2016.csv" << endl;
        std::cout << " 2. eBid_Monthly_Sales.csv" << endl;
        std::cout << "Enter choice: ";
        cin >> fileChoice;

        if (fileChoice == 1) {
            csvPath = csvPath1;
        }
        else if (fileChoice == 2) {
            csvPath = csvPath2;
        }
        else {
            std::cout << "Invalid choice. Please enter 1 or 2. " << endl;
        }
    }

    std::cout << "Selected file path: " << csvPath << endl;

    try { 
        ticks = clock(); 
        bids = loadBids(csvPath); 
        std::cout << bids.size() << " bids read" << endl; 
        ticks = clock() - ticks; 
        std::cout << "time: " << ticks << " clock ticks" << endl; 
        std::cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl; 
    } catch (csv::Error &e) { 
        cerr << "CSV Error: " << e.what() << endl; 
        return 1; // Exit the program on error 
    } catch (exception &e) { 
        cerr << "Standard Exception: " << e.what() << endl; 
        return 1; // Exit the program on error 
    }
    catch (...) {
        cerr << "Unknown error occurred while loading CSV file." << endl;
        return 1; // Exit the program on error
    }

    int choice = 0;
    while (choice != 9) {
        std::cout << "Menu:" << endl;
        std::cout << "  1. Display All Bids" << endl;
        std::cout << "  2. Bid Filtering" << endl;
        std::cout << "  3. Selection Sort All Bids" << endl;
        std::cout << "  4. Quick Sort All Bids" << endl;
        std::cout << "  9. Exit" << endl;
        std::cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            // Loop and display the bids read
            //for (int i = 0; i < bids.size(); ++i) {
                //displayBid(bids[i]);
            //}
           // std::cout << endl;
            displayFilteredBids(bids, 0);
            break;

        case 2:
            // Call the function to display the filter menu
            //std::cout << "Calling displayFilterMenu" << std::endl; // Debug statement
            displayFilterMenu(bids);
            break;

        case 3:
            // Selection sort
            ticks = clock();
            selectionSort(bids);
            ticks = clock() - ticks;
            std::cout << "Selection Sort time: " << ticks << " clock ticks" << endl;
            std::cout << "Selection Sort time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
            
        case 4:
            // Quick sort
            ticks = clock();
            quickSort(bids, 0, bids.size() - 1);
            ticks = clock() - ticks;
            std::cout << "Quick Sort time: " << ticks << " clock ticks" << endl;
            std::cout << "Quick Sort time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 9:
            std::cout << "Exiting program" << std::endl;
            break;

        default:
            std::cout << "Invalid choice. Please enter a valid number" << std::endl;
        }
    }

    std::cout << "Good bye." << endl;

    return 0;
}
