#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

struct Patient {
    string name;
    string issue;
    string appointment_time;
    int visit_number;
};

struct Doctor {
    string name;
    string specialty;
    string visit_fee;
    string visit_duration;
    string wait_time_average;
    vector<string> availability_day;
    vector<string> availability_arrival;
    vector<string> availability_departure;
    string availability_minute;
};

struct Disease {
    string specialty;
    vector<string> diseases;
};


vector <Patient> readPatientsFile(string address)
{
    vector<Patient> patients;

    fstream patientsFile(address);
    if (patientsFile.is_open()) {
        string line;
        while (getline(patientsFile, line)) {
            stringstream ss(line);
            string name, issue, appointment_time;
            getline(ss, name, ',');
            getline(ss, issue, ',');
            getline(ss, appointment_time, ',');
            patients.push_back({name, issue, appointment_time});
        }
        patientsFile.close();
    }

    return patients;
}

vector <Doctor> readDoctorsFile(string address)
{
    vector <Doctor> doctors;
    
    fstream doctorsFile(address);
    if (doctorsFile.is_open()) {
        string line;
        while (getline(doctorsFile, line)) {
            stringstream ss(line);
            string name, specialty, availability_str;
            string visit_fee, visit_duration, wait_time_average;
            vector <int> numbers;

            getline(ss, name, ',');
            getline(ss, specialty, ',');
            getline(ss, visit_fee, ',');
            getline(ss, visit_duration, ',');
            getline(ss, wait_time_average, ',');
            getline(ss, availability_str);

            stringstream availability_ss(availability_str);
            vector<string> availability_day;
            vector<string> availability_arrival;
            vector<string> availability_departure;
            string day_and_time;
            while (getline(availability_ss, day_and_time, '$')) {
                stringstream day_time_ss(day_and_time);
                string day, arrival, departure;
                getline(day_time_ss, day, '-');
                getline(day_time_ss, arrival, '-');
                getline(day_time_ss, departure, '-');
                availability_day.push_back({day});
                availability_arrival.push_back({arrival});
                availability_departure.push_back({departure});
            }
            doctors.push_back({name, specialty, visit_fee, visit_duration, wait_time_average, availability_day,availability_arrival, availability_departure});
        }
        doctorsFile.close();
    }    

    return doctors;
}


vector <Disease> readDiseasesFile(string address)
{
    vector <Disease> diseases;

    fstream diseasesFile(address);
    if (diseasesFile.is_open()) {
        string line;
        while (getline(diseasesFile, line)) {
            stringstream ss(line);
            string specialty, diseases_str;
            getline(ss, specialty, ',');
            getline(ss, diseases_str);
            stringstream diseases_ss(diseases_str);
            vector<string> disease_list;
            string disease;
            while (getline(diseases_ss, disease, '$')) {
                disease_list.push_back(disease);
            }
            diseases.push_back({specialty, disease_list});
        }
        diseasesFile.close();
    }    

    return diseases;
}


bool isStringInVector(vector<string> vec,string str)
{
    bool flag=0;

    for(int i=0; i < vec.size(); i++)
    {
        if(vec[i]== str)
        {
            flag=1; 
            break; 
        }
    }

    return flag;
}



string findSpecialty(Patient patient,vector<Disease> diseases)
{
    int nDiseases=diseases.size()-1;
    string special;

    string medIssue=patient.issue;

    for(int j=1; j <= nDiseases; j++)
    {
        if(isStringInVector(diseases[j].diseases, medIssue))
        {
            special=diseases[j].specialty;
            break;
        }
    }
    return special;
}

bool isDoctorAvailable(Doctor doctor)
{
    bool flag=1;

    if(doctor.availability_day.size()== 0)
    {
        flag=0;
    }
    else
    {
        flag=1;
    }

    return flag;
}

vector <Doctor> specialDoctor(Patient patient, vector<Disease> diseases, vector<Doctor> doctors)
{
    int nDoctors=doctors.size()-1;
    vector <Doctor> Doctors;

    string special=findSpecialty(patient,diseases);

    for(int j=1; j <= nDoctors; j++)
    {
        if(doctors[j].specialty == special)
        {
            if(isDoctorAvailable(doctors[j]))
            {
                Doctors.push_back(doctors[j]);
            }
            
        }
    }

    return Doctors;    
}



int cheapestFee(vector <Doctor> doctors)
{
    int nDoctors= doctors.size();
    int cheapest= stoi(doctors[0].visit_fee);

    
    for(int i=0; i < nDoctors; i++)
    {
        int visitFee=stoi(doctors[i].visit_fee);
        if(cheapest > visitFee)
        {
            cheapest=visitFee;
        }
        else
        {
            continue;
        }
    }  

    return cheapest;
}


vector <Doctor> cheapestDoctor(vector <Doctor> doctors)
{
    int nDoctors= doctors.size();
    int cheapest= stoi(doctors[0].visit_fee);
    vector <Doctor> doctor;

    cheapest=cheapestFee(doctors);

    for(int j=0; j < nDoctors; j++)
    {
        int visitFee=stoi(doctors[j].visit_fee);
        if(cheapest == visitFee)
        {
            doctor.push_back(doctors[j]);
        }        
    }


    return doctor;
}


