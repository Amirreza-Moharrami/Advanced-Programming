#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;



//------------------------------------------------------------------------------------------

class Bank
{
private:
    int id;
    int shortTermProfitMargin;
    int shortTermMinimumInvestment;
    vector<int> STD_ids={};

public:
    Bank(int Id, int stProfitMargin, int stMinimumInvestment) {id=Id;shortTermProfitMargin=stProfitMargin;shortTermMinimumInvestment=stMinimumInvestment;};
    
    int get_id() { return id;}
    int get_shortTermProfitMargin() { return shortTermProfitMargin;}
    int get_shortTermMinimumInvestment() { return shortTermMinimumInvestment;}
    vector<int> get_STD_ids() { return STD_ids;}
    vector<int> add_STD_ids(int STD_id) { STD_ids.push_back(STD_id);return STD_ids;}

};

//------------------------------------------------------------------------------------------

vector <Bank> read_bank_file(string address)
{
    vector <Bank> banks;
    
    fstream bankFile(address);
    if (bankFile.is_open()) {
        string line;
        while (getline(bankFile, line)) 
        {
            stringstream ss(line);
            string id, STPM,STMI;
            getline(ss, id, ',');
            getline(ss, STPM, ',');
            getline(ss, STMI, ',');

            if(id=="Id")
            {
                id="0";
            }
            if(STPM=="Short-term profit margin")
            {
                STPM="0";
            }      
            if(STMI=="Short-term minimum investment")
            {
                STMI="0";
            }     

            Bank bank(stoi(id), stoi(STPM),stoi(STMI));      
            banks.push_back(bank);
        }
        bankFile.close();
    }

    return banks;    
}


//------------------------------------------------------------------------------------------


class Person
{
private:
    int id;
    int wallet;
public:
    Person(int Id, int Wallet) {id=Id;wallet=Wallet;};

    int get_id() { return id;}
    int get_wallet() { return wallet;}
    int update_wallet(int investment) {return wallet=wallet-investment;};
};  

//------------------------------------------------------------------------------------------

vector <Person> read_user_file(string address)
{
    vector <Person> users;
    
    fstream userFile(address);
    if (userFile.is_open()) {
        string line;
        while (getline(userFile, line)) 
        {
            stringstream ss(line);
            string id, wallet;
            getline(ss, id, ',');
            getline(ss, wallet, ',');
            if(id=="Id")
            {
                id="0";
            }
            if(wallet=="Wallet")
            {
                wallet="0";
            }      
            Person user(stoi(id), stoi(wallet));      
            users.push_back(user);
        }
        userFile.close();
    }

    return users;    
}

//------------------------------------------------------------------------------------------

class ShortTermDeposit
{
private:
    Person* user;
    Bank* bank;
    vector<int> Id={};
    int minimumInvestment;
    int initialInvestment;
public:
    ShortTermDeposit(Person* User, Bank* UserBank,int initInvest);
    bool is_deposit_possible();
    int get_id() { return Id[0];}
    void update_id(int IIdd) {Id.push_back(IIdd);}
    int get_bank_id() {return bank->get_id();}
    int get_user_id() {return user->get_id();}
}; 

//------------------------------------------------------------------------------------------


ShortTermDeposit::ShortTermDeposit(Person* User, Bank* UserBank,int initInvest)
{
    user=User;
    bank=UserBank;
    minimumInvestment=UserBank->get_shortTermMinimumInvestment();
    initialInvestment=initInvest;
}

//------------------------------------------------------------------------------------------

bool ShortTermDeposit::is_deposit_possible()
{
    bool flag=0;
    int personWallet= user->get_wallet();

    if(personWallet > minimumInvestment)
    {
        if(personWallet > initialInvestment)
        {
            flag=1;
            personWallet=user->update_wallet(initialInvestment);
        }
    }
    return flag;
}



//------------------------------------------------------------------------------------------

