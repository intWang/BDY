/*
  File Name:JSONObject.h
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
#ifndef __CJSON__H__
#define __CJSON__H__
#include "cJSON.h"
#include <iostream>
#include <exception>
using namespace std;
using std::string;

typedef std::string String;
typedef unsigned char Boolean_t;

extern "C" void Log(const char* ptzFormat, ...);

#define LOGV(format,...) TRACE(format,__VA_ARGS__);
#define LOGD(format,...) TRACE(format,__VA_ARGS__);
#define LOGI(format,...) TRACE(format,__VA_ARGS__);
#define LOGW(format,...) TRACE(format,__VA_ARGS__);
#define LOGE(format,...) TRACE(format,__VA_ARGS__);
namespace PJson
{

    class Object {
    public:
        Object() { mcJSON = 0; deletemcJSON = true; }
        Object(cJSON *json) { mcJSON = json; deletemcJSON = true; }
        Object(cJSON *json, Boolean_t dc) { mcJSON = json; deletemcJSON = dc; }
        ~Object() { if (mcJSON && deletemcJSON) { cJSON_Delete(mcJSON); mcJSON = 0; } }
        int mType;//JSONObject:0 JSONArray:1
        cJSON *mcJSON;
        Boolean_t deletemcJSON;
    };
    class JSONObject;
    class JSONArray :public Object {
    public:
        JSONArray();
        JSONArray(const char*string);
        JSONArray(cJSON *json);
        JSONArray(cJSON *json, Boolean_t dc);
        ~JSONArray();

        JSONArray *putBoolean(int index, Boolean_t value);
        JSONArray *put(int index, double value);
        JSONArray *put(int index, int value);
        JSONArray *put(int index, long value);
        JSONArray *put(int index, String &value);
        JSONArray *put(int index, Object &value);
        //JSONArray *putBoolean(Boolean_t value);
        //JSONArray *put(double value);
        //JSONArray *put(int value);
        //JSONArray *put(long value);
        //JSONArray *put(String &value);
        //JSONArray *put(Object &value);

        JSONObject *getJSONObject(int index);
        JSONArray *getJSONArray(int index);
        int getString(int index, String&str);
        int getInt(int index, int &value);
        int getDouble(int index, double&value);
        int getBoolean(int index, Boolean_t&value);

        int getLength();
        int toString(String&str);

        Object *remove(int index);
    };
    class JSONObject :public Object {
    public:
        JSONObject();
        JSONObject(const char*string);
        JSONObject(cJSON *json);
        JSONObject(cJSON *json, Boolean_t dc);
        ~JSONObject();

        JSONObject *put(const char* name, Boolean_t value);
        JSONObject *put(const char* name, double value);
        JSONObject *put(const char* name, int value);
        JSONObject *put(const char* name, long value);
        JSONObject *put(const char* name, const char* value);
        JSONObject *put(const char* name, String&str);
        JSONObject *put(String&str, const char* name);
        JSONObject *put(const char* name, Object &value);

        JSONObject *getJSONObject(const char*name);
        JSONArray *getJSONArray(const char*name);
        int getString(const char*name, String&str);
        int getInt(const char*name, int &value);
        int getDouble(const char*name, double&value);
        int getBoolean(const char*name, Boolean_t&value);

        //JSONObject& operator =(const JSONObject& str);

        int toString(String&str);

        Boolean_t isNull(const char* name);
        Boolean_t has(const char* name);
        Boolean_t isValid();

        Object* remove(const char* name);
    };

}

#endif