int cheapestDoctorIndex(vector <Doctor> doctors)
{
    int nDoctors= doctors.size();
    int index=0;
    int cheapFee=0;
    

    cheapFee=cheapestFee(doctors);

    for(int i=0; i < nDoctors; i++)
    {
        int visitFee=stoi(doctors[i].visit_fee);
        if(visitFee== cheapFee)
        {
            index=i;
            break; 
        }
        else
        {
            continue;
        }
    }

    return index;

}
Doctor findDoctor(vector <Doctor> specialDocs)
{
    int nSpecialDoc=specialDocs.size();
    int cheapFee=0;
    vector <Doctor> cheapDocs;
    vector <Doctor> resultDocs;
    Doctor resultDoctor;
    
    cheapFee=cheapestFee(specialDocs);
    cheapDocs=cheapestDoctor(specialDocs);

    resultDoctor=cheapDocs[0];

    resultDoctor.name=cheapDocs[0].name;
    resultDoctor.specialty=cheapDocs[0].specialty;
    resultDoctor.visit_fee=cheapDocs[0].visit_fee;
    resultDoctor.visit_duration=cheapDocs[0].visit_duration;
    resultDoctor.wait_time_average=cheapDocs[0].wait_time_average;
    resultDoctor.availability_day[0]=cheapDocs[0].availability_day[0];
    resultDoctor.availability_arrival[0]=cheapDocs[0].availability_arrival[0];
    resultDoctor.availability_departure[0]=cheapDocs[0].availability_departure[0];


    return resultDoctor;
}


/*
vector <string> timeVisitChange(Doctor doctor)
{
    vector <string> time;
    string minute;
    string hour;
    float timeDiff=stof(doctor.availability_departure[0])-stof(doctor.availability_arrival[0]);
    float visitDuration=stof(doctor.visit_duration)/60;
    float finalMin=(timeDiff-visitDuration)*60;
    int arrival;

    if(finalMin <= 0)
    {
        minute="00";
        hour=doctor.availability_arrival[0];
    }
    else if((finalMin < 60) && (finalMin > 0))
    {
        minute=to_string(int(finalMin));
        hour=doctor.availability_arrival[0];
    }
    else if((finalMin == 60))
    {
        minute="00";
        hour=doctor.availability_arrival[0];
    }   
    else
    {
        minute=to_string(int(finalMin)-60);
        hour=doctor.availability_arrival[0];
    }

    time.push_back(hour);
    time.push_back(minute);
    time.push_back(to_string(int(finalMin)));


    return time;
}

Doctor changeDocArrivalTime(Doctor doctor)
{
    vector <string> time=timeVisitChange(doctor);
    int timeDiff=stoi(time[2]);

    if(timeDiff >= 60)
    {
        doctor.availability_arrival[0]=to_string(stoi(doctor.availability_arrival[0])+1);
    }

    return doctor;
}

*/


vector <Doctor> earaseDoctorTime(Doctor findedDoctor, vector <Doctor> doctors)
{

    for(int i=1; i< doctors.size(); i++)
    {
        if(doctors[i].name==findedDoctor.name)
        {
            doctors[i].availability_day.erase(doctors[i].availability_day.begin());
            doctors[i].availability_arrival.erase(doctors[i].availability_arrival.begin());
            doctors[i].availability_departure.erase(doctors[i].availability_departure.begin());

            break;
        }
    }


    return doctors;
}


void printDoctorVisit(Patient patient,Doctor findedDoctor)
{
    cout << "Name: " << patient.name << endl;
    cout << "Doctor: "<< findedDoctor.name << endl;
    cout << "Visit: " << findedDoctor.availability_day[0] << " " << patient.visit_number << " " << findedDoctor.availability_arrival[0] << ":" << findedDoctor.availability_minute << endl;
    cout << "Charge: "<< findedDoctor.visit_fee << endl;
    cout << "----------"<< endl;

}

void process(vector <Patient> patients, vector<Doctor> doctors, vector<Disease> diseases)
{

    for(int i=1; i<patients.size(); i++)
    {

        //Find special Doctors----------------------------------------------
        
        string Special;
        Special=findSpecialty(patients[i],diseases);
        vector <Doctor> SpecialDoctors;
        SpecialDoctors=specialDoctor(patients[i],diseases,doctors);

        //Find Doctor----------------------------------------------
    
        Doctor finalDoctor;
        finalDoctor=findDoctor(SpecialDoctors);
        finalDoctor.availability_minute="00";
        patients[i].visit_number=1;
        doctors=earaseDoctorTime(finalDoctor,doctors);

        //Print Data----------------------------------------------

        printDoctorVisit(patients[i], finalDoctor);

    }

}



int main() {
    vector<Patient> patients;
    vector<Doctor> doctors;
    vector<Disease> diseases;

    //Read Files

    patients=readPatientsFile("patients.csv");
    doctors=readDoctorsFile("doctors.csv");
    diseases=readDiseasesFile("diseases.csv");


    //Process

    process(patients,doctors, diseases);


    return 0;
}
