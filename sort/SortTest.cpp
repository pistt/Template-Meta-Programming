#include "headers/BubbleSort.hpp"
#include "headers/InsertionSort.hpp"
#include "headers/SelectionSort.hpp"
#include "headers/QuickSort.hpp"
#include "headers/MergeSort.hpp"

int main(int argc, char const *argv[]) {
	using namespace std;
	// Test example1
	using testtype = int_seq<1,-992,89,23,9982,123,90,7,2391,98734,6105,-972,-773,-614,-1,0,702>;
	auto x = quick_sort<lessthan, testtype>::type();
	auto y = bubble_sort<lessthan, testtype>::type();
	auto z = selection_sort<lessthan, testtype>::type();
	auto u = insertion_sort<lessthan, testtype>::type();
	auto v = merge_sort<lessthan, testtype>::type();
	print(x);
	print(y);
	print(z);
	print(u);
	print(v);

	// Test example2
	// using testtype = int_seq<109, 22, 3, 55, 6, 23, 343>;
	// auto x = quick_sort<lessthan, testtype>::type();
	// auto y = bubble_sort<lessthan, testtype>::type();
	// auto z = selection_sort<lessthan, testtype>::type();
	// auto u = insertion_sort<lessthan, testtype>::type();
	// auto v = merge_sort<lessthan, testtype>::type();
	// print(x);
	// print(y);
	// print(z);
	// print(u);
	// print(v);

	return 0;
}