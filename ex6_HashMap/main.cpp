#include <iostream>
#include <string>
#include <vector>

/** A container for sentences that are made of words */
class SentenceContainer
{
  /*** Types ***/
 public:
  typedef std::vector<std::string> t_sentence;
  /*** Member variables ***/
 private:
  std::vector<t_sentence> sentences_;
  /*** Constructors ***/
 public:
  SentenceContainer (const std::vector<t_sentence>& sentences)
      : sentences_ (sentences) {}
  /*****************************************************************
   * Nested class ConstInterator -                                 *
   * A const forward iterator over every word in every sentence in *
   * SequenceContainer                                             *
   *****************************************************************/
  class ConstIterator
  {
    friend class SentenceContainer;
   private:
    const SentenceContainer& sentence_container_;
    size_t sentence_index_;
    size_t word_index_;
   public:
    // Iterator traits
    typedef std::string value_type;
    typedef const value_type &reference;
    typedef const value_type *pointer;
    typedef int difference_type; // required but irrelevant as we have no op-()
    typedef std::forward_iterator_tag iterator_category;
   private:
    /** NOTE: It's ok if you prefer a public constructor, private is better
      * at encapsulating implementation details - we use friend to allow
      * SentenceContainer to access the private ctr
      */
    ConstIterator (const SentenceContainer& sentence_container,
                   size_t sentence_index,
                   size_t word_index)
        : sentence_container_ (sentence_container),
          sentence_index_ (sentence_index),
          word_index_ (word_index) {}
   public:
    ConstIterator &operator++ ()
    {
      // NOTE: a nested class has access to its parent's private members!
      const t_sentence& cur_sentence =
          sentence_container_.sentences_[sentence_index_];
      if (++word_index_ >= cur_sentence.size ())
      {
        word_index_ = 0;
        sentence_index_++;
      }
      return *this;
    }
    ConstIterator operator++ (int)
    {
      ConstIterator it (*this); // default copy-ctr at work
      this->operator++ ();
      return it;
    }
    bool operator== (const ConstIterator &rhs) const
    {
      return (&sentence_container_ == &rhs.sentence_container_)
             && (sentence_index_ == rhs.sentence_index_)
             && (word_index_ == rhs.word_index_);
    }
    bool operator!= (const ConstIterator &rhs) const
    {
      return !operator== (rhs);
    }
    reference operator* () const
    {
      return sentence_container_.sentences_[sentence_index_][word_index_];
    }
    pointer operator-> () const
    {
      return &(operator* ());
    }
  };
 public:
  /*****************************************************************
   * SequenceContainer member functions                            *
   *****************************************************************/
  using const_iterator = ConstIterator;

  const_iterator cbegin () const
  {
    return ConstIterator (*this, 0, 0);
  }
  const_iterator cend () const
  {
    return ConstIterator (*this, sentences_.size(), 0);
  }
  const_iterator begin () const
  {
    return cbegin ();
  }
  const_iterator end () const
  {
    return cend ();
  }
};

int main()
{
  std::vector<std::string> sentence1
      {"I", "am", "the", "first",
       "sentence,", "look", "at", "me!"};
  std::vector<std::string> sentence2
      {"I" ,"am", "stuck", "in", "the", "middle."};
  std::vector<std::string> sentence3
      {"I", "am", "the", "last", "and", "prettiest",
       "of", "them", "all."};
  SentenceContainer sc ({sentence1, sentence2, sentence3});
  for (auto it = sc.begin(); it != sc.end(); it++)
  {
    std::cout << "Word: '" << *it << "'" << std::endl;
  }
}






