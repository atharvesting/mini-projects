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

    Trigram t("school.txt");
    t.predict("the", 15);

    return 0; 
}