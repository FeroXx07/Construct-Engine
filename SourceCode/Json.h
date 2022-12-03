#ifndef _JSON_
#define _JSON_
#include <nlohmann/json.hpp>
// for convenience
using json = nlohmann::json;
// Return variable name as str
#define getNameStr(var)  #var
// Return variable name as str and save it in another string
#define getNameStrToStr(var, str)  sprintf(str, "%s", #var) 
#endif