class LongTermDeposit
{
private:
    Person* user;
    Bank* bank;
    ShortTermDeposit* shortDeposit;
    int years;
    int minimumInvestment;
    int initialInvestment;
public:
    LongTermDeposit(Person* User, Bank* UserBank,ShortTermDeposit* ShortDeposit,int Years,double initInvest);
    int is_deposit_possible();

}; 

//------------------------------------------------------------------------------------------


LongTermDeposit::LongTermDeposit(Person* User, Bank* UserBank,ShortTermDeposit* ShortDeposit,int Years,double initInvest)
{
    user=User;
    bank=UserBank;
    shortDeposit=ShortDeposit;
    years=Years;
    minimumInvestment=UserBank->get_shortTermMinimumInvestment();
    initialInvestment=initInvest;
}

//------------------------------------------------------------------------------------------

int LongTermDeposit::is_deposit_possible()
{
    int flag=0;
    int personWallet= user->get_wallet();

    if(user->get_id()== shortDeposit->get_user_id())
    {
        if(initialInvestment > minimumInvestment*years)
        {
            if(personWallet > initialInvestment)
            {
                flag=2;
                personWallet=user->update_wallet(initialInvestment);
            

            }
        }
    }
    else if(user->get_id()!= shortDeposit->get_user_id())
    {
        flag=1;
    }
    return flag;    

}

//------------------------------------------------------------------------------------------


class GharzolhasaneDeposit
{
private:
    Person* user;
    Bank* bank;
    double initialInvestment;
public:
    GharzolhasaneDeposit(Person* User, Bank* UserBank,double initInvest);
    bool is_deposit_possible();

}; 

//------------------------------------------------------------------------------------------

GharzolhasaneDeposit::GharzolhasaneDeposit(Person* User, Bank* UserBank,double initInvest)
{
    user=User;
    bank=UserBank;
    initialInvestment=initInvest;
}

//------------------------------------------------------------------------------------------

bool GharzolhasaneDeposit::is_deposit_possible()
{
    bool flag=0;
    int personWallet= user->get_wallet();

    if(personWallet > initialInvestment)
    {
        flag=1;
        personWallet=user->update_wallet(initialInvestment);
    }

    return flag;    
}


//------------------------------------------------------------------------------------------

class Ebanking
{
private:
    vector<Person> persons;
    vector<Bank> banks;
    vector<ShortTermDeposit> shortTermDeposits;
    vector<LongTermDeposit> longTermDeposits;
    vector<GharzolhasaneDeposit> gharzolhasaneDeposits;
public:
    Ebanking(vector<Person> Persons,vector<Bank> Banks,vector<ShortTermDeposit> ShortTermDeposits,vector<LongTermDeposit> LongTermDeposits,vector<GharzolhasaneDeposit> GharzolhasaneDeposits);    
    Person* find_person(int user_id);
    Bank* find_bank(int bank_id);
    ShortTermDeposit create_STD(ShortTermDeposit STD, int STD_Id);
    int create_STD_id(Bank* bank);
    ShortTermDeposit find_STD(int STD_id,int bank_id);

    void call_classes(string input);
};

//------------------------------------------------------------------------------------------

Ebanking::Ebanking(vector<Person> Persons,vector<Bank> Banks,vector<ShortTermDeposit> ShortTermDeposits,vector<LongTermDeposit> LongTermDeposits,vector<GharzolhasaneDeposit> GharzolhasaneDeposits)
{
    persons=Persons;
    banks=Banks;
    shortTermDeposits=ShortTermDeposits;
    longTermDeposits=LongTermDeposits;
    gharzolhasaneDeposits=GharzolhasaneDeposits;
}
//------------------------------------------------------------------------------------------

Person* Ebanking::find_person(int user_id)
{
    Person* person;

    for(int i=1; i<persons.size(); i++)
    {
        if(persons[i].get_id()==user_id)
        {
            person= &persons[i];
            break;
        }
    }

    return person;
}
//------------------------------------------------------------------------------------------

