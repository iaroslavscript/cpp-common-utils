/* MIT License
 * 
 * Copyright (c) 2020 Iaroslav Akimov
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CCU_ALGO_H
#define CCU_ALGO_H


#include <iterator>


namespace ccu {
namespace algo {

template <typename OutputItValue, typename OutputIt, typename InputIt1,
	typename InputIt2, typename LessOp>
double diff_status(InputIt1 first1,
	InputIt1 last1,
	InputIt2 first2,
	InputIt2 last2,
	OutputIt d_first,
	std::function<OutputItValue(const InputIt1::value_type&)> gen1_op,
	std::function<OutputItValue(const InputIt2::value_type&)> gen2_op,
	std::function<OutputItValue(const InputIt1::value_type&,
		const InputIt2::value_type&, bool&)> gen_both_op,
	LessOp less_op) {

	double total = 0.0;
	double intersect = 0.0;

	while ((first1 != last1) && (first2 != last2)) {

		if (less_op(*first1, *first2)) {
			*d_first++ = gen1_op(*first1);
			std::advance(first1, 1);
			intersect--;
		} else if (!less_op(*first1, *first2) && !less_op(*first2, *first1)) {
			bool changed = false;
			*d_first++ = gen_both_op(*first1, *first2, changed);
			std::advance(first1);
			std::advance(first2);
			if (changed)
				intersect--;
			total++;
			
		} else {
			*d_first++ = gen2_op(*first2);
			std::advance(first2);
			intersect--;
		}

		total++;
	}

	// copy tail from first1
	while (first1 != last1) {
		*d_first++ = gen1_op(*first1);
		std::advance(first1);
		total++;
		intersect--;
	}

	// copy tail from first2
	while (first2 != last2) {
		*d_first++ = gen2_op(*first2);
		std::advance(first2);
		total++;
		intersect--;
	}

	return (total+intersect) / total;
}

} // namespace algo
} // namespace ccu
#endif
