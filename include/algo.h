
#ifndef CCU_ALGO_H
#define CCU_ALGO_H

namespace ccu {

template <typename T, typename R, typename U, typename K>
struct DiffStatusTrait {
	typedef T value_type;
	typedef R return_type
	typedef U user_type;
	typedef K key_type;
	typedef std::function<key_type(const user_type&, const value_type&)> key_op_type;
	typedef std::function<return_type(const user_type&, const key_type &, const value_type&)> gen_op_type;

	DiffStatusTrait(user_type value, key_op_type fn_key, gen_op_type fn_gen):
		user(value),
		key_op(fn_key),
		gen_op(fn_gen) {
	}

	const gen_op_type gen_op;
	const key_op_type key_op;
	const user_type user;
}


template <typename T>
struct CalcDiffDistance {
	typedef std::function<bool(const T&)> both_present_op_type;
	typedef std::function<bool(const T&)> changed_op_type;

	CalcDiffDistance() {}
	void operator()(T item) {
		total++;

		if (is_both_present_op(T)) {
			total++;

			if (check_changed_op(T))
				intersect--;
		} else
			intersect--;

		distance = (total+intersect) / total;
	}

	const both_present_op_type both_present_op;
	const changed_op_type changed_op;

	double distance{0.0};
	double intersect{0.0};
	double total{0.0};
}


/*template <typename InputIt1, typename InputIt2, typename OutputIt, typename LessOp, typename DiffOp, typename Wrapper1, typename Wrapper2>
void diff_status(InputIt1 first1,
	InputIt1 last1,
	InputIt2 first2,
	InputIt2 last2,
	OutputIt d_first,
	LessOp less_op,
	Wrapper1 wrapper1,
	Wrapper2 wrapper2,
	LessOp less_op,
	DiffOp diff_op) {

	while (first1 != last1) {
		Wrapper1::key_type key1 = wrapper1.key_op(wrapper1.user, *first1);

		if (first2 != last2) {
			Wrapper2::key_type key2 = wrapper2.key_op(wrapper2.user, *first2);

			if (less_op(key1, key2)) {
				*d_first = wrapper1.gen_op(wrapper.user, key1, *first1); // present in first1
				std::advance(first1);
			} else if (!less_op(key1, key2) && !less_op(key2, key1)) {
				*d_first = diff_op(key1, key2, *first1, *first2);
				std::advance(first1);
				std::advance(first2);
			} else {
				*d_first = wrapper2.gen_op(wrapper2.user, key2, *first2); // present in first2
				std::advance(first2);
			}
			
		} else {
			*d_first = wrapper1.gen_op(wrapper1.user, key1, *first1); // present in first1
			std::advance(first1);
		}

		std::advance(d_first);
	}

	while (first2 != last2) {
		Wrapper2::key_type key2 = wrapper2.key_op(wrapper2.user, *first2);

		*d_first = wrapper2.gen_op(wrapper2.user, key2, *first2); // present in first2
		std::advance(d_first);
		std::advance(first2);
	}
}*/

template <typename InputIt1, typename InputIt2, typename OutputIt, typename LessOp>
double diff_status(InputIt1 first1,
	InputIt1 last1,
	InputIt2 first2,
	InputIt2 last2,
	OutputIt d_first,
	std::function<OutputIt::value_type(const InputIt1::value_type&)> gen1_op,
	std::function<OutputIt::value_type(const InputIt2::value_type&)> gen2_op,
	std::function<OutputIt::value_type(const InputIt1::value_type&, const InputIt2::value_type&, bool)> gen_both_op,
	LessOp less_op) {

	double total = 0.0;
	double intersect = 0.0;

	while ((first1 != last1) && (first2 != last2)) {

		if (less_op(*first1, *first2)) {
			*d_first = gen1_op(*first1);
			std::advance(first1);
			intersect--;
		} else if (!less_op(*first1, *first2) && !less_op(*first2, *first1)) {
			bool changed = false;
			*d_first = gen_both_op(*first1, *first2, changed);
			std::advance(first1);
			std::advance(first2);
			if (changed)
				intersect--;
			total++;
			
		} else {
			*d_first = gen2_op(*first2);
			std::advance(first2);
			intersect--;
		}

		std::advance(d_first);
		total++;
	}

	// copy tail from first1
	while (first1 != last1) {
		*d_first = gen1_op(*first1);
		std::advance(first1);
		std::advance(d_first);
		total++;
		intersect--;
	}

	// copy tail from first2
	while (first2 != last2) {
		*d_first = gen2_op(*first2);
		std::advance(first2);
		std::advance(d_first);
		total++;
		intersect--;
	}

	return (total+intersect) / total;
}

} // namespace ccu
#endif
