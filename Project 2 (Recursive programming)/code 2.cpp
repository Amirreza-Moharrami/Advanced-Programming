#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

bool search(const vector<double>& numbers, double target, int index, double current, string current_str) 
{
    if (index == numbers.size() - 1) 
    {
        // check if the result matches the target
        if (abs(current - target) == 0) 
        {
            current_str += "=" + to_string(int(target));
            cout << current_str << endl;
            return true;
        }
        return false;
    }

    char operators[3] = {'+', '-', '*'};
    bool found = false;

    for (int i = 0; i < 3; i++) 
    {
        string new_str = current_str + operators[i] + to_string(int(numbers[index + 1]));
        double new_res = (operators[i] == '+') ? current + numbers[index + 1] :
                          (operators[i] == '-') ? current - numbers[index + 1] :
                          current * numbers[index + 1];

        found = search(numbers, target, index + 1, new_res, new_str);

        if (found) 
        {
            break;
        }
    }
    
    return found;
}



vector<double> get_inputs()
{
    int n;
    cin >> n;                                // Read the count of numbers

    if (n < 3 || n > 10) 
    {
        cout << "Error: n should be between 3 and 10." << endl;
    }

    vector<double> input_numbers(n);

    for (int i=0 ; i < n; i++) 
    {
        (cin >> input_numbers[i]);          //Get all the numbers
    }

    return input_numbers;
}


vector<double> extract_left_inputs(vector<double> input_numbers)
{
    int n=input_numbers.size();
    vector<double> numbers(n-1);

    for (int j=0 ; j < n-1; j++) 
    {
        numbers[j] = input_numbers[j];
    }    

    return numbers;
}


int main() {

    vector<double> input_numbers;
    vector<double> numbers;

    input_numbers=get_inputs();                     // Get all inputs
    numbers=extract_left_inputs(input_numbers);     // Extract left parts from the target number


    int n=input_numbers.size();
    double target;
    target=input_numbers[n-1];                      // Extract target number


    // Start the recursive search 
    bool solution = search(numbers, target, 0, numbers[0], to_string(int(numbers[0])));


    // No solution!
    if (!solution) 
    {
        cout << "No Solution!" << endl;
    }

    return 0;
}
