#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <vector>

#include "rb_tree.hpp"

using namespace std;

// Teszt fgvk elore deklaralasa
void test_insert();
void test_remove();
void small_random_test();
void big_random_test();

int main() {
  try {
    cout << "\n*** Insert tesztek ***\n";
    test_insert();
    cout << "\n*** Insert tesztek sikeresek ***\n";
    cout << "\n*** Remove tesztek ***\n";
    test_remove();
    cout << "\n*** Remove tesztek sikeresek ***\n";
    cout << "\n*** Kis elemszamu, reszletes teszt futtatasa ***\n" << endl;
    small_random_test();
    cout << "\n*** Nagy elemszamu teszt futtatasa ***\n" << endl;
    big_random_test();
  } catch (const exception &e) {
    cout << "HIBA: " << e.what() << endl;
    return 1;
  }
  return 0;
}

/**
 * @brief Piros fekete fa insertje eseten felmerulo rebalance lepeseket
 * teszteli. Sorra veszi a felmerulo eseteket, az esetek felmerulese utan a
 * validate fgv segitsegevel ellenorzi a PF fa tulajdonsagokat.
 */
void test_insert() {
  {
    cout << "\n_rebalance_after_insert, 1. eset\n" << endl;
    rb_tree<int> bal, jobb;
    vector<int> elemek = {10, 5, 11, 1, 7, 0};
    for (int e : elemek) {
      bal.insert(e);
      jobb.insert(-e);
    }
    bal.validate();
    jobb.validate();
  }
  {
    cout << "\n_rebalance_after_insert, 3. eset\n" << endl;
    rb_tree<int> bal, jobb;
    vector<int> elemek = {10, 12, 5, 1, 0};
    for (int e : elemek) {
      bal.insert(e);
      jobb.insert(-e);
    }
    bal.validate();
    jobb.validate();
  }
  {
    cout << "\n_rebalance_after_insert, 2. eset\n" << endl;
    rb_tree<int> bal, jobb;
    vector<int> elemek = {10, 12, 5, 1, 2, 4};
    for (int e : elemek) {
      bal.insert(e);
      jobb.insert(-e);
    }
    bal.validate();
    jobb.validate();
  }
}

/**
 * @brief Piros fekete fabol valo torles utan felmerulo rebalance lepeseket
 * teszteli. Sorra veszi a felmerulo eseteket es a validate-el ellenorzi a PF fa
 * tulajdonsagokat.
 */
void test_remove() {
  {
    cout << "\n_rebalance_after_remove, 4. eset\n" << endl;
    rb_tree<int> bal, jobb;
    vector<int> elemek = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int e : elemek) {
      bal.insert(e);
      jobb.insert(-e);
    }
    bal.remove(7);
    jobb.remove(-7);
    bal.validate();
    jobb.validate();
  }
  {
    cout << "\n_rebalance_after_remove, 3. eset\n" << endl;
    rb_tree<int> bal, jobb;
    vector<int> elemek = {1, 2, 3, 4, 5, 6, 7, 8, 10, 9};
    for (int e : elemek) {
      bal.insert(e);
      jobb.insert(-e);
    }
    bal.remove(7);
    jobb.remove(-7);
    bal.validate();
    jobb.validate();
  }
  {
    cout << "\n_rebalance_after_remove, 1. eset\n" << endl;
    rb_tree<int> bal, jobb;
    vector<int> elemek = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for (int e : elemek) {
      bal.insert(e);
      jobb.insert(-e);
    }
    bal.remove(5);
    jobb.remove(-5);
    bal.validate();
    jobb.validate();
  }
}

