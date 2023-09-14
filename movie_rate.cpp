#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>

using namespace std;

struct MovieRating {
    string movie_id;
    string rating;
};

map<string, vector<MovieRating>> get_organised_data(vector<string> all_data) {
    map<string, vector<MovieRating>> organsied_rating_data;
    string userid, temp;
    MovieRating single_movie_rating;
    for (const string &single_rating_line: all_data) {
        stringstream ss(single_rating_line);
        if (getline(ss, userid, ':') && getline(ss, temp, ':') && getline(ss, single_movie_rating.movie_id, ':') &&
            getline(ss, temp, ':') && getline(ss, single_movie_rating.rating, ':')) {
            organsied_rating_data[userid].push_back(single_movie_rating);
        }
    }
    return organsied_rating_data;
}

vector<string>
get_prediction(map<string, vector<MovieRating>> rating_data, vector<string> users_with_movies, string user_id,
               string movie_id) {
    vector<string> prediction;
    string closest_user, best_rating;
    double best_distance = 10000000;

    // Get user to be predicted for
    vector<MovieRating> user_id_data;
    auto iter = rating_data.begin();
    while (iter != rating_data.end()) {
        if (iter->first == user_id) {
            user_id_data = iter->second;
            break;
        }
        ++iter;
    }

    for (string user: users_with_movies) {
        double movies_in_common = 0;
        double score = 0;
        vector<MovieRating> user_data = rating_data[user];
        if (user_id_data.size() <= user_data.size()) {
            for (MovieRating rating: user_id_data) {
                for (MovieRating r: user_data) {
                    if (r.movie_id == rating.movie_id) {
                        ++movies_in_common;
                        score += abs(stoi(rating.rating) - stoi(r.rating));
                    }
                }
            }
        } else {
            for (MovieRating rating: user_data) {
                for (MovieRating r: user_id_data) {
                    if (r.movie_id == rating.movie_id) {
                        ++movies_in_common;
                        score += abs(stoi(rating.rating) - stoi(r.rating));
                    }
                }
            }
        }
        score = score / movies_in_common;

        if (score < best_distance) {
            best_distance = score;
            closest_user = user;

        }
    }

    prediction.push_back(closest_user);
    prediction.push_back(to_string(best_distance));

    return prediction;
}

int main(int argc, char *argv[]) {

    map<string, vector<MovieRating>> organsied_rating_data;
    ifstream ratings_file("/Users/pradeep/CLionProjects/assign1Mult/ratings.dat");
    string rating_data;
    string user_id = argv[1], movie_id = argv[2];
    vector<string> all_data;
    vector<MovieRating> user_id_data;
    stringstream rating_line(rating_data);
    string data;
    while (getline(ratings_file, rating_data)) {
        all_data.push_back(rating_data);
    }
    ratings_file.close();
    organsied_rating_data = get_organised_data(all_data);

    // Fetch data of the user to be predicted for
    auto iter = organsied_rating_data.begin();
    while (iter != organsied_rating_data.end()) {
        if (iter->first == user_id) {
            user_id_data = iter->second;
            break;
        }
        ++iter;
    }

    // Filter only those users which have required movie rating
    vector<string> users_with_movie;
    iter = organsied_rating_data.begin();
    while (iter != organsied_rating_data.end()) {
        for (auto data: iter->second) {
            if (data.movie_id == movie_id) {
                users_with_movie.push_back(iter->first);
                break;
            }
        }
        ++iter;
    }

    // Get prediction and distance
    vector<string> prediction = get_prediction(organsied_rating_data, users_with_movie, user_id, movie_id);
    string pred_rating;
    vector<MovieRating> mr = organsied_rating_data.at(prediction[0]);
    for (MovieRating mrs: mr) {
        if (mrs.movie_id == movie_id) {
            pred_rating = mrs.rating;
            break;
        }
    }

    cout << "The predicted rating for UserID: " << user_id << " and MovieID: " << movie_id << " is "
         << pred_rating << " based on the most similar user: " << prediction[0] << " with a ratingdistance: "
         << prediction[1] << endl;
    return 0;
}

