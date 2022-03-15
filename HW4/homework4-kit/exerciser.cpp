#include "exerciser.h"

void exercise(connection *C)
{
    query1(C, 1, 35, 40, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    query1(C, 1, 35, 40, 0, 0, 0, 1, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0.1, 0.3);
    query1(C, 1, 35, 40, 0, 0, 0, 1, 1, 5, 0, 0, 0, 1, 1, 1.5, 1, 0.1, 0.3);
    query1(C, 1, 35, 40, 0, 0, 0, 1, 1, 5, 0, 0, 0, 0, 0, 0, 1, 0.1, 0.3);
    query1(C, 1, 35, 40, 0, 0, 0, 1, 1, 5, 1, 1, 5, 0, 0, 0, 1, 0.1, 0.3);
    query2(C, "Maroon");
    query2(C, "LightBlue");
    query3(C, "Miami");
    query3(C, "UNC");
    query3(C, "NCSU");
    query4(C, "NC", "DarkBlue");
    query5(C, 3);
    query5(C, 2);
}
