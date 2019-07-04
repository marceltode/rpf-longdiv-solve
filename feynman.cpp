#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

#define toDigit(c) ((c)-'0')

using step = tuple<string, string>;

struct puzzle
{
  string npattern;
  string dpattern;
  string qpattern;
  int dlow, dhigh;
  int qlow, qhigh;
  vector<step> steps;
};

struct solution
{
  int n;
  int d;
  int q;
};

bool valid_nbr_fmt(char A, int nbr, string nbr_pattern)
{
  const string dec = to_string(nbr);

  if (dec.size() != nbr_pattern.length())
    return false;

  for (size_t pos = 0; pos < nbr_pattern.length(); ++pos)
  {
    if (nbr_pattern[pos] == 'A' && dec[pos] != A)
      return false;
    if (nbr_pattern[pos] == '.' && dec[pos] == A)
      return false;
  }

  return true;
}

bool check_division(char A, int div, int quot, puzzle p)
{
  int n2 = div * quot;
  if (!valid_nbr_fmt(A, n2, p.npattern))
    return false;

  const string qdec = to_string(quot);
  for (size_t step = 0; step < p.qpattern.length(); step++)
  {
    string prod_pattern = get<0>(p.steps[step]);
    int prod = div * toDigit(*(qdec.c_str() + step));
    if (!valid_nbr_fmt(A, prod, prod_pattern))
      return false;
  }

  return true;
}

auto read() -> puzzle
{
  puzzle p;
  ifstream in("puzzle.txt");

  // read first line N, D and Q
  string line;
  getline(in, line);
  vector<string> dnq;
  split(dnq, line, is_any_of("/\\"));
  p.dpattern = dnq[0];
  p.npattern = dnq[1];
  p.qpattern = dnq[2];
  p.dlow = pow(10, p.dpattern.length() - 1);
  p.dhigh = p.dlow * 10;
  p.qlow = pow(10, p.qpattern.length() - 1);
  p.qhigh = p.qlow * 10;

  // read steps
  int i = -1;
  string product, remainder, dashes;
  while (++i, remainder != "0")
  {
    if (i % 3 == 0) {
      std::getline(in, product);
      trim(product);
    }
    else if (i % 3 == 1) {
      std::getline(in, dashes);
    }
    else if (i % 3 == 2) {
      std::getline(in, remainder);
      trim(remainder);
      p.steps.push_back(make_tuple(product, remainder));
    }
  }

  in.close();
  return p;
}

auto solve(puzzle &p) -> vector<solution>
{
  vector<solution> solutions;
  for (char A = '0'; A <= '9'; A++)
  {
    for (int div = p.dlow; div < p.dhigh; div++)
    {
      if (!valid_nbr_fmt(A, div, p.dpattern))
        continue;
      for (int quot = p.qlow; quot < p.qhigh; quot++)
      {
        if (!valid_nbr_fmt(A, quot, p.qpattern))
          continue;
        if (!check_division(A, div, quot, p))
          continue;
        solutions.push_back({div * quot, div, quot});
      }
    }
  }
  return solutions;
}

void write(puzzle &p, vector<solution> &solutions)
{
  cout << p.dpattern << " / " << p.npattern << " \\ " << p.qpattern
       << endl;

  if (solutions.size() == 0)
  {
    cout << "No solutions found.";
    return;
  }

  for (auto s : solutions)
  {
    cout << s.d << " / " << s.n << " \\ " << s.q
         << endl;
  }
}

int main()
{
  auto puzzle = read();
  auto solutions = solve(puzzle);
  write(puzzle, solutions);
  return 0;
}