/**
 * @brief Futtatunk egy tesztet, amely 1000 v??letlen sz??mot sz??r be a
 * piros-fekete f??nkba. Minden egyes besz??r??s ut??n ellen??rizz??k, hogy az
 * adatszerkezet ??rv??nyes piros-fekete fa-e.
 *
 * Hogy kiz??rjuk annak lehet??s??g??t, hogy v??letlen??l cs??csokat dr??tozunk ki a
 * f??b??l, amikor nem is akarunk t??r??lni, ugyanezeket a sz??mokat besz??rjuk egy
 * std::set-be is, majd ellen??rizz??k, hogy a k??t adatszerkezet ugyanannyi elemet
 * tartalmaz-e.
 *
 * V??g??l az std::set-b??l az elemeket kim??soljuk egy std::vector-ba, amit
 * megkever??nk, hogy v??letlenszer?? sorrendben tudjuk t??r??lni az ??sszes elemet a
 * f??b??l. Ezzel ellen??rizz??k, hogy a t??rl??s j??l m??k??dik-e, ??s a v??g??n megn??zz??k,
 * hogy t??nyleg ??res lett-e a piros-fekete f??nk.
 *
 */
void small_random_test() {
  random_device rd;
  mt19937 g(rd());
  uniform_int_distribution<int> dist(0, numeric_limits<int>::max());
  set<int> stdShort;
  rb_tree<int> myShort;
  for (int i = 0; i < 1000; i++) {
    if (i % 100 == 0)
      cout << i + 100 << " elem beszurasa...";

    int x = dist(g);
    stdShort.insert(x);
    myShort.insert(x);
    myShort.validate();

    if ((i + 1) % 100 == 0)
      cout << " ok." << endl;
  }

  assert(myShort.size() == stdShort.size() && "Meret nem egyezik!");
  cout << "\nMeret rendben.\n" << endl;

  vector<int> arrayShort(stdShort.begin(), stdShort.end());
  shuffle(arrayShort.begin(), arrayShort.end(), g);

  for (size_t i = 0; i < arrayShort.size(); i++) {
    if (i % 100 == 0)
      cout << i + 100 << " elem torlese...";

    myShort.remove(arrayShort[i]);
    myShort.validate();

    if ((i + 1) % 100 == 0 || i + 1 == arrayShort.size())
      cout << " ok." << endl;
  }
  assert(myShort.size() == 0 &&
         "Meret nem egyezik! Minden elem eltavolitasa utan 0-nak kene lennie.");
  cout << "\nMeret rendben.\n" << endl;
}

/**
 * @brief Most egymilli?? v??letlen sz??mot sz??runk be a piros-fekete f??ba, ??s
 * p??rhuzamosan egy std::set-be. Csak a v??g??n ellen??rizz??k, hogy ??rv??nyes-e
 * az piros-fekete fa ??s egyezik-e a k??t adatszerkezet m??rete.
 *
 * Ezut??n megn??zz??k, hogy minden std::set-beli elem megtal??lhat??-e a
 * piros-fekete f??ban, majd sorban kit??r??ljuk ??ket, ??s ellen??rizz??k, hogy
 * ??res lett-e a fa.
 *
 * Ennek a tesztnek n??h??ny m??sodperc alatt le kell futnia. Ha f??l percn??l
 * tov??bb tart, akkor olyan hiba lehet a k??dban, ami elrontja az
 * aszimptotikus fut??si id??t.
 */
void big_random_test() {
  random_device rd;
  mt19937 g(rd());
  uniform_int_distribution<int> dist(0, numeric_limits<int>::max());
  cout << "Beszuras...";
  set<int> stdLong;
  rb_tree<int> myLong;
  for (int i = 0; i < 1000000; i++) {
    int x = dist(g);
    stdLong.insert(x);
    myLong.insert(x);
  }

  myLong.validate();
  assert(myLong.size() == stdLong.size() && "Meret nem egyezik!");
  cout << " ok." << endl;

  cout << "Torles...";
  for (set<int>::iterator it = stdLong.begin(); it != stdLong.end(); it++) {
    assert(myLong.find(*it) && "Hianyzo elem a fabol torles elott!");
  }

  for (set<int>::iterator it = stdLong.begin(); it != stdLong.end(); it++) {
    myLong.remove(*it);
  }

  myLong.validate();
  assert(myLong.size() == 0 &&
         "Meret nem egyezik! Minden elem eltavolitasa utan 0-nak kene lennie.");
  cout << " ok." << endl;
}
