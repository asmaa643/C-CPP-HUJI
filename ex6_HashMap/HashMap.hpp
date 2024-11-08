
#ifndef _HASHMAP_HPP_
#define _HASHMAP_HPP_
#include <iostream>
#include <vector>
#include <cmath>

#define CAPACITY 16
#define MIN_LOAD_FACTOR 0.25
#define MAX_LOAD_FACTOR 0.75

/**
 * Class Hash Map that supports every Key and Value types.
 */
template<typename KeyT, typename ValueT>
class HashMap
{
 private:
  std::vector<std::pair<KeyT, ValueT>> *my_map;
  std::vector<KeyT> vector_keys;
  int map_capacity;
  int map_keys;
  /**
   * This function rehashes the map after resizing.
   * @param n The old size.
   */
  void calculate_and_update (int n)
  {
    std::hash<KeyT> t1;
    auto v1 = my_map;
    auto v = new std::vector<std::pair<KeyT, ValueT>>[map_capacity];
    for (int i = 0; i < n; i++)
    {
      if (!my_map[i].empty ())
      {
        for (int j = 0; j < (int) my_map[i].size (); j++)
        {
          int m = (t1 (my_map[i][j].first)) & (map_capacity - 1);
          v[m].push_back (std::make_pair (my_map[i][j].first, \
            my_map[i][j].second));
        }
      }
    }
    my_map = v;
    delete[] v1;
  }
  /**
   * This function resizes the map according to capacity and number of values.
   */
  void resize ()
  {
    if (map_keys != 1 || map_capacity == map_keys)
    {
      while (this->get_load_factor () < MIN_LOAD_FACTOR && map_capacity > 1)
      {
        map_capacity /= 2;
        calculate_and_update (map_capacity * 2);
      }
      while (this->get_load_factor () > MAX_LOAD_FACTOR)
      {
        map_capacity *= 2;
        calculate_and_update (map_capacity / 2);
      }
    }
  }

