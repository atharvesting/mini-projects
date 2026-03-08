#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <iostream>
#include "test.h"
#include "utils.h"
#include "models.h"
using namespace std;

int main()
{
    //auto m = JSON2Map("model_school.json");
    //startPredictionLoop(m, "the", 15);

    Bigram b(2, "school.txt");
    b.predict("the", 15);

    return 0; 
}