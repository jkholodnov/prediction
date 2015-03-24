#include "../include/player.h"

player::player(const string& name)
{
    player_name = name;
}

player::~player()
{
    //dtor
}

void player::get_player_scores(shared_ptr<RInside_Container> R_Inside_Container){
    Database* predict_db = new Database("predict.db");

    string _query1 = "SELECT count(*) FROM gamedata WHERE Name = '" + player_name + "';";

    auto counts = predict_db->query(_query1);

    string _query = "SELECT a.minutes, a.fgm, a.fga, a.tpm, a.tpa, a.ftm, a.fta, a.oreb, a.dreb, a.reb, a.assist, a.steal, a.block, a.turnover, a.fouls, a.plus_minus, a.points, a.injury, b.day FROM gameData as a JOIN games as b ON a.gameID = b.Id WHERE Name = '" + player_name + "' ORDER BY b.day;";
    auto game_datas_for_player = predict_db->query(_query);

    for (auto data_vector: game_datas_for_player){
        //The player was NOT injured or scratched for this game.//
        if(data_vector[data_vector.size()-1] != "-1"){
            minutes.emplace_back(atoi(data_vector[0].c_str()));
            fga.emplace_back(atoi(data_vector[1].c_str()));
            fgm.emplace_back(atoi(data_vector[2].c_str()));
            if(atoi(data_vector[2].c_str()) <= 0){
                fg_percent.emplace_back(0);
            }
            else{
                fg_percent.emplace_back(atoi(data_vector[1].c_str())/atoi(data_vector[2].c_str()));
            }
            tpa.emplace_back(atoi(data_vector[3].c_str()));
            tpm.emplace_back(atoi(data_vector[4].c_str()));
            if(atoi(data_vector[4].c_str()) <= 0){
                tp_percent.emplace_back(0);
            }
            else{
                tp_percent.emplace_back(atoi(data_vector[3].c_str())/atoi(data_vector[4].c_str()));
            }
            fta.emplace_back(atoi(data_vector[5].c_str()));
            ftm.emplace_back(atoi(data_vector[6].c_str()));
            if(atoi(data_vector[6].c_str()) <= 0){
                ft_percent.emplace_back(0);
            }
            else{
                ft_percent.emplace_back(atoi(data_vector[5].c_str())/atoi(data_vector[6].c_str()));
            }
            oreb.emplace_back(atoi(data_vector[7].c_str()));
            dreb.emplace_back(atoi(data_vector[8].c_str()));
            reb.emplace_back(atoi(data_vector[9].c_str()));
            assist.emplace_back(atoi(data_vector[10].c_str()));
            steal.emplace_back(atoi(data_vector[11].c_str()));
            block.emplace_back(atoi(data_vector[12].c_str()));
            turnover.emplace_back(atoi(data_vector[13].c_str()));
            fouls.emplace_back(atoi(data_vector[14].c_str()));
            plus_minus.emplace_back(atoi(data_vector[15].c_str()));
            points.emplace_back(atoi(data_vector[16].c_str()));
        }
        else{
            cout << "PLAYER WAS INJURED THIS GAME." << endl;
        }
    }
    delete predict_db;
    /*SKIPPING FG, 3PT, FT FOR NOW COME BACK.//*/

    //initialize statistics objects for all data vectors.//
    statistics minute_stats{minutes};
    statistics fga_stats{fga};
    statistics fgm_stats{fgm};
    statistics fg_percent_stats{fg_percent};
    statistics tpa_stats{tpa};
    statistics tpm_stats{tpm};
    statistics tp_percent_stats{tp_percent};
    statistics fta_stats{fta};
    statistics ftm_stats{ftm};
    statistics ft_percent_stats{ft_percent};
    statistics oreb_stats{oreb};
    statistics dreb_stats{dreb};
    statistics reb_stats{reb};
    statistics assist_stats{assist};
    statistics steal_stats{steal};
    statistics block_stats{block};
    statistics turnover_stats{turnover};
    statistics fouls_stats{fouls};
    statistics plus_minus_stats{plus_minus};
    statistics points_stats{points};

    //place statistics objects into std::unordered_map//
    mean_and_stdevs.emplace("minutes",minute_stats);
    mean_and_stdevs.emplace("fga", fga_stats);
    mean_and_stdevs.emplace("fgm", fgm_stats);
    mean_and_stdevs.emplace("fg_percent_stats", fg_percent);
    mean_and_stdevs.emplace("tpa", tpa_stats);
    mean_and_stdevs.emplace("tpm", tpm_stats);
    mean_and_stdevs.emplace("tp_percent_stats", tp_percent);
    mean_and_stdevs.emplace("fta", fta_stats);
    mean_and_stdevs.emplace("ftm", ftm_stats);
    mean_and_stdevs.emplace("ft_percent_stats", ft_percent);
    mean_and_stdevs.emplace("oreb",oreb_stats);
    mean_and_stdevs.emplace("dreb",dreb_stats);
    mean_and_stdevs.emplace("reb",reb_stats);
    mean_and_stdevs.emplace("assist",assist_stats);
    mean_and_stdevs.emplace("steal",steal_stats);
    mean_and_stdevs.emplace("block",block_stats);
    mean_and_stdevs.emplace("turnover",turnover_stats);
    mean_and_stdevs.emplace("fouls",fouls_stats);
    mean_and_stdevs.emplace("plus_minus",plus_minus_stats);
    mean_and_stdevs.emplace("points",points_stats);

    //spin up 100 threads to simulate each player's games. This might be slower than sequential. Need to test.//
    vector<thread> worker_threads;
    size_t i;
    for(i=0;i<100;i++){
    	game_simulations.emplace_back();
    }

    //Sequential version//
    for(auto& simulation: game_simulations){
    	simulation.simulate_players_performance(mean_and_stdevs, keys_to_map, R_Inside_Container, player_name);
    }

    //Multithreaded version//
    /*
    for(i=0;i<game_simulations.size();i++){
    	worker_threads.emplace_back(&simulation::simulate_players_performance, game_simulations[i], mean_and_stdevs, keys_to_map, R_Inside_Container);
    }

    for(i=0;i<worker_threads.size();i++){
    	worker_threads[i].join();
    }
    */
    //each player has simulations, each simulation contains the simulated performances 
}

