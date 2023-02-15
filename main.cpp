//
//  Project 1 Starter Code - DNA Profiling
//  
//  Ryder Douglas
// Creative component: debug() function, accessed by entering "debug" at main menu prompt. Automates user input and allows user to advance through each command
// by pressing enter key. Allows user to specify whether to run program on large.txt or small.txt and uses appropriate DNA sequence files.

#include <fstream>
#include <string>
#include <sstream>

#include "ourvector.h"

using namespace std;

// Takes filename and two nested ourvectors as parameters. Reads file, storing STRs in one nested ourvector
// and database entries in another
void load_db(string filename, ourvector<ourvector<string>>& database, ourvector<ourvector<char>>& tandems) {
    fstream fs;
    ourvector<char> STR;
    ourvector<string> entry;
    char c;
    string line, count;

    cout << "Loading database..." << endl;

    fs.open(filename);

    // error check
    if (!fs.is_open()) {
        cout << "Error: unable to open \'" << filename << "\'\n";
        return;
    }

    // Get first line, lose 'name', store STR strings in vector
    fs.ignore(256, ',');

    while (fs.get(c)) {
        if (c == '\n') {            // reached the end of the line, so STR vector holds all of the final STR, so push it to nested vector and we're done with the loop
            tandems.push_back(STR);
            break;
        } else if (c == ',') {      // reached the end of the STR, push it to the nested vector, and start again with next STR
            tandems.push_back(STR);
            STR.clear();
        } else {
            STR.push_back(c);
        }
    }

    // Process database entries
    while (getline(fs, line)) {
        stringstream ss(line);

        while (getline(ss, count, ',')) {
            entry.push_back(count);
        }
        database.push_back(entry);
        entry.clear();
    }

    fs.close();

}

// Displays loaded data. If a particular part has not been loaded, displays message saying so
void display(ourvector<ourvector<string>>& database, ourvector<char>& dna,
			 ourvector<int>& counts, ourvector<ourvector<char>>& tandems) {
	
	// make sure database is NOT empty
	if (database.size() < 1) {
		cout << "No database loaded." << endl;
	} else {	// if it's not, iterate through strings in each database entry and print them
		cout << "Database loaded: \n";
		for (auto entry : database) {
			for (auto s : entry) {
				cout << s << " ";
			}
			cout << endl;
		}
	}
	cout << endl;

    // output DNA string if loaded
	if (dna.size() < 1) {
		cout << "No DNA loaded." << endl;
	} else {
		cout << "DNA loaded: " << endl;
		for (auto c : dna) {
			cout << c;
		}
	}
	cout << endl;

	// output counts if DNA has been processed
	if (counts.size() < 1) {
		cout << "No DNA has been processed." << endl;
	} else {
		cout << "DNA processed, STR counts: " << endl;
		// format:
		// (STR) : (count)
		for (int i = 0; i < tandems.size(); i++) {
			for (auto c : tandems[i]) {
				cout << c;
			}
			cout << ": " << counts[i] << endl;
		}
		cout << endl;
	}
}

// Reads DNA sequence from file, storing it in an ourvector
void load_dna(string filename, ourvector<char>& dnaVector) {
    char c;

    fstream fs;

    cout << "Loading DNA..." << endl;

    fs.open(filename);
    if(!fs.is_open()) {
        cout << "Error: unable to open \'" << filename << "\'\n";
        return;
    }

    while (fs.get(c)) {
        dnaVector.push_back(c);
    }
    fs.close();
}

// helper function to compare two ourvector<char>
// returns true if ourvectors are equal, otherwise returns false
bool _compSequences(ourvector<char>& seq1, ourvector<char>& seq2) {
	for (int i = 0; i < seq1.size(); i++) {
		if (seq1[i] != seq2[i]) {
			return false;
		}
	}

	return true;
}

