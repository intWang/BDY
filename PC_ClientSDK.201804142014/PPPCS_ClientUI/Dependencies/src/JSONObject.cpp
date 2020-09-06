/*
  File Name:JSONObject.cpp
  Author:hujian
  Date:2017/12/03 11:03

  Copyright (c) 2017 HUJIAN

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include "JSONObject.hpp"
#include <iostream>
using namespace std;
namespace PJson
{

    JSONArray::JSONArray()
    {
        mType = cJSON_Array;
        mcJSON = cJSON_CreateArray();
    }
    JSONArray::JSONArray(const char*string)
    {
        mcJSON = cJSON_Parse(string);
    }
    JSONArray::JSONArray(cJSON *json)
    {
        mType = 1;
        mcJSON = json;
        deletemcJSON = true;
    }
    JSONArray::JSONArray(cJSON *json, Boolean_t dc)
    {
        mType = 1;
        mcJSON = json;
        deletemcJSON = dc;
    }
    JSONArray::~JSONArray()
    {
    }
    JSONArray* JSONArray::putBoolean(int index, Boolean_t value)
    {
        int i;
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *jb = cJSON_CreateBool(value);
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            for (i = size; i < index; i++) {
                cJSON_AddItemToArray(mcJSON, cJSON_CreateNull());
            }
            cJSON_AddItemToArray(mcJSON, jb);
        }
        else {
            cJSON_ReplaceItemInArray(mcJSON, index, jb);
        }

        return this;
    }
    JSONArray *JSONArray::put(int index, double value)
    {
        int i;
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *jb = cJSON_CreateNumber(value);
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            for (i = size; i < index; i++) {
                cJSON_AddItemToArray(mcJSON, cJSON_CreateNull());
            }
            cJSON_AddItemToArray(mcJSON, jb);
        }
        else {
            cJSON_ReplaceItemInArray(mcJSON, index, jb);
        }

        return this;
    }
    JSONArray *JSONArray::put(int index, int value)
    {
        return put(index, (double)value);
    }
    JSONArray *JSONArray::put(int index, long value)
    {
        return put(index, (double)value);
    }
    JSONArray *JSONArray::put(int index, String &value)
    {
        int i;
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *jb = cJSON_CreateString(value.data());
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            for (i = size; i < index; i++) {
                cJSON_AddItemToArray(mcJSON, cJSON_CreateNull());
            }
            cJSON_AddItemToArray(mcJSON, jb);
        }
        else {
            cJSON_ReplaceItemInArray(mcJSON, index, jb);
        }

        return this;
    }
    JSONArray *JSONArray::put(int index, Object &value)
    {
        int i;
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *jb = cJSON_Duplicate(value.mcJSON, 1);
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            for (i = size; i < index; i++) {
                cJSON_AddItemToArray(mcJSON, cJSON_CreateNull());
            }
            cJSON_AddItemToArray(mcJSON, jb);
        }
        else {
            cJSON_ReplaceItemInArray(mcJSON, index, jb);
        }

        return this;
    }
    JSONObject *JSONArray::getJSONObject(int index)
    {
        if (mcJSON == 0) {
            return 0;
        }
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d out of bounds in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        cJSON*jo = cJSON_GetArrayItem(mcJSON, index);
        if (jo->type != cJSON_Object) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d is not JSONObject in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        jo = cJSON_Duplicate(jo, 1);
        return new JSONObject(jo);
    }
    JSONArray *JSONArray::getJSONArray(int index)
    {
        if (mcJSON == 0) {
            return 0;
        }
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d out of bounds in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        cJSON*jo = cJSON_GetArrayItem(mcJSON, index);
        if (jo->type != cJSON_Array) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d is not Array in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        jo = cJSON_Duplicate(jo, 1);
        return new JSONArray(jo);
    }
    int JSONArray::getString(int index, String&str)
    {
        if (mcJSON == 0) {
            return 0;
        }
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d out of bounds in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        cJSON*jo = cJSON_GetArrayItem(mcJSON, index);
        if (jo->type != cJSON_String) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d is not String in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        str.resize(strlen(jo->valuestring) + 1);
        str = jo->valuestring;
        return strlen(jo->valuestring);
    }
    int JSONArray::getInt(int index, int &value)
    {
        double v;
        int ret = getDouble(index, v);
        value = (int)v;
        return ret;
    }
    int JSONArray::getDouble(int index, double&value)
    {
        if (mcJSON == 0) {
            return 0;
        }
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d out of bounds in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        cJSON*jo = cJSON_GetArrayItem(mcJSON, index);
        if (jo->type != cJSON_Number) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d is not Double in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        value = jo->valueint;
        return 1;
    }
    int JSONArray::getBoolean(int index, Boolean_t &value)
    {
        if (mcJSON == 0) {
            return 0;
        }
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d out of bounds in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        cJSON*jo = cJSON_GetArrayItem(mcJSON, index);
        if (jo->type != cJSON_False && jo->type != cJSON_True) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d is not Boolean_t in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        value = (Boolean_t)jo->type;
        return 1;
    }

    int JSONArray::toString(String&str)
    {
        if (mcJSON == 0) {
            return 0;
        }

        char*jstr = cJSON_Print(mcJSON);
        str = jstr;
        free(jstr);
        return str.length();
    }
    int JSONArray::getLength()
    {
        if (mcJSON == 0) {
            return 0;
        }
        return cJSON_GetArraySize(mcJSON);
    }
    Object *JSONArray::remove(int index)
    {
        if (mcJSON == 0) {
            return 0;
        }
        int size = cJSON_GetArraySize(mcJSON);
        if (size <= index) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("index %d out of bounds in\n%s", index, arraystr);
            free(arraystr);
            return 0;
        }

        cJSON*json = cJSON_DetachItemFromArray(mcJSON, index);
        return new Object(json);
    }

    JSONObject::JSONObject()
    {
        mType = cJSON_Object;
        mcJSON = cJSON_CreateObject();
    }
    JSONObject::JSONObject(const char*string)
    {
        mcJSON = cJSON_Parse(string);
        if (mcJSON == 0) {
            mType = -1;
            return;
        }
        mType = mcJSON->type;
    }
    JSONObject::~JSONObject()
    {
    }
    JSONObject::JSONObject(cJSON *json)
    {
        mType = json->type;
        mcJSON = json;
        deletemcJSON = true;
    }
    JSONObject::JSONObject(cJSON *json, Boolean_t dc)
    {
        mType = json->type;
        mcJSON = json;
        deletemcJSON = dc;
    }

    JSONObject *JSONObject::put(const char* name, Boolean_t value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            cJSON_AddBoolToObject(mcJSON, name, value);
        }
        else {
            json->type = value ? cJSON_True : cJSON_False;
        }

        cJSON*jo = cJSON_Duplicate(mcJSON, 1);
        return new JSONObject(jo);
    }
    JSONObject *JSONObject::put(const char* name, double value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            cJSON_AddNumberToObject(mcJSON, name, value);
        }
        else {
            json->valuedouble = value;
        }
        cJSON*jo = cJSON_Duplicate(mcJSON, 1);
        return new JSONObject(jo);
    }
    JSONObject *JSONObject::put(const char* name, int value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            cJSON_AddNumberToObject(mcJSON, name, value);
        }
        else {
            json->valueint = value;
        }
        cJSON*jo = cJSON_Duplicate(mcJSON, 1);
        return new JSONObject(jo);
    }
    JSONObject *JSONObject::put(const char* name, long value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            cJSON_AddNumberToObject(mcJSON, name, value);
        }
        else {
            json->valueint = value;
        }
        cJSON*jo = cJSON_Duplicate(mcJSON, 1);
        return new JSONObject(jo);
    }
    JSONObject *JSONObject::put(const char* name, const char* value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json) {
            cJSON_DeleteItemFromObject(mcJSON, name);
        }
        cJSON_AddStringToObject(mcJSON, name, value);
        cJSON*jo = cJSON_Duplicate(mcJSON, 1);
        return new JSONObject(jo);
    }
    JSONObject *JSONObject::put(const char* name, String& value)
    {
        return put(name, value.data());
    }
    JSONObject *JSONObject::put(String& name, const char* value)
    {
        return put(name.data(), value);
    }
    JSONObject *JSONObject::put(const char* name, Object &value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json != 0) {
            cJSON_DeleteItemFromObject(mcJSON, name);
        }

        cJSON_AddItemToObject(mcJSON, name, cJSON_Duplicate(value.mcJSON, 1));
        cJSON*jo = cJSON_Duplicate(mcJSON, 1);
        return new JSONObject(jo);
    }

    JSONObject *JSONObject::getJSONObject(const char*name)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        json = cJSON_Duplicate(json, 1);
        return new JSONObject(json);
    }
    JSONArray *JSONObject::getJSONArray(const char*name)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }
        if (json->type != cJSON_Array) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("%s is not an Array in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        json = cJSON_Duplicate(json, 1);
        return new JSONArray(json);
    }
    int JSONObject::getString(const char*name, String&str)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        str = json->valuestring;
        return 1;
    }

    int JSONObject::getInt(const char*name, int &value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }
        if (json->type != cJSON_Number) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("%s is not int in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        value = json->valueint;
        return 1;
    }

    int JSONObject::getDouble(const char*name, double&value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }
        if (json->type != cJSON_Number) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("%s is not double in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        value = json->valuedouble;
        return 1;
    }

    int JSONObject::getBoolean(const char*name, Boolean_t&value)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        if (json->type != cJSON_False && json->type != cJSON_True) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("%s is not Boolean_t in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }
        value = json->type;
        return 1;
    }

    int JSONObject::toString(String&str)
    {
        if (mcJSON == 0) {
            return 0;
        }

        char*jstr = cJSON_Print(mcJSON);
        str = jstr;
        free(jstr);
        return str.length();
    }

    Boolean_t JSONObject::isNull(const char* name)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        if (json->type == cJSON_NULL)
            return 1;

        return 0;
    }
    Boolean_t JSONObject::has(const char* name)
    {
        if (mcJSON == 0) {
            return 0;
        }

        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            return 1;
        }

        return 0;
    }

    Boolean_t  JSONObject::isValid()
    {
        if (mType<0 || mType>cJSON_Object)
            return false;
        return true;
    }

    Object *JSONObject::remove(const char* name)
    {
        if (mcJSON == 0) {
            return 0;
        }


        cJSON *json = cJSON_GetObjectItem(mcJSON, name);
        if (json == 0) {
            char *arraystr = cJSON_Print(mcJSON);
            //Log("no %s element in\n%s", name, arraystr);
            free(arraystr);
            return 0;
        }

        return new Object(cJSON_DetachItemFromObject(mcJSON, name));
    }

}
