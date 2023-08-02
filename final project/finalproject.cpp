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
        cout << "予想した" << digits << "桁の数字を入力してください: ";
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
                    cout << "数字以外の文字が含まれています。もう一度入力してください。" << endl;
                }
            }
            else {
                cout << "重複した数字が含まれています。もう一度入力してください。" << endl;
            }
        }
        else {
            cout << digits << "桁の数字を入力してください。もう一度入力してください。" << endl;
        }
    }

    return guessVec;
}

string getPlayerName() {
    string name;
    cout << "プレイヤーの名前を入力してください: ";
    cin.ignore();
    getline(cin, name);
    return name;
}

int main() {
    bool playAgain = true;
    vector<Score> scores;

    while (playAgain) {
        int level;
        cout << "難易度を選択してください（1: 初級, 2: 中級, 3: 上級）: ";
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
            cout << "無効な選択です。初級（3桁）でプレイします。" << endl;
            digits = 3;
            break;
        }

        vector<int> target = RandomNumber(digits);

        cout << "<Hit&Blowゲーム>" << endl;
        string playerName = getPlayerName();
        cout << "ようこそ、" << playerName << "さん！" << endl;
        cout << "コンピュータが生成した" << digits << "桁の数字を当ててください。\n"
            << "数字の位置が合っていればHit、位置は合っていないがその数字が含まれていればBlowと表示されます。\n"
            << "少ない回数で当てられるように頑張ってください！" << endl;

        int attempts = 0;
        auto startTime = high_resolution_clock::now();

        while (true) {
            vector<int> playerGuess = getGuess(digits);

            int hits, blows;
            calculateHB(target, playerGuess, hits, blows);

            cout << "Hit: " << hits << ", Blow: " << blows << endl;

            if (hits == digits) {
                cout << "おめでとう！正解です！" << endl;
                break;
            }

            ++attempts;
        }

        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(endTime - startTime);

        scores.push_back(Score(playerName, attempts, duration.count()));

        char playAgainInput;
        cout << "もう一度プレイしますか？ (y/n): ";
        cin >> playAgainInput;
        playAgain = (playAgainInput == 'y' || playAgainInput == 'Y');

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    sort(scores.begin(), scores.end(), [](const Score& a, const Score& b) {
        return a.getTotalScore() < b.getTotalScore();
        });

    cout << "ランキング：" << endl;
    int rank = 1;
    for (const auto& score : scores) {
        int totalScore = score.getTotalScore();
        cout << "第" << rank << "位: " << score.name << "さん, トライ回数: " << score.attempts << ", 経過時間: " << score.time << "秒, 総合スコア: " << totalScore << endl;
        ++rank;
    }

    return 0;
}
