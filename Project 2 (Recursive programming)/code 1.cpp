#include <iostream>
#include <string>

using namespace std;

//Is the character a letter or not.
bool is_letter(char character)
{
    if((65<=int(character) && int(character)<=90))    //Capital
    {
        return true;
    }
    else if((97<=int(character) && int(character)<=122))  //Lower
    {
        return true;
    }
    else           //Not Letter
    {
        return false;
    }
}

//compare first and last characters
bool low_up(char first,char last)
{
    if(int(first)==int(last)+32)    //example:  first=a Last=A
    {
        return true;
    }
    else if(int(first)==int(last)-32)   //example:  first=A Last=a
    {
        return true;
    }   
    else if(int(first)==int(last))   //example:  first=A Last=A
    {
        return true;
    } 
    else
    {
        return false;                  //example:  first=A Last=b
    }    
}

// Give final Result by comparing.
bool compare(string& words, int& first_ind, int& last_ind)
{
    bool res;

    while(true)
    {
        if (first_ind >= last_ind)
        {
            res=true;             //The result is true and symmetric
            break;        
        }

        else
        {
            if(is_letter(words[first_ind]))     //First is Letter or not
            {
                if(is_letter(words[last_ind]))  //Last is Letter or not
                {
                    if(low_up(words[first_ind],words[last_ind]))    // Both are Letters and same
                    {
                        first_ind++;
                        last_ind--;
                        compare(words,first_ind,last_ind);
                    }
                    else                                            // Both are Letters and but not same
                    {
                        res=false;
                        break;
                    }
                }
                else
                {
                    last_ind--;
                    compare(words,first_ind,last_ind);
                }    
            }
            else
            {
                first_ind++;
                compare(words,first_ind,last_ind);
            }
        }
    }
    return res;

}

void print_res(bool res)
{
    if(res)
    {
		cout<<"true"<<endl;
    }
	else
    {
		cout<<"false"<<endl;
    }
}

int main()
{
    string sentence;
    bool result;
    while (!cin.eof()&& getline(cin, sentence))
    {
        int first=0;
        int last=sentence.size()-1;

        result=compare(sentence,first,last);
        print_res(result);
	}
  

    return 0;
}