Bank* Ebanking::find_bank(int bank_id)
{
    Bank* bank;

    for(int i=1; i<banks.size(); i++)
    {
        if(banks[i].get_id()==bank_id)
        {
            bank=&banks[i];
            break;
        }
    }

    return bank;
}


//------------------------------------------------------------------------------------------

int Ebanking::create_STD_id(Bank* bank)
{
    int id;
    vector<int> bank_STD_ids=bank->get_STD_ids();
    int n=bank_STD_ids.size();
    if(n==0)
    {
        id=1;
    }
    else
    {
        id=bank_STD_ids[n-1]+1;
    }

    bank->add_STD_ids(id);    

    return id;
}
//------------------------------------------------------------------------------------------

ShortTermDeposit Ebanking::create_STD(ShortTermDeposit STD, int STD_Id)
{
    
    STD.update_id(STD_Id);
    shortTermDeposits.push_back(STD);

    ShortTermDeposit NewSTD=STD;
    return NewSTD;
}


//------------------------------------------------------------------------------------------

ShortTermDeposit Ebanking::find_STD(int STD_id,int bank_id)
{
    ShortTermDeposit STD=shortTermDeposits[0];
    int n=banks.size();

    for(int i=0; i < n ; i++)
    {
        if(shortTermDeposits[i].get_id()==STD_id)
        {
            if(shortTermDeposits[i].get_bank_id()==bank_id)
            {
                STD=shortTermDeposits[i];
                break;
            }

        }
    }

    return STD;
}

//------------------------------------------------------------------------------------------

void Ebanking::call_classes(string input)
{

    // Short Term Deposit

    if(input=="create_short_term_deposit")
    {
        
        int user_id, bank_id, initial_investment;

        cin >> user_id >> bank_id >> initial_investment;

        Person* person=find_person(user_id);
        Bank* bank=find_bank(bank_id);
        ShortTermDeposit STD(person,bank,initial_investment);

        
        if(STD.is_deposit_possible()==true)
        {
            int STD_id=create_STD_id(bank);
            ShortTermDeposit STD_add=create_STD(STD,STD_id);
            cout << STD_id << endl;
        }
        else
        {
            cout << "Not enough money" << endl;
        }
    }

//------------------------------------------------------------------------------------------

    // Long Term Deposit

    if(input=="create_long_term_deposit")
    {
        int user_id, bank_id, STD_id, years, initial_investment;

        cin >> user_id >> bank_id >> STD_id >> years >> initial_investment;

        Person* person=find_person(user_id);
        Bank* bank=find_bank(bank_id);

        ShortTermDeposit STD=find_STD(STD_id,bank_id);
        LongTermDeposit LTD(person,bank, &STD, years,initial_investment);

        int flag=LTD.is_deposit_possible();

        if(flag==2)
        {
            cout<< "OK" << endl;
        }
        else if (flag==1)
        {
            cout << "Invalid short-term deposit" << endl;
        }
        else if (flag==0)
        {
            cout << "Not enough money" << endl;
        }
    }

//------------------------------------------------------------------------------------------

    //Gharzol Hasane    

    if(input=="create_gharzolhasane_deposit")
    {

        int user_id, bank_id,initial_investment;

        cin >> user_id >> bank_id >> initial_investment;

        Person* person=find_person(user_id);
        Bank* bank=find_bank(bank_id);

        GharzolhasaneDeposit GHDP(person,bank,initial_investment);

        bool flag=GHDP.is_deposit_possible();

        if(flag==true)
        {
            cout << "OK" << endl;
        }

    }

}


//------------------------------------------------------------------------------------------


int main()
{

    vector<Bank> banks=read_bank_file("Banks.csv");
    vector<Person> users=read_user_file("Users.csv");

    Ebanking Ebanking(users,banks,{},{},{});

    //--------------------------------------------------------------------------------

    string input;
    while(cin >> input)
    {
        Ebanking.call_classes(input);
    }

    return 0;
}