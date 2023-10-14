#include "./filtered_string_view.h"

#include <catch2/catch.hpp>

TEST_CASE("filter me if you can") {

  auto fsv = fsv::filtered_string_view();
  for (char c = std::numeric_limits<char>::min(); c != std::numeric_limits<char>::max(); c++) {
    assert( fsv.default_predicate(c) );
  }



  //REQUIRE(false);
}

TEST_CASE("Test Default constructor") {

  auto sv = fsv::filtered_string_view{};
  std::cout << sv.size() << std::endl;


  REQUIRE(sv.size() == 0);
}


TEST_CASE("Test implict string constructor") {

  auto s = std::string{"cat"};
  auto sv = fsv::filtered_string_view{s};
  std::cout << sv.size() << std::endl;

  REQUIRE(sv.size() == 3);
}


TEST_CASE("Test string constructor with predicate") {

  auto s = std::string{"cat"};
  auto pred = [](const char& c) { return c == 'a'; };
  auto sv = fsv::filtered_string_view{s, pred};
  std::cout << sv.size() << std::endl;


  REQUIRE(sv.size() == 1);
}


TEST_CASE("Test Null-terminate implict string constructor") {

  auto sv = fsv::filtered_string_view{"cat"};
  std::cout << sv.size() << std::endl;

  REQUIRE(sv.size() == 3);
}


TEST_CASE("Test null-terminate string constructor with predicate") {

  auto pred = [](const char &c) { return c == 'a'; };
  auto sv = fsv::filtered_string_view{"cat", pred};
  std::cout << sv.size() << std::endl;


  REQUIRE(sv.size() == 1);
}

TEST_CASE("Test copy and move constructor") {

  auto sv1 = fsv::filtered_string_view{"bulldog"};
  const auto copy = sv1;

  assert(copy.data() == sv1.data()); // pointers compare equal.

  const auto move = std::move(sv1);
  std::cout<<move.data()<<std::endl;
  assert(sv1.data() == nullptr); // true: sv1's guts were moved into `move`


}


TEST_CASE("Test copy assignment operator= overload func") {

  auto pred = [](const char &c) { return c == '4' || c == '2'; };
  auto fsv1 = fsv::filtered_string_view{"42 bro", pred};
  auto fsv2 = fsv::filtered_string_view{};
  fsv2 = fsv1;
  std::cout<<"fsv2"<<fsv2<<std::endl;
  std::cout<<"fsv1"<<fsv1<<std::endl;
  assert(fsv1 == fsv2);

}

TEST_CASE("Test move assignment operator= overload func")
{  auto pred = [](const char &c) { return c == '8' || c == '9'; };
  auto fsv1 = fsv::filtered_string_view{"'89 baby", pred};
  auto fsv2 = fsv::filtered_string_view{};

  fsv2 = std::move(fsv1);

  std::cout<<"test move assignment"<<std::endl;
  std::cout<<fsv2<<std::endl;
  //assert(fsv2 == "89");

  assert(fsv1.size() == 0 && fsv1.data() == nullptr);
}

TEST_CASE("Test operator[] overload")
{
  auto pred = [](const char &c) { return c == '9' || c == '0' || c == ' '; };
  auto fsv1 = fsv::filtered_string_view{"only 90s kids understand", pred};
  std::cout<<"in fsv1"<<std::endl;
  std::cout << "fsv1[2]"<<std::endl <<fsv1[2] << std::endl;
  std::cout << "fsv1[1]"<<std::endl  <<fsv1[1] << std::endl;
  std::cout << "fsv1[0]"<<std::endl  <<fsv1[0] << std::endl;


  assert(fsv1[2] == '0');
}

TEST_CASE("Test operator std::string() overload")
{
  auto sv = fsv::filtered_string_view("vizsla");
  auto s = static_cast<std::string>(sv);
  std::cout << std::boolalpha << (sv.data() == s.data()) << std::endl;

  assert((sv.data() == s.data()) == false);
}

TEST_CASE("Test operator at() overload")
{
  auto vowels = std::set<char>{'a', 'A', 'e', 'E', 'i', 'I', 'o', 'O', 'u', 'U'};
  auto is_vowel = [&vowels](const char &c){ return vowels.contains(c); };
  auto sv = fsv::filtered_string_view{"Malamute", is_vowel};

  //auto sv = fsv::filtered_string_view{""};
  try {
    sv.at(0);
  } catch (const std::domain_error &e) {
    std::cout << e.what()<<std::endl;
  }
  std::cout<<sv.at(0)<<std::endl;

  assert(sv.at(0) == 'a');

}

TEST_CASE("Test size() func")
{
  // auto sv = fsv::filtered_string_view{"Maltese"};
  // std::cout << sv.size();

  // assert(sv.size() == 7);

  auto sv = fsv::filtered_string_view{"Toy Poodle", [](const char &c) {
    return c == 'o' || c == ' ' || c == 'e';
  }};

  std::cout << sv.size()<<std::endl;

  assert(sv.size() == 5);  

}


