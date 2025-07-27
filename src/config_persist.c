#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../include/config_persist.h"
#include "../include/parson.h"    //дай╕     "https://github.com/kgabis/parson#"


bool loadSystemConfig(SystemConfig *config)
{
    JSON_Value *root =json_parse_file("../data/config.json");
    if (!root) return false;
    JSON_Object *obj = json_value_get_object(root);

    strcpy(config->systemMode, json_object_get_string(obj, "systemMode"));
    config->maxStudents = (int)json_object_get_number(obj, "maxStudents");
    config->maxMajors = (int)json_object_get_number(obj, "maxMajors");
    config->enableLog = (int)json_object_get_number(obj, "enableLog");

    json_value_free(root);
    return true;
}

bool saveSystemConfig(const SystemConfig *config)
{
    JSON_Value *root = json_value_init_object(); 
    JSON_Object *obj = json_value_get_object(root);

    json_object_set_string(obj, "systemMode", config->systemMode);
    json_object_set_number(obj, "maxStudents", config->maxStudents);
    json_object_set_number(obj, "maxMajors", config->maxMajors);
    json_object_set_number(obj, "enableLog", config->enableLog);

    return json_serialize_to_file_pretty(root, "data/config.json") == JSONSuccess;
}