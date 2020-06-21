/**
 * @file print_ip.cpp
 *
 * Functions for printing different types values as ip addresses.
 */
#include <list>
#include <tuple>
#include <vector>
#include <bitset>
#include <iostream>
#include <type_traits>


/**
 * @brief Prints ip address as integer value.
 */
template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
void print(T val)
{
	const uint8_t bits{ 8 };
	int8_t offset = sizeof(T) - 1;

	std::cout << (uint32_t)(std::numeric_limits<uint8_t>::max() & (val >> bits * offset));

	for (--offset; offset >= 0; --offset)
	{
		std::cout << '.' << (uint32_t)(std::numeric_limits<uint8_t>::max() & (val >> bits * offset));
	}
}


/**
 * @brief Prints ip address as string value.
 */
template<typename T, typename = std::enable_if_t<std::is_same_v<T, std::string>>>
void print(const T& val)
{
	std::cout << val.c_str();
}


/**
 * @brief Prints ip address as container (vector or list) value.
 */
template
<
	template<typename...> typename T, 
	typename... Args,
	typename = std::enable_if_t
	<
		std::is_same_v
		<
			T<Args...>,
			std::vector<Args...>
		>

		||

		std::is_same_v
		<
			T<Args...>,
			std::list<Args...>
		>
	>
>
void print(const T<Args...>& container)
{
	auto it = container.cbegin();
	std::cout << *it;

	++it;

	for (; it != container.cend(); ++it)
	{
		std::cout << '.' << *it;
	}
}


/**
 * \defgroup helpers
 * Helpers for tuple printer.
 */
/**@{*/
template<size_t index, typename Head, typename... Args>
struct print_tuple_elements
{
	static void print(const std::tuple<Head, Args...>& tup)
	{
		print_tuple_elements<index - 1, Head, Args...>::print(tup);

		std::cout << '.' << std::get<index>(tup);
	}
};


template<typename Head, typename... Args>
struct print_tuple_elements<0, Head, Args...>
{
	static void print(const std::tuple<Head, Args...>& tup)
	{
		std::cout << std::get<0>(tup);
	}
};
/**@}*/


/**
 * @brief Prints ip address as tuple value.
 */
template
<
	template<typename...> typename T, 
	typename Head,
	typename... Args,
	typename = std::enable_if_t		
	< 
			std::is_same_v
			< 
				T<Head, Args...>,
				std::tuple<Head, Args...>
			>

			&&

			(std::is_same_v<Head, Args> && ...)
	> 
>
void print(const T<Head, Args...>& tup)
{
	print_tuple_elements<std::tuple_size_v<T<Head, Args...>> - 1, Head, Args...>::print(tup);
}


/**
 * @brief Function wraper for all printers.
 */
template<typename T>
void print_ip(const T& val)
{
	print(val);
	std::cout << std::endl;
}


/**
 * @brief Main function.
 */
int main()
{
	{
		char val{ -1 };
		print_ip(val);
	}

	{
		short val{ 0 };
		print_ip(val);
	}

	{
		int val{ 2130706433 };
		print_ip(val);
	}

	{
		long long val{ 8875824491850138409 };
		print_ip(val);
	}

	{
		std::string val{ "Hello" };
		print_ip(val);
	}

	{
		std::vector<int> val{ 1, 2, 3 };
		print_ip(val);
	}

	{
		std::list<int> val{ 4, 5, 6 };
		print_ip(val);
	}

	{
		std::tuple val{ 7, 8, 9 };
		print_ip(val);
	}

	return 0;
}