 public:
  /**
   * Default constructor.
   * Constructs a empty map, capacity 16.
   */
  HashMap ()
  {
    my_map = new std::vector<std::pair<KeyT, ValueT>>[CAPACITY];
    map_capacity = CAPACITY;
    map_keys = 0;
  }
  /**
   * Constructor that start the map with keys and values.
   */
  HashMap (const std::vector<KeyT> &keys, const std::vector<ValueT> &values)
      : HashMap ()
  {
    if (keys.size () != values.size ())
    {
      throw std::length_error ("Vectors have different sizes!!");
    }
    map_capacity = CAPACITY;
    map_keys = 0;
    for (int i = 0; i < (int) keys.size (); i++)
    {
      std::hash<KeyT> t1;
      int k = t1 (keys[i]);
      int place_key = k & (map_capacity - 1);
      if (contains_key (keys[i]))
      {
        for (int j = 0; j < (int) my_map[place_key].size (); j++)
        {
          if (my_map[place_key][j].first == keys[i])
          {
            my_map[place_key][j] = std::make_pair (keys[i], values[i]);
          }
        }
        continue;
      }
      vector_keys.push_back (keys[i]);
      KeyT t_1 = keys[i];
      ValueT t_2 = values[i];
      std::pair<KeyT, ValueT> p = std::make_pair (t_1, t_2);
      my_map[place_key].push_back (p);
      map_keys++;
    }
    resize ();
  }
  /**
   * Copy constructor.
   * @param other A hash map to copy.
   */
  HashMap (const HashMap &other) : HashMap ()
  {
    map_capacity = other.map_capacity;
    map_keys = other.map_keys;
    vector_keys.clear ();
    auto v1 = my_map;
    auto v = new std::vector<std::pair<KeyT, ValueT>>[map_capacity];
    my_map = v;
    delete[] v1;
    for (int i = 0; i < map_capacity; i++)
    {
      if (!other.my_map[i].empty ())
      {
        my_map[i] = other.my_map[i];
      }
    }
    vector_keys = other.vector_keys;
  }
  /**
   * Destructor.
   */
  virtual ~HashMap ()
  {
    for (int i = 0; i < map_capacity; i++)
    {
      my_map[i].clear ();
    }
    delete[] my_map;
  }
  /**
   * Assignment operator.
   * @param other A hash map to copy.
   * @return The hash map (by reference) after copying.
   */
  HashMap &operator= (const HashMap &other)
  {
    if (this != &other)
    {
      for (int i = 0; i < map_capacity; i++)
      {
        my_map[i].clear ();
      }
      delete[] my_map;
      map_capacity = other.map_capacity;
      map_keys = other.map_keys;
      my_map = new std::vector<std::pair<KeyT, ValueT>>[map_capacity];
      for (int i = 0; i < map_capacity; i++)
      {
        if (!other.my_map[i].empty ())
        {
          my_map[i] = other.my_map[i];
        }
      }
      vector_keys = other.vector_keys;
    }
    return *this;
  }
  /**
   * @return Hash map size.
   */
  int size () const
  {
    return map_keys;
  }
  /**
   * @return Hash map capacity.
   */
  int capacity () const
  {
    return map_capacity;
  }
  /**
   * @return True if the map is empty, false if not.
   */
  bool empty () const
  {
    return map_keys == 0;
  }
  /**
   * Inserts the value with its key.
   * @param key A key to insert.
   * @param value A value to insert.
   * @return True if key is new and was inserted, false else.
   */
  bool insert (KeyT key, ValueT value)
  {
    std::hash<KeyT> t1;
    int k = t1 (key);
    int place_key = k & (map_capacity - 1);
    if (contains_key (key))
    {
      return false;
    }
    KeyT t_1 = key;
    ValueT t_2 = value;
    map_keys++;
    std::pair<KeyT, ValueT> p = std::make_pair (t_1, t_2);
    my_map[place_key].push_back (p);
    vector_keys.push_back (key);
    resize ();
    return true;
  }
  /**
   * @param key A key to check.
   * @return True if the key is in the map, false else.
   */
  bool contains_key (KeyT key) const
  {
    for (size_t i = 0; i < vector_keys.size (); i++)
    {
      if (vector_keys[i] == key)
      {
        return true;
      }
    }
    return false;
  }
  /**
   * Checks if the kay is in the map and returns its value.
   * @param key A key to check.
   * @return Its value.
   */
  ValueT at (KeyT key) const
  {
    if (!contains_key (key))
    {
      throw std::out_of_range ("The key is not here!");
    }
    std::hash<KeyT> t1;
    int bucket_to_check = (t1 (key)) & (map_capacity - 1);
    for (size_t i = 0; i < (int) my_map[bucket_to_check].size (); i++)
    {
      if (my_map[bucket_to_check][i].first == key)
      {
        return my_map[bucket_to_check][i].second;
      }
    }
    return ValueT ();
  }
  /**
   * Checks if the kay is in the map and returns its value.
   * @param key A key to check.
   * @return Its value by reference.
   */
  ValueT &at (KeyT key)
  {
    if (!contains_key (key))
    {
      throw std::out_of_range ("The key is not here!");
    }
    std::hash<KeyT> t1;
    int bucket_to_check = (t1 (key)) & (map_capacity - 1);
    for (size_t i = 0; i < my_map[bucket_to_check].size (); i++)
    {
      if (my_map[bucket_to_check][i].first == key)
      {
        return my_map[bucket_to_check][i].second;
      }
    }
    return operator[] (key);
  }
  /**
   * @return The map's load factor.
   */
  double get_load_factor () const
  {
    return (double) map_keys / (double) map_capacity;
  }
  /**
   * @param key A key in the map.
   * @return Its bucket size (Where it is placed), if it is in the map.
   */
  int bucket_size (KeyT key) const
  {
    if (!contains_key (key))
    {
      throw std::out_of_range ("The key is not here!");
    }
    std::hash<KeyT> t1;
    std::vector<std::pair<KeyT, ValueT>> t = my_map[(t1 (key))
                                                    & (map_capacity - 1)];
    return (int) t.size ();
  }
  /**
   * @param key A key in the map.
   * @return Its bucket index (Where it is placed), if it is in the map.
   */
  int bucket_index (KeyT key) const
  {
    if (!contains_key (key))
    {
      throw std::out_of_range ("The key is not here!");
    }
    std::hash<KeyT> t1;
    int n = (t1 (key)) & (map_capacity - 1);
    return n;
  }
  /**
   * Erases the key's value from the map.
   * @param key A key to delete its value.
   * @return True if the key is in the map and the value eas deleted.
   */
  virtual bool erase (KeyT key)
  {
    if (!contains_key (key))
    {
      return false;
    }
    std::hash<KeyT> t1;
    int index = (t1 (key)) & (map_capacity - 1);
    for (int i = 0; i < (int) my_map[index].size (); i++)
    {
      if (key == my_map[index][i].first)
      {
        my_map[index].erase (my_map[index].begin () + i);
      }
    }
    for (int i = 0; i < (int) vector_keys.size (); i++)
    {
      if (key == vector_keys[i])
      {
        vector_keys.erase (vector_keys.begin () + i);
      }
    }
    map_keys--;
    resize ();
    return true;
  }
  /**
   * This function clears all the values from the map.
   */
  void clear ()
  {
    for (int i = 0; i < map_capacity; i++)
    {
      my_map[i].clear ();
    }
    vector_keys.clear ();
    map_keys = 0;
  }
  /**
   * Checks if the kay is in the map and returns its value, else it add a
   * default value.
   * @param key A key to check.
   * @return Its value.
   */
  ValueT operator[] (KeyT key) const
  {
    if (!contains_key (key))
    {
      return ValueT ();
    }
    std::hash<KeyT> t1;
    int bucket_to_check = (t1 (key)) & (map_capacity - 1);
    for (size_t i = 0; i < my_map[bucket_to_check].size (); i++)
    {
      if (my_map[bucket_to_check][i].first == key)
      {
        return my_map[bucket_to_check][i].second;
      }
    }
    return ValueT ();
  }
  /**
  * Checks if the kay is in the map and returns its value, else it add a
  * default value.
  * @param key A key to check.
  * @return Its value by reference.
  */
  ValueT &operator[] (KeyT key)
  {
    if (!contains_key (key))
    {
      this->insert (key, ValueT ());
    }
    std::hash<KeyT> t1;
    int bucket_to_check = (t1 (key)) & (map_capacity - 1);
    auto h = my_map[bucket_to_check];
    for (size_t i = 0; i < my_map[bucket_to_check].size (); i++)
    {
      if (my_map[bucket_to_check][i].first == key)
      {
        return my_map[bucket_to_check][i].second;
      }
    }
    return operator[] (key);
  }
  /**
   * This function checks if two maps are equal.
   */
  bool operator== (const HashMap &other) const
  {
    if (this->map_keys != other.map_keys)
    {
      return false;
    }
    for (size_t i = 0; i < vector_keys.size (); i++)
    {
      if (!other.contains_key (vector_keys[i]))
      {
        return false;
      }
      if (operator[] (vector_keys[i]) != other.operator[] (vector_keys[i]))
      {
        return false;
      }
    }
    return true;
  }
  /**
   * This function checks if two maps are not equal.
   */
  bool operator!= (const HashMap &other) const
  {
    return !(this->operator== (other));
  }
  /**
   * Const iterator for the hash map.
   */
  class ConstIterator
  {
    friend class HashMap;
   public:
    typedef std::pair<KeyT, ValueT> value_type;
    typedef value_type &reference;
    typedef value_type *pointer;
    typedef int difference_type;
    typedef std::forward_iterator_tag iterator_category;
    ConstIterator (const HashMap<KeyT, ValueT> &hash,
                   size_t _index,
                   size_t b)
        : ma (hash),
          cur (_index),
          bucket_index (b)
    { operator++ (); }

