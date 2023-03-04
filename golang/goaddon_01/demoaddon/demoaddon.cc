#include <napi.h>
#include <string>
#include <assert.h>
#include <functional>

#include "demoaddon.h"

using namespace Napi;
using namespace std;

#define NAPI_EXPERIMENTAL
// [common]++++++++++++++++++++++++++++++++++++++ start
//---------- genWgAddonArg ----------
typedef struct {
  int type; // [1]char [2]int [3]float [4]double [5]bool
  int len;
  void* value;
} WgAddonArgInfo;
// ------------- genStringSplit -----------
void wg_string_split(const string& str, const char split, vector<string>& res){
  if (str == "") return;
  string strs = str + split;
  size_t pos = strs.find(split);
  while (pos != strs.npos){
	string temp = strs.substr(0, pos);
	res.push_back(temp);
	strs = strs.substr(pos + 1, strs.size());
	pos = strs.find(split);
  }
}
// ------------- genStringToArray2 -----------
string wg_array_to_string(Array arr) {
  string res = "[";
  string last;
  for(uint32_t i = 0; i < arr.Length(); i++){
    Value v = arr[i];
    if (last.size() > 0) {
      res += last;
      last = "";
    }
    if (v.IsArray()){
      Array arr2 = v.As<Array>();
      res += wg_array_to_string(arr2);
      last = ",";
    } else {
      string ss = v.ToString();
      res += "\"" + ss + "\"";
      last = ",";
    }
  }
  res += "]";
  return res;
}
// ------------- genStringToArray -----------
Array wg_string_to_array(string str, Env env) {
  Array arr = Array::New(env);
  vector<string> strList;
  if (str == "") return arr;
  size_t pos;
  while ((pos = str.find("[")) != string::npos) {
	str.replace(pos, 1, ",");
  }
  while ((pos = str.find("]")) != string::npos) {
    str.replace(pos, 1, ",");
  }
  wg_string_split(str, ',', strList);
  int index = 0;
  for (auto s : strList) {
    if (s.size() > 0) {
      int _spos = s.find("\"");
      s = s.substr(_spos + 1);
      int _epos = s.find("\"");
      s = s.substr(0, _epos);
      arr.Set(Number::New(env, index), String::New(env, s));
      index++;
    }
  }
  return arr;
}
// ------------- genObjectArrToString -----------
string wg_object_to_string(Object objs);
string wg_object_array_to_string(Array arr) {
  string res = "[";
  string last;
  for(uint32_t i = 0; i < arr.Length(); i++){
    Value v = arr[i];
    if (last.size() > 0) {
      res += last;
      last = "";
    }
    if (v.IsArray()){
      Array arr2 = v.As<Array>();
      res += wg_object_array_to_string(arr2);
      last = ",";
    } else if (v.IsObject()){
      Object obj2 = v.As<Object>();
      res += wg_object_to_string(obj2);
      last = ",";
    } else {
      string ss = v.ToString();
      res += "\"" + ss + "\"";
      last = ",";
    }
  }
  res += "]";
  return res;
}
// ------------- genObjectToString -----------
string wg_object_to_string(Object objs) {
  string res = "{";
  Array keyArr = objs.GetPropertyNames();
  string last;
  for(uint32_t i = 0; i < keyArr.Length(); i++){
    Value key = keyArr[i];
    Value v = objs.Get(key);
    string name = key.As<String>().Utf8Value();
    if (last.size() > 0) {
      res += last;
      last = "";
    }
    res += "\"" + name + "\":";
    if (v.IsArray()) {
      Array arr = v.As<Array>();
      res += wg_object_array_to_string(arr);
      last = ",";
    } else if (v.IsObject()){
      Object obj2 = v.As<Object>();
      res += wg_object_to_string(obj2);
      last = ",";
    } else {
      string ss = v.ToString();
      res += "\"" + ss + "\"";
      last = ",";
    }
  }
  res += "}";
  return res;
}
// ------------- genStringToObject -----------
Object wg_string_to_object(string str, Env env) {
  Object obj = Object::New(env);
  vector<string> strList;
  if (str == "") return obj;
  size_t pos;
  while ((pos = str.find("{")) != string::npos) {
    str.replace(pos, 1, ",");
  }
  while ((pos = str.find("}")) != string::npos) {
    str.replace(pos, 1, ",");
  }
  wg_string_split(str, ',', strList);
  for (auto s : strList) {
    if (s.size() > 0) {
      while ((pos = s.find("\"")) != string::npos) {
        s.replace(pos, 1, "");
      }
      vector<string> keyValue;
      wg_string_split(s, ':', keyValue);
      string key = keyValue.size() >= 0 ? keyValue[0] : "";
      string value = keyValue.size() >= 1 ? keyValue[1] : "";
      obj.Set(String::New(env, key), String::New(env, value));
    }
  }
  return obj;
}
// [common]++++++++++++++++++++++++++++++++++++++ end
// ---------- GenCode ---------- 
Value _Hello(const CallbackInfo& wg_info) {
  Env wg_env = wg_info.Env();
  if(wg_info.Length() > 0 && !wg_info[0].IsString()){
    TypeError::New(wg_env, "The 1 parameter must be of string!").ThrowAsJavaScriptException();
    return wg_env.Null();
  }
  string wg_name = "";
  if (wg_info.Length() > 0) {
    wg_name = wg_info[0].As<String>().Utf8Value();
  }
  char *name = new char[wg_name.length() + 1];
  strcpy(name, wg_name.c_str());
  string wg_res_ = Hello(name);
  delete [] name;
  return String::New(wg_env, wg_res_);
}
Object Init(Env env, Object exports) {
  exports.Set(String::New(env, "hello"), Function::New(env, _Hello));
  return exports;
}

NODE_API_MODULE(demoaddon, Init)