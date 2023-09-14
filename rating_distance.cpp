#include <iostream>
#include<fstream>
#include<map>
#include "vector"
#include "sstream"

using namespace std;

vector<string> get_rating_data(string rating_data) {
    vector<string> data;
    string temp;
    stringstream ss(rating_data);
    while (getline(ss, temp, ':')) {
        data.push_back(temp);
    }
    return data;
}

int main(int argc, char *argv[]) {
    if (argc == 3) {
        double score = 0;
        int number_of_movies_common = 0;
        map<string, string> user1_ratings, user2_ratings;
        string user1_id = argv[1], user2_id = argv[2];
        ifstream ratings_file("/Users/pradeep/CLionProjects/assign1Mult/ratings.dat");
        string rating_data;
        vector<string> all_data;
        // Todo check if arguments have space in them

        // Get data from file
        while (getline(ratings_file, rating_data)) {
            all_data.push_back(rating_data);
        }
        ratings_file.close();

        //Create
        for (const auto rating: all_data) {
            if (rating.rfind(user1_id, 0) != 0 && rating.rfind(user2_id, 0) != 0)
                continue;
            vector<string> rating_split = get_rating_data(rating);
            if (rating_split[0] == user1_id) {
                user1_ratings.insert({rating_split[2], rating_split[4]});
            } else if (rating_split[0] == user2_id) {
                user2_ratings.insert({rating_split[2], rating_split[4]});
            } else {}
        }

        // Calculate score

        if (user1_ratings.size() <= user2_ratings.size()) {
            auto iter = user1_ratings.begin();
            while (iter != user1_ratings.end()) {
                if ((user2_ratings.find(iter->first) != user2_ratings.end())) {
                    ++number_of_movies_common;
                    score += abs(stoi(iter->second) - stoi(user2_ratings.at(iter->first)));
                }
                ++iter;
            }
        } else {
            auto iter = user2_ratings.begin();
            while (iter != user2_ratings.end()) {
                if (user1_ratings.find(iter->first) != user1_ratings.end()) {
                    ++number_of_movies_common;
                    score += abs(stoi(iter->second) - stoi(user1_ratings[iter->first]));
                }
            }
        }
        if (number_of_movies_common > 0) {
            score = score / number_of_movies_common;
        }
        cout << "The Rating Distance is " << score << endl;
    } else {
        cout << "expects only 2 numerical arguments" << endl;
        return -1;
    }
    return 0;
}
