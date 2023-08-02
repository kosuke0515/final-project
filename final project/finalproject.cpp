#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <chrono>
#include <string>
#include <random>

using namespace std;
using namespace std::chrono;

struct Score {
    string name;
    int attempts;
    int time;

    Score(const string& playerName, int attempts, int time)
        : name(playerName), attempts(attempts), time(time) {}

    int getTotalScore() const {
        return 2 * attempts + time;
    }
};

bool compareScores(const Score& score1, const Score& score2) {
    return score1.getTotalScore() < score2.getTotalScore();
}

vector<int> RandomNumber(int digits) {
    unsigned int seed = static_cast<unsigned int>(high_resolution_clock::now().time_since_epoch().count());
    mt19937 rng(seed);

    vector<int> numbers;
    for (int i = 0; i <= 9; ++i) {
        numbers.push_back(i);
    }

    shuffle(numbers.begin(), numbers.end(), rng);

    vector<int> result(numbers.begin(), numbers.begin() + digits);
    return result;
}

void calculateHB(const vector<int>& target, const vector<int>& guess, int& hits, int& blows) {
    hits = blows = 0;

    for (auto targetIt = target.begin(); targetIt != target.end(); ++targetIt) {
        auto guessIt = find(guess.begin(), guess.end(), *targetIt);
        if (guessIt != guess.end()) {
            if (distance(guess.begin(), guessIt) == distance(target.begin(), targetIt)) {
                ++hits;
            }
            else {
                ++blows;
            }
        }
    }
}

vector<int> getGuess(int digits) {
    vector<int> guessVec;
    bool validInput = false;

    while (!validInput) {
        string guess;
        cout << "�\�z����" << digits << "���̐�������͂��Ă�������: ";
        cin >> guess;

        if (guess.size() == digits) {
            bool hasDuplicate = false;
            for (size_t i = 0; i < guess.size(); ++i) {
                for (size_t j = i + 1; j < guess.size(); ++j) {
                    if (guess[i] == guess[j]) {
                        hasDuplicate = true;
                        break;
                    }
                }
                if (hasDuplicate) {
                    break;
                }
            }

            if (!hasDuplicate) {
                bool validNumber = true;
                for (size_t i = 0; i < guess.size(); ++i) {
                    if (!isdigit(guess[i])) {
                        validNumber = false;
                        break;
                    }
                }

                if (validNumber) {
                    guessVec.clear();
                    for (size_t i = 0; i < guess.size(); ++i) {
                        guessVec.push_back(guess[i] - '0');
                    }
                    validInput = true;
                }
                else {
                    cout << "�����ȊO�̕������܂܂�Ă��܂��B������x���͂��Ă��������B" << endl;
                }
            }
            else {
                cout << "�d�������������܂܂�Ă��܂��B������x���͂��Ă��������B" << endl;
            }
        }
        else {
            cout << digits << "���̐�������͂��Ă��������B������x���͂��Ă��������B" << endl;
        }
    }

    return guessVec;
}

string getPlayerName() {
    string name;
    cout << "�v���C���[�̖��O����͂��Ă�������: ";
    cin.ignore();
    getline(cin, name);
    return name;
}

int main() {
    bool playAgain = true;
    vector<Score> scores;

    while (playAgain) {
        int level;
        cout << "��Փx��I�����Ă��������i1: ����, 2: ����, 3: �㋉�j: ";
        cin >> level;

        int digits;
        switch (level) {
        case 1:
            digits = 3;
            break;
        case 2:
            digits = 4;
            break;
        case 3:
            digits = 5;
            break;
        default:
            cout << "�����ȑI���ł��B�����i3���j�Ńv���C���܂��B" << endl;
            digits = 3;
            break;
        }

        vector<int> target = RandomNumber(digits);

        cout << "<Hit&Blow�Q�[��>" << endl;
        string playerName = getPlayerName();
        cout << "�悤�����A" << playerName << "����I" << endl;
        cout << "�R���s���[�^����������" << digits << "���̐����𓖂ĂĂ��������B\n"
            << "�����̈ʒu�������Ă����Hit�A�ʒu�͍����Ă��Ȃ������̐������܂܂�Ă����Blow�ƕ\������܂��B\n"
            << "���Ȃ��񐔂œ��Ă���悤�Ɋ撣���Ă��������I" << endl;

        int attempts = 0;
        auto startTime = high_resolution_clock::now();

        while (true) {
            vector<int> playerGuess = getGuess(digits);

            int hits, blows;
            calculateHB(target, playerGuess, hits, blows);

            cout << "Hit: " << hits << ", Blow: " << blows << endl;

            if (hits == digits) {
                cout << "���߂łƂ��I�����ł��I" << endl;
                break;
            }

            ++attempts;
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(endTime - startTime);

        scores.push_back(Score(playerName, attempts, duration.count()));

        char playAgainInput;
        cout << "������x�v���C���܂����H (y/n): ";
        cin >> playAgainInput;
        playAgain = (playAgainInput == 'y' || playAgainInput == 'Y');

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) {
        return a.getTotalScore() < b.getTotalScore();
        });

    cout << "�����L���O�F" << endl;
    int rank = 1;
    for (const auto& score : scores) {
        int totalScore = score.getTotalScore();
        cout << "��" << rank << "��: " << score.name << "����, �g���C��: " << score.attempts << ", �o�ߎ���: " << score.time << "�b, �����X�R�A: " << totalScore << endl;
        ++rank;
    }

    return 0;
}
