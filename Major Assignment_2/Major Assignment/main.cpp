    #include <iostream>
    #include <vector>
    #include <cstdlib>
    #include <ctime>
    #include <fstream>
    #include <string>
    #include <algorithm>
    #include <iomanip>

    using namespace std;

    // Function declarations
    void displayRules();
    vector<int> generateNumbers(int count, int maxNum);
    void saveTicketToFile(const vector<vector<int>>& tickets, const vector<vector<int>>& bonusLines, const vector<int>& encoreLine, const string& fileName);
    void saveFinalResults(const vector<vector<int>>& tickets, const vector<vector<int>>& bonusLines, const vector<int>& winningNumbers, const vector<int>& encoreLine, int totalLines, int totalCost, bool encoreOption);
    int calculateWinnings(const vector<int>& ticket, const vector<int>& winningNumbers);
    string generateFileName();
    void playLottery();
    bool askEncore();
    bool askYesNo(const string& prompt);

    int main() {
        srand(static_cast<unsigned>(time(0))); // Seed random number generator

        if (!askYesNo("Would you like to play Lotario? (y/n): ")) {
            cout << "Thank you! Maybe next time.\n";
            return 0;
        }

        char playAgain = 'y';
        while (playAgain == 'y' || playAgain == 'Y') {
            displayRules();
            playLottery();
            playAgain = askYesNo("Do you want to play again? (y/n): ") ? 'y' : 'n';
        }

        cout << "Thank you for playing Lotario!\n";
        return 0;
    }

    void displayRules() {
        cout << "\nWelcome to Lotario!\n";
        cout << "Rules:\n";
        cout << "- Each ticket consists of 6 random numbers between 1 and 45.\n";
        cout << "- A free bonus line will be added for each ticket purchased.\n";
        cout << "- Price per line: $3\n";
        cout << "- Encore (optional): $1 extra\n";
        cout << "- Match 3 numbers: $10\n";
        cout << "- Match 4 numbers: $100\n";
        cout << "- Match 5 numbers: $1000\n";
        cout << "- Match 6 numbers: JACKPOT!\n";
        cout << "- Good luck!\n";
    }

    vector<int> generateNumbers(int count, int maxNum) {
        vector<int> numbers;
        while (numbers.size() < count) {
            int randomNumber = rand() % maxNum + 1;
            if (find(numbers.begin(), numbers.end(), randomNumber) == numbers.end()) {
                numbers.push_back(randomNumber);
            }
        }
        return numbers;
    }

    string generateFileName() {
        time_t currentTime = time(nullptr);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "ticket_%Y%m%d_%H%M%S.txt", localtime(&currentTime));
        return buffer;
    }

    void saveTicketToFile(const vector<vector<int>>& tickets, const vector<vector<int>>& bonusLines, const vector<int>& encoreLine, const string& fileName) {
        ofstream ticketFile(fileName);
        if (!ticketFile) {
            cerr << "Error: Could not open file " << fileName << " for writing.\n";
            return;
        }

        cout << "\n===== Your Lotario Ticket =====\n";
        ticketFile << "Lotario Ticket\n\n";

        for (size_t i = 0; i < tickets.size(); ++i) {
            cout << "Lottario Line " << (i + 1) << ": ";
            ticketFile << "Lottario Line " << (i + 1) << ": ";
            for (int num : tickets[i]) {
                cout << setw(2) << setfill('0') << num << ", ";
                ticketFile << setw(2) << setfill('0') << num << ", ";
            }
            cout << "\n";
            ticketFile.seekp(-2, ios_base::cur);
            ticketFile << "\n";

            cout << "Bonus Line " << (i + 1) << ": ";
            ticketFile << "Bonus Line " << (i + 1) << ": ";
            for (int num : bonusLines[i]) {
                cout << setw(2) << setfill('0') << num << ", ";
                ticketFile << setw(2) << setfill('0') << num << ", ";
            }
            cout << "\n";
            ticketFile << "\n";
        }

        cout << "Encore Line: ";
        ticketFile << "\nEncore Line: ";
        for (int num : encoreLine) {
            cout << num << " ";
            ticketFile << num << " ";
        }
        cout << "\n===============================\n";
        ticketFile << "\n";

        ticketFile.close();
        cout << "Ticket saved as " << fileName << "\n";
    }

    int calculateWinnings(const vector<int>& ticket, const vector<int>& winningNumbers) {
        int matchCount = 0;
        for (int num : ticket) {
            if (find(winningNumbers.begin(), winningNumbers.end(), num) != winningNumbers.end()) {
                ++matchCount;
            }
        }

        switch (matchCount) {
            case 3: return 10;
            case 4: return 100;
            case 5: return 1000;
            case 6: return 1000000; // Jackpot
            default: return 0;
        }
    }

    void saveFinalResults(const vector<vector<int>>& tickets, const vector<vector<int>>& bonusLines, const vector<int>& winningNumbers, const vector<int>& encoreLine, int totalLines, int totalCost, bool encoreOption) {
        string fileName = "final_result.txt"; // Use a relative path for portability
        ofstream resultFile(fileName, ios::app);
        if (!resultFile) {
            cerr << "Error: Could not open file " << fileName << " for writing.\n";
            return;
        }

        cout << "\n===== Lucky Draw Results =====\n";
        resultFile << "===== Lucky Draw Result =====\n";

        // Display basic information
        cout << "Total lines played: " << totalLines << "\n";
        resultFile << "Total lines played: " << totalLines << "\n";

        cout << "Encore: " << (encoreOption ? "Yes" : "No") << "\n";
        resultFile << "Encore: " << (encoreOption ? "Yes" : "No") << "\n";

        cout << "Total cost: $" << totalCost << "\n\n";
        resultFile << "Total cost: $" << totalCost << "\n\n";

        int totalWinnings = 0;

        // Display and write each Lottario line with winnings
        for (size_t i = 0; i < tickets.size(); ++i) {
            int winnings = calculateWinnings(tickets[i], winningNumbers);
            totalWinnings += winnings;

            cout << "Lottario Line " << (i + 1) << ": ";
            resultFile << "Lottario Line " << (i + 1) << ": ";
            for (int num : tickets[i]) {
                cout << setw(2) << setfill('0') << num << ", ";
                resultFile << setw(2) << setfill('0') << num << ", ";
            }
            cout << " -> Winnings: $" << winnings << "\n";
            resultFile << " -> Winnings: $" << winnings << "\n";

            cout << "Bonus Line " << (i + 1) << ": ";
            resultFile << "Bonus Line " << (i + 1) << ": ";
            for (int num : bonusLines[i]) {
                cout << setw(2) << setfill('0') << num << ", ";
                resultFile << setw(2) << setfill('0') << num << ", ";
            }
            cout << "\n";
            resultFile << "\n";
        }

        // Display and write Encore line
        if (encoreOption) {
            cout << "\nEncore Line: ";
            resultFile << "\nEncore Line: ";
            for (int num : encoreLine) {
                cout << num << " ";
                resultFile << num << " ";
            }
            cout << "\n";
            resultFile << "\n";
        }

        // Display and write winning numbers
        cout << "\nWinning Numbers: ";
        resultFile << "\nWinning Numbers: ";
        for (int num : winningNumbers) {
            cout << setw(2) << setfill('0') << num << " ";
            resultFile << setw(2) << setfill('0') << num << " ";
        }
        cout << "\n";

        // Display and write total winnings
        cout << "Total Winnings: $" << totalWinnings << "\n";
        resultFile << "\nTotal Winnings: $" << totalWinnings << "\n";

        resultFile.close();
        cout << "Results appended to " << fileName << "\n";
    }

    void playLottery() {
        int numOfLines;
        bool encoreOption = askEncore();

        cout << "\nEnter the number of lines to play: ";
        cin >> numOfLines;
        while (numOfLines < 1) {
            cout << "Invalid input! Enter a valid number of lines: ";
            cin >> numOfLines;
        }

        int totalCost = numOfLines * 3 + (encoreOption ? 1 : 0);
        cout << "Total cost: $" << totalCost << "\n";

        if (!askYesNo("Do you want to proceed with this purchase? (y/n): ")) {
            cout << "Purchase canceled.\n";
            return;
        }

        vector<vector<int>> tickets;
        vector<vector<int>> bonusLines;
        for (int i = 0; i < numOfLines; ++i) {
            tickets.push_back(generateNumbers(6, 45));
            bonusLines.push_back(generateNumbers(6, 45));
        }
        vector<int> winningNumbers = generateNumbers(6, 45);
        vector<int> encoreLine = generateNumbers(7, 9);

        string fileName = generateFileName();
        saveTicketToFile(tickets, bonusLines, encoreLine, fileName);
        saveFinalResults(tickets, bonusLines, winningNumbers, encoreLine, numOfLines, totalCost, encoreOption);
    }

    bool askEncore() {
        return askYesNo("Would you like to play Encore for an additional $1? (y/n): ");
    }

    bool askYesNo(const string& prompt) {
        char response;
        cout << prompt;
        cin >> response;
        while (response != 'y' && response != 'Y' && response != 'n' && response != 'N') {
            cout << "Invalid input. " << prompt;
            cin >> response;
        }
        return (response == 'y' || response == 'Y');
    }
