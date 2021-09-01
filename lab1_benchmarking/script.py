import subprocess
import sys
import math


PROGRAM_PATH = "./bin/tester"

OUTPUT_PATH = "data"

METHODS = ["STOD", "LEXICAL_CAST", "STRINGSTREAM"]


def main():
    args = sys.argv
    if len(args) != 4:
        print("Wrong number of arguments")
        return -1

    try:
        for method in range(1, len(METHODS) + 1):

            output_path = f"{OUTPUT_PATH}/output{method}.txt"
            f = open(output_path, "w")
            f.close()
            str_method = str(method)
            for iteration in range(int(args[1])):
                subprocess.call([PROGRAM_PATH,
                                 str_method, args[2],
                                 output_path])

        result_sum = []
        result_average = []
        min_time = [math.inf] * len(METHODS)

        for method in range(1, len(METHODS) + 1):
            with open(f"{OUTPUT_PATH}/output{method}.txt") as in_file:
                for line in in_file.readlines():
                    words = line.split()
                    sum_num = float(words[0])
                    for num in result_sum:
                        if math.isclose(num, sum_num):
                            break
                    else:
                        result_sum.append(sum_num)

                    average_num = float(words[1])
                    for num in result_average:
                        if math.isclose(num, average_num):
                            break
                    else:
                        result_average.append(average_num)

                    min_time[method - 1] = min(min_time[method - 1],
                                               int(words[2]))

        if len(result_sum) != 1 or len(result_average) != 1:
            print("Something is wrong with calculated results")
            print(result_sum, result_average)
            return -4

        for i in range(len(METHODS)):
            print(f"{METHODS[i]}: {min_time[i]}")
        print("No problems were found")

        with open(args[3], "w") as output:
            for i in range(len(METHODS)):
                output.write(f"{METHODS[i]}: {min_time[i]}\n")

    except ValueError:
        print("Invalid arguments")
        return -2

    except IndexError:
        print("Something is wrong with output files")
        return -3

    return 0


if __name__ == "__main__":
    main()
