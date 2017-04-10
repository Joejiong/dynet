#ifndef DYNET_IO_H_
#define DYNET_IO_H_

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <unordered_map>

#include "dynet/dim.h"
#include "dynet/model.h"
#include "dynet/tensor.h"
#include "dynet/except.h"
#include "dynet/str_util.h"

namespace dynet {

template <class T>
std::istream& operator>>(std::istream& is, std::vector<T> & v) {
  std::copy(std::istream_iterator<T>(is), std::istream_iterator<T>(), v.begin());
  return is;
}

class Pack {
 public:
  Pack(std::string filename) : fn(filename), fn_meta(filename + ".meta") {}
  void reinit(std::string filename) {
    fn = filename;
    fn_meta = filename + ".meta";
    offset = 0;
  }
  /**
   * @brief Save ParameterCollection with key.
   *        use internal namespace if key is not given.
   *
   * @param model: input ParameterCollection object to save
   * @param key: optional parameter, the key for model
   * @param is_append: optional parameter
   *                   to specify whether the model file should be appended or not
   */
  void save(const ParameterCollection & model,
            const std::string & key = "", bool is_append = true);
  /**
   * @brief Save ParameterCollection's parameters and lookup parameters with filter_lst and key.
   *        use internal namespace if key is not given.
   *
   * @param model: input ParameterCollection object to save
   * @param filter_lst: save parameters and lookup parameters satisfies the filter_lst condition
   *                    each filter can be regex expression
   * @param key: optional parameter, the key for model
   * @param is_append: optional parameter
   *                   to specify whether the model file should be appended or not
   */
  void save(const ParameterCollection & model,
            const std::vector<std::string> & filter_lst,
            const std::string & key = "", bool is_append = true);
  void save(const Parameter & param, const std::string & key = "", bool is_append = true);
  void save(const LookupParameter & param, const std::string & key = "", bool is_append = true);
  /**
   * @brief Populate ParameterCollection object with key equals to key.
   * 
   * @param model: input/output parameter, the ParameterCollection object to be populated in. 
   * @param key: optional parameter, the key for loading the model
   *
   */
  void populate(ParameterCollection & model, const std::string & key = "");
  /**
   * @brief Populate ParameterCollection object with filter_lst and with key equals to key.
   * 
   * @param model: input/output parameter, the ParameterCollection object to be populated
   * @param filter_lst: populate parameters and lookup parameters satisfies the filter_lst condition
   *                    each filter can be regex expression
   * @param key: optional parameter, the key for loading the model
   *
   */
  void populate(ParameterCollection & model,
                const std::vector<std::string> & filter_lst,
                const std::string & key = "");
  void populate(Parameter & param, const std::string & key = "");
  void populate(Parameter & param,
                const std::string & model_name,
                const std::string & key) {}
  void populate(LookupParameter & lookup_param,
                const std::string & key = "");
  void populate(LookupParameter & lookup_param,
                const std::string & model_name,
                const std::string & key) {}
 
 private:
  bool duplicate_key_check(const std::string & key); 
  void serialize(const ParameterCollection & model,
                 const std::string & key,
                 bool is_append,
                 std::unordered_map<std::string, long long> & offset_dict);
  void serialize(const Parameter & param,
                 const std::string & key,
                 bool is_append);
  void serialize(const LookupParameter & lookup_param,
                 const std::string & key,
                 bool is_append);
  void deserialize(ParameterCollection & model, const std::string & key);
  void deserialize(Parameter & param, const std::string & key);
  void deserialize(Parameter & param,
                   const std::string & model_name,
                   const std::string key) {}
  void deserialize(LookupParameter & lookup_param, const std::string & key);
  void deserialize(LookupParameter & lookup_param,
                   const std::string & model_name,
                   const std::string & key) {}
  void deserialize_tensor(std::ifstream & f, const Dim & d,
                          std::vector<float> & params_order_lst);

 private:
  std::string fn, fn_meta;
  long long offset = 0;
}; // class Pack

} // namespace dynet

#endif
