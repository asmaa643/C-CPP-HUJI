
#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_
#include "HashMap.hpp"
#include <iterator>
#include <map>

/**
 * Class invalid key.
 */
class InvalidKey : public std::invalid_argument
{
 public:
  InvalidKey () : invalid_argument ("")
  {};
  explicit InvalidKey (const std::string &w) : invalid_argument (w)
  {}

};

/**
 * Class Dictionary.
 */
class Dictionary : public HashMap<std::string, std::string>
{
 public:
  /**
   * Default Constructor.
   */
  Dictionary () = default;
  /**
   * Constructor that start the dictionary with keys and values.
   */
  Dictionary (const std::vector<std::string> &keys, \
      const std::vector<std::string> &values)
      : HashMap<std::string, std::string> (keys, values)
  {};
  /**
   * This function erases the keys's word from the dictionary.
   * @param key A key to check.
   * @return True if the key was deleted, Exception else.
   */
  bool erase (std::string key) override
  {
    if (!HashMap::erase (key))
    {
      throw InvalidKey ("Invalid Key!!");
    }
    return true;
  }
  /**
   * This function accepts two iterators, begin and end, iterates over them
   * and updates the dictionary with their values.
   */
  template<typename Iterator>
  void update (Iterator begin, Iterator end)
  {
    std::map<std::string, std::string> vectors;
    for (Iterator it = begin; it != end; it++)
    {
      vectors[(*it).first] = (*it).second;
    }
    for (auto &m: vectors)
    {
      if (contains_key (m.first))
      {
        erase (m.first);
      }
      insert (m.first, m.second);
    }
  }
};

#endif //_DICTIONARY_HPP_
