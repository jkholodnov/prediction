#include "../include/game.h"

game::game(const string& id, team* _team1, team* _team2,const int& team1Points, const int& team2Points)
{
    gameid = id;
    team1 = _team1;
    team2 = _team2;
    team1Score = team1Points;
    team2Score = team2Points;
}

game::~game()
{

}

//parallelize this call//
pair<int,string> game::update_Team_Ratings()
{
    auto t1_Rating = team1->get_Rating();
    auto t2_Rating = team2->get_Rating();

    team1Expected = 1/(1+pow(10,((t2_Rating-t1_Rating)/400)));
    team2Expected = 1/(1+pow(10,((t1_Rating-t2_Rating)/400)));
    //Calculating Expected Win Ratio based on standard ELO formula.
    //cout << t1_Rating << "~" << team1Expected << "##" << t2_Rating << "~" << team2Expected << endl;
    if(team1Score > team2Score)
    {
        team1 -> update_Rating(50*(1 - team1Expected));
        team2 -> update_Rating(50*(0 - team2Expected));

        string _query = "UPDATE games SET team1ELO = " + to_string(t1_Rating) + ", team2ELO = " + to_string(t2_Rating) + " WHERE gameid = '" + gameid + "';";

        if(t1_Rating > t2_Rating)
        {
            //number_correct++;
            auto pair = make_pair(1,_query);
            return pair;
        }
        else
        {
            auto pair = make_pair(0,_query);
            return pair;
        }
    }
    else
    {
        team1 -> update_Rating(50*(0 - team1Expected));
        team2 -> update_Rating(50*(1 - team2Expected));

        string _query = "UPDATE games SET team1ELO = " + to_string(t1_Rating) + ", team2ELO = " + to_string(t2_Rating) + " WHERE gameid = '" + gameid + "';";

        if(t2_Rating > t1_Rating)
        {
            //number_correct++;
            auto pair = make_pair(1,_query);
            return pair;
        }
        else
        {
            auto pair = make_pair(0,_query);
            return pair;
        }
    }
}