    ConstIterator &operator++ ()
    {
      if (cur < (size_t) ma.map_capacity)
      {
        if (++bucket_index >= ma.my_map[cur].size ())
        {
          bucket_index = 0;
          cur++;
        }
      }
      while (cur < (size_t) ma.map_capacity && ma.my_map[cur].empty ())
      {
        cur += 1;
      }
      return *this;
    }

    ConstIterator operator++ (int)
    {
      ConstIterator it (*this);
      this->operator++ ();
      return it;
    }

    bool operator== (const ConstIterator &rhs) const
    {
      return (&ma == &rhs.ma) && (cur == rhs.cur)
             && (bucket_index == rhs.bucket_index);
    }

    bool operator!= (const ConstIterator &rhs) const
    {
      return !operator== (rhs);
    }

    reference operator* () const
    {
      return ma.my_map[cur][bucket_index];
    }

    pointer operator-> () const
    {
      return &(operator* ());
    }
   private:
    const HashMap<KeyT, ValueT> &ma;
    size_t cur;
    size_t bucket_index;
  };

  using const_iterator = ConstIterator;
  const_iterator begin () const
  { return ConstIterator (*this, 0, 0); }
  const_iterator cbegin () const
  { return ConstIterator (*this, 0, 0); }
  const_iterator end () const
  { return ConstIterator (*this, map_capacity, 0); }
  const_iterator cend () const
  { return ConstIterator (*this, map_capacity, 0); }
};

#endif //_HASHMAP_HPP_