pair player::simulate_game_scores(int i){
    auto the_simulation = game_simulations[i];
    auto map_of_performances = the_simulation.simulated_performance;

    auto minutes = map_of_performances.find("minutes");
    auto fga = map_of_performances.find("fga");
    auto tpa = map_of_performances.find("tpa");
    auto fta = map_of_performances.find("fta");
    auto oreb = map_of_performances.find("oreb");
    auto assist = map_of_performances.find("assist");
    auto steal = map_of_performances.find("steal");
    auto turnover = map_of_performances.find("turnover");
    auto fouls = map_of_performances.find("fouls");
    auto plus_minus = map_of_performances.find("plus_minus");

    double minutes_value = minutes->second;
    double fga_value = fga->second;
    double tpa_value = tpa->second;
    double fta_value = fta->second;
    double oreb_value = oreb->second;
    double assist_value = assist->second;
    double steal_value = steal->second;
    double turnover_value = turnover->second;
    double fouls_value = fouls->second;
    double plus_minus_value = plus_minus->second;

    cout << players[j].player_name << " mins: " << minutes_value << " fga: " << fga_value << " tpa: " << tpa_value<< " fta: " <<fta_value<< " oreb: " <<oreb_value<< " ast: " <<assist_value<< " stl: " <<steal_value<< " to: " <<turnover_value<< " fl: " <<fouls_value<< " +-: " <<plus_minus_value<<endl;
    
    //These values are hard coded from my lm(Score ~ .) output. Need to figure out a way to route RInside to here.
    double predicted_score = -0.58433440 + 0.03139078*minutes_value + 0.95074756*fga_value + 0.20738638*tpa_value + 0.78631074*fta_value - 0.07971875*oreb_value - 0.15509996*assist_value - 0.09794145*steal_value + 0.10168076*turnover_value + 0.03700077*fouls_value + 0.08231120*plus_minus_value; 
    
    return make_pair(round(minutes_value), round(predicted_score));
}