#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <cmath>

const int captainFromTop=5; //take captain from top 5 players
using namespace std;
class player{ //todo privatize these and make getters and setters if not lazy
public:
    string name;
    double price;
    bool captain;
    string team;
    string position;
    double ppg;
    double projectedPoints;
    bool inLineup=false;
    player(string nameInput, double priceInput, bool isCaptain, string teamInput, string positionInput, double ppgInput);
};

player::player(string nameInput, double priceInput, bool isCaptain, string teamInput, string positionInput,
               double ppgInput) {
    name=nameInput;price=priceInput;captain=isCaptain;team=teamInput;position=positionInput;ppg=ppgInput;inLineup=false;
    projectedPoints=priceInput*.005;


}
vector <player> knapsacklineupcreation(int totalSalarydiv, int NumberofPlayers, vector <player> playersAvailable){
    vector<int> projectedPoints;
    vector<int> salaries;
    vector<player> lineup;
    player temp=player("temp",1,true,"whatev","SF",6);
    for (int i=0;i<NumberofPlayers;i++)
        lineup.push_back(temp);
    for (int i=0;i<playersAvailable.size();i++) {
        salaries.push_back(playersAvailable[i].price/100);
        projectedPoints.push_back(playersAvailable[i].projectedPoints);
    }
    int i,wt;
    vector<vector<vector<int>>> K(1000,vector<vector<int>>(1000,vector<int>(10)));
    for (i=1;i<=playersAvailable.size()-1;i++) {
        for (wt = 1; wt <= totalSalarydiv; wt++) {
            for (int k = 1; k <= NumberofPlayers; k++) {
                if (wt >= salaries[i-1]) {
                    K[i][wt][k] = fmax(K[i - 1][wt][k], K[i - 1][wt-salaries[i]][k - 1] + projectedPoints[i]);
                } else {
                    K[i][wt][k] = K[i - 1][wt][k];
                }
            }
        }
    }
    int result=K[playersAvailable.size()-1][totalSalarydiv][NumberofPlayers];
    int x=0;
    int w=totalSalarydiv;
    int y=NumberofPlayers;
    for (i=playersAvailable.size();i>0 &&result>0&&y>0;i--){
        if (result==K[i-1][w][y])
            continue;
        else{
            lineup[x]=playersAvailable[i];
            x+=1;
            result=result-projectedPoints[i];
            w=w-salaries[i];
            y--;
        }
    }
    return lineup;
}
void inputPlayers(vector <player>& playersAvailable);
void inputPlayers(vector <player>& playersAvailable) {
    string name;
    double price;
    bool captain;
    string team;
    string position;
    string temp;
    string cptTest;
    double ppg;
    char tempChar;
    ifstream inputFile;
     inputFile.open("DKSalaries.csv");
     getline(inputFile,temp);//clear headers
     while(!inputFile.eof())
        {
         getline(inputFile,position,',');
         getline(inputFile,temp,',');
         getline(inputFile,name,',');
         getline(inputFile,temp,',');
         getline(inputFile,cptTest,',');
         inputFile>>price>>temp;
         getline(inputFile,temp,',');
         getline(inputFile,team,',');
         inputFile>>ppg;inputFile.ignore(); //clear the end line character
         if (cptTest!="CPT" && position.length()>0)//cleanse the captains want them separate also the dupe at end
             playersAvailable.push_back(player(name, price, false, team, position, ppg));
     }
     inputFile.close();
}
vector <player> lineupCreation(double salary, int totalPlayers, vector <player> playersAvailable);
vector <player> lineupCreation(double salary, int totalPlayers, vector <player> playersAvailable){
    vector <player> lineup;
    srand(time (0));
    player player1=playersAvailable[rand()%captainFromTop]; //assigns our captaincy to a top player
    player1.price=player1.price*1.5; //assigns captaincy
    player1.inLineup=true;
    totalPlayers--; salary-=player1.price;
    lineup.push_back(player1);
    while (totalPlayers>0){
        player temp=playersAvailable[rand()%playersAvailable.size()];
        if (salary-temp.price>0 && !temp.inLineup)
            {
                temp.inLineup=true;
                lineup.push_back(temp);
                totalPlayers--;
                salary-=temp.price;
            }
    }
    while (salary>500) // excess salary? lets spend it, won't optimize over multiple players todo incorporate projected points
    {
        double minSalary = 50000;
        int minLocation;
            for (int i = 1; i < lineup.size(); i++)  //find the lowest salary,exclude captain
                if (lineup[i].price <= minSalary) {
                    minSalary = lineup[i].price;
                    minLocation = i;
                }
            player tempNewPlayer=playersAvailable[rand()%playersAvailable.size()];
            double priceDifference=tempNewPlayer.price-lineup[minLocation].price;
            if (priceDifference>0 && priceDifference<salary && !tempNewPlayer.inLineup)    {//replace lowest salary;
                lineup[minLocation].inLineup=false;
                lineup[minLocation]=tempNewPlayer;
                salary-=priceDifference;
                tempNewPlayer.inLineup=true;
            }

    }
    return lineup;
}
//todo import team matchup stats, put as separate category
int main() {
    vector<player> playersAvailable;
    inputPlayers(playersAvailable);
    vector <player> team=knapsacklineupcreation(50000/100,6,playersAvailable);
   // vector <player> team=lineupCreation(50000,6, playersAvailable);
    double sumOfSalary=0;
    double projectedPoints=0;
    for (int i=0;i<team.size();i++) {
        cout << team[i].name << " " << team[i].price << endl;
        sumOfSalary+=team[i].price;
        projectedPoints+=team[i].projectedPoints;
    }
    cout<<"Salary Used: "<<sumOfSalary<<endl;
    cout<<"Total projected points: "<<projectedPoints;
    //for (int i=0;i<playersAvailable.size();i++)
    //    cout<<playersAvailable[i].name<<" "<<playersAvailable[i].position<<" "<<playersAvailable[i].price<<endl;
    return 0;
}