TEST_CASE("Test empty() func")
{
  // auto sv = fsv::filtered_string_view{"Australian Shephard"};
  // auto empty_sv = fsv::filtered_string_view{};
  // std::cout << std::boolalpha << sv.empty() << ' ' << empty_sv.empty();
  auto sv = fsv::filtered_string_view{"Border Collie", [](const char &c) {
      return c == 'z';
  }};
  std::cout << std::boolalpha << sv.empty()<<std::endl;


}

TEST_CASE("Test data() func")
{
  auto s = "Sum 42";
  auto sv = fsv::filtered_string_view{s, [](const char &){ return false;}};
  
  for (auto ptr = sv.data(); *ptr; ++ptr) {
    std::cout << *ptr;
  }
  std::cout<<std::endl;
}

TEST_CASE("Test predicate() func"){

  const auto print_and_return_true = [](const char &) {
      std::cout << "hi!"<<std::endl;
      return true;
  };
  const auto s = fsv::filtered_string_view{"doggo", print_and_return_true};

  const auto& predicate = s.predicate();
  predicate(char{});


}


TEST_CASE("Test operator== overload")
{
  auto const lo = fsv::filtered_string_view{"aaa"};
  auto const hi = fsv::filtered_string_view{"zzz"};

  std::cout << std::boolalpha 
            << (lo == hi) << ' '
            << (lo != hi)<<std::endl;

}

TEST_CASE("Test operator<=> overload"){

  auto const lo = fsv::filtered_string_view{"aaa"};
  auto const hi = fsv::filtered_string_view{"zzz"};

  std::cout << std::boolalpha 
            << (lo < hi) << ' '
            << (lo <= hi) << ' '
            << (lo > hi) << ' '
            << (lo >= hi) << ' '
            << (lo <=> hi == std::strong_ordering::less)<<std::endl;

}

TEST_CASE("Test output stream")
{
  auto fsv = fsv::filtered_string_view{"c++ > rust > java", [](const char &c){ return c == 'c' || c == '+'; }};
  std::cout << fsv <<std::endl;
}

//Non-Member Utility Functions Tests

TEST_CASE("Test compose")
{
  auto best_languages = fsv::filtered_string_view{"c / c++"};
  auto vf = std::vector<fsv::filter>{
    [](const char &c){ return c == 'c' || c == '+' || c == '/'; },
    [](const char &c){ return c > ' '; },
    [](const char &){ return true; }
  };

  std::cout<<best_languages<<std::endl;
  // fsv::filtered_string_view sv;
  // std::cout << sv <<std::endl;
  // std::cout<<"after construct sv"<<std::endl;
  auto sv = fsv::compose(best_languages, vf);
  std::cout<<"type"<<typeid(sv).name()<<std::endl;
  std::cout << sv <<std::endl;

}

// TEST_CASE("Test split()"){

//   auto interest = std::set<char>{'a', 'A', 'b', 'B', 'c', 'C', 'd', 'D', 'e', 'E', 'f', 'F', ' ', '/'};
//   auto sv = fsv::filtered_string_view{"0xDEADBEEF / 0xdeadbeef", [&interest](const char &c){ return interest.contains(c); }};
//   std::cout<<std::endl<<"splict() test"<<sv<<std::endl;
//   auto tok = fsv::filtered_string_view{" / "};
//   auto v = fsv::split(sv, tok);
//   // std::cout<<"after split(), cout sv"<<std::endl;
//   // std::cout<<sv<<std::endl;

//   std::cout << v[0] << " " << v[1];

//   auto sv1 = fsv::filtered_string_view{"xax"};
//   auto tok1  = fsv::filtered_string_view{"x"};
//   auto v1 = fsv::split(sv1, tok1);

//   std::cout<<v1[0]<<v1[1]<<v1[2]<<std::endl;
//   // // auto expected = std::vector<fsv::filtered_string_view> vec{"", "a", ""};

//   // CHECK(v == expected);

//   // auto sv = fsv::filtered_string_view{"xx"};
//   // auto tok  = fsv::filtered_string_view{"x"};
//   // auto v = fsv::split(sv, tok);
//   // auto expected = std::vector<fsv::filtered_string_view>{"", "", ""};

//   // CHECK(v == expected);



// }

TEST_CASE("Test abstr()"){

  
  auto sv = fsv::filtered_string_view{"Siberian Husky"};
  std::cout << fsv::substr(sv, 9);

  auto is_upper = [](const char &c) { return std::isupper(static_cast<unsigned char>(c));};
  auto sv1 = fsv::filtered_string_view{"Sled Dog", is_upper};
  std::cout << fsv::substr(sv1, 0, 2);



}


//test iterator
// TEST_CASE("iterator")
// {
 
//   auto print_via_iterator = [](fsv::filtered_string_view const& sv) {
//   std::copy(sv.begin(), sv.end(), std::ostream_iterator<char>(std::cout, " "));
//   };

  

// // With default predicate:
//   auto fsv1 = fsv::filtered_string_view{"corgi"};
//   print_via_iterator(fsv1);

// }