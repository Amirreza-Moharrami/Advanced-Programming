#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <cstring>
#include <chrono>
#include <cmath>

using namespace std;
using namespace chrono;


//Variables related to subway's lines
struct Line
{
    string name;
    int number;
    int delay; 
    vector <string> lines; 
    vector <int> delays;

};

//Variables related to subway's stations
struct Station
{
    string name;
    int delay;      
};

//Variables related to subway's Start stations
struct Start
{
    int number;
    vector <vector <string>> stations;
    vector <vector <int>> delays;
};

//Variables related to subway's End stations
struct End
{
    int number;
    vector <vector <string>> stations;
    vector <vector <int>> delays;
}; 

//Function for Station information (The main purpose is to search the desired station from different states.)
vector<int> arrival_station(string Station,vector <string> Lines,vector <vector <string>> Start_Stations,vector <vector <string>> End_Stations)
{
    vector <int> output;
    int flag=0;
    
    for (int n=0; n<Lines.size() ; n++)
    {
        for(int i=0; i<Start_Stations[n].size(); i++)
        {
            if(Start_Stations[n][i]==Station)
            {
                flag=1; // flag==1 means start
                output.push_back(flag);
                output.push_back(n);
                output.push_back(i+1);
                cout << "Towards start of "<< Lines[n] << " in " << i+1 << " station(s)"<< endl;
                break;
                break;
            }
        }
        for(int j=0; j<End_Stations[n].size(); j++)
        {
            if(End_Stations[n][j]==Station)
            {
                flag=2; // flag==2 means end
                output.push_back(flag);
                output.push_back(n);
                output.push_back(j+1);                
                cout << "Towards end of "<< Lines[n] << " in " << j+1 << " station(s)"<< endl;
                break;
                break;
            }
        }  
    }  
    return output;  // Output is [a,b,c] ---->  a is flag(start or end), b is Line number, c is station numbers
}

//Function for Vector Sum 
int Sum_Vec(vector<int> vec,int n)
{
    int sum=0;
    for(int i=0; i<n; i++)
    {
        sum=sum+vec[i];
    }
    return sum;
}

//Function for time information
void arrival_time(vector<int> arrival,int init_hour, int init_minute,vector <int> Line_delays,vector <vector <int>> Start_Station_delays,vector <vector <int>> End_Station_delays)
{
    int n=arrival[1];   //Line number
    int s=arrival[2];   //Station numbers

    int duration;
    int Total_duration;

    //Start
    if(arrival[0]==1)
    {
        duration=Sum_Vec(Start_Station_delays[n],s);
    }

    //End
    else if(arrival[0]==2)
    {
        duration=Sum_Vec(End_Station_delays[n],s);
    }

    if((init_minute%Line_delays[n])==0)
    {
        Total_duration=duration;      //Calculate the total duration
    }
    else
    {   
        Total_duration=duration+Line_delays[n]-(init_minute%Line_delays[n]);      //Calculate the total duration
    }
        
    auto min=hours(init_hour)+minutes(init_minute)+minutes(Total_duration);   //Calculate total minutes
    auto hour=min.count() / 60;
    auto minute=min.count() %60;
    

    if(hour >= 24 || hour<10)
    {  
        if(hour >= 24)
        {
            hour=0;
        }
        if(minute==0)
        {
            cout << "0"<< hour <<":00"<< endl;
        } 
        else
        {
          cout << "0"<< hour <<":" << minute << endl;
        }
    }
    else
    {
        if(minute==0)
        {
            cout << hour <<":00"<< endl;
        } 
        else
        {
          cout << hour << ":" << minute << endl;
        }
        
    }
    
}

//Function for Cost information
void arrival_cost(vector<int> arrival)
{
    int s=arrival[2];   //Station numbers   
    float cost=0;

    cost=ceil(1000*log10(10*s));        //Calculate the Cost

    cout<< cost << endl;
}


int main()
{
    struct Line Line;
    struct Station Station;
    struct Start Start;
    struct End End;
    int hour_start; int minute_start;


    scanf("%d %d:%d", &Line.number, &hour_start, &minute_start);    //Get Line numbers and initial time

    for(int i=0; i<Line.number; i++)
    {
        vector <string> Empty_string(0);                //Empty vector
        vector <int> Empty_int(0);                      //Empty vector

        Start.stations.push_back(Empty_string);         //Give inital value to vectors
        End.stations.push_back(Empty_string);           //Give inital value to vectors
        Start.delays.push_back(Empty_int);              //Give inital value to vectors
        End.delays.push_back(Empty_int);                //Give inital value to vectors

        cin >> Line.name >> Line.delay;                 //Get Line name and duration time
        Line.lines.push_back(Line.name);            
        Line.delays.push_back(Line.delay);

        cin >> Start.number;                            //Get the number of start stations

        for(int j=0; j<Start.number; j++)
        {
            cin >> Station.delay >> Station.name;       //Get station name and duration time
            Start.delays[i].push_back(Station.delay);
            Start.stations[i].push_back(Station.name);
        }

        cin >> End.number;                              //Get the number of end stations

        for(int k=0; k<End.number; k++)
        {
            cin >> Station.delay >> Station.name;       //Get station name and duration time
            End.delays[i].push_back(Station.delay);
            End.stations[i].push_back(Station.name);
        }
    }

    cin >> Station.name;                                //Get the desired station name 

    vector<int> arrival;

    //Output of Station information
    arrival=arrival_station(Station.name,Line.lines,Start.stations,End.stations);
    
    //Output of time information
    arrival_time(arrival,hour_start,minute_start,Line.delays,Start.delays,End.delays); 
    
    //Output of Cost information
    arrival_cost(arrival);

    return 0;
}