// Searches through DNA sequence loaded by load_dna, comparing to STRs loaded from database file
// Counts longest streak of consecutive repetitions for each STR, storing the counts in an ourvector
void process(ourvector<ourvector<char>>& tandems, ourvector<char>& dna, ourvector<int>& counts) {
    int currStreak, maxStreak, length, i, pos;
    ourvector<char> temp;

    cout << "Processing DNA..." << endl;

    for (auto strand : tandems) {
        length = strand.size();
        pos = 0;
        maxStreak = 0;
        currStreak = 0;

        // iterate through entire dna vector
        while (pos <= dna.size() - length) {
            // temp holds each sequence to compare
            for (i = pos; i < length + pos; i++) {
                temp.push_back(dna[i]);
            }

            // compare strand and temp to see if they match
            if (_compSequences(strand, temp)) {
                currStreak++;
                pos += length;
            } else {
                pos++;
                currStreak = 0;
            }
            if (maxStreak < currStreak) {
                maxStreak = currStreak;
            }
            temp.clear();
        }
        counts.push_back(maxStreak);
    }
    return;
}

// Searches database for entries whose STR counts match those found in the DNA sequence, and prints
// message telling user which entry matched, or that none matched
void search(ourvector<ourvector<string>>& database, ourvector<int>& counts) {
    string match;
    int i;

    cout << "Searching database..." << endl;

    match = "";

    for (auto entry : database) {
        for (i = 0; i < entry.size() - 1; i++) {
            if (counts[i] == stoi(entry[i+1])) {
                continue;
            } else {
                break;
            }
        }
        if (i == entry.size() - 1) {
            match = entry[0];
            cout << "Found in database!    DNA matches: " << match << endl;
            return;
        }
    }
    cout << "Not found in database." << endl;
}

// Creative component
// Allows user to quickly iterate through program, one command at a time, using the enter key to advance through commands
// size parameter allows user to specify whether to use small or large database files
void debug(string size, ourvector<ourvector<char>>& tandems, ourvector<ourvector<string>>& database, ourvector<char>& dna, ourvector<int>& counts) {
	string command;
	int counter;


	cout << "Debug activated." << endl;
	counter = 0;
	while (getline(cin, command)) {
		if (command == "#") {
			return;
		}
		counter++;

		// Load database
		switch(counter) {
			case 1:
				 load_db(size + ".txt", database, tandems);
				break;
		// Load DNA
			case 2:
				(size == "small") ? load_dna("1.txt", dna) : load_dna("6.txt", dna);
				break;
		// Process DNA
			case 3:
				process(tandems, dna, counts);
				break;
		// Display data
			case 4:
				cout << endl;
				display(database, dna, counts, tandems);
				break;
		// Search for match
			case 5:
				search(database, counts);
				break;
			default:
				cout << "Debug ended. Press # to return to main menu." << endl;
		}
		cout << "Press enter to proceed, or # to exit.";
	}
}

int main() {
    string command, filename;
    ourvector<ourvector<char>> tandems;
    ourvector<ourvector<string>> database;
    ourvector<char> dna;
    ourvector<int> counts;

    // Menu loop
    cout << "Welcome to the DNA Profiling Application." << endl;

    cout << "Enter command or # to exit: ";
    cin >> command;

    while (command != "#") {

        // load_db
        if (command == "load_db") {
            cin >> filename;
            database.clear();
            load_db(filename, database, tandems);
        }

        // Display
        else if (command == "display") {
            display(database, dna, counts, tandems);
        }
        
        // load_dna
        else if (command == "load_dna") {
            cin >> filename;
            dna.clear();
            load_dna(filename, dna);
        }

        // Process
        else if (command == "process") {
            if (database.size() < 1) {
                cout << "No database loaded." << endl;
            } else if (dna.size() < 1) {
                cout << "No DNA loaded." << endl;
            } else {
                counts.clear();
                process(tandems, dna, counts);
            }
        }

        // Search
        else if (command == "search") {
            if (database.size() < 1) {
                cout << "No database loaded." << endl;
            } else if (dna.size() < 1) {
                cout << "No DNA loaded." << endl;
            } else if (counts.size() < 1) {
                cout << "No DNA processed." << endl;
            } else {
                search(database, counts);
            }
        }

        // Debug (creative component)
        else if (command == "debug") {
			cin >> filename;
			database.clear();
			dna.clear();
			counts.clear();
			debug(filename, tandems, database, dna, counts);
		}

        cout << "Enter command or # to exit: ";
        cin >> command;
    }

    return 0;
}
